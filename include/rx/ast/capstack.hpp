// Copyright (C) 2026 Peter Wild
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#pragma once

#include <algorithm>
#include <cstdint>
#include <meta>
#include <ranges>
#include <vector>


namespace rx::detail::parser
{
    struct capture_flags
    {
        enum class flag_value : unsigned char
        {
            enabled,
            disabled,
            inherit,
        };

        flag_value caseless  : 2;
        flag_value multiline : 2; /* not fully implemented -> need to implement in matcher */
        flag_value dotall    : 2;
        flag_value ungreedy  : 2;
    };

    class capture_stack
    {
        using number_t = std::uint_least16_t;

        struct cse
        {
            enum class modes : unsigned char
            {
                normal,
                flag_assigning,
                non_capturing,
                branch_reset,
            };

            using cf = capture_flags::flag_value;

            number_t number;
            number_t number_end;
            capture_flags flags;
            modes mode;

            constexpr cse() noexcept
                : number{ 1 }
                , number_end{ 1 }
                , flags{ .caseless = cf::disabled, .multiline = cf::disabled, .dotall = cf::disabled, .ungreedy = cf::disabled }
                , mode{ modes::non_capturing } {}

            constexpr explicit cse(number_t cur, number_t end) noexcept
                : number{ cur }
                , number_end{ end }
                , flags{ .caseless = cf::inherit, .multiline = cf::inherit, .dotall = cf::inherit, .ungreedy = cf::inherit }
                , mode{ modes::normal } {}
        };

    public:
        using cf = capture_flags::flag_value;

        constexpr void push_non_capturing()
        {
            auto next = next_number();
            elems_.emplace_back(next, next);
            elems_.back().mode = cse::modes::non_capturing;
        }

        [[nodiscard]] constexpr bool push()
        {
            /* return true if overflow and false otherwise */

            auto next = next_number();

            if (next + 1 == 0) /* unsigned integer overflow */
                return true;

            elems_.emplace_back(next, next + 1);
            return false;
        }

        constexpr void set_non_capturing() noexcept
        {
            if (elems_.empty())
                return;

            if (auto& elem = elems_.back(); elem.mode != cse::modes::non_capturing)
            {
                elem.mode = cse::modes::non_capturing;
                elem.number_end = elem.number;
            }
        }

        constexpr void set_branch_reset() noexcept
        {
            if (elems_.empty())
                return;

            if (auto& elem = elems_.back(); elem.mode != cse::modes::branch_reset)
            {
                elem.mode = cse::modes::branch_reset;
                elem.number_end = elem.number;
            }
        }

        constexpr void set_flag_assigning() noexcept
        {
            if (elems_.empty())
                return;

            if (auto& elem = elems_.back(); elem.mode != cse::modes::flag_assigning)
            {
                elem.mode = cse::modes::flag_assigning;
                elem.number_end = elem.number;
            }
        }

        constexpr void branch_reset_if_enabled() noexcept
        {
            if (elems_.empty())
                return;

            if (auto& elem = elems_.back(); elem.mode == cse::modes::branch_reset)
            {
                auto& target = (elems_.size() < 2) ? base_ : *(std::ranges::next(elems_.rbegin()));
                target.number_end = std::max(target.number_end, elem.number_end);
                elem.number_end = elem.number;
            }
        }

        [[nodiscard]] constexpr std::optional<number_t> capture_count() const noexcept
        {
            if (elems_.empty())
                return base_.number_end;
            return {};
        }

        constexpr std::optional<number_t> pop()
        {
            if (elems_.empty())
                return {};

            auto elem = elems_.back();
            elems_.pop_back();

            auto& target = elems_.empty() ? base_ : elems_.back();

            /* overwrite containing capturing group's flags when elem is an empty capturing group */
            if (elem.mode == cse::modes::flag_assigning)
            {
                template for (constexpr auto e : define_static_array(nonstatic_data_members_of(^^capture_flags, std::meta::access_context::unchecked())))
                    if (elem.flags.[: e :] != cf::inherit)
                        target.flags.[: e :] = elem.flags.[: e :];
            }

            if (elem.mode == cse::modes::branch_reset)
                target.number_end = std::max(target.number_end, elem.number_end);
            else
                target.number_end = elem.number_end;

            if (elem.mode == cse::modes::normal)
                return elem.number;
            else
                return {};
        }

    private:
        template<std::meta::info CaptureFlagReflection>
        [[nodiscard]] constexpr bool get() const
        {
            for (const auto& elem : elems_ | std::views::reverse)
                if (elem.flags.[: CaptureFlagReflection :] != cf::inherit)
                    return elem.flags.[: CaptureFlagReflection :] == cf::enabled;
            return base_.flags.[: CaptureFlagReflection :] == cf::enabled;
        }

        template<std::meta::info CaptureFlagReflection>
        constexpr void set(bool value)
        {
            ((elems_.empty()) ? base_ : elems_.back()).flags.[: CaptureFlagReflection :] = (value) ? cf::enabled : cf::disabled;
        }

    public:
        [[nodiscard]] constexpr bool caseless()  const { return get<^^capture_flags::caseless>(); }
        [[nodiscard]] constexpr bool multiline() const { return get<^^capture_flags::multiline>(); }
        [[nodiscard]] constexpr bool dotall()    const { return get<^^capture_flags::dotall>(); }
        [[nodiscard]] constexpr bool ungreedy()  const { return get<^^capture_flags::ungreedy>(); }

        constexpr void set_caseless(bool value)  noexcept { return set<^^capture_flags::caseless>(value); }
        constexpr void set_multiline(bool value) noexcept { return set<^^capture_flags::multiline>(value); }
        constexpr void set_dotall(bool value)    noexcept { return set<^^capture_flags::dotall>(value); }
        constexpr void set_ungreedy(bool value)  noexcept { return set<^^capture_flags::ungreedy>(value); }

    private:
        [[nodiscard]] constexpr number_t next_number() const noexcept
        {
            auto& target = elems_.empty() ? base_ : elems_.back();
            return target.number_end;
        }

        std::vector<cse> elems_{};
        cse              base_{};
    };
}
