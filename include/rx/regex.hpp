#pragma once

// #include <meta>
#include <concepts>
#include <cstddef>
#include <string_view>

#include <rx/etc/util.hpp>
#include <rx/ast/tree.hpp>
#include <rx/fsm/tnfa.hpp>
#include <rx/fsm/tnfa.hpp>
#include <rx/fsm/tdfa.hpp>


namespace rx
{
    template<detail::string_literal Pattern>
    requires (std::same_as<char, typename decltype(Pattern.value)::value_type> /* temporary: remove later */) 
    class match
    {
        consteval
        {
            std::string_view pattern{ Pattern.value.begin(), Pattern.value.end() };
        }

    public:
        [[nodiscard]] consteval int test() const noexcept
        {
            using namespace detail;

            using char_type = decltype(Pattern.value)::value_type;
            using pattern_view_t = std::basic_string_view<char_type>;

            const pattern_view_t pattern{ Pattern.value.begin(), Pattern.value.end() };
            
            /* parse pattern string into tree */
            expr_tree ast{ pattern };

            /* convert to tnfa */
            tagged_nfa nfa{ ast }; 

            // TODO: implement the rest!

            return pattern.size();
        }

    };

    namespace literals
    {
        template<detail::string_literal P>
        consteval auto operator ""_rx()
        {
            return match<P>();
        }
    }
}


