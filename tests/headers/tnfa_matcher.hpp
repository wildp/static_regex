#pragma once

#include <algorithm>
#include <optional>
#include <ranges>
#include <string_view>
#include <vector>

#include <rx/api/regex_error.hpp>
#include <rx/etc/cdarray.hpp>
#include <rx/ast/tree.hpp>
#include <rx/fsm/tnfa.hpp>


namespace rx::testing
{
    template<typename CharT>
    class tnfa_matcher : public detail::tagged_nfa<CharT>
    {
    public:
        using tag_result = std::vector<std::size_t>;
        using detail::tagged_nfa<CharT>::tagged_nfa;

        [[nodiscard]] constexpr std::optional<tag_result> match(std::basic_string_view<CharT> input) const;

    private:
        using tag_vector = detail::cdarray<std::size_t>;
        using closure_entry = std::pair<std::size_t, tag_vector>;
        using closure_t = std::vector<closure_entry>;

        [[nodiscard]] constexpr closure_t e_closure(closure_t&& closure, std::size_t k) const;
        [[nodiscard]] constexpr closure_t step(const closure_t& closure, CharT a) const;
        [[nodiscard]] constexpr tag_result make_submatch_results(const tag_vector& v, std::size_t size) const;
    };

    template<typename CharT>
    tnfa_matcher(const detail::expr_tree<CharT>&, detail::fsm_flags) -> tnfa_matcher<CharT>;
    

    /* tagged nfa simulation */

    template<typename CharT>
    constexpr auto tnfa_matcher<CharT>::e_closure(closure_t&& closure, const std::size_t k) const -> closure_t
    {
        static constexpr auto compose = [](const auto& g, const auto& f) {
            return [=]<typename T>(T&& arg) { 
                return std::invoke(g, std::invoke(f, std::forward<T>(arg)));
            };
        };

        using namespace rx::detail;
        using namespace rx::detail::tnfa;

        closure_t new_closure;
        closure_t stack{ std::move(closure) };
        std::ranges::reverse(stack);

        while (not stack.empty())
        {
            new_closure.push_back(std::move(stack.back()));
            stack.pop_back();
            const auto& [q, m]{ new_closure.back() };
            
            using epsilon_t = std::pair<tnfa::state_t, tnfa::epsilon_tr>;
            std::vector et{ std::from_range,
                            this->get_node(q).out_tr
                            | std::views::transform([&](const std::size_t i) { return this->get_tr(i); })
                            | std::views::filter([](const auto& t) { return std::holds_alternative<tnfa::epsilon_tr>(t.type); })
                            | std::views::transform([](const auto& t) -> epsilon_t { return { t.dst, std::get<tnfa::epsilon_tr>(t.type) }; }) };
            
            std::ranges::sort(et, std::ranges::greater{}, compose(&epsilon_tr::priority, &epsilon_t::second));

            for (const auto& [next, e] : et)
            {
                if (not std::ranges::contains(new_closure, next, &closure_entry::first))
                {
                    auto new_m{ m };
                    if (e.tag > 0)
                        new_m.at(e.tag - 1) = k;
                    else if (e.tag < 0)
                        new_m.at((-e.tag) - 1) = no_tag;
                    stack.emplace_back(next, std::move(new_m));
                }   
            }            
        }

        std::erase_if(new_closure, [this](const closure_entry& e) -> bool {
            if (this->get_node(e.first).is_final)
                return false;
            return 0 != std::ranges::count_if(this->get_node(e.first).out_tr,
                                              [&](const std::size_t i) { return not std::holds_alternative<normal_tr<CharT>>(this->get_tr(i).type); });
        });

        return new_closure;
    }

    template<typename CharT>
    constexpr auto tnfa_matcher<CharT>::step(const closure_t& closure, const CharT a) const -> closure_t
    {
        using namespace rx::detail;
        using namespace rx::detail::tnfa;

        closure_t new_closure;

        for (auto& [q, m] : closure)
        {
            for (std::size_t i{ 0 }; i < this->get_node(q).out_tr.size(); ++i)
            {
                const auto& tr{ this->get_tr(this->get_node(q).out_tr[i]) };
                
                if (const auto* const ptr{ std::get_if<normal_tr<CharT>>(&tr.type) }; ptr != nullptr)
                {
                    if (ptr->cs.contains(a))
                    {
                        if (i + 1 == this->get_node(q).out_tr.size())
                            new_closure.emplace_back(tr.dst, std::move(m));
                        else
                            new_closure.emplace_back(tr.dst, m);
                    }
                }
            }      
        }

        return new_closure;
    }

    template<typename CharT>
    constexpr auto tnfa_matcher<CharT>::match(std::basic_string_view<CharT> input) const -> std::optional<tag_result>
    {
        using namespace rx::detail;

        tag_vector m0(this->tag_count(), no_tag);
        closure_t c{{ this->start_node(), std::move(m0) }};

        for (std::size_t k{ 0 }; k < input.size(); ++k)
        {
            c = e_closure(std::move(c), k);
            c = step(c, input.at(k));
            
            if (c.empty())
                return {};
        }

        c = e_closure(std::move(c), input.size());
        auto result{ std::ranges::find_if(c, [&](std::size_t arg){ return this->get_node(arg).is_final; }, &closure_entry::first) };

        if (result == c.end())
            return {};

        return make_submatch_results(result->second, input.size());
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
            auto rng{ ci.lookup(i) | std::views::filter(f)
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