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
        m0.at(0) = 0;
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

        if (result != c.end())
            return std::vector(result->second.begin(), result->second.end());
        return {};
        
    }   

}