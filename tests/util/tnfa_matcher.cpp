module;

#include <algorithm>
#include <optional>
#include <ranges>
#include <string_view>
#include <vector>

export module rx.tests:tnfa_matcher;

// import std;
import rx.ast;
import rx.fsm;
import rx.util;

namespace rx::testing
{
    export template<typename CharT>
    class tnfa_matcher : public detail::tagged_nfa<CharT>
    {
    public:
        using tag_result = std::vector<std::size_t>;
        using detail::tagged_nfa<CharT>::tagged_nfa;

        [[nodiscard]] constexpr std::optional<tag_result> submatches(std::basic_string_view<CharT> input) const;

        [[nodiscard]] constexpr bool match(std::basic_string_view<CharT> input) const
        {
            return submatches(input).has_value();
        }

    private:
        using tag_vector = detail::cdarray<std::size_t>;
        using closure_entry = std::pair<std::size_t, tag_vector>;
        using closure_t = std::vector<closure_entry>;

        [[nodiscard]] constexpr closure_t e_closure(closure_t&& closure, std::size_t k) const;
        [[nodiscard]] constexpr closure_t step(const closure_t& closure, CharT a) const;
        [[nodiscard]] constexpr tag_result make_submatch_results(const tag_vector& v, std::size_t size) const;
    };
    
    /* tagged nfa simulation */

    template<typename CharT>
    constexpr auto tnfa_matcher<CharT>::e_closure(closure_t&& closure, const std::size_t k) const -> closure_t
    {
        using namespace rx::detail;

        closure_t new_closure;
        closure_t stack{ std::move(closure) };
        std::ranges::reverse(stack);

        while (not stack.empty())
        {
            new_closure.push_back(std::move(stack.back()));
            stack.pop_back();
            const auto& [q, m]{ new_closure.back() };
            
            std::vector<epsilon_tr> et{ this->get_node(q).tr
                                        | std::views::filter([](const auto& t) { return std::holds_alternative<epsilon_tr>(t); })
                                        | std::views::transform([](const auto& t) -> epsilon_tr { return std::get<epsilon_tr>(t); })
                                        | std::ranges::to<std::vector>() };
            
            std::ranges::sort(et, std::ranges::greater{}, &epsilon_tr::priority);

            for (const epsilon_tr& e : et)
            {
                if (not std::ranges::contains(new_closure, e.next, &closure_entry::first))
                {
                    auto new_m{ m };
                    if (e.tag > 0)
                        new_m.at(e.tag - 1) = k;
                    else if (e.tag < 0)
                        new_m.at((-e.tag) - 1) = no_tag;
                    stack.emplace_back(e.next, std::move(new_m));
                }   
            }            
        }

        std::erase_if(new_closure, [this](const closure_entry& e) -> bool {
            if (e.first == this->end)
                return false;
            return 0 != std::ranges::count_if(this->get_node(e.first).tr,
                                              [](const auto& t) { return not std::holds_alternative<n_tr<CharT>>(t); });
        });

        return new_closure;
    }

    template<typename CharT>
    constexpr auto tnfa_matcher<CharT>::step(const closure_t& closure, const CharT a) const -> closure_t
    {
        using namespace rx::detail;

        closure_t new_closure;

        for (auto& [q, m] : closure)
        {
            std::vector<n_tr<CharT>> ct{ this->get_node(q).tr
                                         | std::views::filter([](const auto& t) { return std::holds_alternative<n_tr<CharT>>(t); })
                                         | std::views::transform([](const auto& t) -> n_tr<CharT> { return std::get<n_tr<CharT>>(t); })
                                         | std::ranges::to<std::vector>() };

            for (const n_tr<CharT>& c : ct)
            {
                if (c.lower <= a and a <= c.upper)
                {
                    if (ct.size() == 1)
                        new_closure.emplace_back(c.next, std::move(m));
                    else
                        new_closure.emplace_back(c.next, m);
                }
            }      
        }

        return new_closure;
    }

    template<typename CharT>
    constexpr auto tnfa_matcher<CharT>::submatches(std::basic_string_view<CharT> input) const -> std::optional<tag_result>
    {
        using namespace rx::detail;

        tag_vector m0(this->tag_count(), no_tag);
        closure_t c{{ this->match_start, std::move(m0) }};

        for (std::size_t k{ 0 }; k < input.size(); ++k)
        {
            c = e_closure(std::move(c), k);
            c = step(c, input.at(k));
            
            if (c.empty())
                return {};
        }

        c = e_closure(std::move(c), input.size());
        auto result{ std::ranges::find(c, this->end, &closure_entry::first) };

        if (result == c.end())
            return {};
        // return make_submatch_results(result->second, input.size());
        return std::vector(result->second.begin(), result->second.end());
    }

    /* tag remapping */

    template<typename CharT>
    constexpr auto tnfa_matcher<CharT>::make_submatch_results(const tag_vector& v, std::size_t size) const -> tag_result
    {
        using namespace rx::detail;

        std::vector<std::size_t> result;
        const capture_info& ci{ this->get_capture_info() };
        const auto capture_count{ ci.capture_count() };

        auto f = [&](const capture_info::tag_pair_t& p) -> bool {
            return not ((p.first.tag_number >= 0 and v.at(p.first.tag_number) == no_tag)
                         or (p.second.tag_number >= 0 and v.at(p.second.tag_number) == no_tag));
        };

        auto t = [&](const capture_info::tag_pair_t& p) -> std::pair<std::size_t, std::size_t> {
            return { ((p.first.tag_number >= 0) ? v.at(p.first.tag_number)
                      : ((p.first.tag_number == start_of_input_tag) ? 0 : size)) + p.first.offset,
                     ((p.second.tag_number >= 0) ? v.at(p.second.tag_number)
                      : ((p.second.tag_number == start_of_input_tag) ? 0 : size)) + p.second.offset };
        };

        for (std::size_t i{ 0 }; i < capture_count; ++i)
        {
            const auto [beg, end]{ ci.lookup(i) };

            auto rng{ std::ranges::subrange(beg, end) | std::views::filter(f)
                                                      | std::views::transform(t)
                                                      | std::ranges::to<std::vector>() };
                    
            if (std::ranges::size(rng) == 0)
            {
                result.insert(result.end(), { no_tag, no_tag });
                continue;
            }

            auto max_elem{ std::ranges::max_element(rng, std::ranges::less{}, &std::pair<std::size_t, std::size_t>::first) };

            result.push_back(max_elem->first);
            result.push_back(max_elem->second);
        }
        
        return result;
    }
}