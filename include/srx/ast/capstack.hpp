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


namespace srx {
namespace detail {
namespace parser {

struct capture_flags
{
    enum class flag_value : unsigned char
    {
        inherit,
        disabled,
        enabled,
        enabled_more, /* only used for extended */
    };

    flag_value caseless  : 2 { flag_value::inherit };
    flag_value multiline : 2 { flag_value::inherit };
    flag_value noautocap : 2 { flag_value::inherit };
    flag_value dotall    : 2 { flag_value::inherit };
    flag_value ungreedy  : 2 { flag_value::inherit };
    flag_value extended  : 2 { flag_value::inherit };
};

enum class group_modes : unsigned char
{
    normal,
    comment,
    flag_assigning,
    non_capturing,
    branch_reset,
    atomic,
};

class capture_stack
{
    using number_t = std::uint_least16_t;
    using cf = capture_flags::flag_value;
    using gm = group_modes;

    struct cse
    {
        static constexpr capture_flags all_disabled{
            .caseless  = cf::disabled,
            .multiline = cf::disabled,
            .noautocap = cf::disabled,
            .dotall    = cf::disabled,
            .ungreedy  = cf::disabled,
            .extended  = cf::disabled,
        };

        constexpr cse() noexcept
            : number{ 1 } , number_end{ 1 } , flags{ all_disabled }, mode{ gm::non_capturing } {}

        constexpr explicit cse(number_t cur, number_t end, capture_flags f, group_modes m) noexcept
            : number{ cur } , number_end{ end } , flags{ f } , mode{ m } {}

        number_t number;
        number_t number_end;
        capture_flags flags;
        group_modes mode;
    };

public:
    [[nodiscard]] constexpr bool caseless()  const { return get<^^capture_flags::caseless>(); }
    [[nodiscard]] constexpr bool multiline() const { return get<^^capture_flags::multiline>(); }
    [[nodiscard]] constexpr bool dotall()    const { return get<^^capture_flags::dotall>(); }
    [[nodiscard]] constexpr bool ungreedy()  const { return get<^^capture_flags::ungreedy>(); }

    [[nodiscard]] constexpr bool push(capture_flags flags, group_modes mode)
    {
        /* return true if overflow and false otherwise */
        auto next_beg = next_number();
        auto next_end = next_beg;

        if (mode == gm::normal and ++next_end == 0)
            return true; /* unsigned integer overflow */

        elems_.emplace_back(next_beg, next_end, flags, mode);
        return false;
    }

    constexpr void push_non_capturing(capture_flags flags, group_modes mode)
    {
        auto next = next_number();
        elems_.emplace_back(next, next, flags, (mode == gm::normal) ? gm::non_capturing : mode);
    }

    constexpr void branch_reset_if_enabled() noexcept
    {
        if (elems_.empty())
            return;

        if (auto& elem = elems_.back(); elem.mode == gm::branch_reset)
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
        if (elem.mode == gm::flag_assigning)
            template for (constexpr auto e : define_static_array(nonstatic_data_members_of(^^capture_flags, std::meta::access_context::unchecked())))
                if (elem.flags.[: e // line break to avoid breaking syntax highlighting
                                :] != cf::inherit)
                    target.flags.[: e :] = elem.flags.[: e :];

        if (elem.mode == gm::branch_reset)
            target.number_end = std::max(target.number_end, elem.number_end);
        else
            target.number_end = elem.number_end;

        if (elem.mode == gm::normal)
            return elem.number;
        else
            return {};
    }

private:
    template<std::meta::info CaptureFlagReflection>
    [[nodiscard]] constexpr bool get() const
    {
        for (const auto& elem : elems_ | std::views::reverse)
            if (elem.flags.[: CaptureFlagReflection // line break to avoid breaking syntax highlighting
                            :] != cf::inherit)
                return elem.flags.[: CaptureFlagReflection :] == cf::enabled;
        return base_.flags.[: CaptureFlagReflection :] == cf::enabled;
    }

    [[nodiscard]] constexpr number_t next_number() const noexcept
    {
        auto& target = elems_.empty() ? base_ : elems_.back();
        return target.number_end;
    }

    std::vector<cse> elems_{};
    cse              base_{};
};

} // namespace parser
} // namespace detail
} // namespace srx
