#pragma once

#include <cstdint>
#include <ranges>
#include <vector>

namespace rx::detail::parser
{

    struct capture_flags
    {
        enum class flag_value : std::uint8_t
        {
            enabled,
            disabled,
            inherit,
        };

        flag_value caseless  : 2;
        flag_value multiline : 2;  /* not fully implemented -> need to implement in matcher */
        flag_value dotall    : 2;
        flag_value ungreedy  : 2;
    };

    class capture_stack
    {
        struct cse
        {
            enum class modes : std::int_least8_t
            {
                normal,
                flag_assigning,
                non_capturing,
                branch_reset,
            };

            using cf = capture_flags::flag_value;

            std::uint_least16_t number;
            std::uint_least16_t number_end;
            capture_flags flags;
            modes mode;

            constexpr cse() noexcept :
                number{ 0 },
                number_end{ 0 },
                flags{ .caseless = cf::disabled, .multiline = cf::disabled, .dotall = cf::disabled, .ungreedy = cf::disabled },
                mode{ modes::non_capturing }
            {
            }

            constexpr explicit cse(std::uint_least16_t cur, std::uint_least16_t end) noexcept :
                number{ cur },
                number_end{ end },
                flags{ .caseless = cf::inherit, .multiline = cf::inherit, .dotall = cf::inherit, .ungreedy = cf::inherit },
                mode{ modes::normal }
            {
            }
        };

    public:
        using cf = capture_flags::flag_value;

        constexpr void push_non_capturing()
        {
            auto next{ next_number() };
            elems_.emplace_back(next, next);
            elems_.back().mode = cse::modes::non_capturing;
        }

        [[nodiscard]] constexpr bool push()
        {
            /* return true if overflow and false otherwise */

            auto next{ next_number() };

            if (next + 1 == 0) /* unsigned integer overflow */
                return true;

            elems_.emplace_back(next, next + 1);
            return false;
        }

        constexpr void set_non_capturing() noexcept
        {
            if (elems_.empty())
                return;

            if (auto& elem{ elems_.back() }; elem.mode != cse::modes::non_capturing)
            {
                elem.mode = cse::modes::non_capturing;
                elem.number_end = elem.number;
            }
        }

        constexpr void set_branch_reset() noexcept
        {
            if (elems_.empty())
                return;

            if (auto& elem{ elems_.back() }; elem.mode != cse::modes::branch_reset)
            {
                elem.mode = cse::modes::branch_reset;
                elem.number_end = elem.number;
            }
        }

        constexpr void set_flag_assigning() noexcept
        {
            if (elems_.empty())
                return;

            if (auto& elem{ elems_.back() }; elem.mode != cse::modes::flag_assigning)
            {
                elem.mode = cse::modes::flag_assigning;
                elem.number_end = elem.number;
            }
        }

        constexpr void branch_reset_if_enabled() noexcept
        {
            if (elems_.empty())
                return;

            if (auto& elem{ elems_.back() }; elem.mode == cse::modes::branch_reset)
            {
                auto& target{ (elems_.size() < 2) ? base_ : *(std::next(elems_.rbegin())) };
                target.number_end = std::max(target.number_end, elem.number_end) ;
                elem.number_end = elem.number;
            }
        }

        [[nodiscard]] constexpr std::optional<uint_least16_t> capture_count() const noexcept
        {
            if (elems_.empty())
                return base_.number_end;
            return {};
        }

        constexpr std::optional<uint_least16_t> pop()
        {
            if (elems_.empty())
                return {};

            auto elem{ elems_.back() };
            elems_.pop_back();

            auto& target{ elems_.empty() ? base_ : elems_.back() };
            
            /* overwrite containing capturing group's flags when elem is an empty capturing group */
            if (elem.mode == cse::modes::flag_assigning)
            {
                // template for (constexpr auto e : std::define_static_array(std::meta::nonstatic_data_members_of(^^capture_flags, std::meta::access_context::unchecked())))
                // {
                //     if (elem.flags.[:e:] != cf::inherit)
                //         target.flags.[:e:] = elem.flags.[:e:] ;
                // }

                if (elem.flags.caseless  != cf::inherit) target.flags.caseless  = elem.flags.caseless;
                if (elem.flags.multiline != cf::inherit) target.flags.multiline = elem.flags.multiline;
                if (elem.flags.dotall    != cf::inherit) target.flags.dotall    = elem.flags.dotall;
                if (elem.flags.ungreedy  != cf::inherit) target.flags.ungreedy  = elem.flags.ungreedy;
            }

            if (elem.mode == cse::modes::branch_reset)
                target.number_end = std::max(target.number_end, elem.number_end) ;
            else
                target.number_end = elem.number_end;

            if (elem.mode == cse::modes::normal)
                return elem.number;
            else
                return {};
    }

        // template<std::meta::info CaptureFlagReflection>
        // requires (std::ranges::contains(std::meta::nonstatic_data_members_of(^^capture_flags, std::meta::access_context::unchecked()), CaptureFlagReflection))
        // [[nodiscard]] constexpr bool get() const
        // {
        //     for (const auto& elem : elems_ | std::views::reverse)
        //     {
        //         if (elem.flags.[:CaptureFlagReflection:] != cf::inherit)
        //             return elem.flags.[:CaptureFlagReflection:] == cf::enabled;
        //     }
            
        //     return base_.flags.[:CaptureFlagReflection:] == cf::enabled;
        // }


        [[nodiscard]] constexpr bool caseless() const
        {
            for (const auto& elem : elems_ | std::views::reverse) 
                if (elem.flags.caseless != cf::inherit)
                    return elem.flags.caseless == cf::enabled;
            return base_.flags.caseless == cf::enabled;
        }

        [[nodiscard]] constexpr bool multiline() const
        {
            for (const auto& elem : elems_ | std::views::reverse) 
                if (elem.flags.multiline != cf::inherit)
                    return elem.flags.multiline == cf::enabled;
            return base_.flags.multiline == cf::enabled;
        }

        [[nodiscard]] constexpr bool dotall() const
        {
            for (const auto& elem : elems_ | std::views::reverse) 
                if (elem.flags.dotall != cf::inherit)
                    return elem.flags.dotall == cf::enabled;
            return base_.flags.dotall == cf::enabled;
        }

        [[nodiscard]] constexpr bool ungreedy() const
        {
            for (const auto& elem : elems_ | std::views::reverse) 
                if (elem.flags.ungreedy != cf::inherit)
                    return elem.flags.ungreedy == cf::enabled;
            return base_.flags.ungreedy == cf::enabled;
        }

        // template<std::meta::info CaptureFlagReflection>
        // requires (std::ranges::contains(std::meta::nonstatic_data_members_of(^^capture_flags, std::meta::access_context::unchecked()), CaptureFlagReflection))
        // [[nodiscard]] constexpr void set(bool value)
        // {
        //     ((elems_.empty()) ? base_ : elems_.back()).flags.[:CaptureFlagReflection:] = (value) ? cf::enabled : cf::disabled;
        // }

        constexpr void set_caseless(bool value) noexcept
        {
            ((elems_.empty()) ? base_ : elems_.back()).flags.caseless = (value) ? cf::enabled : cf::disabled;
        }

        constexpr void set_multiline(bool value) noexcept
        {
            ((elems_.empty()) ? base_ : elems_.back()).flags.multiline = (value) ? cf::enabled : cf::disabled;
        }

        constexpr void set_dotall(bool value) noexcept
        {
            ((elems_.empty()) ? base_ : elems_.back()).flags.dotall = (value) ? cf::enabled : cf::disabled;
        }

        constexpr void set_ungreedy(bool value) noexcept
        {
            ((elems_.empty()) ? base_ : elems_.back()).flags.ungreedy = (value) ? cf::enabled : cf::disabled;
        }
        
    private:

        [[nodiscard]] constexpr std::uint_least16_t next_number() const noexcept
        {
            auto& target{ elems_.empty() ? base_ : elems_.back() };
            return target.number_end;
        }

        std::vector<cse>    elems_{};
        cse                 base_{};
    };

}