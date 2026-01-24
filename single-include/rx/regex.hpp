// THIS FILE IS AUTOMATICALLY AMALGAMATED FROM "include/rx/regex.hpp"
// Copyright (C) 2026 Peter Wild


// Mozilla Public License Version 2.0
// ==================================
// 
// 1. Definitions
// --------------
// 
// 1.1. "Contributor"
//     means each individual or legal entity that creates, contributes to
//     the creation of, or owns Covered Software.
// 
// 1.2. "Contributor Version"
//     means the combination of the Contributions of others (if any) used
//     by a Contributor and that particular Contributor's Contribution.
// 
// 1.3. "Contribution"
//     means Covered Software of a particular Contributor.
// 
// 1.4. "Covered Software"
//     means Source Code Form to which the initial Contributor has attached
//     the notice in Exhibit A, the Executable Form of such Source Code
//     Form, and Modifications of such Source Code Form, in each case
//     including portions thereof.
// 
// 1.5. "Incompatible With Secondary Licenses"
//     means
// 
//     (a) that the initial Contributor has attached the notice described
//         in Exhibit B to the Covered Software; or
// 
//     (b) that the Covered Software was made available under the terms of
//         version 1.1 or earlier of the License, but not also under the
//         terms of a Secondary License.
// 
// 1.6. "Executable Form"
//     means any form of the work other than Source Code Form.
// 
// 1.7. "Larger Work"
//     means a work that combines Covered Software with other material, in
//     a separate file or files, that is not Covered Software.
// 
// 1.8. "License"
//     means this document.
// 
// 1.9. "Licensable"
//     means having the right to grant, to the maximum extent possible,
//     whether at the time of the initial grant or subsequently, any and
//     all of the rights conveyed by this License.
// 
// 1.10. "Modifications"
//     means any of the following:
// 
//     (a) any file in Source Code Form that results from an addition to,
//         deletion from, or modification of the contents of Covered
//         Software; or
// 
//     (b) any new file in Source Code Form that contains any Covered
//         Software.
// 
// 1.11. "Patent Claims" of a Contributor
//     means any patent claim(s), including without limitation, method,
//     process, and apparatus claims, in any patent Licensable by such
//     Contributor that would be infringed, but for the grant of the
//     License, by the making, using, selling, offering for sale, having
//     made, import, or transfer of either its Contributions or its
//     Contributor Version.
// 
// 1.12. "Secondary License"
//     means either the GNU General Public License, Version 2.0, the GNU
//     Lesser General Public License, Version 2.1, the GNU Affero General
//     Public License, Version 3.0, or any later versions of those
//     licenses.
// 
// 1.13. "Source Code Form"
//     means the form of the work preferred for making modifications.
// 
// 1.14. "You" (or "Your")
//     means an individual or a legal entity exercising rights under this
//     License. For legal entities, "You" includes any entity that
//     controls, is controlled by, or is under common control with You. For
//     purposes of this definition, "control" means (a) the power, direct
//     or indirect, to cause the direction or management of such entity,
//     whether by contract or otherwise, or (b) ownership of more than
//     fifty percent (50%) of the outstanding shares or beneficial
//     ownership of such entity.
// 
// 2. License Grants and Conditions
// --------------------------------
// 
// 2.1. Grants
// 
// Each Contributor hereby grants You a world-wide, royalty-free,
// non-exclusive license:
// 
// (a) under intellectual property rights (other than patent or trademark)
//     Licensable by such Contributor to use, reproduce, make available,
//     modify, display, perform, distribute, and otherwise exploit its
//     Contributions, either on an unmodified basis, with Modifications, or
//     as part of a Larger Work; and
// 
// (b) under Patent Claims of such Contributor to make, use, sell, offer
//     for sale, have made, import, and otherwise transfer either its
//     Contributions or its Contributor Version.
// 
// 2.2. Effective Date
// 
// The licenses granted in Section 2.1 with respect to any Contribution
// become effective for each Contribution on the date the Contributor first
// distributes such Contribution.
// 
// 2.3. Limitations on Grant Scope
// 
// The licenses granted in this Section 2 are the only rights granted under
// this License. No additional rights or licenses will be implied from the
// distribution or licensing of Covered Software under this License.
// Notwithstanding Section 2.1(b) above, no patent license is granted by a
// Contributor:
// 
// (a) for any code that a Contributor has removed from Covered Software;
//     or
// 
// (b) for infringements caused by: (i) Your and any other third party's
//     modifications of Covered Software, or (ii) the combination of its
//     Contributions with other software (except as part of its Contributor
//     Version); or
// 
// (c) under Patent Claims infringed by Covered Software in the absence of
//     its Contributions.
// 
// This License does not grant any rights in the trademarks, service marks,
// or logos of any Contributor (except as may be necessary to comply with
// the notice requirements in Section 3.4).
// 
// 2.4. Subsequent Licenses
// 
// No Contributor makes additional grants as a result of Your choice to
// distribute the Covered Software under a subsequent version of this
// License (see Section 10.2) or under the terms of a Secondary License (if
// permitted under the terms of Section 3.3).
// 
// 2.5. Representation
// 
// Each Contributor represents that the Contributor believes its
// Contributions are its original creation(s) or it has sufficient rights
// to grant the rights to its Contributions conveyed by this License.
// 
// 2.6. Fair Use
// 
// This License is not intended to limit any rights You have under
// applicable copyright doctrines of fair use, fair dealing, or other
// equivalents.
// 
// 2.7. Conditions
// 
// Sections 3.1, 3.2, 3.3, and 3.4 are conditions of the licenses granted
// in Section 2.1.
// 
// 3. Responsibilities
// -------------------
// 
// 3.1. Distribution of Source Form
// 
// All distribution of Covered Software in Source Code Form, including any
// Modifications that You create or to which You contribute, must be under
// the terms of this License. You must inform recipients that the Source
// Code Form of the Covered Software is governed by the terms of this
// License, and how they can obtain a copy of this License. You may not
// attempt to alter or restrict the recipients' rights in the Source Code
// Form.
// 
// 3.2. Distribution of Executable Form
// 
// If You distribute Covered Software in Executable Form then:
// 
// (a) such Covered Software must also be made available in Source Code
//     Form, as described in Section 3.1, and You must inform recipients of
//     the Executable Form how they can obtain a copy of such Source Code
//     Form by reasonable means in a timely manner, at a charge no more
//     than the cost of distribution to the recipient; and
// 
// (b) You may distribute such Executable Form under the terms of this
//     License, or sublicense it under different terms, provided that the
//     license for the Executable Form does not attempt to limit or alter
//     the recipients' rights in the Source Code Form under this License.
// 
// 3.3. Distribution of a Larger Work
// 
// You may create and distribute a Larger Work under terms of Your choice,
// provided that You also comply with the requirements of this License for
// the Covered Software. If the Larger Work is a combination of Covered
// Software with a work governed by one or more Secondary Licenses, and the
// Covered Software is not Incompatible With Secondary Licenses, this
// License permits You to additionally distribute such Covered Software
// under the terms of such Secondary License(s), so that the recipient of
// the Larger Work may, at their option, further distribute the Covered
// Software under the terms of either this License or such Secondary
// License(s).
// 
// 3.4. Notices
// 
// You may not remove or alter the substance of any license notices
// (including copyright notices, patent notices, disclaimers of warranty,
// or limitations of liability) contained within the Source Code Form of
// the Covered Software, except that You may alter any license notices to
// the extent required to remedy known factual inaccuracies.
// 
// 3.5. Application of Additional Terms
// 
// You may choose to offer, and to charge a fee for, warranty, support,
// indemnity or liability obligations to one or more recipients of Covered
// Software. However, You may do so only on Your own behalf, and not on
// behalf of any Contributor. You must make it absolutely clear that any
// such warranty, support, indemnity, or liability obligation is offered by
// You alone, and You hereby agree to indemnify every Contributor for any
// liability incurred by such Contributor as a result of warranty, support,
// indemnity or liability terms You offer. You may include additional
// disclaimers of warranty and limitations of liability specific to any
// jurisdiction.
// 
// 4. Inability to Comply Due to Statute or Regulation
// ---------------------------------------------------
// 
// If it is impossible for You to comply with any of the terms of this
// License with respect to some or all of the Covered Software due to
// statute, judicial order, or regulation then You must: (a) comply with
// the terms of this License to the maximum extent possible; and (b)
// describe the limitations and the code they affect. Such description must
// be placed in a text file included with all distributions of the Covered
// Software under this License. Except to the extent prohibited by statute
// or regulation, such description must be sufficiently detailed for a
// recipient of ordinary skill to be able to understand it.
// 
// 5. Termination
// --------------
// 
// 5.1. The rights granted under this License will terminate automatically
// if You fail to comply with any of its terms. However, if You become
// compliant, then the rights granted under this License from a particular
// Contributor are reinstated (a) provisionally, unless and until such
// Contributor explicitly and finally terminates Your grants, and (b) on an
// ongoing basis, if such Contributor fails to notify You of the
// non-compliance by some reasonable means prior to 60 days after You have
// come back into compliance. Moreover, Your grants from a particular
// Contributor are reinstated on an ongoing basis if such Contributor
// notifies You of the non-compliance by some reasonable means, this is the
// first time You have received notice of non-compliance with this License
// from such Contributor, and You become compliant prior to 30 days after
// Your receipt of the notice.
// 
// 5.2. If You initiate litigation against any entity by asserting a patent
// infringement claim (excluding declaratory judgment actions,
// counter-claims, and cross-claims) alleging that a Contributor Version
// directly or indirectly infringes any patent, then the rights granted to
// You by any and all Contributors for the Covered Software under Section
// 2.1 of this License shall terminate.
// 
// 5.3. In the event of termination under Sections 5.1 or 5.2 above, all
// end user license agreements (excluding distributors and resellers) which
// have been validly granted by You or Your distributors under this License
// prior to termination shall survive termination.
// 
// ************************************************************************
// *                                                                      *
// *  6. Disclaimer of Warranty                                           *
// *  -------------------------                                           *
// *                                                                      *
// *  Covered Software is provided under this License on an "as is"       *
// *  basis, without warranty of any kind, either expressed, implied, or  *
// *  statutory, including, without limitation, warranties that the       *
// *  Covered Software is free of defects, merchantable, fit for a        *
// *  particular purpose or non-infringing. The entire risk as to the     *
// *  quality and performance of the Covered Software is with You.        *
// *  Should any Covered Software prove defective in any respect, You     *
// *  (not any Contributor) assume the cost of any necessary servicing,   *
// *  repair, or correction. This disclaimer of warranty constitutes an   *
// *  essential part of this License. No use of any Covered Software is   *
// *  authorized under this License except under this disclaimer.         *
// *                                                                      *
// ************************************************************************
// 
// ************************************************************************
// *                                                                      *
// *  7. Limitation of Liability                                          *
// *  --------------------------                                          *
// *                                                                      *
// *  Under no circumstances and under no legal theory, whether tort      *
// *  (including negligence), contract, or otherwise, shall any           *
// *  Contributor, or anyone who distributes Covered Software as          *
// *  permitted above, be liable to You for any direct, indirect,         *
// *  special, incidental, or consequential damages of any character      *
// *  including, without limitation, damages for lost profits, loss of    *
// *  goodwill, work stoppage, computer failure or malfunction, or any    *
// *  and all other commercial damages or losses, even if such party      *
// *  shall have been informed of the possibility of such damages. This   *
// *  limitation of liability shall not apply to liability for death or   *
// *  personal injury resulting from such party's negligence to the       *
// *  extent applicable law prohibits such limitation. Some               *
// *  jurisdictions do not allow the exclusion or limitation of           *
// *  incidental or consequential damages, so this exclusion and          *
// *  limitation may not apply to You.                                    *
// *                                                                      *
// ************************************************************************
// 
// 8. Litigation
// -------------
// 
// Any litigation relating to this License may be brought only in the
// courts of a jurisdiction where the defendant maintains its principal
// place of business and such litigation shall be governed by laws of that
// jurisdiction, without reference to its conflict-of-law provisions.
// Nothing in this Section shall prevent a party's ability to bring
// cross-claims or counter-claims.
// 
// 9. Miscellaneous
// ----------------
// 
// This License represents the complete agreement concerning the subject
// matter hereof. If any provision of this License is held to be
// unenforceable, such provision shall be reformed only to the extent
// necessary to make it enforceable. Any law or regulation which provides
// that the language of a contract shall be construed against the drafter
// shall not be used to construe this License against a Contributor.
// 
// 10. Versions of the License
// ---------------------------
// 
// 10.1. New Versions
// 
// Mozilla Foundation is the license steward. Except as provided in Section
// 10.3, no one other than the license steward has the right to modify or
// publish new versions of this License. Each version will be given a
// distinguishing version number.
// 
// 10.2. Effect of New Versions
// 
// You may distribute the Covered Software under the terms of the version
// of the License under which You originally received the Covered Software,
// or under the terms of any subsequent version published by the license
// steward.
// 
// 10.3. Modified Versions
// 
// If you create software not governed by this License, and you want to
// create a new license for such software, you may create and use a
// modified version of this License if you rename the license and remove
// any references to the name of the license steward (except to note that
// such modified license differs from this License).
// 
// 10.4. Distributing Source Code Form that is Incompatible With Secondary
// Licenses
// 
// If You choose to distribute Source Code Form that is Incompatible With
// Secondary Licenses under the terms of this version of the License, the
// notice described in Exhibit B of this License must be attached.
// 
// Exhibit A - Source Code Form License Notice
// -------------------------------------------
// 
//   This Source Code Form is subject to the terms of the Mozilla Public
//   License, v. 2.0. If a copy of the MPL was not distributed with this
//   file, You can obtain one at https://mozilla.org/MPL/2.0/.
// 
// If it is not possible or desirable to put the notice in a particular
// file, then You may include the notice in a location (such as a LICENSE
// file in a relevant directory) where a recipient would be likely to look
// for such a notice.
// 
// You may add additional accurate notices of copyright ownership.
// 
// Exhibit B - "Incompatible With Secondary Licenses" Notice
// ---------------------------------------------------------
// 
//   This Source Code Form is "Incompatible With Secondary Licenses", as
//   defined by the Mozilla Public License, v. 2.0.


#pragma once

#include <algorithm>
#include <array>
#include <bit>
#include <concepts>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <flat_map>
#include <format>
#include <functional>
#include <iosfwd>
#include <iterator>
#include <limits>
#include <meta>
#include <numeric>
#include <optional>
#include <ranges>
#include <span>
#include <stdexcept>
#include <string>
#include <string_view>
#include <tuple>
#include <type_traits>
#include <utility>
#include <variant>
#include <vector>


namespace rx::detail
{
    template<class... Ts>
    struct overloads : Ts... { using Ts::operator()...; };

    template<typename T, typename... Ts>
    concept one_of = (std::same_as<T, Ts> or ...);

    template<typename CharT>
    concept character = one_of<CharT, char, wchar_t, char8_t, char16_t, char32_t>;

    consteval bool is_template_instantiation_of_impl(std::meta::info type, std::meta::info templ)
    {
        if (not std::meta::is_type(type) or not std::meta::is_class_template(templ))
            return false;

        auto dealiased{ std::meta::dealias(type) };

        if (not std::meta::has_template_arguments(dealiased))
            return false;

        return std::meta::template_of(dealiased) == templ;
    }

    template<typename T, std::meta::info Template>
    concept template_instantiation_of = is_template_instantiation_of_impl(^^T, Template);

    template<typename, typename>
    struct type_in_variant_impl {};

    template<typename T, typename... Ts>
    struct type_in_variant_impl<T, std::variant<Ts...>> : std::bool_constant<one_of<T, Ts...>> {};

    template<typename T, typename Variant>
    concept in_variant = type_in_variant_impl<T, Variant>::value;

    template<typename CharT>
    concept char_is_utf8 = std::same_as<CharT, char8_t>; /* Assume a regular char is ascii (or some single byte superset of) */

    template<typename CharT>
    concept char_is_utf16 = std::same_as<CharT, char16_t>
                            or (std::same_as<CharT, wchar_t> and sizeof(CharT) == sizeof(char16_t)); /* Assume a 16-bit wchar_t is encoded in utf16 */

    template<typename CharT>
    concept char_is_utf32 = std::same_as<CharT, char32_t>
                            or (std::same_as<CharT, wchar_t> and sizeof(CharT) == sizeof(char32_t)); /* Assume a 32-bit wchar_t is encoded in utf32 */

    template<typename CharT>
    concept char_is_multibyte = char_is_utf8<CharT> or char_is_utf16<CharT>;

    template<typename V, typename T, std::size_t I = 0>
    struct index_of_impl : std::conditional_t<std::same_as<T, std::variant_alternative_t<I, V>>,
                                              std::integral_constant<std::size_t, I>,
                                              index_of_impl<V, T, I + 1>> {};

    template<typename V, typename T, std::size_t I>
    requires (I >= std::variant_size_v<V>)
    struct index_of_impl<V, T, I> {};

    template<typename Derived>
    struct flag_base
    {
        friend constexpr Derived operator|(Derived x, Derived y)
        {
            Derived result{};
            template for (constexpr std::meta::info member : std::define_static_array(std::meta::nonstatic_data_members_of(std::meta::dealias(^^Derived), std::meta::access_context::current())))
            {
                result.[:member:] = x.[:member:] or y.[:member:];
            }
            return result;
        }
    };

    inline constexpr std::size_t no_tag{ std::numeric_limits<std::size_t>::max() };

    template<bool Const, typename T>
    using maybe_const_t = std::conditional_t<Const, const T, T>;

    struct cstr_sentinel_t
    {
        template<character CharT>
        friend constexpr bool operator==(const CharT* c, cstr_sentinel_t)
        {
            return *c == CharT{};
        }
    };

    inline constexpr cstr_sentinel_t cstr_sentinel;

    struct terminal_object
    {
        template<typename... Ts>
        constexpr explicit terminal_object(Ts&&...) {}
    };

    template<bool Enabled, typename T>
    using maybe_type_t = std::conditional_t<Enabled, T, terminal_object>;
}

namespace rx
{
    namespace detail
    {
        template<std::bidirectional_iterator Iter>
        class submatch_factory;
    }

    template<std::bidirectional_iterator I>
    class submatch
    {
    public:
        using iterator               = I;
        using reverse_iterator       = std::reverse_iterator<iterator>;
#if __cpp_lib_ranges_as_const >= 202311L
        using const_iterator         = std::const_iterator<iterator>;
        using const_reverse_iterator = std::reverse_iterator<const_iterator>;
#endif
        using value_type             = std::iter_value_t<iterator>;
        using size_type              = std::size_t;
        using string_type            = std::basic_string<value_type>;
        using view_type              = std::basic_string_view<value_type>;

        constexpr submatch() = default;

        /* observers */

        [[nodiscard]] constexpr bool matched() const noexcept
        {
            if constexpr (std::contiguous_iterator<I>)
                return std::to_address(first_) != std::to_address(I{});
            else
                return match_;
        }

        [[nodiscard]] constexpr explicit(false) operator bool() const noexcept
        {
            return this->matched();
        }

        [[nodiscard]] constexpr bool empty() const noexcept
        {
            return first_ == last_;
        }

        [[nodiscard]] constexpr size_type size() const
        {
            return std::saturate_cast<size_type>(std::ranges::distance(first_, last_));
        }

        [[nodiscard]] constexpr size_type length() const
        {
            return this->size();
        }

        /* iterators */

        [[nodiscard]] constexpr iterator begin() const noexcept
        {
            return first_;
        }

        [[nodiscard]] constexpr iterator end() const noexcept
        {
            return last_;
        }

        [[nodiscard]] constexpr reverse_iterator rbegin() const noexcept
        {
            return std::make_reverse_iterator(this->end());
        }

        [[nodiscard]] constexpr reverse_iterator rend() const noexcept
        {
            return std::make_reverse_iterator(this->begin());
        }

#if __cpp_lib_ranges_as_const >= 202311L
        [[nodiscard]] constexpr const_iterator cbegin() const noexcept
        requires std::bidirectional_iterator<const_iterator>
        {
            return std::make_const_iterator(this->begin());
        }

        [[nodiscard]] constexpr const_iterator cend() const noexcept
        requires std::bidirectional_iterator<const_iterator>
        {
            return std::make_const_iterator(this->end());
        }

        [[nodiscard]] constexpr const_reverse_iterator crbegin() const noexcept
        requires std::bidirectional_iterator<const_iterator>
        {
            return std::make_reverse_iterator(this->cend());
        }

        [[nodiscard]] constexpr const_reverse_iterator crend() const noexcept
        requires std::bidirectional_iterator<const_iterator>
        {
            return std::make_reverse_iterator(this->cbegin());
        }
#endif

        /* structured binding support */

        template<std::size_t N>
        requires (N < 2)
        [[nodiscard]] constexpr const auto& get() const &
        {
            if constexpr (N == 0)
                return first_;
            if constexpr (N == 1)
                return last_;
        }

        template<std::size_t N>
        requires (N < 2)
        [[nodiscard]] constexpr auto&& get() &&
        {
            if constexpr (N == 0)
                return std::move(first_);
            if constexpr (N == 1)
                return std::move(last_);
        }

        /* conversion */

        [[nodiscard]] constexpr string_type str() const
        {
            return { first_, last_ };
        }

        [[nodiscard]] constexpr view_type view() const
        requires std::contiguous_iterator<I>
        {
            return { first_, last_ };
        }

        [[nodiscard]] constexpr explicit(false) operator string_type() const
        {
            return this->str();
        }

        [[nodiscard]] constexpr explicit(false) operator view_type() const
        requires std::contiguous_iterator<I>
        {
            return this->view();
        }

#if __cpp_lib_ranges_as_const >= 202311L
        [[nodiscard]] constexpr explicit(false) operator submatch<const_iterator>() const &
        requires (not std::same_as<const_iterator, iterator>)
        {
            return { first_, last_ };
        }

        [[nodiscard]] constexpr explicit(false) operator submatch<const_iterator>() &&
        requires (not std::same_as<const_iterator, iterator>)
        {
            return { std::move(first_), std::move(last_) };
        }
#endif

        /* operators */

        [[nodiscard]] friend constexpr bool operator==(const submatch& lhs, const submatch& rhs)
        {
            return (lhs and rhs)
                   ? std::ranges::equal(lhs, rhs)
                   : lhs.matched() == rhs.matched();
        }

        [[nodiscard]] friend constexpr bool operator==(const submatch& sub, const view_type view)
        {
            return sub
                   ? std::ranges::equal(sub, view)
                   : false;
        }

        [[nodiscard]] friend constexpr auto operator<=>(const submatch& lhs, const submatch& rhs)
        {
            return (lhs and rhs)
                   ? std::lexicographical_compare_three_way(lhs.begin(), lhs.end(), rhs.begin(), rhs.end())
                   : lhs.matched() <=> rhs.matched();
        }

        [[nodiscard]] friend constexpr auto operator<=>(const submatch& sub, const view_type view)
        {
            return sub
                   ? std::lexicographical_compare_three_way(sub.begin(), sub.end(), view.begin(), view.end())
                   : std::strong_ordering::less;
        }

        template<typename CharT, typename Traits>
        [[nodiscard]] friend std::basic_ostream<CharT, Traits> operator<<(std::basic_ostream<CharT, Traits>& os, const submatch& sub)
        {
            if constexpr (std::contiguous_iterator<I>)
                return os << sub.view();
            else
                return os << sub.str();
        }

        /* misc */

        void swap(submatch& other) noexcept(std::is_nothrow_swappable_v<I>)
        {
            std::swap(first_, other.first_);
            std::swap(last_, other.last_);
        }

    private:
        static constexpr bool use_bool{ not std::contiguous_iterator<I> };  
        using maybe_bool = detail::maybe_type_t<use_bool, bool>;

        constexpr submatch(I first, I last)
            : first_{ std::move(first) }, last_{ std::move(last) }, match_{ true } {}

        friend class detail::submatch_factory<I>;

        I first_{};
        I last_{};

        [[no_unique_address]] maybe_bool match_{ false };
    };
}

/* structured binding support for submatch */

template<std::bidirectional_iterator I>
struct std::tuple_size<rx::submatch<I>> : integral_constant<std::size_t, 2> {};

template<std::size_t N, std::bidirectional_iterator I>
requires (N < 2)
struct std::tuple_element<N, rx::submatch<I>>
{
    using type = rx::submatch<I>::iterator;
};

/* formatting support for submatch */

template<std::bidirectional_iterator I>
inline constexpr auto std::format_kind<rx::submatch<I>> = std::range_format::string;

static_assert(std::formattable<rx::submatch<const char*>, char>);

/* submatch factory implementation */

namespace rx::detail
{
    template<std::bidirectional_iterator I>
    class submatch_factory
    {
    public:
        [[nodiscard]] static constexpr submatch<I> make_submatch(I first, I last)
        {
            return { first, last };
        }
    };
}

namespace rx
{
    template<typename CharT, std::size_t N>
    struct string_literal
    {
        static_assert(N != 0);
        using char_type = CharT;

        consteval string_literal(const char_type (&str)[N])
        {
            std::ranges::copy_n(str, N, value_);
        }

        [[nodiscard]] constexpr std::basic_string_view<char_type> view() const
        {
            return { value_, N - 1 };
        }

        char_type value_[N]{};
    };
}

namespace rx::detail
{
    struct fsm_flags : flag_base<fsm_flags>
    {
        bool is_search       : 1;
        bool longest_match   : 1;
        bool enable_fallback : 1;
        bool is_iterator     : 1;
        bool no_captures     : 1;
        bool return_bool     : 1;
    };

    namespace default_fsm_flags
    {
        inline constexpr fsm_flags full_match{
            .is_search       = false,
            .longest_match   = true,
            .enable_fallback = false,
            .is_iterator     = false,
            .no_captures     = false,
            .return_bool     = false,
        };

        inline constexpr fsm_flags partial_match{
            .is_search       = false,
            .longest_match   = false,
            .enable_fallback = true,
            .is_iterator     = false,
            .no_captures     = false,
            .return_bool     = false,
        };

        inline constexpr fsm_flags search_single{
            .is_search       = true,
            .longest_match   = false,
            .enable_fallback = true,
            .is_iterator     = false,
            .no_captures     = false,
            .return_bool     = false,
        };

        inline constexpr fsm_flags search_all{
            .is_search       = true,
            .longest_match   = false,
            .enable_fallback = true,
            .is_iterator     = true,
            .no_captures     = false,
            .return_bool     = false,
        };

        inline constexpr fsm_flags return_bool_modifier{
            .is_search       = false,
            .longest_match   = false,
            .enable_fallback = false,
            .is_iterator     = false,
            .no_captures     = true,
            .return_bool     = true,
        };

        inline constexpr fsm_flags no_capture_modifier{
            .is_search       = false,
            .longest_match   = false,
            .enable_fallback = false,
            .is_iterator     = false,
            .no_captures     = true,
            .return_bool     = false,
        };

    }
}

namespace rx::detail
{
    using tag_number_t = int; /* note: tags must be non-negative */
    inline constexpr tag_number_t start_of_input_tag{ std::numeric_limits<tag_number_t>::min() };
    inline constexpr tag_number_t end_of_input_tag{ std::numeric_limits<tag_number_t>::min() + 1 };

    class capture_info
    {
    public:
        using capture_number_t = std::uint_least16_t;

        struct pair_entry
        {
            tag_number_t tag_number;
            int          offset;     /* note: offset should normally be negative */
        };

        using tag_pair_t     = std::pair<pair_entry, pair_entry>;
        using tag_remapper_t = std::flat_map<tag_number_t, tag_number_t>;

        using key_type   = capture_number_t;
        using value_type = tag_pair_t;

        constexpr capture_info()
        {
            keys_.emplace_back(0);
            values_.emplace_back(pair_entry{ .tag_number = start_of_input_tag, .offset = 0 }, pair_entry{ .tag_number = end_of_input_tag, .offset = 0 });
        }

        constexpr void insert(capture_number_t cap, tag_number_t lhs, tag_number_t rhs)
        {
            if (cap == 0)
                throw std::invalid_argument("Cannot insert capture with number 0");

            const auto key_it{ std::ranges::upper_bound(keys_, cap) };
            const auto value_it{ std::ranges::begin(values_) + std::distance(std::ranges::begin(keys_), key_it) };

            keys_.emplace(key_it, cap);
            values_.emplace(value_it, pair_entry{ .tag_number = lhs, .offset = 0 }, pair_entry{ .tag_number = rhs, .offset = 0 });
        }

        constexpr void set_match_start_tag(tag_number_t value)
        {
            values_.at(0).first.tag_number = value;
        }

        [[nodiscard]] constexpr capture_number_t capture_count() const
        {
            auto key_copy{ keys_ };
            auto [last, _]{ std::ranges::unique(key_copy) };
            return std::saturate_cast<capture_number_t>(std::ranges::distance(std::ranges::begin(key_copy), last));
        }

        [[nodiscard]] constexpr std::pair<bool, bool> capture_side(tag_number_t tag) const
        {
            static constexpr auto compose = [](const auto& g, const auto& f) {
                return [=]<typename T>(T&& arg) {
                    return std::invoke(g, std::invoke(f, std::forward<T>(arg)));
                };
            };

            /* for some reason, this doesn't seem to work? */
            return {
                std::ranges::contains(values_, tag, compose(&pair_entry::tag_number, &tag_pair_t::first)),
                std::ranges::contains(values_, tag, compose(&pair_entry::tag_number, &tag_pair_t::second))
            };
        }

        [[nodiscard]] constexpr auto lookup(capture_number_t cap) const
        {
            auto [key_beg, key_end]{ std::ranges::equal_range(keys_, cap) };

            return std::ranges::subrange{
                std::ranges::begin(values_) + std::distance(std::ranges::begin(keys_), key_beg),
                std::ranges::begin(values_) + std::distance(std::ranges::begin(keys_), key_end)
            };
        }

        [[nodiscard]] constexpr tag_remapper_t remap_tags(const std::flat_map<tag_number_t, pair_entry>& map)
        {
            static constexpr auto compose = [](const auto& g, const auto& f) {
                return [=]<typename T>(T&& arg) {
                    return std::invoke(g, std::invoke(f, std::forward<T>(arg)));
                };
            };

            for (auto& val : values_)
            {
                if (auto it{ map.find(val.first.tag_number) }; it != map.end())
                {
                    val.first.offset += it->second.offset;
                    val.first.tag_number = it->second.tag_number;
                }

                if (auto it{ map.find(val.second.tag_number) }; it != map.end())
                {
                    val.second.offset += it->second.offset;
                    val.second.tag_number = it->second.tag_number;
                }
            }

            tag_remapper_t remapper;

            std::vector<tag_number_t> set;
            set.append_range(values_ | std::views::transform(compose(&pair_entry::tag_number, &tag_pair_t::first)));
            set.append_range(values_ | std::views::transform(compose(&pair_entry::tag_number, &tag_pair_t::second)));
            std::ranges::sort(set);
            auto [tmp_beg, tmp_end]{ std::ranges::unique(set) };
            set.erase(tmp_beg, tmp_end);
            std::erase_if(set, [](tag_number_t n) { return n < 0; });

            for (tag_number_t i{ 0 }; std::cmp_less(i, set.size()); ++i)
                remapper[set.at(i)] = i;

            for (auto& val : values_)
            {
                if (auto it{ remapper.find(val.first.tag_number) }; it != remapper.end())
                    val.first.tag_number = it->second;

                if (auto it{ remapper.find(val.second.tag_number) }; it != remapper.end())
                    val.second.tag_number = it->second;
            }

            return remapper;
        }

    private:
        std::vector<capture_number_t> keys_;
        std::vector<tag_pair_t> values_;
    };
}

namespace rx
{
    class pattern_error : public std::runtime_error
    {
    public:
        using runtime_error::runtime_error;
    };

    class parser_error : public std::runtime_error
    {
    public:
        using runtime_error::runtime_error;
    };

    class tree_error : public std::runtime_error
    {
    public:
        using runtime_error::runtime_error;
    };

    class tnfa_error : public std::runtime_error
    {
    public:
        using runtime_error::runtime_error;
    };
}

namespace rx::detail
{
    template<typename CharT>
    class bitcharset
    {
        using integer_type = std::uint64_t;
        static constexpr std::size_t byte_bits{ std::numeric_limits<unsigned char>::digits };
        static constexpr std::size_t integer_bits{ std::numeric_limits<integer_type>::digits };
        static constexpr std::size_t total_size{ (0b1uz << (sizeof(CharT) * byte_bits)) };
        static constexpr std::size_t array_size{ total_size / integer_bits };
        static constexpr std::size_t min_offset{ (std::is_signed_v<CharT>) ? (array_size / 2) : 0z };

        static constexpr std::size_t acceptable_numbers_of_bits_in_a_byte{ 8 };
        static_assert(byte_bits == acceptable_numbers_of_bits_in_a_byte);
        static_assert(sizeof(CharT) < sizeof(int));

    public:
        using char_type = CharT;
        using char_interval = std::pair<char_type, char_type>;

        consteval bitcharset() noexcept = default;

        template<typename... Args>
        requires (sizeof...(Args) >= 1) and ((std::convertible_to<Args, char_type> or std::convertible_to<Args, char_interval>) and ...)
        constexpr explicit bitcharset(Args... args)
        {
            template for (constexpr std::size_t i : std::views::iota(0uz, sizeof...(Args)))
            {
                if constexpr (std::convertible_to<Args...[i], char_type>)
                    insert(args...[i]);
                else if constexpr (std::convertible_to<Args...[i], char_interval>)
                    insert(args...[i].first, args...[i].second);
            }
        }

        /* observers */

        [[nodiscard]] constexpr bool empty() const noexcept
        {
            integer_type result{ 0uz };
            for (std::size_t i{ 0 }; i < array_size; ++i)
                result |= data_[i];
            return (result == 0);
        }

        [[nodiscard]] constexpr std::size_t count() const noexcept
        {
            std::size_t result{ 0 };
            for (std::size_t i{ 0 }; i < array_size; ++i)
                result += std::popcount(data_[i]);
            return result;
        }

        [[nodiscard]] constexpr bool contains(char_type c) const noexcept
        {
            /* widen to accommodate signed chars */
            const int input{ static_cast<std::make_unsigned_t<char_type>>(c) };

            return data_[input / integer_bits] & (0b1uz << (input % integer_bits));
        }

        [[nodiscard]] constexpr std::vector<char_interval> get_intervals() const
        {
            static constexpr auto offset_max{ static_cast<int>(integer_bits) };

            std::vector<char_interval> result;

            /* widen to allow position to equal std::numeric_limits<char_type>::max() + 1 */
            int position{ std::numeric_limits<char_type>::min() };

            for (std::size_t i{ 0 }; i < array_size; ++i)
            {
                integer_type tmp{ data_[(i + min_offset) % array_size] };
                int offset{ 0 };

                while (true)
                {
                    const int zeros{ std::countr_zero(tmp) };

                    position += std::min(zeros, offset_max - offset);
                    offset += zeros;

                    if (offset >= offset_max)
                        break;

                    tmp >>= zeros;

                    const int ones{ std::countr_one(tmp) }; /* note: ones >= 1 is always true */
                    const auto prev_pos{ position };

                    position += ones;
                    offset += ones;

                    if (not result.empty() and result.back().second == static_cast<char_type>(prev_pos - 1))
                        result.back().second = static_cast<char_type>(position - 1);
                    else
                        result.emplace_back(prev_pos, static_cast<char_type>(position - 1));

                    if (offset >= offset_max)
                        break;

                    tmp >>= ones;
                }
            }

            return result;
        }

        /* modifiers */

        constexpr void clear() noexcept
        {
            for (std::size_t i{ 0 }; i < array_size; ++i)
                data_[i] = 0;
        }

        constexpr void negate() noexcept
        {
            for (std::size_t i{ 0 }; i < array_size; ++i)
                data_[i] = ~data_[i];
        }

        constexpr void insert(char_type c) noexcept
        {
            /* widen to accommodate signed chars */
            const int input{ static_cast<std::make_unsigned_t<char_type>>(c) };

            data_[input / integer_bits] |= 0b1uz << (input % integer_bits);
        }

        constexpr void insert(char_type first, char_type last) noexcept /* preconditions: first <= last */
        {
            /* widen to accommodate signed chars and to ensure last + 1 > last */
            const int beg{ static_cast<int>(first) - std::numeric_limits<char_type>::min() };
            const int end{ static_cast<int>(last) - std::numeric_limits<char_type>::min() + 1 };

            const std::size_t select1{ (beg / integer_bits) };
            const std::size_t select2{ (end / integer_bits) };
            const integer_type mask1{ (0b1uz << (beg % integer_bits)) - 1 };
            const integer_type mask2{ (0b1uz << (end % integer_bits)) - 1 };

            for (std::size_t i{ 0 }; i < array_size; ++i)
            {
                data_[(i + min_offset) % array_size] |= (((i == select1) * mask1) | ((i < select1) * ~0uz))
                                                        ^ (((i == select2) * mask2) | ((i < select2) * ~0uz));
            }
        }

        /* operators */

        constexpr bitcharset& operator&=(const bitcharset& other) noexcept
        {
            for (std::size_t i{ 0 }; i < array_size; ++i)
                data_[i] &= other.data_[i];
            return *this;
        }

        constexpr bitcharset& operator|=(const bitcharset& other) noexcept
        {
            for (std::size_t i{ 0 }; i < array_size; ++i)
                data_[i] |= other.data_[i];
            return *this;
        }

        constexpr bitcharset& operator^=(const bitcharset& other) noexcept
        {
            for (std::size_t i{ 0 }; i < array_size; ++i)
                data_[i] ^= other.data_[i];
            return *this;
        }

        constexpr bitcharset& operator-=(const bitcharset& other) noexcept
        {
            for (std::size_t i{ 0 }; i < array_size; ++i)
                data_[i] &= ~other.data_[i];
            return *this;
        }

        friend constexpr bitcharset operator&(const bitcharset& lhs, const bitcharset& rhs) noexcept
        {
            bitcharset result;
            for (std::size_t i{ 0 }; i < array_size; ++i)
                result.data_[i] = lhs.data_[i] & rhs.data_[i];
            return result;
        }

        friend constexpr bitcharset operator|(const bitcharset& lhs, const bitcharset& rhs) noexcept
        {
            bitcharset result;
            for (std::size_t i{ 0 }; i < array_size; ++i)
                result.data_[i] = lhs.data_[i] | rhs.data_[i];
            return result;
        }

        friend constexpr bitcharset operator^(const bitcharset& lhs, const bitcharset& rhs) noexcept
        {
            bitcharset result;
            for (std::size_t i{ 0 }; i < array_size; ++i)
                result.data_[i] = lhs.data_[i] ^ rhs.data_[i];
            return result;
        }

        friend constexpr bitcharset operator-(const bitcharset& lhs, const bitcharset& rhs) noexcept
        {
            bitcharset result;
            for (std::size_t i{ 0 }; i < array_size; ++i)
                result.data_[i] = lhs.data_[i] & ~rhs.data_[i];
            return result;
        }

        friend constexpr bitcharset operator~(const bitcharset& arg) noexcept
        {
            bitcharset result;
            for (std::size_t i{ 0 }; i < array_size; ++i)
                result.data_[i] = ~arg.data_[i];
            return result;
        }

        friend constexpr bool operator==(const bitcharset&, const bitcharset&) noexcept = default;
        friend constexpr auto operator<=>(const bitcharset&, const bitcharset&) noexcept = default;

        /* additional member functions */

        constexpr void make_ascii_case_insensitive() noexcept requires std::same_as<char_type, char>
        {
            static constexpr int uppercase_beg{ static_cast<std::make_unsigned_t<char_type>>('a') };
            static constexpr int uppercase_end{ static_cast<int>(static_cast<std::make_unsigned_t<char_type>>('z')) + 1 };
            static constexpr int lowercase_beg{ static_cast<std::make_unsigned_t<char_type>>('A') };
            static constexpr int lowercase_end{ static_cast<int>(static_cast<std::make_unsigned_t<char_type>>('Z')) + 1 };

            /* ensure that A-Za-z lies in the same integer_t in bitcharset<char> */
            static constexpr auto index{ uppercase_beg / integer_bits };
            static_assert(uppercase_beg / integer_bits == index);
            static_assert(uppercase_end / integer_bits == index);
            static_assert(lowercase_beg / integer_bits == index);
            static_assert(lowercase_end / integer_bits == index);

            static constexpr integer_type uppercase_mask{ ((0b1uz << (uppercase_beg % integer_bits)) - 1) ^ ((0b1uz << (uppercase_end % integer_bits)) - 1) };
            static constexpr integer_type lowercase_mask{ ((0b1uz << (lowercase_beg % integer_bits)) - 1) ^ ((0b1uz << (lowercase_end % integer_bits)) - 1) };
            static constexpr int uppercase_offset{ std::countr_zero(uppercase_mask) };
            static constexpr int lowercase_offset{ std::countr_zero(lowercase_mask) };

            const integer_type mask1{ (data_[index] & lowercase_mask) >> lowercase_offset };
            const integer_type mask2{ (data_[index] & uppercase_mask) >> uppercase_offset };

            data_[index] |= (mask1 << uppercase_offset);
            data_[index] |= (mask2 << lowercase_offset);
        }

        /* partition functions and type aliases */

        using ref = std::reference_wrapper<const bitcharset>;

        template<typename T>
        using ref_pair = std::pair<ref, T>;

        using partition_result = std::vector<bitcharset>;

        template<typename T>
        using partition_pair_result = std::vector<std::pair<bitcharset, std::vector<T>>>;

        template<typename T>
        using partition_contents_result = std::vector<std::vector<T>>;

        [[nodiscard]] static constexpr auto partition(const std::vector<ref>& input) -> partition_result;

        template<typename T>
        [[nodiscard]] static constexpr auto partition_ext(const std::vector<ref_pair<T>>& input) -> partition_pair_result<T>;

        template<typename T>
        [[nodiscard]] static constexpr auto partition_contents(const std::vector<ref_pair<T>>& input) -> partition_contents_result<T>;

    private:
        std::array<integer_type, array_size> data_{};
    };

    template<typename CharT>
    constexpr auto bitcharset<CharT>::partition(const std::vector<ref>& input) -> partition_result
    {
        if (input.empty())
            return {};
        else if (input.size() == 1)
            return { input.back().get() };

        /* note: partitions.size() >= 1 is always true, since for each iteration we insert
         *       at least one element into next_gen per element in partitions, since at
         *       most one of partitions[i] & val and partitions[i] & ~val will be empty */
        std::vector<bitcharset> partitions(1);
        partitions.back().negate();

        for (const bitcharset& val : input)
        {
            std::vector<bitcharset> next_gen;
            next_gen.reserve(partitions.size() * 2);

            for (std::size_t i{ 0 }; i < partitions.size(); ++i)
                if (auto cs{ partitions[i] & val }; not cs.empty())
                    next_gen.emplace_back(cs);

            const auto complement{ ~val };

            for (std::size_t i{ 0 }; i + 1 < partitions.size(); ++i)
                if (auto cs{ partitions[i] & complement }; not cs.empty())
                    next_gen.emplace_back(cs);

            /* always insert intersection of all complements as last element */
            next_gen.emplace_back(partitions.back() & complement);

            partitions = std::move(next_gen);
        }

        /* remove last element, which corresponds to intersection of all complements */
        partitions.pop_back();

        std::ranges::reverse(partitions); 

        return partitions;
    }

    template<typename CharT>
    template<typename T>
    constexpr auto bitcharset<CharT>::partition_ext(const std::vector<ref_pair<T>>& input) -> partition_pair_result<T>
    {
        using part_pair = std::pair<bitcharset, std::vector<bool>>;

        if (input.empty())
            return {};
        else if (input.size() == 1)
            return { { input.back().first, { input.back().second } } };

        std::vector<part_pair> partitions(1);
        partitions.back().first.negate();

        for (const auto& [val, _] : input)
        {
            std::vector<part_pair> next_gen;
            next_gen.reserve(partitions.size() * 2);

            const auto complement{ ~val.get() };

            for (const auto& [v, from] : partitions)
            {
                if (auto cs{ v & complement }; not cs.empty())
                {
                    next_gen.emplace_back(cs, from);
                    next_gen.back().second.emplace_back(false);
                }
            }

            for (const auto& [v, from] : partitions)
            {
                if (auto cs{ v & val.get() }; not cs.empty())
                {
                    next_gen.emplace_back(cs, from);
                    next_gen.back().second.emplace_back(true);
                }
            }

            partitions = std::move(next_gen);
        }

        partition_pair_result<T> result;
        const std::vector<bool> empty(input.size(), false);

        for (const auto& [v, from] : partitions)
        {
            if (from == empty)
                continue;

            result.emplace_back(v, std::vector<T>{});
            for (std::size_t i{ 0 }; i < input.size(); ++i)
                if (from.at(i))
                    result.back().second.emplace_back(input[i].second);
        }

        return result;
    }

    template<typename CharT>
    template<typename T>
    constexpr auto bitcharset<CharT>::partition_contents(const std::vector<ref_pair<T>>& input) -> partition_contents_result<T>
    {
        using part_pair = std::pair<bitcharset, std::vector<bool>>;

        if (input.empty())
            return {};
        else if (input.size() == 1)
            return { std::vector<T>{ input.back().second } };

        std::vector<part_pair> partitions(1);
        partitions.back().first.negate();

        for (const auto& [val, _] : input)
        {
            std::vector<part_pair> next_gen;
            next_gen.reserve(partitions.size() * 2);

            const auto complement{ ~val.get() };

            for (const auto& [v, from] : partitions)
            {
                if (auto cs{ v & complement }; not cs.empty())
                {
                    next_gen.emplace_back(cs, from);
                    next_gen.back().second.emplace_back(false);
                }
            }

            for (const auto& [v, from] : partitions)
            {
                if (auto cs{ v & val.get() }; not cs.empty())
                {
                    next_gen.emplace_back(cs, from);
                    next_gen.back().second.emplace_back(true);
                }
            }

            partitions = std::move(next_gen);
        }

        std::vector<std::vector<T>> result;
        const std::vector<bool> empty(input.size(), false);

        for (const auto& [v, from] : partitions)
        {
            if (from == empty)
                continue;

            result.emplace_back();
            for (std::size_t i{ 0 }; i < input.size(); ++i)
                if (from.at(i))
                    result.back().emplace_back(input[i].second);
        }

        return result;
    }
}

namespace rx::detail
{
    template<typename T, typename CharT>
    concept charset_interval_range = std::ranges::contiguous_range<T>
                                     and std::same_as<std::ranges::range_value_t<T>, std::pair<CharT, CharT>>;

    template<typename CharT>
    class charset
    {
    public:
        using char_type = CharT;
        using char_interval = std::pair<char_type, char_type>;

        consteval charset() noexcept = default;

        /* observers */

        [[nodiscard]] constexpr bool empty() const noexcept
        {
            return data_.empty();
        }

        [[nodiscard]] constexpr std::size_t count() const noexcept
        {
            std::size_t result{ 0 };
            for (const auto [first, second] : data_)
                result += (first + 1 - second);
            return result;
        }

        [[nodiscard]] constexpr bool contains(char_type c) const
        {
            const auto it{ std::ranges::lower_bound(data_, c, {}, &char_interval::second) };
            if (it == data_.end())
                return false;
            return c >= it->first;
        }

        [[nodiscard]] constexpr const std::vector<char_interval>& get_intervals() const noexcept
        {
            return data_;
        }

        /* modifiers */

        constexpr void clear() noexcept
        {
            data_.clear();
        }

        constexpr void negate()
        {
            data_ = std::move(make_absolute_complement(data_));
        }

        constexpr void insert(char_type c)
        {
            this->insert_single(c);
        }

        constexpr void insert(char_type first, char_type last)
        {
            this->merge_into(this->insert_single(first), last);
        }

        /* operators */

        constexpr charset& operator&=(const charset& other)
        {
            if (this != &other)
                data_ = std::move(make_intersection(data_, other.data_));
            return *this;
        }

        constexpr charset& operator|=(const charset& other)
        {
            if (this != &other)
                data_ = std::move(make_union(data_, other.data_));
            return *this;
        }

        constexpr charset& operator^=(const charset& other)
        {
            if (this != &other)
                data_ = std::move(make_symmetric_difference(data_, other.data_));
            else
                data_.clear();
            return *this;
        }

        constexpr charset& operator-=(const charset& other)
        {
            if (this != &other)
                data_ = std::move(make_relative_complement(data_, other.data_));
            else
                data_.clear();
            return *this;
        }

        friend constexpr charset operator&(const charset& lhs, const charset& rhs)
        {
            if (&lhs == &rhs)
                return lhs;
            return charset{ make_intersection(lhs.data_, rhs.data_) };
        }

        friend constexpr charset operator|(const charset& lhs, const charset& rhs)
        {
            if (&lhs == &rhs)
                return lhs;
            return charset{ make_union(lhs.data_, rhs.data_) };
        }

        friend constexpr charset operator^(const charset& lhs, const charset& rhs)
        {
            if (&lhs == &rhs)
                return lhs;
            return charset{ make_symmetric_difference(lhs.data_, rhs.data_) };
        }

        friend constexpr charset operator-(const charset& lhs, const charset& rhs)
        {
            if (&lhs == &rhs)
                return charset{};
            return charset{ make_relative_complement(lhs.data_, rhs.data_) };
        }

        friend constexpr charset operator~(const charset& cs)
        {
            return charset{ make_absolute_complement(cs.data_) };
        }

        friend constexpr bool operator==(const charset&, const charset&) = default;
        friend constexpr auto operator<=>(const charset&, const charset&) = default; /* largely meaningless but necessary */

        /* partition functions and type aliases */

        using ref = std::reference_wrapper<const charset>;

        template<typename T>
        using ref_pair = std::pair<ref, T>;

        using partition_result = std::vector<charset>;

        template<typename T>
        using partition_pair_result = std::vector<std::pair<charset, std::vector<T>>>;

        template<typename T>
        using partition_contents_result = std::vector<std::vector<T>>;

        [[nodiscard]] static constexpr auto partition(const std::vector<ref>& input) -> partition_result;

        template<typename T>
        [[nodiscard]] static constexpr auto partition_ext(const std::vector<ref_pair<T>>& input) -> partition_pair_result<T>;

        template<typename T>
        [[nodiscard]] static constexpr auto partition_contents(const std::vector<ref_pair<T>>& input) -> partition_contents_result<T>;

        /* friends :) */

        template<typename CT>
        friend class static_charset;

    private:
        using underlying_t = std::vector<char_interval>;
        using partition_entry = std::pair<char_interval, std::vector<bool>>;
        using partitioned_intervals = std::vector<partition_entry>;

        constexpr explicit charset(underlying_t&& data) : data_{ std::move(data) } {}

        constexpr underlying_t::iterator insert_single(char_type c);
        constexpr underlying_t::iterator merge_into(underlying_t::iterator inserted, char_type c);

        static constexpr underlying_t make_intersection(const charset_interval_range<char_type> auto& lhs, const charset_interval_range<char_type> auto& rhs);
        static constexpr underlying_t make_union(const charset_interval_range<char_type> auto& lhs, const charset_interval_range<char_type> auto& rhs);
        static constexpr underlying_t make_symmetric_difference(const charset_interval_range<char_type> auto& lhs, const charset_interval_range<char_type> auto& rhs);
        static constexpr underlying_t make_relative_complement(const charset_interval_range<char_type> auto& lhs, const charset_interval_range<char_type> auto& rhs);
        static constexpr underlying_t make_absolute_complement(const charset_interval_range<char_type> auto& und);

        static constexpr void part_sort_lookahead(partitioned_intervals& part, std::size_t current_idx);
        static constexpr void part_sort_lookahead_and_insert(partitioned_intervals& part, std::size_t current_idx, partition_entry&& to_insert);
        static constexpr void part_sort_and_dedup(partitioned_intervals& sorted_part);
        static constexpr void part_merge_intervals(partitioned_intervals& sorted_part);
        static constexpr auto part_make_map(const partitioned_intervals& part) -> std::flat_map<std::vector<bool>, charset>;

        underlying_t data_;
    };

    template<typename CharT>
    constexpr auto charset<CharT>::insert_single(const char_type c) -> underlying_t::iterator
    {
        if (data_.empty())
            return data_.emplace(data_.end(), c, c);

        /* form a closed interval */
        auto lower_bound{ data_.begin() };
        auto upper_bound{ std::ranges::prev(data_.end()) };

        while (true)
        {
            auto midpoint{ lower_bound + (std::ranges::distance(lower_bound, upper_bound) / 2) };

            if (midpoint->first != std::numeric_limits<char_type>::min() and c + 1 == midpoint->first)
            {
                /* extend range of pair by 1 */
                midpoint->first -= 1;

                /* attempt to merge with (mid - 1) */
                if (midpoint != data_.begin())
                {
                    if (auto prev{ std::ranges::prev(midpoint) }; midpoint->first - prev->second <= 1)
                    {
                        midpoint->first = prev->second;
                        midpoint = data_.erase(prev);
                    }
                }

                return midpoint;
            }
            else if (midpoint->second != std::numeric_limits<char_type>::max() and c == midpoint->second + 1)
            {
                /* extend range of pair by 1 */
                midpoint->second += 1;

                /* attempt to merge with (mid + 1) */
                if (auto next{ std::ranges::next(midpoint) }; next != data_.end() and next->first - midpoint->second <= 1)
                {
                    midpoint->second = next->second;
                    data_.erase(next);
                }

                return midpoint;
            }
            else if (c < midpoint->first)
            {
                if (midpoint == lower_bound)
                {
                    /* insert pair before mid */
                    return data_.emplace(midpoint, c, c);
                }
                else
                {
                    /* continue search */
                    upper_bound = std::ranges::prev(midpoint);
                }
            }
            else if (c > midpoint->second)
            {
                if (midpoint == upper_bound)
                {
                    /* insert pair after mid */
                    return data_.emplace(std::ranges::next(midpoint), c, c);
                }
                else
                {
                    /* continue search */
                    lower_bound = std::ranges::next(midpoint);
                }
            }
            else /* (lower <= c and c <= upper) */
            {
                /* c is already in char class */
                return midpoint;
            }
        }
    }

    template<typename CharT>
    constexpr auto charset<CharT>::merge_into(underlying_t::iterator inserted, const char_type c) -> underlying_t::iterator
    {
        /* assume inserted != data_.end() */

        if (inserted->second >= c)
            return inserted; /* no need to merge */

        inserted->second = c;

        /* attempt to merge */

        while (true)
        {
            auto next{ std::ranges::next(inserted) };

            if (next == data_.end())
                break; /* inserted is at end of data_: can't merge */

            if (c >= next->second)
            {
                /* range of (inserted) is a superset of (next): erase (next) */
                data_.erase(next);
            }
            else if (c + 1 >= next->first)
            {
                /* we don't need to worry about signed integer overflow here, since if
                * `last == numeric_limits<CharT>::max()`, then `next->first == next->second`
                * is true and so the previous branch (`last >= next->second`) is taken instead */

                /* ranges of (inserted) and (next) partially overlap or are adjacent: merge */
                inserted->second = next->second;
                data_.erase(next);
                break;
            }
            else
            {
                /* ranges of (inserted) and (next) do not overlap and are not adjacent: can't merge */
                break;
            }
        }

        return inserted;
    }

    template<typename CharT>
    constexpr auto charset<CharT>::make_intersection(const charset_interval_range<char_type> auto& lhs, const charset_interval_range<char_type> auto& rhs) -> underlying_t
    {
        underlying_t result;

        auto lit{ std::ranges::cbegin(lhs) };
        auto rit{ std::ranges::cbegin(rhs) };
        const auto lend{ std::ranges::cend(lhs) };
        const auto rend{ std::ranges::cend(rhs) };

        while (lit != lend and rit != rend)
        {
            if (lit->second < rit->first)
            {
                ++lit;
            }
            else if (rit->second < lit->first)
            {
                ++rit;
            }
            else
            {
                const char_type first{ std::max(lit->first, rit->first) };

                if (lit->second == rit->second)
                {
                    result.emplace_back(first, lit->second);
                    ++lit, ++rit;
                }
                else if (lit->second < rit->second)
                {
                    result.emplace_back(first, lit->second);
                    ++lit;
                }
                else /* (lit->second > rit->second) */
                {
                    result.emplace_back(first, rit->second);
                    ++rit;
                }
            }
        }

        return result;
    }

    template<typename CharT>
    constexpr auto charset<CharT>::make_union(const charset_interval_range<char_type> auto& lhs, const charset_interval_range<char_type> auto& rhs) -> underlying_t
    {
        underlying_t result;

        std::ranges::merge(lhs, rhs, std::back_inserter(result), {}, &char_interval::first, &char_interval::first);

        for (auto it{ result.begin() }; it != result.end();)
        {
            const auto erase_begin{ std::ranges::next(it) };
            auto next{ erase_begin };

            for (; next != result.end(); ++next)
            {
                if (it->second != std::numeric_limits<char_type>::max() and it->second + 1 < next->first)
                    break; /* no overlap */

                if (it->second < next->second)
                    it->second = next->second;
            }

            it = result.erase(erase_begin, next);
        }

        return result;
    }

    template<typename CharT>
    constexpr auto charset<CharT>::make_symmetric_difference(const charset_interval_range<char_type> auto& lhs, const charset_interval_range<char_type> auto& rhs) -> underlying_t
    {
        if (lhs.empty())
            return underlying_t{ std::from_range, rhs };
        if (rhs.empty())
            return underlying_t{ std::from_range, lhs };

        underlying_t result;

        auto lit{ std::ranges::cbegin(lhs) };
        auto rit{ std::ranges::cbegin(rhs) };
        const auto lend{ std::ranges::cend(lhs) };
        const auto rend{ std::ranges::cend(rhs) };

        std::optional<char_type> tmp;

        while (lit != lend and rit != rend)
        {
            const auto [min_first, max_first]{ std::minmax(lit->first, rit->first) };
            const auto min_first_or_tmp{ tmp.value_or(min_first) };

            const auto [min_second, max_second]{ std::minmax(lit->second, rit->second) };
            auto& smaller_it{ (lit->second < rit->second) ? lit : rit };

            if (min_second < max_first)
            {
                if (not result.empty() and result.back().second + 1 == min_first_or_tmp)
                    result.back().second = min_second;
                else
                    result.emplace_back(min_first_or_tmp, min_second);

                tmp.reset();
                std::ranges::advance(smaller_it, 1);
            }
            else
            {
                /* lit and rit overlap */

                if (min_first_or_tmp < max_first)
                {
                    if (not result.empty() and result.back().second + 1 == min_first_or_tmp)
                        result.back().second = max_first - 1;
                    else
                        result.emplace_back(min_first_or_tmp, max_first - 1);
                }

                if (min_second < max_second)
                {
                    tmp = min_second + 1; /* tmp <= max_second on next iteration */
                    std::ranges::advance(smaller_it, 1);
                }
                else /* (min_second == max_second) */
                {
                    tmp.reset();
                    std::ranges::advance(lit, 1);
                    std::ranges::advance(rit, 1);
                }
            }
        }

        if (lit != lend or rit != rend)
        {
            auto it{ (lit != lend) ? lit : rit };
            const auto end{ (lit != lend) ? lend : rend };
            const auto min_first_or_tmp{ tmp.value_or(it->first) };

            if (not result.empty() and result.back().second + 1 == min_first_or_tmp)
                result.back().second = it->second;
            else
                result.emplace_back(min_first_or_tmp, it->second);

            std::ranges::advance(it, 1);

            while (it != end)
            {
                result.emplace_back(it->first, it->second);
                std::ranges::advance(it, 1);
            }
        }

        return result;
    }

    template<typename CharT>
    constexpr auto charset<CharT>::make_relative_complement(const charset_interval_range<char_type> auto& lhs, const charset_interval_range<char_type> auto& rhs) -> underlying_t
    {
        if (rhs.empty())
            return underlying_t{ std::from_range, lhs };

        underlying_t result;

        auto lit{ lhs.cbegin() };
        auto rit{ rhs.cbegin() };
        const auto lend{ lhs.cend() };
        const auto rend{ rhs.cend() };

        char_type rhs_lower{ std::numeric_limits<char_type>::min() };

        while (lit != lend)
        {
            char_type rhs_upper{};

            if (rit == rend)
            {
                if (rhs_lower == std::numeric_limits<char_type>::min())
                    break; /* reached end of complement of rhs */
                else
                    rhs_upper = std::numeric_limits<char_type>::max();
            }
            else if (rhs_lower >= rit->first)
            {
                ++rit;
                continue;
            }
            else
            {
                rhs_upper = rit->first - 1;
            }

            bool advance_rit{ false };

            if (lit->second < rhs_lower)
            {
                ++lit;
            }
            else if (rhs_upper < lit->first)
            {
                advance_rit = true;
            }
            else
            {
                const char_type first{ std::max(lit->first, rhs_lower) };

                if (lit->second == rhs_upper)
                {
                    result.emplace_back(first, lit->second);
                    ++lit;
                    advance_rit = true;
                }
                else if (lit->second < rhs_upper)
                {
                    result.emplace_back(first, lit->second);
                    ++lit;
                }
                else /* (lit->second > rhs_upper) */
                {
                    result.emplace_back(first, rhs_upper);
                    advance_rit = true;
                }
            }

            if (advance_rit)
            {
                if (rit == rend)
                    break; /* reached end of complement of rhs */

                rhs_lower = static_cast<char_type>(static_cast<std::make_unsigned_t<char_type>>(rit->second) + 1);
                ++rit;
            }
        }

        return result;
    }

    template<typename CharT>
    constexpr auto charset<CharT>::make_absolute_complement(const charset_interval_range<char_type> auto& und) -> underlying_t
    {
        underlying_t result;
        char_type tmp{ std::numeric_limits<char_type>::min() };

        for (const auto& [lower, upper] : und)
        {
            if (tmp < lower)
                result.emplace_back(tmp, lower - 1);

            /* avoid UB by casting to unsigned for addition */
            tmp = static_cast<char_type>(static_cast<std::make_unsigned_t<char_type>>(upper) + 1);
        }

        if (tmp != std::numeric_limits<char_type>::min() or std::ranges::empty(und))
            result.emplace_back(tmp, std::numeric_limits<char_type>::max());

        return result;
    }

    template<typename CharT>
    constexpr void charset<CharT>::part_sort_lookahead(partitioned_intervals& part, std::size_t current_idx)
    {
        if (auto it{ part.begin() + current_idx + 1 }; it + 1 != part.end())
        {
            if (*it >= *(it + 1))
            {
                /* re-sort lookahead */
                const auto pos{ std::ranges::lower_bound(it + 1, part.end(), it->first, {}, &partition_entry::first) };
                if (pos == part.end() or *pos != *it)
                {
                    std::ranges::rotate(it, it + 1, pos);
                }
                else
                {
                    /* remove lookahead as a duplicate */
                    part.erase(it);
                }
            }
        }
    }

    template<typename CharT>
    constexpr void charset<CharT>::part_sort_lookahead_and_insert(partitioned_intervals& part, std::size_t current_idx, partition_entry&& to_insert)
    {
        if (auto it{ part.begin() + current_idx + 1 }; it + 1 != part.end())
        {
            if (*it >= *(it + 1))
            {
                /* re-sort lookahead */
                const auto pos{ std::ranges::lower_bound(it + 1, part.end(), it->first, {}, &partition_entry::first) };
                if (pos == part.end() or *pos != *it)
                {
                    const auto rot{ std::ranges::rotate(it, it + 1, pos) };

                    /* insert new pair */
                    const auto pos2{ std::ranges::upper_bound(std::ranges::begin(rot) + 1, part.end(), to_insert) };
                    if (pos2 == part.end() or *pos2 != to_insert)
                        part.emplace(pos2, std::move(to_insert));
                }
                else
                {
                    /* remove lookahead as a duplicate */
                    if (to_insert >= *(it + 1))
                    {
                        const auto pos2{ std::ranges::lower_bound(it + 1, part.end(), to_insert.first, {}, &partition_entry::first) };
                        if (pos2 == part.end() or pos2->first != to_insert.first)
                        {
                            *it = std::move(to_insert);
                            std::ranges::rotate(it, it + 1, pos2);
                        }
                        else
                        {
                            /* remove to_insert as a duplicate */
                            part.erase(it);
                        }
                    }
                    else
                    {
                        *it = std::move(to_insert);
                    }
                }
            }
            else
            {
                const auto pos2{ std::ranges::lower_bound(it + 1, part.end(), to_insert.first, {}, &partition_entry::first) };
                if (pos2 == part.end() or pos2->first != to_insert.first)
                    part.emplace(pos2, std::move(to_insert));
            }
        }
        else
        {
            /* lookahead is last element in vec; append new pair*/
            if (to_insert.first > part.back().first)
            {
                part.emplace(part.end(), std::move(to_insert));
            }
            else if (to_insert.first < part.back().first)
            {
                part.emplace(part.end() - 1, std::move(to_insert));
            }
            else
            {
                /* lookahead is same as element to insert */
            }
        }
    };

    template<typename CharT>
    constexpr void charset<CharT>::part_sort_and_dedup(partitioned_intervals& part)
    {
        std::ranges::sort(part, {}, &partition_entry::first);

        for (auto it{ part.begin() }; it != part.end(); ++it)
        {
            const auto duplicate_begin{ std::next(it) };
            auto duplicate_it{ duplicate_begin };

            for (; duplicate_it != part.end() and duplicate_it->first == it->first; ++duplicate_it)
                for (std::size_t j{ 0 }, j_max{ std::min(it->second.size(), duplicate_it->second.size()) }; j < j_max; ++j)
                    it->second[j] = it->second[j] or duplicate_it->second[j];

            if (duplicate_it != duplicate_begin)
                part.erase(duplicate_begin, duplicate_it);
        }
    }

    template<typename CharT>
    constexpr void charset<CharT>::part_merge_intervals(partitioned_intervals& part)
    {
        for (std::size_t i{ 0 }; i + 1 < part.size();)
        {
            auto& [current, current_mask]{ part[i] };
            auto& [lookahead, lookahead_mask]{ part[i + 1] };

            if (current.first == lookahead.first)
            {
                /* current.second <= lookahead.second
                 * because vec is sorted and has unique elements */

                if (current.second < lookahead.second)
                {
                    /* -----
                     * ---------
                     */

                    lookahead.first = current.second + 1;

                    for (std::size_t j{ 0 }, j_max{ std::min(current_mask.size(), lookahead_mask.size()) }; j < j_max; ++j)
                        current_mask[j] = current_mask[j] or lookahead_mask[j];

                    part_sort_lookahead(part, i);
                }
                else
                {
                    /* ---------
                     * ---------
                     */

                    for (std::size_t j{ 0 }, j_max{ std::min(current_mask.size(), lookahead_mask.size()) }; j < j_max; ++j)
                        current_mask[j] = current_mask[j] or lookahead_mask[j];

                    part.erase(part.begin() + i + 1); /* dedup */
                }
            }
            else if (current.second >= lookahead.first)
            {
                /* current and lookahead overlap
                 * and current.first < lookahead.first */

                if (current.second == lookahead.second)
                {
                    /* -----------
                     *      ------
                     */

                    current.second = lookahead.first - 1;

                    for (std::size_t j{ 0 }, j_max{ std::min(current_mask.size(), lookahead_mask.size()) }; j < j_max; ++j)
                        lookahead_mask[j] = lookahead_mask[j] or current_mask[j];

                    part_sort_lookahead(part, i);
                }
                else if (current.second < lookahead.second)
                {
                    /* ------
                     *    ---------
                     */

                    partition_entry to_insert{ { current.second + 1, lookahead.second }, lookahead_mask };
                    lookahead.second = current.second;
                    current.second = lookahead.first - 1;

                    for (std::size_t j{ 0 }, j_max{ std::min(current_mask.size(), lookahead_mask.size()) }; j < j_max; ++j)
                        lookahead_mask[j] = lookahead_mask[j] or current_mask[j];

                    part_sort_lookahead_and_insert(part, i, std::move(to_insert));
                }
                else
                {
                    /* --------------
                     *    --------
                     */

                    partition_entry to_insert{ { lookahead.second + 1, current.second }, current_mask };
                    current.second = lookahead.first - 1;

                    for (std::size_t j{ 0 }, j_max{ std::min(current_mask.size(), lookahead_mask.size()) }; j < j_max; ++j)
                        lookahead_mask[j] = lookahead_mask[j] or current_mask[j];

                    part_sort_lookahead_and_insert(part, i, std::move(to_insert));
                }
            }
            else
            {
                ++i;
            }
        }
    }

    template<typename CharT>
    constexpr auto charset<CharT>::part_make_map(const partitioned_intervals& part) -> std::flat_map<std::vector<bool>, charset>
    {
        std::flat_map<std::vector<bool>, charset> map;

        for (const auto& [interval, mask] : part)
        {
            /* since part is already sorted, we can directly insert
             * instead of calling charset::insert() */
            map[mask].data_.emplace_back(interval);
        }

        return map;
    }

    template<typename CharT>
    constexpr auto charset<CharT>::partition(const std::vector<ref>& input) -> partition_result
    {
        if (input.empty())
            return {};
        else if (input.size() == 1)
            return { input.back().get() };

        partitioned_intervals part;

        for (std::size_t i{ 0 }; i < input.size(); ++i)
        {
            std::vector<bool> mask(input.size(), false);
            mask[input.size() - i - 1] = true;
            for (const auto& pair : input[i].get().data_)
                part.emplace_back(pair, mask);
        }

        part_sort_and_dedup(part);
        part_merge_intervals(part);
        const auto map{ part_make_map(part) };

        return map.values();
    }

    template<typename CharT>
    template<typename T>
    constexpr auto charset<CharT>::partition_ext(const std::vector<ref_pair<T>>& input) -> partition_pair_result<T>
    {
        if (input.empty())
            return {};
        else if (input.size() == 1)
            return { { input.back().first, { input.back().second } } };

        partitioned_intervals part;

        for (std::size_t i{ 0 }; i < input.size(); ++i)
        {
            std::vector<bool> mask(input.size(), false);
            mask[input.size() - i - 1] = true;
            for (const auto& pair : input[i].first.get().data_)
                part.emplace_back(pair, mask);
        }

        part_sort_and_dedup(part);
        part_merge_intervals(part);
        auto map{ part_make_map(part) };

        partition_pair_result<T> result;

        for (auto it{ map.begin() }, end{ map.end() }; it != end; ++it)
        {
            result.emplace_back(std::move(it->second), std::vector<T>{});
            for (std::size_t i{ 0 }; i < input.size(); ++i)
                if (it->first.at(input.size() - i - 1))
                    result.back().second.emplace_back(input[i].second);
        }

        return result;
    }

    template<typename CharT>
    template<typename T>
    constexpr auto charset<CharT>::partition_contents(const std::vector<ref_pair<T>>& input) -> partition_contents_result<T>
    {
        if (input.empty())
            return {};
        else if (input.size() == 1)
            return { { input.back().second } };

        partitioned_intervals part;

        for (std::size_t i{ 0 }; i < input.size(); ++i)
        {
            std::vector<bool> mask(input.size(), false);
            mask[input.size() - i - 1] = true;
            for (const auto& pair : input[i].first.get().data_)
                part.emplace_back(pair, mask);
        }

        part_sort_and_dedup(part);
        part_merge_intervals(part);
        const auto map{ part_make_map(part) };

        partition_contents_result<T> result;

        for (auto it{ map.cbegin() }, end{ map.cend() }; it != end; ++it)
        {
            result.emplace_back();
            for (std::size_t i{ 0 }; i < input.size(); ++i)
                if (it->first.at(input.size() - i - 1))
                    result.back().emplace_back(input[i].second);
        }

        return result;
    }
}

namespace rx::detail
{
    template<class T>
    requires std::is_const_v<T>
    class static_span
    {
    public:
        /* types */
        using element_type           = T;
        using value_type             = std::remove_cv_t<element_type>;
        using size_type              = std::size_t;
        using difference_type        = std::ptrdiff_t;
        using pointer                = element_type*;
        using const_pointer          = element_type*;
        using reference              = element_type&;
        using const_reference        = const element_type&;
        using const_iterator         = const_pointer;
        using iterator               = const_iterator;
        using const_reverse_iterator = std::reverse_iterator<iterator>;
        using reverse_iterator       = const_reverse_iterator;

        /* constructors and assignment */
        constexpr static_span() noexcept = default;

        template<std::size_t N = std::dynamic_extent>
        consteval static_span(std::span<T, N> span) noexcept
            : data_{ span.data() }, size_{ span.size() } {}

        template<std::size_t N = std::dynamic_extent>
        consteval static_span& operator=(std::span<T, N> span) noexcept
        {
            data_ = span.data();
            size_ = span.size();
            return *this;
        }

        /* size observers */
        [[nodiscard]] constexpr size_type size() const noexcept { return size_; }
        [[nodiscard]] constexpr size_type size_bytes() const noexcept { return this->size() * sizeof(element_type); }
        [[nodiscard]] constexpr bool empty() const noexcept { return this->size() == 0; }

        /* element access */
        [[nodiscard]] constexpr const_reference operator[](size_type idx) const { return data_[idx]; }
        [[nodiscard]] constexpr const_reference at(size_type idx) const { this->range_check(idx); return this->operator[](idx); }
        [[nodiscard]] constexpr const_reference front() const { return data_[0]; }
        [[nodiscard]] constexpr const_reference back() const { return data_[this->size() - 1]; }
        [[nodiscard]] constexpr const_pointer data() const noexcept { return data_; }

        /* iterators */
        [[nodiscard]] constexpr const_iterator begin() const noexcept { return data_; }
        [[nodiscard]] constexpr const_iterator end() const noexcept { return data_ + this->size(); }
        [[nodiscard]] constexpr const_iterator cbegin() const noexcept { return this->begin(); }
        [[nodiscard]] constexpr const_iterator cend() const noexcept { return this->end(); }
        [[nodiscard]] constexpr const_reverse_iterator rbegin() const noexcept { return std::make_reverse_iterator(this->begin()); }
        [[nodiscard]] constexpr const_reverse_iterator rend() const noexcept { return std::make_reverse_iterator(this->end()); }
        [[nodiscard]] constexpr const_reverse_iterator crbegin() const noexcept { return this->rbegin(); }
        [[nodiscard]] constexpr const_reverse_iterator crend() const noexcept { return this->rend(); }

    private:
        constexpr void range_check(size_type n) const
        {
            if (n >= this->size())
                throw std::out_of_range("static_span::range_check: n >= this->size()");
        }

    public:
        /* data members (public so that static_span is structural) */
        pointer   data_{ nullptr };
        size_type size_{ 0 };
    };

    template<class R>
    static_span(R&&) -> static_span<std::add_const_t<std::remove_reference_t<std::ranges::range_reference_t<R>>>>;
}

namespace rx::detail
{
    template<typename CharT>
    class static_charset;

    template<typename T, typename CharT>
    concept charset_like = one_of<T, charset<CharT>, static_charset<CharT>>;

    template<typename CharT>
    class static_charset
    {
    public:
        using charset_type = charset<CharT>;
        using char_type = charset_type::char_type;
        using char_interval = charset_type::char_interval;

        consteval static_charset() noexcept = default;

        consteval explicit static_charset(const charset_type& cs)
            : data_{ std::define_static_array(cs.data_) } {}

        template<typename... Args>
        requires (sizeof...(Args) >= 1) and ((std::convertible_to<Args, char_type> or std::convertible_to<Args, char_interval>) and ...)
        constexpr explicit static_charset(Args... args)
        {
            charset_type tmp;

            template for (constexpr std::size_t i : std::views::iota(0uz, sizeof...(Args)))
            {
                if constexpr (std::convertible_to<Args...[i], char_type>)
                    tmp.insert(args...[i]);
                else if constexpr (std::convertible_to<Args...[i], char_interval>)
                    tmp.insert(args...[i].first, args...[i].second);
            }

            data_ = std::define_static_array(tmp.data_);
        }

        /* observers */

        [[nodiscard]] constexpr bool empty() const noexcept
        {
            return data_.empty();
        }

        [[nodiscard]] constexpr std::size_t count() const noexcept
        {
            std::size_t result{ 0 };
            for (const auto [first, second] : data_)
                result += (first + 1 - second);
            return result;
        }

        [[nodiscard]] constexpr bool contains(char_type c) const
        {
            const auto it{ std::ranges::lower_bound(data_, c, {}, &char_interval::second) };
            if (it == data_.end())
                return false;
            return c >= it->first;
        }

        [[nodiscard]] constexpr const static_span<const char_interval>& get_intervals() const noexcept
        {
            return data_;
        }

        /* operators */

        constexpr explicit operator charset_type() const
        {
            return charset_type{ typename charset_type::underlying_t{ std::from_range, data_ } };
        }

        constexpr charset_type operator~()
        {
            return charset_type{ charset_type::make_absolute_complement(data_) };
        }

        friend constexpr charset_type operator&(const charset_like<char_type> auto& lhs, const charset_like<char_type> auto& rhs)
        {
            return charset_type{ charset_type::make_intersection(lhs.data_, rhs.data_) };
        }

        friend constexpr charset_type operator|(const charset_like<char_type> auto& lhs, const charset_like<char_type> auto& rhs)
        {
            return charset_type{ charset_type::make_union(lhs.data_, rhs.data_) };
        }

        friend constexpr charset_type operator^(const charset_like<char_type> auto& lhs, const charset_like<char_type> auto& rhs)
        {
            return charset_type{ charset_type::make_symmetric_difference(lhs.data_, rhs.data_) };
        }

        friend constexpr charset_type operator-(const charset_like<char_type> auto& lhs, const charset_like<char_type> auto& rhs)
        {
            return charset_type{ charset_type::make_relative_complement(lhs.data_, rhs.data_) };
        }

        friend constexpr bool operator==(const charset_like<char_type> auto& lhs, const charset_like<char_type> auto& rhs)
        {
            return std::ranges::equal(lhs.data_, rhs.data_);
        };

        friend constexpr auto operator<=>(const charset_like<CharT> auto& lhs, const charset_like<CharT> auto& rhs)
        {
            return std::lexicographical_compare_three_way(lhs.data_.begin(), lhs.data_.end(), rhs.data_.begin(), rhs.data_.end());
        }

        /* additional operators for charset */

        friend constexpr charset_type& operator&=(charset_type& lhs, const static_charset& rhs)
        {
            lhs.data_ = std::move(charset_type::make_intersection(lhs.data_, rhs.data_));
            return lhs;
        }

        friend constexpr charset_type& operator|=(charset_type& lhs, const static_charset& rhs)
        {
            lhs.data_ = std::move(charset_type::make_union(lhs.data_, rhs.data_));
            return lhs;
        }

        friend constexpr charset_type& operator^=(charset_type& lhs, const static_charset& rhs)
        {
            lhs.data_ = std::move(charset_type::make_symmetric_difference(lhs.data_, rhs.data_));
            return lhs;
        }

        friend constexpr charset_type& operator-=(charset_type& lhs, const static_charset& rhs)
        {
            lhs.data_ = std::move(charset_type::make_relative_complement(lhs.data_, rhs.data_));
            return lhs;
        }

    private:
        static_span<const char_interval> data_;
    };

    template<typename T>
    struct nontransient_constexpr_version_of {};

    template<typename CharT>
    struct nontransient_constexpr_version_of<bitcharset<CharT>>
    {
        using type = bitcharset<CharT>;
    };

    template<typename CharT>
    struct nontransient_constexpr_version_of<charset<CharT>>
    {
        using type = static_charset<CharT>;
    };

    template<typename CharT>
    struct nontransient_constexpr_version_of<static_charset<CharT>>
    {
        using type = static_charset<CharT>;
    };

    template<typename T>
    using nontransient_constexpr_version_of_t = nontransient_constexpr_version_of<T>::type;
}

namespace rx::detail
{
    enum class named_character_class : std::uint8_t
    {
        alphanumeric,
        alphabetic,
        ascii,
        blank,
        control,
        digits,
        graphical,
        lowercase,
        printable,
        punctuation,
        posix_whitespace,
        perl_whitespace,
        uppercase,
        word,
        hexdigits,
    };

    /* class definitions */

    template<bool IsNarrow>
    class char_class_impl
    {
    public:
        using char_type = std::conditional_t<IsNarrow, char, char32_t>;
        using underlying_type = std::conditional_t<IsNarrow, bitcharset<char_type>, charset<char_type>>;

        constexpr explicit char_class_impl(named_character_class ncc, bool negate = false) noexcept :
            negated_{ negate }, orig_negated_{ negate } { insert(ncc); }

        constexpr explicit char_class_impl(bool negate = false) noexcept :
            negated_{ negate }, orig_negated_{ negate } {}

        constexpr void insert(char_type c) noexcept(IsNarrow) { data_.insert(c); }
        constexpr void insert(char_type first, char_type last) noexcept(IsNarrow) { data_.insert(first, last); }
        constexpr void insert(named_character_class ncc) noexcept(IsNarrow);
        constexpr void insert(const char_class_impl& cc) noexcept(IsNarrow) { data_ |= cc.data_; };

        [[nodiscard]] constexpr bool empty() const noexcept { return data_.empty(); }
        [[nodiscard]] constexpr std::size_t count() const noexcept { return data_.count(); }
        [[nodiscard]] constexpr bool is_negated() const noexcept { return negated_; }
        [[nodiscard]] constexpr auto intervals() const noexcept(not IsNarrow) { return data_.get_intervals(); }
        [[nodiscard]] constexpr const auto& get() const noexcept { return data_; }

        constexpr void normalise() noexcept { if (negated_) { data_.negate(); negated_ = false; } }
        constexpr void make_caseless() noexcept(IsNarrow);

        [[nodiscard]] constexpr char_class_impl denormalise() const noexcept(IsNarrow);

    private:
        underlying_type data_;
        bool            negated_;
        bool            orig_negated_;
    };

    /* member function definitions */

    template<bool IsNarrow>
    constexpr void char_class_impl<IsNarrow>::make_caseless() noexcept(IsNarrow)
    {
        if constexpr (IsNarrow)
        {
            data_.make_ascii_case_insensitive();
        }
        else
        {
            static constexpr char_type offset{ 'a' - 'A' };
            std::vector<typename underlying_type::char_interval> to_insert;

            for (const auto [lower, upper] : data_.get_intervals())
            {
                if (upper >= 'a' and lower <= 'Z')
                {
                    /* [lower, upper] only covers both uppercase and lowercase letters */
                    to_insert.emplace_back(std::max<char_type>(lower, 'A') + offset, std::min<char_type>(upper, 'a'));
                    to_insert.emplace_back(std::max<char_type>(lower, 'A'), std::min<char_type>(upper, 'a') - offset);
                }
                else if (upper >= 'A' and lower <= 'Z')
                {
                    /* [lower, upper] only covers uppercase letters */
                    to_insert.emplace_back(std::max<char_type>(lower, 'A') + offset, std::min<char_type>(upper, 'A') + offset);
                }
                else if (upper >= 'a' and lower <= 'z')
                {
                    /* [lower, upper] only covers lowercase letters */
                    to_insert.emplace_back(std::max<char_type>(lower, 'a') - offset, std::min<char_type>(upper, 'a') - offset);
                }
            }

            for (const auto [lower, upper] : to_insert)
                insert(lower, upper);
        }
    }

    template<bool IsNarrow>
    constexpr void char_class_impl<IsNarrow>::insert(named_character_class ncc) noexcept(IsNarrow)
    {
        using cs = nontransient_constexpr_version_of_t<underlying_type>;
        using p = cs::char_interval;

        static constexpr cs alnum{ p{ '0', '9' }, p{ 'A', 'Z' }, p{ 'a', 'z' } };
        static constexpr cs alpha{ p{ 'A', 'Z' }, p{ 'a', 'z' } };
        static constexpr cs ascii{ p{ 0x00, 0x7F } };
        static constexpr cs blank{ '\t', ' ' };
        static constexpr cs cntrl{ p{ 0x00, 0x1F }, 0x7F };
        static constexpr cs digit{ p{ '0', '9' } };
        static constexpr cs graph{ p{ '!', '~' } };
        static constexpr cs lower{ p{ 'a', 'z' } };
        static constexpr cs print{ p{ ' ', '~' } };
        static constexpr cs punct{ p{ '!', '/' }, p{ ':', '@' }, p{ '[', '`' }, p{ '{', '~' } };
        static constexpr cs space{ '\v', '\t', '\n', '\f', '\r', ' ' };
        static constexpr cs perls{ '\t', '\n', '\f', '\r', ' ' };
        static constexpr cs upper{ p{ 'A', 'Z' } };
        static constexpr cs word{ p{ '0', '9' }, p{ 'A', 'Z' }, p{ 'a', 'z' }, '_' };
        static constexpr cs xdigit{ p{ '0', '9' }, p{ 'A', 'F' }, p{ 'a', 'f' } };

        switch (ncc)
        {
        case named_character_class::alphanumeric:     data_ |= alnum;  break;
        case named_character_class::alphabetic:       data_ |= alpha;  break;
        case named_character_class::ascii:            data_ |= ascii;  break;
        case named_character_class::blank:            data_ |= blank;  break;
        case named_character_class::control:          data_ |= cntrl;  break;
        case named_character_class::digits:           data_ |= digit;  break;
        case named_character_class::graphical:        data_ |= graph;  break;
        case named_character_class::lowercase:        data_ |= lower;  break;
        case named_character_class::printable:        data_ |= print;  break;
        case named_character_class::punctuation:      data_ |= punct;  break;
        case named_character_class::posix_whitespace: data_ |= space;  break;
        case named_character_class::perl_whitespace:  data_ |= perls;  break;
        case named_character_class::uppercase:        data_ |= upper;  break;
        case named_character_class::word:             data_ |= word;   break;
        case named_character_class::hexdigits:        data_ |= xdigit; break;
        }
    }

    template<bool IsNarrow>
    constexpr auto char_class_impl<IsNarrow>::denormalise() const noexcept(IsNarrow) -> char_class_impl
    {
        char_class_impl result{ orig_negated_ };
        if (negated_ == false and orig_negated_ == true)
            result.data_ = ~data_;
        else
            result.data_ = data_;
        return result;
    }
}

/* Note: We assume the literal character encoding is a superset of ASCII */

namespace rx::detail
{
    /* RE2 limits counted repetitions to 1000 - we do the same here.
     * Note: clang reaches the consteval step limit with >512 repetitions */
    inline constexpr std::int_least16_t counted_repetition_limit{ 1000 };

    enum class assert_type : int_least8_t 
    {
        text_start,
        text_end,
        line_start,
        line_end,

        word_boundary,
        not_word_boundary,
    };

    namespace parser
    {
        template<typename CharT>
        class ll1;
    }

    /* token definitions */

    namespace tok
    {
        struct end_of_input {};
        struct vert {};
        struct dot {};
        struct hat {};
        struct dollar {};
        struct lparen {};
        struct rparen {};
        struct star {};
        struct plus {};
        struct quest {};

        struct repeat_n_m
        {
            std::int_least16_t min;
            std::int_least16_t max; /* use max=min for {min} or max<min for {min,} */
        };

        struct assertion
        {
            assert_type type;
        };

        template<typename CharT>
        struct char_class
        {
            using impl_type = char_class_impl<std::same_as<char, CharT>>;
            using underlying_char_type = impl_type::char_type;

            impl_type data;

            template<typename... Args>
            constexpr explicit char_class(Args&&... args) : data{ std::forward<Args>(args)... } {}
        };

        template<typename CharT>
        struct char_str
        {
            std::basic_string<CharT> data;

            constexpr explicit char_str() : data{} {} /* empty string */
            constexpr explicit char_str(CharT c) : data{ c } {}

            template<std::input_iterator I, std::sentinel_for<I> S>
            requires std::convertible_to<std::iter_value_t<I>, CharT>
            constexpr explicit char_str(I first, S last) : data(first, last) {}

            constexpr explicit char_str(char c) requires (not std::same_as<CharT, char>)
            {
                data += c;
            }

            constexpr explicit char_str(std::size_t parse_result)
            {
                if (parse_result <= std::numeric_limits<std::make_unsigned_t<CharT>>::max())
                {
                    data = static_cast<CharT>(parse_result);
                }
                else
                {

                    throw pattern_error("Multibyte characters are unimplemented");
                }
            }

            [[nodiscard]] constexpr std::optional<typename char_class<CharT>::underlying_char_type> get_if_single()
            {

                if (data.size() == 1)
                    return data.front();
                return {};
            }
        };

        struct backref
        {
            std::uint_least16_t number;
        };
    }

    /* lexer class definition */

    template<typename CharT>
    class lexer
    {
        using sv_type = std::basic_string_view<CharT>;
        using it_type = sv_type::const_iterator;

    public:
        using token_t = std::variant<tok::end_of_input, tok::dot, tok::hat, tok::dollar,
                                     tok::lparen, tok::rparen, tok::vert,
                                     tok::star, tok::plus, tok::quest, tok::repeat_n_m,
                                     tok::char_str<CharT>, tok::char_class<CharT>,
                                     tok::backref, tok::assertion>;

        constexpr explicit lexer(const sv_type& sv) : it_{ sv.cbegin() }, end_{ sv.cend() } {}
        constexpr token_t nexttok();

        friend class parser::ll1<CharT>;

    private:
        constexpr std::size_t            parse_hex(std::size_t fixed_amt);
        constexpr std::size_t            parse_remaining_oct(std::size_t init);
        constexpr std::size_t            parse_arbitrary_oct();
        constexpr tok::backref           parse_bref();
        constexpr tok::repeat_n_m        parse_repeat();
        constexpr token_t                parse_bref_or_octal(CharT init);
        constexpr tok::char_str<CharT>   parse_literal_string(it_type begin);
        constexpr tok::char_class<CharT> parse_char_class();
        constexpr named_character_class  parse_posix_char_class();

        it_type it_;
        it_type end_;
    };

    /* lexer implementation */

    template<typename CharT>
    constexpr lexer<CharT>::token_t lexer<CharT>::nexttok()
    {
        using namespace tok;
        using char_str = char_str<CharT>;
        using char_class = char_class<CharT>;

        if (it_ == end_)
            return end_of_input{};

        const auto current{ it_ };

        switch (*it_++)
        {
        case '(': return lparen{};
        case ')': return rparen{};
        case '.': return dot{};
        case '*': return star{};
        case '+': return plus{};
        case '?': return quest{};
        case '^': return hat{};
        case '$': return dollar{};
        case '|': return vert{};

        case '\\':
        {
            using ncc = named_character_class;

            if (it_ == end_)
                throw pattern_error("Pattern cannot end with '\\'");

            const auto escaped{ *it_++ };

            switch (escaped)
            {
            /* standard escape sequences */

            case 'a': return char_str{ '\a' };

            case 'f': return char_str{ '\f' };
            case 't': return char_str{ '\t' };
            case 'n': return char_str{ '\n' };
            case 'r': return char_str{ '\r' };
            case 'v': return char_str{ '\v' };

            /* numeric escape sequences */

            case 'o': return char_str{ parse_arbitrary_oct() };
            case 'x': return char_str{ parse_hex(0) };
            case 'u': return char_str{ parse_hex(4) };
            case 'U': return char_str{ parse_hex(8) };

            /* perl character classes */

            case 'd': return char_class{ ncc::digits };
            case 'D': return char_class{ ncc::digits, true };
            case 's': return char_class{ ncc::perl_whitespace };
            case 'S': return char_class{ ncc::perl_whitespace, true };
            case 'w': return char_class{ ncc::word };
            case 'W': return char_class{ ncc::word, true };

            /* octal escape sequences and backreferences */

            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7': return parse_bref_or_octal(escaped);

            case '8':
            case '9': return backref{ std::saturate_cast<std::uint_least16_t>(escaped - '0') };

            case 'g': return parse_bref();

            /* assertions */

            case 'A': return assertion{ assert_type::text_start };
            case 'b': return assertion{ assert_type::word_boundary };
            case 'B': return assertion{ assert_type::not_word_boundary };
            case 'G': throw parser_error("Assertion (\\G) is not implemented");
            case 'Z': throw parser_error("End of text or newlines followed by end of text (\\Z) is not implemented");
            case 'z': return assertion{ assert_type::text_end };

            /* literal string */

            case 'Q': return parse_literal_string(current);

            default:
                if (('A' <= escaped and escaped <= 'Z') or ('a' <= escaped and escaped <= 'z'))
                    throw pattern_error("Invalid control character");
                else
                    return char_str{ escaped }; /* TODO: extract multibyte character */
            }
        }

        case '{': return parse_repeat();
        case '[': return parse_char_class();

        default:
            return char_str{ *current }; /* TODO: extract multibyte character */
        }
    }

    /* general helpers for lexer implementation */

    template<typename CharT>
    constexpr std::size_t lexer<CharT>::parse_hex(const std::size_t fixed_amt)
    {
        static constexpr std::size_t hexadecimal_base{ 0x10 };
        static constexpr std::size_t decimal_base{ 10 };

        std::size_t result{ 0 };

        if (it_ == end_)
            throw pattern_error("EOF in escape sequence");

        const auto lookahead{ *it_ };

        if (lookahead == '{')
        {
            /* arbitrary number of digits contained in {} */

            std::size_t digits{ 0 };
            ++it_;

            for (bool loop{ true }; loop; ++digits)
            {
                if (it_ == end_)
                    throw pattern_error("EOF in escape sequence");;

                const auto c{ *it_ };

                if (c == '}')
                    loop = false;
                else if ('0' <= c and c <= '9')
                    result = (result * hexadecimal_base) + (c - '0');
                else if ('A' <= c and c <= 'F')
                    result = (result * hexadecimal_base) + decimal_base + (c - 'A');
                else if ('a' <= c and c <= 'f')
                    result = (result * hexadecimal_base) + decimal_base + (c - 'a');
                else
                    throw pattern_error("Invalid character in hexadecimal escape sequence");

                ++it_;
            }

            if (digits == 0)
                throw pattern_error("Delimited escape sequence cannot be empty");;
        }
        else if (fixed_amt != 0)
        {
            for (std::size_t i{ 0 }; i < fixed_amt; ++i)
            {
                if (it_ == end_)
                    throw pattern_error("EOF in escape sequence");

                const auto c{ *it_ };

                if ('0' <= c and c <= '9')
                    result = (result * hexadecimal_base) + (c - '0');
                else if ('A' <= c and c <= 'F')
                    result = (result * hexadecimal_base) + decimal_base + (c - 'A');
                else if ('a' <= c and c <= 'f')
                    result = (result * hexadecimal_base) + decimal_base + (c - 'a');
                else
                    throw pattern_error("Invalid character in hexadecimal escape sequence");

                ++it_;
            }
        }
        else
        {
            std::size_t digits{ 0 };

            while (true)
            {
                if (it_ == end_)
                    break;

                const auto c{ *it_ };

                if ('0' <= c and c <= '9')
                    result = (result * hexadecimal_base) + (c - '0');
                else if ('A' <= c and c <= 'F')
                    result = (result * hexadecimal_base) + decimal_base + (c - 'A');
                else if ('a' <= c and c <= 'f')
                    result = (result * hexadecimal_base) + decimal_base + (c - 'a');
                else
                    break;

                ++it_;
                ++digits;
            }

            if (digits == 0)
                throw pattern_error("Invalid escape sequence");
        }

        return result;
    }

    template<typename CharT>
    constexpr std::size_t lexer<CharT>::parse_remaining_oct(const std::size_t init)
    {
        static constexpr std::size_t octal_base{ 010 };

        std::size_t result{ init };

        for (std::size_t i{ 0 }; i < 2; ++i)
        {
            if (it_ == end_)
                break;

            const auto c{ *it_ };

            if ('0' <= c and c <= '7')
                result = (result * octal_base) + (c - '0');
            else
                break;

            ++it_;
        }

        return result;
    }

    template<typename CharT>
    constexpr std::size_t lexer<CharT>::parse_arbitrary_oct()
    {
        static constexpr std::size_t octal_base{ 010 };

        std::size_t result{ 0 };

        if (it_ == end_)
            throw pattern_error("EOF in escape sequence");

        const auto lookahead{ *it_ };

        if (lookahead != '{')
            throw pattern_error("Invalid escape sequence");

        /* arbitrary number of digits contained in {} */

        std::size_t digits{ 0 };
        ++it_;

        for (bool loop{ true }; loop; ++digits)
        {
            if (it_ == end_)
                throw pattern_error("EOF in escape sequence");;

            const auto c{ *it_ };

            if (c == '}')
                loop = false;
            else if ('0' <= c and c <= '7')
                result = (result * octal_base) + (c - '0');
            else
                throw pattern_error("Invalid character in octal escape sequence");

            ++it_;
        }

        if (digits == 0)
            throw pattern_error("Delimited escape sequence cannot be empty");

        return result;
    }

    /* specific helpers for lexer implementation */

    template<typename CharT>
    constexpr tok::backref lexer<CharT>::parse_bref()
    {
        using namespace tok;
        static constexpr std::size_t base{ 10 };

        backref bref{ 0 };

        if (it_ == end_)
            throw pattern_error("Incomplete escape sequence");

        auto next{ *it_++ };

        if (next == '{')
        {
            /* \g{n...} */

            while (true)
            {
                if (it_ == end_)
                    throw pattern_error("Incomplete escape sequence");;

                next = *it_++;

                if (next == '}')
                    break;

                if (not ('0' <= next and next <= '9'))
                    throw pattern_error("Incomplete escape sequence");;

                bref.number *= base;
                bref.number += next - '0';
            }
        }
        else if ('0' <= next and next <= '9')
        {
            /* \gn */
            bref.number = next - '0';
        }
        else
        {
            throw pattern_error("Incomplete escape sequence");
        }

        if (bref.number == 0)
            throw pattern_error("Backreference to non-existent submatch");

        return bref;
    }

    template<typename CharT>
    constexpr tok::repeat_n_m lexer<CharT>::parse_repeat()
    {
        using namespace tok;
        static constexpr std::int_least16_t base{ 10 };

        std::int_least16_t min{ -1 };
        std::int_least16_t max{ -1 };

        bool parse_min{ true };
        bool parse_max{ true };

        while (parse_min)
        {
            if (it_ == end_)
                throw pattern_error("Repeater is incomplete");

            auto c{ *it_++ };

            if ('0' <= c and c <= '9')
            {
                if (min == -1)
                    min = c - '0';
                else
                    min = std::add_sat<std::int_least16_t>(std::mul_sat(min, base), c - '0');
            }
            else if (c == ',')
            {
                parse_min = false;
            }
            else if (c == '}')
            {
                parse_min = false;

                /* skip parsing max */
                max = min;
                parse_max = false;
            }
            else
            {
                throw pattern_error("Invalid character in repeater");
            }
        }

        while (parse_max)
        {
            if (it_ == end_)
                throw pattern_error("Repeater is incomplete");

            auto c{ *it_++ };

            if ('0' <= c and c <= '9')
            {
                if (max == -1)
                    max = c - '0';
                else
                    max = std::add_sat<std::int_least16_t>(std::mul_sat(max, base), c - '0');
            }
            else if (c == '}')
            {
                if (max != -1 and max < min)
                    throw pattern_error("Invalid repeater (max is less than min)");

                parse_max = false;
            }
            else
            {
                throw pattern_error("Invalid character in repeater");
            }
        }

        if (min > counted_repetition_limit)
        {
            if (min == max)
                throw pattern_error("Finite number of counted repetitions exceeds limit");
            else
                throw pattern_error("Lower bound of counted repetitions exceeds limit");
        }
        else if (max > counted_repetition_limit)
        {
            throw pattern_error("Finite upper bound of counted repetitions exceeds limit");
        }

        return repeat_n_m{ .min = min, .max = max };
    }

    template<typename CharT>
    constexpr lexer<CharT>::token_t lexer<CharT>::parse_bref_or_octal(const CharT init)
    {
        using namespace tok;
        static constexpr std::size_t base{ 010 };

        std::size_t result{ static_cast<std::size_t>(init - '0') };
        backref bref{ std::saturate_cast<std::uint_least16_t>(init - '0') };

        for (int i{ 0 }; i < 2; ++i)
        {
            if (it_ == end_)
                break;

            const auto lookahead{ *it_ };

            if (not ('0' <= lookahead and lookahead <= '7'))
                break;

            bref.number = 0;

            result *= base;
            result += lookahead - '0';
            ++it_;
        }

        if (bref.number == 0)
            return char_str<CharT>{ result };
        else
            return bref;
    }

    template<typename CharT>
    constexpr tok::char_str<CharT> lexer<CharT>::parse_literal_string(const it_type begin)
    {
        using namespace tok;

        for (bool slash{ true }; true;)
        {
            if (it_ == end_)
                throw pattern_error("Reached end of input in literal text");

            const auto cur{ *it_++ };

            if (not slash and cur == '\\')
                slash = true;
            else if (slash and cur == 'E')
                break;
            else
                slash = false;
        }

        return char_str<CharT>{ std::next(begin, 2), std::prev(it_, 2) };
    }

    template<typename CharT>
    constexpr tok::char_class<CharT> lexer<CharT>::parse_char_class()
    {
        using namespace tok;
        using ncc = named_character_class;
        using char_class = char_class<CharT>;
        using underlying_char_t = char_class::underlying_char_type;

        if (it_ == end_)
            throw pattern_error("EOF in character class");

        const bool is_negated{ *it_ == '^' };

        if (is_negated)
            ++it_;

        char_class result{ is_negated };

        std::optional<underlying_char_t> c{};
        bool is_range{ false };

        for (bool loop{ true }; loop;)
        {
            if (it_ == end_)
                throw pattern_error("EOF in character class");

            const auto current{ it_ };

            std::optional<std::pair<ncc, bool>> selected_cc;
            std::optional<underlying_char_t> nc;

            switch (*it_++)
            {
            case ']':
                if (result.data.empty() and not c.has_value())
                    nc = ']';
                else
                    loop = false;
                break;

            case '-':
                if (c.has_value() and not is_range)
                    is_range = true;
                else
                    nc = '-';
                break;

            case '\\':
            {
                if (it_ == end_)
                    throw pattern_error("Pattern cannot end with '\\'");

                const auto escaped{ *it_++ };

                switch (escaped)
                {
                /* standard escape sequences */

                case 'a': nc = '\a'; break;
                case 'b': nc = '\b'; break;
                case 'f': nc = '\f'; break;
                case 't': nc = '\t'; break;
                case 'n': nc = '\n'; break;
                case 'r': nc = '\r'; break;
                case 'v': nc = '\v'; break;

                /* numeric escape sequences */

                case '0':
                case '1':
                case '2':
                case '3':
                case '4':
                case '5':
                case '6':
                case '7': nc = parse_remaining_oct(escaped - '0'); break;

                case 'o': nc = parse_arbitrary_oct(); break;
                case 'x': nc = parse_hex(0); break;
                case 'u': nc = parse_hex(4); break;
                case 'U': nc = parse_hex(8); break;

                /* perl character classes */

                case 'd': selected_cc = { ncc::digits, false }; break;
                case 'D': selected_cc = { ncc::digits, true }; break;
                case 's': selected_cc = { ncc::perl_whitespace, false }; break;
                case 'S': selected_cc = { ncc::perl_whitespace, true }; break;
                case 'w': selected_cc = { ncc::word, false }; break;
                case 'W': selected_cc = { ncc::word, true }; break;

                default:
                    if (('A' <= escaped and escaped <= 'Z') or ('a' <= escaped and escaped <= 'z'))
                        throw pattern_error("Invalid control character");
                    else
                        nc = escaped; /* TODO: extract multibyte character */
                    break;
                }
                break;
            }

            case '[':
                selected_cc = { parse_posix_char_class(), false };
                break;

            default:
                nc = *current; /* TODO: extract multibyte character */
                break;
            }

            if (selected_cc)
            {
                if (c)
                {
                    /* insert c */
                    result.data.insert(c.value());
                    c.reset();
                }

                if (selected_cc->second)
                {
                    /* insert negated char class */
                    typename char_class::impl_type tmp{ selected_cc->first, true };
                    result.data.insert(tmp);
                }
                else
                {
                    /* insert char class */
                    result.data.insert(selected_cc->first);
                }

                selected_cc.reset();
            }
            else if (nc)
            {
                if (not c)
                {
                    /* delay insert */
                    c = nc;
                }
                else if (is_range)
                {
                    if (*c > *nc)
                        throw pattern_error("Invalid character class range");

                    /* insert c - nc */
                    result.data.insert(*c, *nc);
                    is_range = false;
                    c.reset();
                }
                else
                {
                    /* insert c */
                    result.data.insert(*c);
                    c = nc;
                }
            }
            else if (not loop)
            {
                if (c)
                    result.data.insert(*c);

                if (is_range)
                    result.data.insert(underlying_char_t{ '-' });
            }
        }

        result.data.normalise();
        return result;
    }

    template<typename CharT>
    constexpr named_character_class lexer<CharT>::parse_posix_char_class()
    {
        using ncc = named_character_class;

        static constexpr auto test = [](it_type& it, const it_type& end, std::string_view rest) -> bool {
            if (std::ranges::starts_with(it, end, std::ranges::begin(rest), std::ranges::end(rest)))
            {
                std::ranges::advance(it, std::ranges::size(rest));
                return true;
            }
            return false;
        };

        if (it_ == end_ or *it_ != ':')
            throw pattern_error("Invalid POSIX Character Class");

        ++it_;

        if (it_ != end_)
        {
            switch (*it_++)
            {
            case 'a':
                if (it_ == end_)
                    break;
                if (*it_ == 'l')
                {
                    ++it_;
                    if (it_ == end_)
                    {
                        break;
                    }
                    else if (*it_ == 'n')
                    {
                        if (test(++it_, end_, "um:]"))
                            return ncc::alphanumeric;
                    }
                    else if (*it_ == 'p')
                    {
                        if (test(++it_, end_, "ha:]"))
                            return ncc::alphabetic;
                    }
                }
                else if (*it_ == 's')
                {
                    if (test(++it_, end_, "cii:]"))
                        return ncc::ascii;
                }
                break;

            case 'b':
                if (test(it_, end_, "lank:]"))
                    return ncc::blank;
                break;

            case 'c':
                if (test(it_, end_, "ntrl:]"))
                    return ncc::control;
                break;

            case 'd':
                if (test(it_, end_, "igit:]"))
                    return ncc::digits;
                break;

            case 'g':
                if (test(it_, end_, "raph:]"))
                    return ncc::graphical;
                break;

            case 'l':
                if (test(it_, end_, "ower:]"))
                    return ncc::lowercase;
                break;

            case 'p':
                if (it_ == end_)
                    break;
                if (*it_ == 'r')
                {
                    if (test(++it_, end_, "int:]"))
                        return ncc::printable;
                }
                else if (*it_ == 'u')
                {
                    if (test(++it_, end_, "nct:]"))
                        return ncc::punctuation;
                }
                break;

            case 's':
                if (test(it_, end_, "pace:]"))
                    return ncc::posix_whitespace;
                break;

            case 'u':
                if (test(it_, end_, "pper:]")) return ncc::uppercase;
                break;

            case 'w':
                if (test(it_, end_, "ord:]"))
                    return ncc::word;
                break;

            case 'x':
                if (test(it_, end_, "digit:]"))
                    return ncc::hexdigits;
                break;

            default:
                break;
            }
        }

        throw pattern_error("Invalid POSIX Character Class");
    }
}

namespace rx::detail
{
    namespace parser
    {
        template<typename CharT>
        class ll1;
    }

    /* types */

    enum class repeater_mode : int_least8_t
    {
        greedy = 0,
        lazy,
        possessive
    };

    struct parser_flags
    {
        bool enable_captures    : 1 { true };
        bool enable_start_tag   : 1 { true };
        bool enable_possessive  : 1 { false };
        bool enable_backrefs    : 1 { false };
        bool enable_branchreset : 1 { false };
        bool enable_alttocc     : 1 { true };
    };

    /* ast definition */

    template<typename CharT>
    class expr_tree
    {
    public:
        using char_type = CharT;
        using sv_type = std::basic_string_view<char_type>;

        struct alt
        {
            std::vector<std::size_t> idxs;
        };

        struct concat
        {
            std::vector<std::size_t> idxs;
        };

        struct tag
        {
            tag_number_t number;
        };

        using backref = tok::backref;

        struct repeat
        {
            std::size_t idx;
            std::int_least16_t min;
            std::int_least16_t max; /* use max=min for {min} or max<min for {min,} */
            repeater_mode mode;     /* default = greedy */
        };

        using assertion = tok::assertion;
        using char_str = tok::char_str<char_type>;
        using char_class = tok::char_class<char_type>;

        using type = std::variant<assertion, char_str, char_class, backref, alt, concat, tag, repeat>;

        constexpr expr_tree(sv_type sv, parser_flags flags = {});

        friend class parser::ll1<char_type>;

        [[nodiscard]] constexpr const type& get_expr(std::size_t i) const { return expressions_.at(i); }
        [[nodiscard]] constexpr std::size_t root_idx() const { return root_idx_; }
        [[nodiscard]] constexpr std::size_t tag_count() const { return tag_count_; }
        [[nodiscard]] constexpr const capture_info& get_capture_info() const { return capture_info_; }

        constexpr void make_tag_vec(std::vector<std::vector<int>>& tag_vec) const;
        constexpr void optimise_tags();
        constexpr void insert_search_prefix();

    private:
        template<in_variant<type> T>
        static constexpr std::size_t ast_index{ index_of_impl<type, T>::value };

        [[nodiscard]] constexpr std::vector<std::optional<std::size_t>> make_const_len_vec();

        std::size_t root_idx_{ 0 };
        std::vector<type> expressions_;
        capture_info capture_info_;
        tag_number_t tag_count_{ 0 };
        parser_flags flags_;
    };

    template<typename CharT>
    expr_tree(const CharT*) -> expr_tree<CharT>;

    template<typename CharT>
    expr_tree(const CharT*, parser_flags) -> expr_tree<CharT>;
}

namespace rx::detail
{
    /* helper for tagged nfa conversion */

    template<typename CharT>
    constexpr void expr_tree<CharT>::make_tag_vec(std::vector<std::vector<int>>& tag_vec) const
    {
        tag_vec.clear();
        tag_vec.resize(expressions_.size());

        using stack_elem_t = std::pair<std::size_t, std::size_t>;

        std::vector<stack_elem_t> stack;
        stack.emplace_back(root_idx(), false);

        while (not stack.empty())
        {
            auto& [idx, pos]{ stack.back() };
            const auto& entry{ expressions_.at(idx) };

            switch (entry.index())
            {
            case ast_index<assertion>:
            case ast_index<char_str>:
            case ast_index<char_class>:
            case ast_index<backref>:
                stack.pop_back();
                break;

            case ast_index<concat>:
            {
                const auto& cat{ std::get<concat>(entry) };

                if (cat.idxs.empty())
                {
                    stack.pop_back();
                }
                else if (pos == cat.idxs.size())
                {
                    auto& vec{ tag_vec.at(idx) };

                    for (const std::size_t i : cat.idxs)
                        std::ranges::copy(tag_vec.at(i), std::back_inserter(vec));

                    std::ranges::sort(vec);
                    auto [_, last]{ std::ranges::unique(vec) };
                    vec.erase(last, vec.end());

                    stack.pop_back();
                }
                else
                {
                    pos += 1;
                    stack.emplace_back(cat.idxs.at(pos - 1), 0);
                }
                break;
            }

            case ast_index<alt>:
            {
                const auto& atl{ std::get<alt>(entry) };

                if (pos == atl.idxs.size())
                {
                    auto& vec{ tag_vec.at(idx) };

                    for (const std::size_t i : atl.idxs)
                        std::ranges::copy(tag_vec.at(i), std::back_inserter(vec));

                    std::ranges::sort(vec);
                    auto [_, last]{ std::ranges::unique(vec) };
                    vec.erase(last, vec.end());

                    stack.pop_back();
                }
                else
                {
                    pos += 1;
                    stack.emplace_back(atl.idxs.at(pos - 1), 0);
                }
                break;
            }

            case ast_index<repeat>:
            {
                const auto& rep{ std::get<repeat>(entry) };

                if (pos == 1)
                {
                    auto& vec{ tag_vec.at(idx) };
                    std::ranges::copy(tag_vec.at(rep.idx), std::back_inserter(vec));

                    stack.pop_back();
                }
                else
                {
                    pos += 1;
                    stack.emplace_back(rep.idx, 0);
                }
                break;
            }

            case ast_index<tag>:
            {
                const auto& tag_entry{ std::get<tag>(entry) };

                auto& vec{ tag_vec.at(idx) };
                vec.emplace_back(tag_entry.number);
                stack.pop_back();
                break;
            }

            default:
                throw tree_error("Invalid tree");
            }
        }
    }

    /* fixed tag optimisation (delete unnecessary tags) */

    template<typename CharT>
    constexpr std::vector<std::optional<std::size_t>> expr_tree<CharT>::make_const_len_vec()
    {
        using opt_t = std::optional<std::size_t>;
        std::vector<opt_t> const_len(expressions_.size());

        using stack_elem_t = std::pair<std::size_t, std::size_t>;

        std::vector<stack_elem_t> stack;
        stack.emplace_back(root_idx(), false);

        while (not stack.empty())
        {
            auto& [idx, pos]{ stack.back() };
            const auto& entry{ expressions_.at(idx) };

            switch (entry.index())
            {
            case ast_index<assertion>:
            case ast_index<tag>:
                const_len.at(idx) = 0;
                stack.pop_back();
                break;

            case ast_index<char_str>:
                const_len.at(idx) = std::get<char_str>(entry).data.size();
                stack.pop_back();
                break;

            case ast_index<char_class>:
                const_len.at(idx) = 1;
                stack.pop_back();
                break;

            case ast_index<backref>:
                stack.pop_back();
                break;

            case ast_index<concat>:
            {
                const auto& cat{ std::get<concat>(entry) };

                if (pos == cat.idxs.size())
                {
                    auto tmp{
                        cat.idxs | std::views::transform([&](std::size_t i) { return const_len.at(i); })
                        | std::ranges::to<std::vector>()
                    };

                    if (std::ranges::all_of(tmp, [](const opt_t& o) { return o.has_value(); }))
                        const_len.at(idx) = std::ranges::fold_left(tmp | std::views::transform([](const opt_t& o) { return *o; }),
                                                                   0, std::plus{});

                    stack.pop_back();
                }
                else
                {
                    pos += 1;
                    stack.emplace_back(cat.idxs.at(pos - 1), 0);
                }
                break;
            }

            case ast_index<alt>:
            {
                const auto& atl{ std::get<alt>(entry) };

                if (pos == atl.idxs.size())
                {
                    auto tmp{
                        atl.idxs | std::views::transform([&](std::size_t i) { return const_len.at(i); })
                        | std::ranges::to<std::vector>()
                    };

                    auto first{ *std::ranges::begin(tmp) };

                    if (std::ranges::size(tmp) > 0 and std::ranges::all_of(tmp, [&](const opt_t& o) { return o == first; }))
                        const_len.at(idx) = first;

                    stack.pop_back();
                }
                else
                {
                    pos += 1;
                    stack.emplace_back(atl.idxs.at(pos - 1), 0);
                }
                break;
            }

            case ast_index<repeat>:
            {
                const auto& rep{ std::get<repeat>(entry) };

                if (pos == 1)
                {
                    if (rep.min == rep.max and const_len.at(rep.idx))
                        const_len.at(idx) = *const_len.at(rep.idx) * rep.min;

                    stack.pop_back();
                }
                else
                {
                    pos += 1;
                    stack.emplace_back(rep.idx, 0);
                }
                break;
            }

            default:
                throw tree_error("Invalid tree");
            }
        }

        return const_len;
    }

    template<typename CharT>
    constexpr void expr_tree<CharT>::optimise_tags()
    {
        const auto const_len{ make_const_len_vec() };

        std::flat_map<tag_number_t, capture_info::pair_entry> tag_remap;

        for (std::size_t i{ 0 }; i < expressions_.size(); ++i)
        {
            if (not std::holds_alternative<concat>(expressions_.at(i)))
                continue;

            std::optional<capture_info::pair_entry> current;
            auto& target{ std::get<concat>(expressions_.at(i)).idxs };

            if (i == root_idx())
                current = { .tag_number = end_of_input_tag, .offset = 0 };

            for (std::size_t j{ target.size() }; j > 0; --j)
            {
                const std::size_t idx{ target.at(j - 1) };

                if (auto* tn{ std::get_if<tag>(&expressions_.at(idx)) }; tn != nullptr)
                {
                    if (current.has_value())
                    {
                        /* remap tag */

                        auto [_, success]{ tag_remap.try_emplace(tn->number, *current) };
                        if (not success)
                            throw tree_error("Tag appears more than once in AST");

                        tn->number = -1; /* set to an invalid value */
                        target.erase(target.begin() + j - 1);
                    }
                    else
                    {
                        current = { .tag_number = tn->number, .offset = 0 };
                    }
                }
                else if (current.has_value())
                {
                    if (const_len.at(idx))
                        current->offset -= *const_len.at(idx);
                    else
                        current = {};
                }
            }
        }

        /* re-number map and re-number tags in capture_info */

        const auto remapper{ capture_info_.remap_tags(tag_remap) };

        /* re-number tags in ast */

        for (auto& expr : expressions_)
        {
            if (auto* tn{ std::get_if<tag>(&expr) }; tn != nullptr)
            {
                if (auto it{ remapper.find(tn->number) }; it != remapper.end())
                    tn->number = it->second;
            }
        }

        if (remapper.size() > std::numeric_limits<tag_number_t>::max())
            throw tree_error("Tag number exceeded");

        tag_count_ = std::saturate_cast<tag_number_t>(remapper.size());
    }

    /* convert pattern for use in regex search */

    template<typename CharT>
    constexpr void expr_tree<CharT>::insert_search_prefix()
    {
        /* make true wildcard */
        const std::size_t wildcard_idx{ expressions_.size() };
        char_class cc{ true };
        cc.data.normalise();
        expressions_.emplace_back(std::in_place_type<char_class>, std::move(cc));

        /* make lazy repeater of wildcard */
        const std::size_t repeater_idx{ expressions_.size() };
        expressions_.emplace_back(std::in_place_type<repeat>, wildcard_idx, std::int_least16_t{ 0 }, std::int_least16_t{ -1 }, repeater_mode::lazy);

        /* conditionally make start tag */
        const std::size_t start_tag_idx{ expressions_.size() };
        if (flags_.enable_start_tag)
        {
            const tag_number_t start_tag{ tag_count_++ };
            if (start_tag < 0)
                throw tree_error("Capture limit exceed");
            capture_info_.set_match_start_tag(start_tag);
            expressions_.emplace_back(std::in_place_type<tag>, start_tag);
        }

        /* insert into pattern */
        if (type& ast{ expressions_.at(root_idx_) }; std::holds_alternative<concat>(ast))
        {
            /* root idx is already concat, so we can avoid creating a new concat as root */
            auto& target{ std::get<concat>(ast).idxs };
            if (flags_.enable_start_tag)
                target.insert(target.begin(), { repeater_idx, start_tag_idx });
            else
                target.insert(target.begin(), { repeater_idx });
        }
        else
        {
            /* make new root node */
            const std::size_t new_root_idx{ expressions_.size() };
            if (flags_.enable_start_tag)
                expressions_.emplace_back(std::in_place_type<concat>, std::vector{ repeater_idx, start_tag_idx, root_idx_ });
            else
                expressions_.emplace_back(std::in_place_type<concat>, std::vector{ repeater_idx, root_idx_ });
            root_idx_ = new_root_idx;
        }
    }
}

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
        flag_value multiline : 2; /* not fully implemented -> need to implement in matcher */
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

            constexpr cse() noexcept
                : number{ 1 },
                  number_end{ 1 },
                  flags{ .caseless = cf::disabled, .multiline = cf::disabled, .dotall = cf::disabled, .ungreedy = cf::disabled },
                  mode{ modes::non_capturing } {}

            constexpr explicit cse(std::uint_least16_t cur, std::uint_least16_t end) noexcept
                : number{ cur },
                  number_end{ end },
                  flags{ .caseless = cf::inherit, .multiline = cf::inherit, .dotall = cf::inherit, .ungreedy = cf::inherit },
                  mode{ modes::normal } {}
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
                target.number_end = std::max(target.number_end, elem.number_end);
                elem.number_end = elem.number;
            }
        }

        [[nodiscard]] constexpr std::optional<std::uint_least16_t> capture_count() const noexcept
        {
            if (elems_.empty())
                return base_.number_end;
            return {};
        }

        constexpr std::optional<std::uint_least16_t> pop()
        {
            if (elems_.empty())
                return {};

            auto elem{ elems_.back() };
            elems_.pop_back();

            auto& target{ elems_.empty() ? base_ : elems_.back() };

            /* overwrite containing capturing group's flags when elem is an empty capturing group */
            if (elem.mode == cse::modes::flag_assigning)
            {

                if (elem.flags.caseless  != cf::inherit) target.flags.caseless  = elem.flags.caseless;
                if (elem.flags.multiline != cf::inherit) target.flags.multiline = elem.flags.multiline;
                if (elem.flags.dotall    != cf::inherit) target.flags.dotall    = elem.flags.dotall;
                if (elem.flags.ungreedy  != cf::inherit) target.flags.ungreedy  = elem.flags.ungreedy;
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

        std::vector<cse> elems_{};
        cse              base_{};
    };
}

namespace rx::detail::parser
{
    template<typename CharT>
    class ll1
    {
    public:
        using char_type = CharT;
        using ast_t = expr_tree<char_type>;
        using sv_type = std::basic_string_view<char_type>;

        constexpr ll1(ast_t& ast, sv_type sv);

    private:
        using assertion  = ast_t::assertion;
        using alt        = ast_t::alt;
        using concat     = ast_t::concat;
        using tag        = ast_t::tag;
        using backref    = ast_t::backref;
        using repeat     = ast_t::repeat;
        using char_str   = ast_t::char_str;
        using char_class = ast_t::char_class;
        using type       = ast_t::type;

        [[nodiscard]] constexpr std::size_t sa_make_empty();
        [[nodiscard]] constexpr std::size_t sa_make_dot();
        [[nodiscard]] constexpr std::size_t sa_make_hat();
        [[nodiscard]] constexpr std::size_t sa_make_dollar();
        [[nodiscard]] constexpr std::size_t sa_make_assert(assertion&& as);
        [[nodiscard]] constexpr std::size_t sa_make_char_lit(char_str&& lit);
        [[nodiscard]] constexpr std::size_t sa_make_char_class(char_class&& cc);
        [[nodiscard]] constexpr std::size_t sa_make_alt(std::size_t lhs_idx, std::size_t rhs_idx);
        [[nodiscard]] constexpr std::size_t sa_make_concat(std::size_t lhs_idx, std::size_t rhs_idx);
        [[nodiscard]] constexpr std::size_t sa_make_bref(backref bref);
        [[nodiscard]] constexpr std::size_t sa_make_star(std::size_t child_idx, repeater_mode mode);
        [[nodiscard]] constexpr std::size_t sa_make_plus(std::size_t child_idx, repeater_mode mode);
        [[nodiscard]] constexpr std::size_t sa_make_quest(std::size_t child_idx, repeater_mode mode);
        [[nodiscard]] constexpr std::size_t sa_make_repeat(std::size_t child_idx, tok::repeat_n_m rep, repeater_mode mode);
        [[nodiscard]] constexpr repeater_mode sa_rep_greedy() const;
        [[nodiscard]] constexpr repeater_mode sa_rep_lazy() const;
        [[nodiscard]] constexpr repeater_mode sa_rep_possessive() const;
        [[nodiscard]] constexpr std::size_t sa_cap_pop(std::size_t child_idx);
        constexpr void sa_cap_push();
        constexpr void sa_cap_parse_flag();
        constexpr void sa_begin_alt();

        [[nodiscard]] constexpr parser_flags& flags() { return ref_.get().flags_; }
        [[nodiscard]] constexpr const parser_flags& flags() const { return ref_.get().flags_; }
        [[nodiscard]] constexpr type& get_expr(std::size_t idx) { return ref_.get().expressions_.at(idx); }
        [[nodiscard]] constexpr std::size_t& root_idx() { return ref_.get().root_idx_; }
        [[nodiscard]] constexpr capture_info& get_capture_info() { return ref_.get().capture_info_; }
        [[nodiscard]] constexpr tag_number_t& tag_count() { return ref_.get().tag_count_; }

        template<in_variant<typename lexer<char_type>::token_t> T>
        static constexpr std::size_t tok_index{ index_of_impl<typename lexer<char_type>::token_t, T>::value };

        template<in_variant<type> T, typename... Args>
        [[nodiscard]] constexpr std::size_t new_expression(Args... args)
        {
            auto& exprs{ ref_.get().expressions_ };

            if (overwritable_.empty())
            {
                const std::size_t idx{ exprs.size() };
                exprs.emplace_back(std::in_place_type<T>, std::forward<Args>(args)...);
                return idx;
            }
            else
            {
                const std::size_t idx{ overwritable_.back() };
                overwritable_.pop_back();
                exprs.at(idx) = T{ std::forward<Args>(args)... };
                return idx;
            }
        }

        std::reference_wrapper<ast_t> ref_;
        std::vector<std::size_t> overwritable_;
        lexer<char_type> lex_;
        capture_stack capstack_;
    };

    /* helper classes (and enum) for parser */

    enum class nonterminal : std::uint_least8_t { S, E, E_, F, F_, G, R, R_, H, P };

    enum class semantic_action : std::int_least8_t
    {
        make_empty,
        make_dot,
        make_hat,
        make_dollar,
        make_assert,

        make_char_lit,
        make_char_class,

        make_alt,
        make_concat,

        make_bref,

        make_star,
        make_plus,
        make_quest,
        make_repeat,

        rep_greedy,
        rep_lazy,
        rep_possessive,

        cap_push,
        cap_pop,
        cap_parse_flag,
        cap_parse_flag_done,

        begin_alt,
    };

    template<typename CharT>
    class ll1_stack
    {
    public:
        using char_type = CharT;
        using terminal = lexer<char_type>::token_t;
        using elem_t = std::variant<terminal, nonterminal, semantic_action>;

        constexpr void pop() { data_.pop_back(); }
        constexpr void push(const std::vector<elem_t>& elems) { data_.append_range(elems | std::views::reverse); }
        [[nodiscard]] constexpr auto& root() { return data_.back(); }
        [[nodiscard]] constexpr const auto& root() const { return data_.back(); }

        constexpr auto begin() const noexcept { return data_.cbegin(); }
        constexpr auto end() const noexcept { return data_.cend(); }
        constexpr auto rbegin() const noexcept { return data_.crbegin(); }
        constexpr auto rend() const noexcept { return data_.crend(); }
        constexpr auto cbegin() const noexcept { return data_.cbegin(); }
        constexpr auto cend() const noexcept { return data_.cend(); }
        constexpr auto crbegin() const noexcept { return data_.crbegin(); }
        constexpr auto crend() const noexcept { return data_.crend(); }
        [[nodiscard]] constexpr bool empty() const noexcept { return data_.empty(); }

    private:
        std::vector<elem_t> data_{ nonterminal::S };
    };

    template<typename CharT>
    class semantic_stack
    {
    public:
        using char_type = CharT;
        using terminal = ll1_stack<char_type>::terminal;
        using elem_t = std::variant<std::size_t, typename lexer<char_type>::token_t, repeater_mode>;

        [[nodiscard]] constexpr elem_t pop() { elem_t tmp{ std::move(data_.back()) }; data_.pop_back(); return tmp; }
        constexpr void push(elem_t&& elems) { data_.push_back(std::move(elems)); }

        constexpr auto begin() const noexcept { return data_.cbegin(); }
        constexpr auto end() const noexcept { return data_.cend(); }
        constexpr auto rbegin() const noexcept { return data_.crbegin(); }
        constexpr auto rend() const noexcept { return data_.crend(); }
        constexpr auto cbegin() const noexcept { return data_.cbegin(); }
        constexpr auto cend() const noexcept { return data_.cend(); }
        constexpr auto crbegin() const noexcept { return data_.crbegin(); }
        constexpr auto crend() const noexcept { return data_.crend(); }
        [[nodiscard]] constexpr bool empty() const noexcept { return data_.empty(); }

    private:
        std::vector<elem_t> data_{};
    };

    /* parser implemenation */

    template<typename CharT>
    constexpr ll1<CharT>::ll1(ast_t& ast, const sv_type sv)
        : ref_{ ast }, lex_{ sv }
    {
        using terminal = ll1_stack<char_type>::terminal;

        if (sv.empty())
        {
            /* ensure expressions is non-empty */
            std::ignore = new_expression<char_str>(/* empty string */);
            return;
        }

        ll1_stack<char_type> stack;
        semantic_stack<char_type> semstack;

        auto token{ lex_.nexttok() };

        for (bool loop{ true }; loop;)
        {
            if (stack.empty())
                throw pattern_error("Invalid pattern");

            const auto top{ std::move(stack.root()) };
            stack.pop();

            if (const auto* const term{ std::get_if<terminal>(&top) })
            {
                if (std::holds_alternative<tok::end_of_input>(*term))
                {
                    /* parsing is done */
                    loop = false;
                }
                else if (term->index() == token.index())
                {
                    /* match */
                    semstack.push(std::move(token));
                    token = lex_.nexttok();
                }
                else
                {
                    /* no match */
                    throw pattern_error("Parse Error");
                }
            }
            else if (const auto* const nonterm{ std::get_if<nonterminal>(&top) })
            {
                /* predict */

                using nt = nonterminal;
                using sa = semantic_action;
                using namespace tok;

                switch (*nonterm)
                {
                case nt::S:
                    switch (token.index())
                    {
                    case tok_index<end_of_input>:
                    case tok_index<dot>:
                    case tok_index<hat>:
                    case tok_index<dollar>:
                    case tok_index<assertion>:
                    case tok_index<lparen>:
                    case tok_index<vert>:
                    case tok_index<char_str>:
                    case tok_index<char_class>:
                    case tok_index<backref>:
                        stack.push({ nt::E, end_of_input{} });
                        break;
                    default:
                        throw pattern_error("Invalid pattern");
                    }
                    break;
                case nt::E:
                    switch (token.index())
                    {
                    case tok_index<end_of_input>:
                    case tok_index<dot>:
                    case tok_index<hat>:
                    case tok_index<dollar>:
                    case tok_index<assertion>:
                    case tok_index<lparen>:
                    case tok_index<rparen>:
                    case tok_index<vert>:
                    case tok_index<char_str>:
                    case tok_index<char_class>:
                    case tok_index<backref>:
                        stack.push({ nt::F, nt::E_ });
                        break;
                    default:
                        throw pattern_error("Invalid pattern");
                    }
                    break;
                case nt::E_:
                    switch (token.index())
                    {
                    case tok_index<end_of_input>:
                    case tok_index<rparen>:
                        /* epsilon */
                        break;
                    case tok_index<vert>:
                        stack.push({ sa::begin_alt, vert{}, nt::E, sa::make_alt });
                        break;
                    default:
                        throw pattern_error("Invalid pattern");
                    }
                    break;
                case nt::F:
                    switch (token.index())
                    {
                    case tok_index<end_of_input>:
                    case tok_index<rparen>:
                    case tok_index<vert>:
                        stack.push({ /* epsilon */ sa::make_empty });
                        break;
                    case tok_index<dot>:
                    case tok_index<hat>:
                    case tok_index<dollar>:
                    case tok_index<assertion>:
                    case tok_index<lparen>:
                    case tok_index<char_str>:
                    case tok_index<char_class>:
                    case tok_index<backref>:
                        stack.push({ nt::G, nt::F_ });
                        break;
                    default:
                        throw pattern_error("Invalid pattern");
                    }
                    break;
                case nt::F_:
                    switch (token.index())
                    {
                    case tok_index<end_of_input>:
                    case tok_index<rparen>:
                    case tok_index<vert>:
                        /* epsilon */
                        break;
                    case tok_index<dot>:
                    case tok_index<hat>:
                    case tok_index<dollar>:
                    case tok_index<assertion>:
                    case tok_index<lparen>:
                    case tok_index<char_str>:
                    case tok_index<char_class>:
                    case tok_index<backref>:
                        stack.push({ nt::G, nt::F_, sa::make_concat });
                        break;
                    default:
                        throw pattern_error("Invalid pattern");
                    }
                    break;
                case nt::G:
                    switch (token.index())
                    {
                    case tok_index<dot>:
                    case tok_index<hat>:
                    case tok_index<dollar>:
                    case tok_index<assertion>:
                    case tok_index<lparen>:
                    case tok_index<char_str>:
                    case tok_index<char_class>:
                    case tok_index<backref>:
                        stack.push({ nt::H, nt::R });
                        break;
                    default:
                        throw pattern_error("Invalid pattern");
                    }
                    break;
                case nt::R:
                    switch (token.index())
                    {
                    case tok_index<end_of_input>:
                    case tok_index<dot>:
                    case tok_index<hat>:
                    case tok_index<dollar>:
                    case tok_index<assertion>:
                    case tok_index<lparen>:
                    case tok_index<rparen>:
                    case tok_index<vert>:
                    case tok_index<char_str>:
                    case tok_index<char_class>:
                    case tok_index<backref>:
                        /* epsilon */
                        break;
                    case tok_index<star>:
                        stack.push({ star{}, nt::R_, sa::make_star });
                        break;
                    case tok_index<plus>:
                        stack.push({ plus{}, nt::R_, sa::make_plus });
                        break;
                    case tok_index<quest>:
                        stack.push({ quest{}, nt::R_, sa::make_quest });
                        break;
                    case tok_index<repeat_n_m>:
                        stack.push({ repeat_n_m{}, nt::R_, sa::make_repeat });
                        break;
                    }
                    break;
                case nt::R_:
                    switch (token.index())
                    {
                    case tok_index<end_of_input>:
                    case tok_index<dot>:
                    case tok_index<hat>:
                    case tok_index<dollar>:
                    case tok_index<assertion>:
                    case tok_index<lparen>:
                    case tok_index<rparen>:
                    case tok_index<vert>:
                    case tok_index<char_str>:
                    case tok_index<char_class>:
                    case tok_index<backref>:
                        stack.push({ /* epsilon, */ sa::rep_greedy });
                        break;
                    case tok_index<quest>:
                        stack.push({ quest{}, sa::rep_lazy });
                        break;
                    case tok_index<plus>:
                        stack.push({ plus{}, sa::rep_possessive });
                        break;
                    default:
                        throw pattern_error("Invalid pattern");
                    }
                    break;
                case nt::H:
                    switch (token.index())
                    {
                    case tok_index<dot>:
                        stack.push({ dot{}, sa::make_dot });
                        break;
                    case tok_index<hat>:
                        stack.push({ hat{}, sa::make_hat });
                        break;
                    case tok_index<dollar>:
                        stack.push({ dollar{}, sa::make_dollar });
                        break;
                    case tok_index<assertion>:
                        stack.push({ assertion{}, sa::make_assert });
                        break;
                    case tok_index<lparen>:
                        stack.push({ sa::cap_push, lparen{}, nt::P, rparen{}, sa::cap_pop });
                        break;
                    case tok_index<char_str>:
                        stack.push({ char_str{}, sa::make_char_lit });
                        break;
                    case tok_index<char_class>:
                        stack.push({ char_class{}, sa::make_char_class });
                        break;
                    case tok_index<backref>:
                        stack.push({ backref{}, sa::make_bref });
                        break;
                    default:
                        throw pattern_error("Invalid pattern");
                    }
                    break;
                case nt::P:
                    switch (token.index())
                    {
                    case tok_index<assertion>:
                    case tok_index<dot>:
                    case tok_index<hat>:
                    case tok_index<dollar>:
                    case tok_index<lparen>:
                    case tok_index<rparen>:
                    case tok_index<char_str>:
                    case tok_index<char_class>:
                    case tok_index<backref>:
                        stack.push({ nt::E });
                        break;
                    case tok_index<quest>:
                        stack.push({ sa::cap_parse_flag, quest{}, sa::cap_parse_flag_done, nt::E });
                        break;
                    default:
                        throw pattern_error("Invalid pattern");
                    }
                    break;
                }
            }
            else if (const auto* const action{ std::get_if<semantic_action>(&top) })
            {
                using sa = semantic_action;

                switch (*action)
                {
                case sa::make_empty:
                {
                    semstack.push(sa_make_empty());
                    break;
                }
                case sa::make_dot:
                {
                    std::ignore = semstack.pop(); /* pop tok::dot */
                    semstack.push(sa_make_dot());
                    break;
                }
                case sa::make_hat:
                {
                    std::ignore = semstack.pop(); /* pop tok::hat */
                    semstack.push(sa_make_hat());
                    break;
                }
                case sa::make_dollar:
                {
                    std::ignore = semstack.pop(); /* pop tok::dollar */
                    semstack.push(sa_make_dollar());
                    break;
                }
                case sa::make_assert:
                {
                    assertion as{ std::get<assertion>(std::get<terminal>(semstack.pop())) }; /* pop tok::assertion */
                    semstack.push(sa_make_assert(std::move(as)));
                    break;
                }
                case sa::make_char_lit:
                {
                    char_str lit{ std::get<char_str>(std::get<terminal>(semstack.pop())) }; /* pop tok::char_str */
                    semstack.push(sa_make_char_lit(std::move(lit)));
                    break;
                }
                case sa::make_char_class:
                {
                    char_class cc{ std::get<char_class>(std::get<terminal>(semstack.pop())) }; /* pop tok::char_str */
                    semstack.push(sa_make_char_class(std::move(cc)));
                    break;
                }
                case sa::make_alt:
                {
                    const auto rhs_idx{ std::get<std::size_t>(semstack.pop()) };
                    std::ignore = semstack.pop(); /* pop tok::vert */
                    const auto lhs_idx{ std::get<std::size_t>(semstack.pop()) };
                    semstack.push(sa_make_alt(lhs_idx, rhs_idx));
                    break;
                }
                case sa::make_concat:
                {
                    const auto rhs_idx{ std::get<std::size_t>(semstack.pop()) };
                    const auto lhs_idx{ std::get<std::size_t>(semstack.pop()) };
                    semstack.push(sa_make_concat(lhs_idx, rhs_idx));
                    break;
                }
                case sa::make_bref:
                {
                    const auto bref{ std::get<tok::backref>(std::get<terminal>(semstack.pop())) }; /* pop tok::backref */
                    semstack.push(sa_make_bref(bref));
                    break;
                }
                case sa::make_star:
                {
                    const auto mode{ std::get<repeater_mode>(semstack.pop()) };
                    std::ignore = semstack.pop(); /* pop tok::star */
                    const auto child_idx{ std::get<std::size_t>(semstack.pop()) };
                    semstack.push(sa_make_star(child_idx, mode));
                    break;
                }
                case sa::make_plus:
                {
                    const auto mode{ std::get<repeater_mode>(semstack.pop()) };
                    std::ignore = semstack.pop(); /* pop tok::plus */
                    const auto child_idx{ std::get<std::size_t>(semstack.pop()) };
                    semstack.push(sa_make_plus(child_idx, mode));
                    break;
                }
                case sa::make_quest:
                {
                    const auto mode{ std::get<repeater_mode>(semstack.pop()) };
                    std::ignore = semstack.pop(); /* pop tok::quest */
                    const auto child_idx{ std::get<std::size_t>(semstack.pop()) };
                    semstack.push(sa_make_quest(child_idx, mode));
                    break;
                }
                case sa::make_repeat:
                {
                    const auto mode{ std::get<repeater_mode>(semstack.pop()) };
                    const auto rep{ std::get<tok::repeat_n_m>(std::get<terminal>(semstack.pop())) };
                    const auto child_idx{ std::get<std::size_t>(semstack.pop()) };
                    semstack.push(sa_make_repeat(child_idx, rep, mode));
                    break;
                }
                case sa::rep_greedy:
                {
                    semstack.push(sa_rep_greedy());
                    break;
                }
                case sa::rep_lazy:
                {
                    std::ignore = semstack.pop(); /* pop tok::quest */
                    semstack.push(sa_rep_lazy());
                    break;
                }
                case sa::rep_possessive:
                {
                    std::ignore = semstack.pop(); /* pop tok::plus */
                    semstack.push(sa_rep_possessive());
                    break;
                }
                case sa::cap_push:
                {
                    sa_cap_push();
                    break;
                }
                case sa::cap_pop:
                {
                    std::ignore = semstack.pop(); /* pop tok::rparen */
                    const auto child_idx{ std::get<std::size_t>(semstack.pop()) };
                    std::ignore = semstack.pop(); /* pop tok::lparen */
                    semstack.push(sa_cap_pop(child_idx));
                    break;
                }
                case sa::cap_parse_flag:
                {
                    sa_cap_parse_flag();
                    break;
                }
                case sa::cap_parse_flag_done:
                {
                    std::ignore = semstack.pop(); /* pop tok::quest */
                    /* this exists only so we can use capture_pop(_empty)? for captures with flags */
                    break;
                }
                case sa::begin_alt:
                {
                    /* exists only so branch resetting captures work */
                    sa_begin_alt();
                    break;
                }
                }
            }
            else
            {
                /* no match */
                throw pattern_error("Parse Error");
            }
        }

        if (not semstack.empty())
            root_idx() = std::get<std::size_t>(semstack.pop());
    }

    /* semantic action implementations */

    template<typename CharT>
    constexpr std::size_t ll1<CharT>::sa_make_empty()
    {
        return new_expression<char_str>(/* empty string */);
    }

    template<typename CharT>
    constexpr std::size_t ll1<CharT>::sa_make_dot()
    {
        /* depending on flags, insert true wildcard instead of [^\n] */
        if (capstack_.dotall())
        {
            char_class result{ true };
            result.data.normalise();
            return new_expression<char_class>(std::move(result));
        }
        else
        {
            char_class result{ true };
            result.data.insert('\n');
            result.data.normalise();
            return new_expression<char_class>(std::move(result));
        }
    }

    template<typename CharT>
    constexpr std::size_t ll1<CharT>::sa_make_hat()
    {
        /* depending on flags, insert assert_type::line_start instead of assert_type::text_start */
        if (capstack_.multiline())
            return new_expression<assertion>(assert_type::line_start);
        else
            return new_expression<assertion>(assert_type::text_start);
    }

    template<typename CharT>
    constexpr std::size_t ll1<CharT>::sa_make_dollar()
    {
        /* depending on flags, insert assert_type::line_end instead of assert_type::text_end */
        if (capstack_.multiline())
            return new_expression<assertion>(assert_type::line_end);
        else
            return new_expression<assertion>(assert_type::text_end);
    }

    template<typename CharT>
    constexpr std::size_t ll1<CharT>::sa_make_assert(assertion&& as)
    {
        /* todo: perform checks on implementability here */
        return new_expression<assertion>(std::move(as));
    }

    template<typename CharT>
    constexpr std::size_t ll1<CharT>::sa_make_char_lit(char_str&& lit)
    {
        if (capstack_.caseless())
        {
            static constexpr auto is_alphabetic = [](char_type c) -> bool { return ('A' <= c and c <= 'Z') or ('a' <= c and c <= 'z'); };

            if (auto c{ lit.get_if_single() })
            {
                /* single character (therefore easier to implement) */

                if (is_alphabetic(*c))
                {
                    const auto new_idx{ new_expression<char_class>() };
                    auto& target{ std::get<char_class>(get_expr(new_idx)).data };

                    target.insert(*c);
                    target.make_caseless();
                    return new_idx;
                }
            }
            else if (not lit.data.empty())
            {
                /* several characters (need to insert concat) */

                if constexpr (char_is_multibyte<char_type>)
                {

                    throw tree_error("Caseless flag on multibyte strings not implemented");
                }

                auto lit_it{ std::ranges::begin(lit.data) };
                const auto lit_end{ std::ranges::end(lit.data) };

                if (std::ranges::any_of(lit_it, lit_end, is_alphabetic))
                {
                    /* create new concat to insert caseless string into */
                    const auto cat_idx{ new_expression<concat>() };

                    while (lit_it != lit_end)
                    {
                        const auto c{ *lit_it++ };

                        if (is_alphabetic(c))
                        {
                            /* insert character class of [cC] into cat */
                            const auto new_idx{ new_expression<char_class>() };
                            auto& target{ std::get<char_class>(get_expr(new_idx)).data };
                            std::get<concat>(get_expr(cat_idx)).idxs.push_back(new_idx);

                            target.insert(c);
                            target.make_caseless();
                        }
                        else
                        {
                            /* insert character string into cat */
                            const auto new_idx{ new_expression<char_str>() };
                            auto& target{ std::get<char_str>(get_expr(new_idx)).data };

                            target.push_back(c);

                            while (lit_it != lit_end and is_alphabetic(*lit_it))
                                target.push_back(*lit_it++);

                            std::get<concat>(get_expr(cat_idx)).idxs.push_back(new_idx);
                        }
                    }

                    return cat_idx;
                }
            }
        }

        return new_expression<char_str>(std::move(lit));
    }

    template<typename CharT>
    constexpr std::size_t ll1<CharT>::sa_make_char_class(char_class&& cc)
    {
        if (capstack_.caseless())
            cc.data.make_caseless();

        return new_expression<char_class>(std::move(cc));
    }

    template<typename CharT>
    constexpr std::size_t ll1<CharT>::sa_make_alt(const std::size_t lhs_idx, const std::size_t rhs_idx)
    {
        if (type& ast{ get_expr(rhs_idx) }; std::holds_alternative<alt>(ast))
        {
            auto& ast_alt{ std::get<alt>(ast) };

            if (flags().enable_alttocc and not ast_alt.idxs.empty())
            {
                /* attempt to replace a|b with [ab] */
                type& rhs{ get_expr(ast_alt.idxs.front()) };
                type& lhs{ get_expr(lhs_idx) };

                if (std::holds_alternative<char_class>(rhs))
                {
                    auto& target{ std::get<char_class>(rhs).data };

                    if (std::holds_alternative<char_class>(lhs))
                    {
                        /* merge char classes */
                        auto& other{ std::get<char_class>(lhs).data };
                        target.insert(other);
                        overwritable_.push_back(lhs_idx);
                        return rhs_idx;
                    }
                    else if (std::holds_alternative<char_str>(lhs))
                    {
                        if (auto to_insert{ std::get<char_str>(lhs).get_if_single() })
                        {
                            /* insert char into char class */
                            target.insert(*to_insert);
                            overwritable_.push_back(lhs_idx);
                            return rhs_idx;
                        }
                    }
                }
                else if (std::holds_alternative<char_str>(rhs))
                {
                    const auto saved_idx{ ast_alt.idxs.front() };

                    if (auto to_insert{ std::get<char_str>(rhs).get_if_single() })
                    {
                        if (std::holds_alternative<char_class>(lhs))
                        {
                            /* replace rhs with lhs in alt, and insert char into char class */
                            auto& target{ std::get<char_class>(lhs).data };
                            ast_alt.idxs.front() = lhs_idx;
                            target.insert(*to_insert);
                            overwritable_.push_back(saved_idx);
                            return rhs_idx;
                        }
                        else if (std::holds_alternative<char_str>(lhs))
                        {
                            if (auto other_insert{ std::get<char_str>(lhs).get_if_single() })
                            {
                                /* replace rhs with new char class in alt */
                                const auto new_idx{ new_expression<char_class>() };
                                auto& target{ std::get<char_class>(get_expr(new_idx)).data };

                                /* calling new_expression invalidates references, so we must re-get ast_alt */
                                auto& ast_alt2{ std::get<alt>(get_expr(rhs_idx)) };
                                ast_alt2.idxs.front() = new_idx;

                                target.insert(*to_insert);
                                target.insert(*other_insert);
                                overwritable_.push_back(lhs_idx);
                                overwritable_.push_back(saved_idx);
                                return rhs_idx;
                            }
                        }
                    }
                }
            }

            /* insert lhs into existing alt */
            ast_alt.idxs.insert(ast_alt.idxs.begin(), lhs_idx);
            return rhs_idx;
        }
        else
        {
            if (flags().enable_alttocc)
            {
                /* attempt to replace a|b with [ab] */
                type& rhs{ get_expr(rhs_idx) };
                type& lhs{ get_expr(lhs_idx) };

                if (std::holds_alternative<char_class>(rhs))
                {
                    auto& target{ std::get<char_class>(rhs).data };

                    if (std::holds_alternative<char_class>(lhs))
                    {
                        /* merge char classes */
                        auto& other{ std::get<char_class>(lhs).data };
                        target.insert(other);
                        overwritable_.push_back(lhs_idx);
                        return rhs_idx;
                    }
                    else if (std::holds_alternative<char_str>(lhs))
                    {
                        if (auto to_insert{ std::get<char_str>(lhs).get_if_single() })
                        {
                            /* insert char into char class */
                            target.insert(*to_insert);
                            overwritable_.push_back(lhs_idx);
                            return rhs_idx;
                        }
                    }
                }
                else if (std::holds_alternative<char_str>(rhs))
                {
                    if (auto to_insert{ std::get<char_str>(rhs).get_if_single() })
                    {
                        if (std::holds_alternative<char_class>(lhs))
                        {
                            /* insert (rhs) char into (lhs) char class */
                            auto& target{ std::get<char_class>(lhs).data };
                            target.insert(*to_insert);
                            overwritable_.push_back(rhs_idx);
                            return lhs_idx;
                        }
                        else if (std::holds_alternative<char_str>(lhs))
                        {
                            if (auto other_insert{ std::get<char_str>(lhs).get_if_single() })
                            {
                                /* create new char class */
                                const auto new_idx{ new_expression<char_class>() };
                                auto& target{ std::get<char_class>(get_expr(new_idx)).data };

                                target.insert(*to_insert);
                                target.insert(*other_insert);
                                overwritable_.push_back(lhs_idx);
                                overwritable_.push_back(rhs_idx);
                                return new_idx;
                            }
                        }
                    }
                }
            }

            /* create new alt */
            return new_expression<alt>(std::vector<std::size_t>{ lhs_idx, rhs_idx });
        }
    }

    template<typename CharT>
    constexpr std::size_t ll1<CharT>::sa_make_concat(const std::size_t lhs_idx, const std::size_t rhs_idx)
    {
        if (type& ast{ get_expr(lhs_idx) }; std::holds_alternative<concat>(ast))
        {
            /* append rhs into existing concat */
            /* this case _should_ only arise from captures, so we can skip merging strings */
            auto& target{ std::get<concat>(ast).idxs };

            type& rhs{ get_expr(rhs_idx) };

            if (std::holds_alternative<concat>(rhs))
            {
                /* append contents of rhs concat to lhs concat  */
                auto& src{ std::get<concat>(rhs).idxs };
                target.append_range(src);
                src.clear();
                overwritable_.push_back(rhs_idx);
            }
            else
            {
                /* append rhs to existing concat */
                target.push_back(rhs_idx);
            }

            return lhs_idx;
        }
        else if (type& ast{ get_expr(rhs_idx) }; std::holds_alternative<concat>(ast))
        {
            /* insert lhs into existing concat */
            auto& ast_concat{ std::get<concat>(ast) };
            bool merged{ false };

            if (not ast_concat.idxs.empty())
            {
                type& rhs{ get_expr(ast_concat.idxs.front()) };
                type& lhs{ get_expr(lhs_idx) };

                if (std::holds_alternative<char_str>(rhs) and std::holds_alternative<char_str>(lhs))
                {
                    /* merge lhs string with first entry of rhs (also a string) */
                    auto& target{ std::get<char_str>(rhs).data };
                    auto& lhs_str{ std::get<char_str>(lhs).data };
                    lhs_str.append(target);
                    std::swap(lhs_str, target);
                    overwritable_.push_back(lhs_idx);
                    merged = true;
                }
            }

            if (not merged)
            {
                /* insert lhs into existing concat */
                ast_concat.idxs.insert(ast_concat.idxs.begin(), lhs_idx);
            }

            return rhs_idx;
        }
        else
        {
            type& rhs{ get_expr(rhs_idx) };
            type& lhs{ get_expr(lhs_idx) };

            if (std::holds_alternative<char_str>(rhs) and std::holds_alternative<char_str>(lhs))
            {
                /* lhs and rhs are both strings: merge strings into one instead of creating concat  */
                auto& target{ std::get<char_str>(rhs).data };
                auto& lhs_str{ std::get<char_str>(lhs).data };
                lhs_str.append(target);
                std::swap(lhs_str, target);
                overwritable_.push_back(lhs_idx);
                return rhs_idx;
            }
            else
            {
                /* create new concat */
                return new_expression<concat>(std::vector{ lhs_idx, rhs_idx });
            }
        }
    }

    template<typename CharT>
    constexpr std::size_t ll1<CharT>::sa_make_bref(const backref bref)
    {
        if (flags().enable_backrefs)
        {
            /* note: we don't perform a check (bref.number <= capture_count) to match the behaviour of other engines */

            return new_expression<backref>(bref.number);
        }
        else
        {
            throw parser_error("Backreferences are not enabled");
        }
    }

    template<typename CharT>
    constexpr std::size_t ll1<CharT>::sa_make_star(const std::size_t child_idx, const repeater_mode mode)
    {
        return new_expression<repeat>(child_idx, std::int_least16_t{ 0 }, std::int_least16_t{ -1 }, mode);
    }

    template<typename CharT>
    constexpr std::size_t ll1<CharT>::sa_make_plus(const std::size_t child_idx, const repeater_mode mode)
    {
        return new_expression<repeat>(child_idx, std::int_least16_t{ 1 }, std::int_least16_t{ 0 }, mode);
    }

    template<typename CharT>
    constexpr std::size_t ll1<CharT>::sa_make_quest(const std::size_t child_idx, const repeater_mode mode)
    {
        return new_expression<repeat>(child_idx, std::int_least16_t{ 0 }, std::int_least16_t{ 1 }, mode);
    }

    template<typename CharT>
    constexpr std::size_t ll1<CharT>::sa_make_repeat(const std::size_t child_idx, const tok::repeat_n_m rep, const repeater_mode mode)
    {
        return new_expression<repeat>(child_idx, rep.min, rep.max, mode);
    }

    template<typename CharT>
    constexpr repeater_mode ll1<CharT>::sa_rep_greedy() const
    {
        /* swap greedy and lazy quantifiers if 'ungreedy' flag is set */
        if (capstack_.ungreedy())
            return repeater_mode::lazy; /* swapped */
        else
            return repeater_mode::greedy; /* default */
    }

    template<typename CharT>
    constexpr repeater_mode ll1<CharT>::sa_rep_lazy() const
    {
        /* swap greedy and lazy quantifiers if 'ungreedy' flag is set */
        if (capstack_.ungreedy())
            return repeater_mode::greedy; /* swapped */
        else
            return repeater_mode::lazy; /* default */
    }

    template<typename CharT>
    constexpr repeater_mode ll1<CharT>::sa_rep_possessive() const
    {
        if (flags().enable_possessive)
            return repeater_mode::possessive;
        else
            throw parser_error("Possessive repetition is not enabled");
    }

    template<typename CharT>
    constexpr std::size_t ll1<CharT>::sa_cap_pop(const std::size_t child_idx)
    {
        const auto cap_number{ capstack_.pop() };

        if (cap_number.has_value())
        {
            type& ast{ get_expr(child_idx) };

            if (char_str* lit{ std::get_if<char_str>(&ast) }; lit != nullptr and lit->data.empty())
            {
                /* empty capturing group; only insert one tag */

                const tag_number_t tag_num{ tag_count()++ };
                get_capture_info().insert(*cap_number, tag_num, tag_num);

                if (tag_num < 0)
                    throw tree_error("Capture limit exceed");

                overwritable_.push_back(child_idx);
                return new_expression<tag>(tag_num);
            }
            else if (std::holds_alternative<concat>(ast))
            {
                /* insert tags on either end of existing concat */
                const tag_number_t lhs_tag{ tag_count()++ };
                const tag_number_t rhs_tag{ tag_count()++ };
                get_capture_info().insert(*cap_number, lhs_tag, rhs_tag);

                if (lhs_tag < 0 or rhs_tag < 0)
                    throw tree_error("Capture limit exceed");

                const auto lhs_tag_entry{ new_expression<tag>(lhs_tag) };
                const auto rhs_tag_entry{ new_expression<tag>(rhs_tag) };

                /* calling new_expression invalidates references, so we must re-get ast for target */
                auto& target{ std::get<concat>(get_expr(child_idx)).idxs };
                target.insert(target.cbegin(), lhs_tag_entry);
                target.insert(target.cend(), rhs_tag_entry);
                return child_idx;
            }
            else
            {
                /* create new concat and put tags on either side */
                const tag_number_t lhs_tag{ tag_count()++ };
                const tag_number_t rhs_tag{ tag_count()++ };
                get_capture_info().insert(*cap_number, lhs_tag, rhs_tag);

                if (lhs_tag < 0 or rhs_tag < 0)
                    throw tree_error("Capture limit exceed");

                const auto lhs_tag_entry{ new_expression<tag>(lhs_tag) };
                const auto rhs_tag_entry{ new_expression<tag>(rhs_tag) };
                return new_expression<concat>(std::vector{ lhs_tag_entry, child_idx, rhs_tag_entry });
            }
        }
        else
        {
            return child_idx; /* non capturing group */
        }
    }

    template<typename CharT>
    constexpr void ll1<CharT>::sa_cap_push()
    {
        if (flags().enable_captures)
        {
            if (capstack_.push()) /* i.e. unsigned integer overflow has occurred */
                throw pattern_error("Capturing group limit exceeded");
        }
        else
        {
            capstack_.push_non_capturing();
        }
    }

    template<typename CharT>
    constexpr void ll1<CharT>::sa_cap_parse_flag()
    {
        /* manually parse flags */

        auto& lit{ lex_.it_ };
        const auto& lend{ lex_.end_ };

        bool flag_value{ true };

        if (lit == lend)
            return;

        switch (*lit)
        {
        case '#':
            ++lit;
            while (lit != lend and *lit != ')')
                ++lit; /* skip comment */
            capstack_.set_non_capturing();
            break;

        case '|':
            if (not flags().enable_branchreset)
                throw parser_error("Branch resetting in captures is not enabled");
            ++lit;
            capstack_.set_branch_reset();
            break;

        case '>':
            throw pattern_error("Atomic capture groups are unsupported");

        case 'P':
        case '<':
        case '\'':
            throw pattern_error("Named capture groups are unsupported");

        default: /* parse options */
            capstack_.set_flag_assigning();
            for (bool loop{ true }; loop;)
            {
                if (lit == lend)
                    throw pattern_error("Invalid Pattern");

                const auto lookahead{ *lit };
                bool increment{ true };

                switch (lookahead)
                {
                case 'i':
                    capstack_.set_caseless(flag_value);
                    break;
                case 'm':
                    capstack_.set_multiline(flag_value);
                    break;
                case 's':
                    capstack_.set_dotall(flag_value);
                    break;
                case 'U':
                    capstack_.set_ungreedy(flag_value);
                    break;
                case 'x':
                    throw pattern_error("Capturing group flag 'x' is unsupported");
                case '-':
                    if (not flag_value)
                        throw pattern_error("Capturing group arguments can only contain one hyphen");
                    flag_value = false;
                    break;

                case ':':
                    capstack_.set_non_capturing();
                    loop = false;
                    break;

                case ')':
                    loop = false;
                    increment = false;
                    break;

                default:
                    throw pattern_error("Invalid capturing group");
                }

                if (increment)
                    ++lit;
            }
            break;
        }
    }

    template<typename CharT>
    constexpr void ll1<CharT>::sa_begin_alt()
    {
        if (flags().enable_branchreset)
            capstack_.branch_reset_if_enabled();
    }
}

namespace rx::detail
{
    /* constructor for tree */

    template<typename CharT>
    constexpr expr_tree<CharT>::expr_tree(const sv_type sv, const parser_flags flags)
        : flags_{ flags }
    {
        parser::ll1<char_type> ll1_parser(*this, sv);
    }
}

namespace rx::detail::tnfa
{
    using state_t = std::size_t;
    using tr_index = std::size_t;
    using continue_at_t = std::uint_least16_t;

    template<typename CharT>
    using charset_t = std::conditional_t<sizeof(CharT) == 1, bitcharset<CharT>, charset<CharT>>;

    namespace assert_category
    {
        struct eof_tag_t { explicit eof_tag_t() = default; };
        struct sof_tag_t { explicit sof_tag_t() = default; };
        struct lookahead1_tag_t { explicit lookahead1_tag_t() = default; };
        struct lookbehind1_tag_t { explicit lookbehind1_tag_t() = default; };

        inline constexpr eof_tag_t         eof_tag{};
        inline constexpr sof_tag_t         sof_tag{};
        inline constexpr lookahead1_tag_t  lookahead1_tag{};
        inline constexpr lookbehind1_tag_t lookbehind1_tag{};
    }

    /* tnfa transitions */

    template<typename CharT>
    struct normal_tr
    {
        charset_t<CharT> cs;
    };

    struct epsilon_tr
    {
        int priority;
        int tag; /* use tag = 0 when tag is absent, tag > 0 for regular tags, and tag < 0 for negative tags */
    };

    struct sof_anchor_tr {};
    struct eof_anchor_tr {};

    template<typename CharT>
    struct lookahead_1_tr
    {
        charset_t<CharT> cs;
    };

    template<typename CharT>
    struct lookbehind_1_tr
    {
        charset_t<CharT> cs;
    };

    template<typename CharT>
    struct transition
    {
        using transition_type = std::variant<std::monostate
                                             , normal_tr<CharT>, epsilon_tr
                                             , eof_anchor_tr, sof_anchor_tr
                                             , lookahead_1_tr<CharT>, lookbehind_1_tr<CharT>

        >;

        state_t         src, dst;
        transition_type type; 

        template<typename... Args>
        constexpr transition(state_t q0, state_t qf, Args&&... args)
            : src{ q0 }, dst{ qf }, type{ (std::forward<Args>(args))... } {}

        constexpr void unset()
        {
            src = std::numeric_limits<state_t>::max();
            dst = std::numeric_limits<state_t>::max();
            type.template emplace<std::monostate>();
        }
    };

    /* tnfa nodes */

    struct node
    {
        std::vector<tr_index> in_tr;
        std::vector<tr_index> out_tr;

        bool is_final{ false };
        bool is_fallback{ false };             /* must equal false if not is_final */
        std::uint_least16_t final_offset{ 0 }; /* only meaningful if is_final */
        continue_at_t continue_at{ 0 };        /* only meaningful if is_final */
    };

    struct continue_info
    {
        using sub_e_closure = std::optional<std::vector<state_t>>;

        state_t value;

    };

    /* predicates for variations of e-closure */

    struct reachable_predicate
    {
        template<typename CharT>
        static constexpr bool operator()(const transition<CharT>& /* tr */)
        {
            /* count everything */
            return true;
        }
    };

    struct e_closure_predicate
    {
        template<typename CharT>
        static constexpr bool operator()(const transition<CharT>& tr)
        {
            /* only count e-transitions */
            return std::holds_alternative<epsilon_tr>(tr.type);
        }
    };
}

namespace rx::detail
{
    /* tnfa class */

    template<typename CharT>
    class tagged_nfa
    {
    public:
        using char_type = CharT;
        using state_t = tnfa::state_t;
        using tr_index = tnfa::tr_index;
        using charset_type = tnfa::charset_t<char_type>;

        using normal_tr = tnfa::normal_tr<char_type>;
        using epsilon_tr = tnfa::epsilon_tr;
        using sof_anchor_tr = tnfa::sof_anchor_tr;
        using eof_anchor_tr = tnfa::eof_anchor_tr;
        using lookbehind_1_tr = tnfa::lookbehind_1_tr<char_type>;
        using lookahead_1_tr = tnfa::lookahead_1_tr<char_type>;

        explicit constexpr tagged_nfa(const expr_tree<char_type>& ast, fsm_flags flags);
        constexpr void rewrite_assertions();

        /* observers */

        [[nodiscard]] constexpr auto get_flags() const noexcept { return flags_; }
        [[nodiscard]] constexpr const tnfa::node& get_node(state_t q) const { return nodes_.at(q); }
        [[nodiscard]] constexpr const tnfa::transition<char_type>& get_tr(tr_index idx) const { return transitions_.at(idx); }
        [[nodiscard]] constexpr std::size_t node_count() const noexcept { return nodes_.size(); }
        [[nodiscard]] constexpr std::size_t transition_count() const noexcept { return transitions_.size(); }
        [[nodiscard]] constexpr std::size_t tag_count() const noexcept { return tag_count_; }
        [[nodiscard]] constexpr const capture_info& get_capture_info() const noexcept { return capture_info_; }
        [[nodiscard]] constexpr state_t start_node() const noexcept { return start_node_; }
        [[nodiscard]] constexpr const auto& get_cont_info() const { return cont_info_; }

    private:
        using ast_t = expr_tree<char_type>;
        using transition_info = tnfa::transition<char_type>::transition_type;

        template<in_variant<typename ast_t::type> T>
        static constexpr state_t ast_index{ index_of_impl<typename ast_t::type, T>::value };

        /* fundamental helpers */

        constexpr state_t node_create();
        constexpr state_t node_copy(state_t q);

        template<typename... Args>
        constexpr void transition_create(state_t q0, state_t qf, Args&&... args);

        /* transition creation */

        constexpr void make_epsilon(state_t q0, state_t qf, int priority = 0, int tag = 0);

        constexpr void make_transition(state_t q0, state_t qf, char_type c);

        template<typename CharSet>
        requires std::convertible_to<std::remove_cvref_t<CharSet>, charset_type>
        constexpr void make_transition(state_t q0, state_t qf, CharSet&& cs);

        template<typename T>
        requires one_of<T, tnfa::assert_category::eof_tag_t, tnfa::assert_category::sof_tag_t>
        constexpr void make_assert(state_t q0, state_t qf, T category);

        template<typename T, typename CharSet>
        requires one_of<T, tnfa::assert_category::lookahead1_tag_t, tnfa::assert_category::lookbehind1_tag_t>
                 and std::convertible_to<std::remove_cvref_t<CharSet>, charset_type>
        constexpr void make_assert(state_t q0, state_t qf, T category, CharSet&& cs);

        constexpr void make_copy(state_t q0, state_t qf, const transition_info& type);

        constexpr void make_ntags(state_t q0, state_t qf, const std::vector<int>& ntags);

        constexpr void thompson(const expr_tree<char_type>& ast);

        template<bool B, typename Vec, typename Pred, typename NodeProj, typename TrProj>
        [[nodiscard]] constexpr auto closure_impl(Vec&& qs, Pred pred, NodeProj node_proj, TrProj tr_proj) const;

        template<bool B = true, typename Vec = std::vector<state_t>, typename Pred = tnfa::e_closure_predicate>
        [[nodiscard]] constexpr auto epsilon_closure(Vec&& qs, Pred pred = {}) const;

        template<bool B = true, typename Vec = std::vector<state_t>, typename Pred = tnfa::e_closure_predicate>
        [[nodiscard]] constexpr auto backwards_epsilon_closure(Vec&& qs, Pred pred = {}) const;

        constexpr void remove_dead_and_unreachable_states();
        constexpr void rewrite_sof_anchors();
        constexpr void rewrite_eof_anchors();
        constexpr void rewrite_sc_lookahead();
        constexpr void rewrite_sc_lookbehind();

        /* constants */

        static constexpr state_t default_start_node{ 0 };
        static constexpr state_t default_final_node{ 1 };

        /* data members */

        std::vector<tnfa::node>                  nodes_{ 2 };
        std::vector<tnfa::transition<char_type>> transitions_;
        capture_info                             capture_info_;
        std::size_t                              tag_count_;
        state_t                                  start_node_{ default_start_node };
        std::vector<tnfa::continue_info>         cont_info_;

        fsm_flags flags_;

        bool has_eof_anchor_   : 1 { false };
        bool has_sof_anchor_   : 1 { false };
        bool has_lookahead_1_  : 1 { false };
        bool has_lookbehind_1_ : 1 { false };
    };
}

namespace rx::detail
{
    struct stack_elem
    {
        tnfa::state_t q0, qf;
        std::size_t idx;
    };

    /* graph helper functions */

    template<typename CharT>
    constexpr auto tagged_nfa<CharT>::node_create() -> state_t
    {
        const state_t p{ nodes_.size() };
        nodes_.emplace_back();
        return p;
    }

    template<typename CharT>
    constexpr auto tagged_nfa<CharT>::node_copy(const state_t q) -> state_t
    {
        const state_t p{ nodes_.size() };
        auto& new_n{ nodes_.emplace_back() };
        const auto& old_n{ nodes_.at(q) };

        new_n.is_final = old_n.is_final;
        new_n.is_fallback = old_n.is_fallback;
        new_n.final_offset = old_n.final_offset;
        new_n.continue_at = old_n.continue_at;

        return p;
    }

    /* transition creation helper functions */

    template<typename CharT>
    template<typename... Args>
    constexpr void tagged_nfa<CharT>::transition_create(const state_t q0, const state_t qf, Args&&... args)
    {
        const std::size_t transition_index{ transitions_.size() };
        transitions_.emplace_back(q0, qf, std::forward<Args>(args)...);
        nodes_.at(q0).out_tr.emplace_back(transition_index);
        nodes_.at(qf).in_tr.emplace_back(transition_index);
    }

    template<typename CharT>
    constexpr void tagged_nfa<CharT>::make_epsilon(const state_t q0, const state_t qf, const int priority, const int tag)
    {
        transition_create(q0, qf, std::in_place_type<epsilon_tr>, priority, tag);
    }

    template<typename CharT>
    constexpr void tagged_nfa<CharT>::make_transition(const state_t q0, const state_t qf, char_type c)
    {
        transition_create(q0, qf, std::in_place_type<normal_tr>, charset_type{ c });
    }

    template<typename CharT>
    template<typename CharSet>
    requires std::convertible_to<std::remove_cvref_t<CharSet>, tnfa::charset_t<CharT>>
    constexpr void tagged_nfa<CharT>::make_transition(const state_t q0, const state_t qf, CharSet&& cs)
    {
        transition_create(q0, qf, std::in_place_type<normal_tr>, std::forward<CharSet>(cs));
    }

    template<typename CharT>
    template<typename T>
    requires one_of<T, tnfa::assert_category::eof_tag_t, tnfa::assert_category::sof_tag_t>
    constexpr void tagged_nfa<CharT>::make_assert(const state_t q0, const state_t qf, T /* category */)
    {
        using type = std::conditional_t<std::same_as<T, tnfa::assert_category::eof_tag_t>, eof_anchor_tr, sof_anchor_tr>;

        transition_create(q0, qf, std::in_place_type<type>);
    }

    template<typename CharT>
    template<typename T, typename CharSet>
    requires one_of<T, tnfa::assert_category::lookahead1_tag_t, tnfa::assert_category::lookbehind1_tag_t>
             and std::convertible_to<std::remove_cvref_t<CharSet>, tnfa::charset_t<CharT>>
    constexpr void tagged_nfa<CharT>::make_assert(const state_t q0, const state_t qf, T /* category */, CharSet&& cs)
    {
        using type = std::conditional_t<std::same_as<T, tnfa::assert_category::lookahead1_tag_t>, lookahead_1_tr, lookbehind_1_tr>;

        transition_create(q0, qf, std::in_place_type<type>, std::forward<CharSet>(cs));
    }

    template<typename CharT>
    constexpr void tagged_nfa<CharT>::make_copy(const state_t q0, const state_t qf, const transition_info& type)
    {
        transition_create(q0, qf, type);
    }

    /* modified thompson's algorithm */

    template<typename CharT>
    constexpr void tagged_nfa<CharT>::make_ntags(state_t q0, const state_t qf, const std::vector<int>& ntags)
    {
        if (not ntags.empty())
        {
            /* note: in tree_expr tags start at 0, whereas here they start at 1 */
            for (const int tag : ntags | std::views::take(ntags.size() - 1))
            {
                const state_t qi{ node_create() };
                make_epsilon(q0, qi, 0, -(tag + 1));
                q0 = qi;
            }

            make_epsilon(q0, qf, 0, -(ntags.back() + 1));
        }
        else
        {
            make_epsilon(q0, qf);
        }
    }

    template<typename CharT>
    constexpr void tagged_nfa<CharT>::thompson(const expr_tree<char_type>& ast)
    {
        std::vector<std::vector<int>> tag_vec{};
        if (tag_count_ > 1) ast.make_tag_vec(tag_vec);

        std::vector<stack_elem> stack;
        stack.emplace_back(default_start_node, default_final_node, ast.root_idx());

        while (not stack.empty())
        {
            auto [q0, qf, idx]{ stack.back() };
            stack.pop_back();

            const auto& entry{ ast.get_expr(idx) };

            switch (entry.index())
            {
            case ast_index<typename ast_t::char_str>:
                if (const auto& str{ std::get<typename ast_t::char_str>(entry) }; not str.data.empty())
                {
                    for (const auto c : str.data | std::views::take(str.data.size() - 1))
                    {
                        auto qi{ node_create() };
                        make_transition(q0, qi, c);
                        q0 = qi;
                    }

                    make_transition(q0, qf, str.data.back());
                }
                else
                {
                    make_epsilon(q0, qf);
                }
                break;

            case ast_index<typename ast_t::char_class>:
            {
                const auto& cla{ std::get<typename ast_t::char_class>(entry) };

                if constexpr (char_is_utf8<char_type>)
                {

                    throw tnfa_error("UTF8 character classes are unimplemented");
                }
                else if constexpr (char_is_utf16<char_type>)
                {

                    throw tnfa_error("UTF16 character classes are unimplemented");
                }
                else if constexpr (std::same_as<charset_type, typename ast_t::char_class::impl_type::underlying_type>)
                {
                    make_transition(q0, qf, cla.data.get());
                }
                else
                {

                    throw tnfa_error("Cannot convert character class to representation used by tnfa");
                }
                break;
            }

            case ast_index<typename ast_t::concat>:
                if (const auto& cat{ std::get<typename ast_t::concat>(entry) }; not cat.idxs.empty())
                {
                    for (const auto i : cat.idxs | std::views::take(cat.idxs.size() - 1))
                    {
                        const state_t qi{ node_create() };
                        stack.emplace_back(q0, qi, i);
                        q0 = qi;
                    }

                    stack.emplace_back(q0, qf, cat.idxs.back());
                }
                else
                {
                    make_epsilon(q0, qf);
                }
                break;

            case ast_index<typename ast_t::alt>:
                if (const auto& alt{ std::get<typename ast_t::alt>(entry) }; not alt.idxs.empty())
                {
                    if (tag_vec.empty() or tag_vec.at(idx).empty())
                    {
                        /* generate naive tag-free nfa */
                        for (std::size_t i{ 0 }; i < alt.idxs.size(); ++i)
                        {
                            const state_t qi{ node_create() };
                            make_epsilon(q0, qi, i);
                            stack.emplace_back(qi, qf, alt.idxs.at(i));
                        }
                    }
                    else
                    {
                        /* generate tag-aware nfa */
                        for (std::size_t i{ 0 }; i < alt.idxs.size(); ++i)
                        {
                            if (i + 1 == alt.idxs.size())
                            {
                                stack.emplace_back(q0, qf, alt.idxs.at(i));
                            }
                            else
                            {
                                const state_t q1{ node_create() };
                                const state_t p2{ node_create() };
                                const state_t p1{ node_create() };
                                const state_t q2{ node_create() };

                                make_epsilon(q0, q1, 0);
                                make_epsilon(q0, p1, 1);

                                stack.emplace_back(q1, p2, alt.idxs.at(i));
                                make_ntags(p1, q2, tag_vec.at(alt.idxs.at(i)));

                                std::vector<int> remaining_ntags;
                                for (std::size_t j{ i + 1 }; j < alt.idxs.size(); ++j)
                                    std::ranges::copy(tag_vec.at(alt.idxs.at(j)), std::back_inserter(remaining_ntags));

                                std::ranges::sort(remaining_ntags);
                                auto [_, last]{ std::ranges::unique(remaining_ntags) };
                                remaining_ntags.erase(last, remaining_ntags.end());

                                make_ntags(p2, qf, remaining_ntags);

                                q0 = q2;
                            }
                        }
                    }
                }
                else
                {
                    make_epsilon(q0, qf);
                }
                break;

            case ast_index<typename ast_t::repeat>:
            {
                const auto& rep{ std::get<typename ast_t::repeat>(entry) };

                if (rep.mode == repeater_mode::possessive)
                    throw tnfa_error("Possessive repetition is unimplemented");

                /* reminder: max < min denotes infinity */
                int max{ (rep.max < rep.min) ? rep.min - 1 : rep.max };
                int min{ rep.min };
                const bool lazy{ rep.mode == repeater_mode::lazy };

                while (true)
                {
                    if (min > 1)
                    {
                        const state_t q1{ node_create() };

                        stack.emplace_back(q0, q1, rep.idx);

                        min -= 1;
                        max -= 1;
                        q0 = q1;
                    }
                    else if (min == 1 and max == 1)
                    {
                        stack.emplace_back(q0, qf, rep.idx);

                        break;
                    }
                    else if (min == 1 and max > 1)
                    {
                        const state_t q1{ node_create() };
                        const state_t q2{ node_create() };

                        stack.emplace_back(q0, q1, rep.idx);
                        make_epsilon(q1, qf, not lazy);
                        make_epsilon(q1, q2, lazy);

                        max -= 1;
                        q0 = q2;
                    }
                    else if (min == 0)
                    {
                        const state_t q1{ node_create() };

                        make_epsilon(q0, q1, lazy);

                        if (tag_vec.empty() or tag_vec.at(idx).empty())
                        {
                            /* generate tag-free nfa */
                            make_epsilon(q0, qf, not lazy);
                        }
                        else
                        {
                            /* generate tag-aware nfa */
                            const state_t p1{ node_create() };
                            make_epsilon(q0, p1, not lazy);
                            make_ntags(p1, qf, tag_vec.at(rep.idx));
                        }

                        min = 1;
                        q0 = q1;
                    }
                    else if (min == 1 and max < min)
                    {
                        const state_t q1{ node_create() };

                        stack.emplace_back(q0, q1, rep.idx);
                        make_epsilon(q1, q0, lazy);
                        make_epsilon(q1, qf, not lazy);

                        break;
                    }

                    else
                    {
                        throw tnfa_error("Invalid repeater in tree");
                    }
                }
                break;
            }

            case ast_index<typename ast_t::tag>:
            {
                /* in tree_expr tags start at 0, whereas here they start at 1 */
                const auto& tag_entry{ std::get<typename ast_t::tag>(entry) };
                make_epsilon(q0, qf, 0, tag_entry.number + 1);
                break;
            }

            case ast_index<typename ast_t::assertion>:
            {
                const typename ast_t::assertion& assertion{ std::get<typename ast_t::assertion>(entry) };

                using cs = nontransient_constexpr_version_of_t<charset_type>;
                using p = cs::char_interval;
                namespace ac = tnfa::assert_category;

                /* note: these are not Unicode-aware; TODO: FIX */
                static constexpr cs newline_cs{ '\n' };
                static constexpr cs word_cs{ p{ '0', '9' }, p{ 'A', 'Z' }, p{ 'a', 'z' }, '_' };
                static constexpr cs not_word_cs{ ~word_cs };

                switch (assertion.type)
                {
                case assert_type::line_start:
                    make_assert(q0, qf, ac::lookbehind1_tag, newline_cs);
                    has_lookbehind_1_ = true;
                    [[fallthrough]];

                case assert_type::text_start:
                    make_assert(q0, qf, ac::sof_tag);
                    has_sof_anchor_ = true;
                    break;

                case assert_type::line_end:
                    make_assert(q0, qf, ac::lookahead1_tag, newline_cs);
                    has_lookahead_1_ = true;
                    [[fallthrough]];

                case assert_type::text_end:
                    make_assert(q0, qf, ac::eof_tag);
                    has_eof_anchor_ = true;
                    break;

                case assert_type::word_boundary:
                {
                    const state_t q1{ node_create() };
                    const state_t q2{ node_create() };

                    make_assert(q0, q1, ac::sof_tag);
                    make_assert(q0, q1, ac::lookbehind1_tag, not_word_cs);
                    make_assert(q1, qf, ac::lookahead1_tag, word_cs);

                    make_assert(q0, q2, ac::lookbehind1_tag, word_cs);
                    make_assert(q2, qf, ac::lookahead1_tag, not_word_cs);
                    make_assert(q2, qf, ac::eof_tag);

                    has_eof_anchor_ = true;
                    has_sof_anchor_ = true;
                    has_lookahead_1_ = true;
                    has_lookbehind_1_ = true;
                    break;
                }

                case assert_type::not_word_boundary:
                {
                    const state_t q1{ node_create() };
                    const state_t q2{ node_create() };

                    make_assert(q0, q1, ac::sof_tag);
                    make_assert(q0, q1, ac::lookbehind1_tag, not_word_cs);
                    make_assert(q1, qf, ac::lookahead1_tag, not_word_cs);
                    make_assert(q1, qf, ac::eof_tag);

                    make_assert(q0, q2, ac::lookbehind1_tag, word_cs);
                    make_assert(q2, qf, ac::lookahead1_tag, word_cs);

                    has_eof_anchor_ = true;
                    has_sof_anchor_ = true;
                    has_lookahead_1_ = true;
                    has_lookbehind_1_ = true;
                    break;
                }

                default:
                    throw tnfa_error("Non-trivial assertions are unimplemented");
                    break;
                }
                break;
            }

            case ast_index<typename ast_t::backref>:
                throw tnfa_error("Backreferences are unsupported");

            default:
                throw tree_error("Invalid tree");
            }
        }
    }

    /* observers */

    template<typename CharT>
    template<bool B, typename Vec, typename Pred, typename NodeProj, typename TrProj>
    constexpr auto tagged_nfa<CharT>::closure_impl(Vec&& qs, Pred pred, NodeProj node_proj, TrProj tr_proj) const
    {
        using result_t = maybe_type_t<B, std::vector<state_t>>;

        std::vector to_visit{ std::forward<Vec>(qs) };
        std::ranges::reverse(to_visit);

        std::vector<bool> visited(node_count(), false);
        result_t result;

        for (const auto q : to_visit)
            visited.at(q) = true;

        while (not to_visit.empty())
        {
            const state_t current{ to_visit.back() };
            to_visit.pop_back();

            if constexpr (B)
                result.emplace_back(current);

            for (const tr_index i : std::invoke(node_proj, get_node(current)) | std::views::reverse)
            {
                const auto& tr{ get_tr(i) };
                const auto val{ std::invoke(tr_proj, tr) };

                if constexpr (requires { { std::invoke(pred, tr, i) } -> std::convertible_to<bool>; })
                {
                    if (not std::invoke(pred, tr, i))
                        continue;
                }
                else if constexpr (requires { { std::invoke(pred, tr) } -> std::convertible_to<bool>; })
                {
                    if (not std::invoke(pred, tr))
                        continue;
                }
                else
                {
                    static_assert(false, "tagged_nfa::closure_impl: invalid predicate");
                }

                if (visited.at(val))
                    continue;

                visited[val] = true;
                to_visit.emplace_back(val);
            }
        }

        if constexpr (B)
            return result;
        else
            return visited;
    }

    template<typename CharT>
    template<bool B, typename Vec, typename Pred>
    constexpr auto tagged_nfa<CharT>::epsilon_closure(Vec&& qs, Pred pred) const
    {
        return closure_impl<B>(std::forward<Vec>(qs), pred, &tnfa::node::out_tr, &tnfa::transition<char_type>::dst);
    }

    template<typename CharT>
    template<bool B, typename Vec, typename Pred>
    constexpr auto tagged_nfa<CharT>::backwards_epsilon_closure(Vec&& qs, Pred pred) const
    {
        return closure_impl<B>(std::forward<Vec>(qs), pred, &tnfa::node::in_tr, &tnfa::transition<char_type>::src);
    }

    /* dead state elimination */

    template<typename CharT>
    constexpr void tagged_nfa<CharT>::remove_dead_and_unreachable_states()
    {
        /* determine reachable states */

        std::vector<state_t> initial_nodes{ start_node_ };

        for (const auto& ci : cont_info_)
            if (ci.value != start_node_)
                initial_nodes.emplace_back(ci.value);

        const auto reachable_nodes{ epsilon_closure<false>(std::move(initial_nodes), tnfa::reachable_predicate{}) };

        /* determine live states */

        std::vector<state_t> final_nodes{};

        for (state_t q{ 0 }; q < nodes_.size(); ++q)
            if (nodes_[q].is_final)
                final_nodes.emplace_back(q);

        const auto live_nodes{ backwards_epsilon_closure<false>(std::move(final_nodes), tnfa::reachable_predicate{}) };

        /* remove transitions containing dead and unreachable nodes */

        std::vector<bool> live_and_reachable(nodes_.size(), false);
        std::ranges::transform(live_nodes, reachable_nodes, live_and_reachable.begin(), std::logical_and{});

        std::vector<bool> removed_transitions(transitions_.size(), false);

        for (tr_index i{ 0 }; i < transitions_.size(); ++i)
        {
            auto& tr{ transitions_[i] };

            if (tr.src == std::numeric_limits<state_t>::max() or tr.dst == std::numeric_limits<state_t>::max())
                continue;

            if (live_and_reachable.at(tr.src) and live_and_reachable.at(tr.dst))
                continue;

            removed_transitions[i] = true;
            tr.unset();
        }

        /* remove dead and unreachable nodes and transitions from nodes */

        const auto pred = [&](const std::size_t i) { return removed_transitions.at(i); };

        for (state_t q{ 0 }; q < nodes_.size(); ++q)
        {
            tnfa::node& node{ nodes_[q] };

            if (live_nodes[q])
            {
                std::erase_if(node.in_tr, pred);
                std::erase_if(node.out_tr, pred);
            }
            else
            {
                /* reset from, to, and is_final */
                node = tnfa::node{};
            }
        }
    }

    /* assertion elimination */

    template<typename CharT>
    constexpr void tagged_nfa<CharT>::rewrite_sof_anchors()
    {
        static constexpr auto pred = [](const tnfa::transition<char_type>& tr) static {
            return not std::holds_alternative<normal_tr>(tr.type) and not std::holds_alternative<lookbehind_1_tr>(tr.type);
        };

        const auto ec{ epsilon_closure({ start_node() }, pred) };

        /* create a copy of the start node's e-closure */

        std::flat_map<std::size_t, std::size_t> mapped_states;

        for (const state_t q : ec)
        {
            if (const auto& node{ get_node(q) }; node.is_final)
                mapped_states[q] = q; /* do not create duplicate final nodes */
            else
                mapped_states[q] = node_copy(q);
        }

        /* duplicate transitions */

        for (const auto [q, p] : mapped_states)
        {
            /* q == p is only possible when there are no transitions from q */
            for (const tr_index i : nodes_.at(q).out_tr)
            {
                /* reminder: reference may be invalidated after one call to emplace_back (when transitions_ is resized) */
                const auto& tr{ get_tr(i) };

                if (std::holds_alternative<normal_tr>(tr.type))
                {
                    /* transition from copied e-closure to main graph */
                    make_copy(p, tr.dst, tr.type);
                }
                else if (std::holds_alternative<sof_anchor_tr>(tr.type))
                {
                    /* replace sof anchor with e-transition in copied subgraph */
                    make_epsilon(p, mapped_states.at(tr.dst));
                }
                else if (not std::holds_alternative<lookbehind_1_tr>(tr.type))
                {
                    /* transition within copied e-closure */
                    make_copy(p, mapped_states.at(tr.dst), tr.type);
                }
            }
        }

        /* remove all sof anchors */

        for (tr_index i{ 0 }; i < transitions_.size(); ++i)
        {
            auto& tr{ transitions_[i] };

            if (not std::holds_alternative<sof_anchor_tr>(tr.type))
                continue;

            /* delete transition */
            std::erase(nodes_.at(tr.src).out_tr, i);
            std::erase(nodes_.at(tr.dst).in_tr, i);
            tr.unset();
        }

        /* reassign start node */

        start_node_ = mapped_states.at(start_node_);
    }

    template<typename CharT>
    constexpr void tagged_nfa<CharT>::rewrite_eof_anchors()
    {
        /* NOTE: this function must be called before any other function which adds final nodes */

        static constexpr auto pred = [](const tnfa::transition<char_type>& tr) static {
            return not std::holds_alternative<normal_tr>(tr.type) and not std::holds_alternative<lookahead_1_tr>(tr.type);
        };

        std::vector<bool> bec{ backwards_epsilon_closure<false>({ default_final_node }, pred) };

        std::vector<state_t> initial;
        std::vector<tr_index> to_revisit;

        for (tr_index i{ 0 }; i < transitions_.size(); ++i)
        {
            const auto& tr{ transitions_[i] };

            if (not std::holds_alternative<eof_anchor_tr>(tr.type))
                continue;

            to_revisit.emplace_back(i);

            if (bec.at(tr.dst))
                initial.emplace_back(tr.dst);
        }

        const auto ec{ epsilon_closure(std::move(initial), pred) };

        /* create copy of the e-closures  */

        std::flat_map<state_t, state_t> mapped_states;

        for (const state_t q : ec)
        {
            if (const auto& node{ get_node(q) }; node.is_final)
            {
                if (node.is_fallback)
                {
                    /* create duplicate non-fallback final node */
                    const state_t p{ node_copy(q) };
                    nodes_[p].is_fallback = false;
                    mapped_states[q] = p;
                }
                else
                {
                    /* avoid creating duplicate non-fallback final nodes */
                    mapped_states[q] = q;
                }
            }
            else
            {
                mapped_states[q] = node_create();
            }
        }

        /* duplicate transitions */

        for (const auto [q, p] : mapped_states)
        {
            /* q == p is only possible when there are no transitions from q */
            for (const tr_index i : nodes_.at(q).out_tr)
            {
                /* reminder: reference may be invalidated after one call to emplace_back (when transitions_ is resized) */
                const auto& tr{ get_tr(i) };

                if (std::holds_alternative<normal_tr>(tr.type))
                {
                    /* do not insert transition */
                }
                else if (std::holds_alternative<eof_anchor_tr>(tr.type))
                {
                    /* replace eof anchor with e-transition in copied subgraph */
                    make_epsilon(p, mapped_states.at(tr.dst));
                }
                else
                {
                    /* transition within copied subgraph */
                    make_copy(p, mapped_states.at(tr.dst), tr.type);
                }
            }
        }

        /* replace all eof anchors with transitions into copied subgraph, or remove if not possible */

        for (const tr_index i : to_revisit)
        {
            auto& tr{ transitions_.at(i) };

            if (auto it{ mapped_states.find(tr.dst) }; it != mapped_states.end())
            {
                /* remap rhs and replace with e-transition */
                const auto [q, p]{ *it };
                tr.type.template emplace<tnfa::epsilon_tr>(0, 0);

                if (q != p)
                {
                    std::erase(nodes_.at(q).in_tr, i);
                    nodes_.at(p).in_tr.emplace_back(i);
                    tr.dst = p;
                }
            }
            else
            {
                /* delete transition */
                std::erase(nodes_.at(tr.src).out_tr, i);
                std::erase(nodes_.at(tr.dst).in_tr, i);
                tr.unset();
            }
        }
    }

    template<typename CharT>
    constexpr void tagged_nfa<CharT>::rewrite_sc_lookahead()
    {
        using transition_map_t = std::flat_map<charset_type, std::vector<tr_index>>;
        using state_map_t = std::flat_map<state_t, state_t>;
        using all_states_map_t = std::flat_map<charset_type, state_map_t>;

        transition_map_t sc_transitions;

        for (tr_index i{ 0 }; i < transitions_.size(); ++i)
        {
            const auto& tr{ transitions_[i] };
            if (const auto* ptr{ std::get_if<lookahead_1_tr>(&tr.type) }; ptr != nullptr)
                sc_transitions[ptr->cs].emplace_back(i);
        }

        /* create copy of the e-closures of destinations of lookahead_1 transitions  */

        all_states_map_t all_mapped_states;

        std::vector<std::pair<charset_type, std::vector<state_t>>> outer_visit;

        for (const auto& [edge, tr_vec] : sc_transitions)
        {
            outer_visit.emplace_back(
                edge,
                std::vector<state_t>{
                    std::from_range,
                    tr_vec | std::views::transform([this](const tr_index i) { return transitions_.at(i).dst; })
                }
            );
        }

        while (not outer_visit.empty())
        {
            const auto edge{ std::move(outer_visit.back().first) };
            std::vector tmp{ std::move(outer_visit.back().second) };
            outer_visit.pop_back();

            /* perform a modified epsilon closure */

            auto pred = [&](const tnfa::transition<char_type>& tr) {
                /* skip normal transitions and eof anchors */
                if (std::holds_alternative<normal_tr>(tr.type) or std::holds_alternative<eof_anchor_tr>(tr.type))
                    return false;

                if (const auto* const ptr{ std::get_if<lookahead_1_tr>(&tr.type) }; ptr != nullptr)
                {
                    if (auto new_edge{ edge & ptr->cs }; not new_edge.empty() and new_edge != ptr->cs)
                    {
                        /* intersection with lookahead_1 transition requires a cloned subgraph */
                        outer_visit.emplace_back(std::move(new_edge), std::vector{ tr.dst });
                    }

                    return false;
                }

                return true;
            };

            const auto ec{ epsilon_closure(std::move(tmp), pred) };

            /* duplicate all nodes in each subgraph at most once */
            auto& mapped_states{ all_mapped_states[edge] };
            for (const state_t q : ec)
                if (not mapped_states.contains(q)) /* probably inefficient */
                    mapped_states[q] = node_create();
        }

        /* duplicate transitions */

        std::optional<state_t> offset_end;

        for (const auto& [edge, mapped_states] : all_mapped_states)
        {
            for (const auto [q, p] : mapped_states)
            {
                if (const auto& node{ nodes_.at(q) }; node.is_final and node.is_fallback)
                {
                    /* reminder: reference 'node' may be invalidated after calling node_create */

                    if (not offset_end.has_value())
                    {
                        /* create new offset end node */
                        const auto saved_cont{ node.continue_at };
                        offset_end = node_create();
                        auto& n{ nodes_.at(*offset_end) };
                        n.is_final = true;
                        n.is_fallback = true;
                        n.final_offset = 1;
                        n.continue_at = saved_cont;
                    }

                    make_transition(p, *offset_end, edge);
                }

                for (const tr_index i : nodes_.at(q).out_tr)
                {
                    /* reminder: reference may be invalidated after one call to emplace_back (when transitions_ is resized) */
                    const auto& tr{ get_tr(i) };

                    if (const auto* const ptr{ std::get_if<normal_tr>(&tr.type) }; ptr != nullptr)
                    {
                        /* conditionally transition from copied e-closure to main graph */
                        if (auto new_edge{ edge & ptr->cs }; not new_edge.empty())
                            make_transition(p, tr.dst, std::move(new_edge));
                    }
                    else if (const auto* const ptr{ std::get_if<lookahead_1_tr>(&tr.type) }; ptr != nullptr)
                    {
                        /* conditionally e-transition between copied subgraphs */
                        if (auto new_edge{ edge & ptr->cs }; not new_edge.empty())
                            make_epsilon(p, all_mapped_states.at(std::move(new_edge)).at(tr.dst));
                    }
                    else if (not std::holds_alternative<eof_anchor_tr>(tr.type))
                    {
                        /* transition within copied e-closure */
                        make_copy(p, mapped_states.at(tr.dst), tr.type);
                    }
                }
            }
        }

        /* replace lookahead_1 transitions with e-transitions into their respective subgraphs */

        for (const auto& [edge, tr_vec] : sc_transitions)
        {
            const auto& mapped_states{ all_mapped_states.at(edge) };
            for (const tr_index i : tr_vec)
            {
                auto& tr{ transitions_.at(i) };

                /* assign the lowest priority to avoid clashes with eof_anchor */
                using priority_t = decltype(epsilon_tr::priority);
                const auto p{ std::saturate_cast<priority_t>(std::sub_sat(nodes_.at(tr.src).out_tr.size(), 1uz)) };

                std::erase(nodes_.at(tr.dst).in_tr, i);
                tr.dst = mapped_states.at(tr.dst);
                tr.type = tnfa::epsilon_tr{ .priority = p, .tag = 0 };
                nodes_.at(tr.dst).in_tr.emplace_back(i);
            }
        }
    }

    template<typename CharT>
    constexpr void tagged_nfa<CharT>::rewrite_sc_lookbehind()
    {
        if (cont_info_.size() > 1)
            throw tnfa_error("tagged_nfa::rewrite_sc_lookbehind: cont_info_.size() > 1");

        using transition_map_t = std::flat_map<charset_type, std::vector<tr_index>>;
        using state_map_t = std::flat_map<state_t, state_t>;
        using all_states_map_t = std::flat_map<charset_type, state_map_t>;

        using charset_set_t = std::flat_map<charset_type, std::monostate>;
        using start_tr_map_t = std::flat_map<tr_index, charset_set_t>;

        /* find fallback and continuation states */

        std::optional<state_t> continue_state;
        if (not cont_info_.empty())
            continue_state = cont_info_.front().value;

        std::vector<state_t> fallback_states;
        for (state_t q{ 0 }; q < nodes_.size(); ++q)
            if (const auto& node{ nodes_[q] }; node.is_final and node.is_fallback and not node.in_tr.empty())
                fallback_states.emplace_back(q);

        /* setup for finding transitions leading into backwards e-closures */

        struct lb_info
        {
            std::vector<tr_index> tr_into;
            std::vector<tr_index> tr_between;
        };

        static constexpr auto search_predicate = [](lb_info& lb, const charset_type& edge, const tnfa::transition<char_type>& tr, const tr_index i) static {
            /* assign results here instead of using the normal return value */
            if (const auto* const ptr{ std::get_if<normal_tr>(&tr.type) }; ptr != nullptr)
            {
                if (const auto new_edge{ edge & ptr->cs }; not new_edge.empty())
                    lb.tr_into.emplace_back(i);

                return false;
            }

            /* skip sof anchors */
            if (std::holds_alternative<sof_anchor_tr>(tr.type))
                return false;

            /* intersection with lookbehind_1 transition requires cloned subgraph */
            if (const auto* const ptr{ std::get_if<lookbehind_1_tr>(&tr.type) }; ptr != nullptr)
            {
                if (const auto new_edge{ edge & ptr->cs }; not new_edge.empty() and new_edge != ptr->cs)
                    lb.tr_between.emplace_back(i);

                return false;
            }

            return true;
        };

        static constexpr auto dedup_transitions_map = [](transition_map_t& map) static {
            for (auto it{ map.begin() }, end{ map.end() }; it != end; ++it)
            {
                auto& vec{ it->second };
                std::ranges::sort(vec);
                const auto [first, last]{ std::ranges::unique(vec) };
                vec.erase(first, last);
            }
        };

        /* find lookbehind transitions and group by edge */

        transition_map_t sc_transitions;
        std::flat_map<tr_index, lb_info> lb_closures;

        std::vector<tr_index> wraparounds; /* sorted; i.e. flat_set */
        charset_type wraparound_union;

        for (tr_index i{ 0 }; i < transitions_.size(); ++i)
        {
            const auto& tr{ transitions_[i] };
            const auto* ptr{ std::get_if<lookbehind_1_tr>(&tr.type) };

            if (ptr == nullptr)
                continue;

            auto [it, _]{ lb_closures.try_emplace(i) };
            auto& lb{ it->second };
            std::vector bec{ backwards_epsilon_closure({ tr.src }, std::bind_front(search_predicate, std::ref(lb), std::cref(ptr->cs))) };

            std::ranges::sort(lb.tr_into);
            std::ranges::sort(lb.tr_between);

            /* check if backwards e-closure includes any continue states */
            if (continue_state.has_value() /* and bec.at(continue_state.value()) */)
            {
                wraparounds.emplace_back(i);
                wraparound_union |= ptr->cs;
            }

            sc_transitions[ptr->cs].emplace_back(i);
        }

        dedup_transitions_map(sc_transitions);

        /* find wraparound transitions */

        std::optional<lb_info> wraparound_lb_closure;

        if (continue_state.has_value() and not fallback_states.empty())
        {
            wraparound_lb_closure.emplace();
            std::ignore = backwards_epsilon_closure<false>(fallback_states, std::bind_front(search_predicate, std::ref(*wraparound_lb_closure), std::cref(wraparound_union)));

            std::ranges::sort(wraparound_lb_closure->tr_into);
            std::ranges::sort(wraparound_lb_closure->tr_between);
        }

        /* determine closure start bitvec */

        std::vector<bool> is_closure_start(transitions_.size(), false);

        for (const auto& [_, lb] : lb_closures)
            for (const tr_index i : lb.tr_into)
                is_closure_start.at(i) = true;

        if (wraparound_lb_closure.has_value())
            for (const tr_index i : wraparound_lb_closure->tr_into)
                is_closure_start.at(i) = true;

        /* partition wraparound starts */

        /* always empty if there are no continue states */
        std::vector<charset_type> wrap_starts;

        if (wraparound_lb_closure.has_value())
        {
            const auto fn = [this](const tr_index i) {
                const auto& tr{ get_tr(i) };
                return std::cref(std::get<lookbehind_1_tr>(tr.type).cs);
            };

            const std::vector refs{ std::from_range, wraparounds | std::views::transform(fn) };
            wrap_starts = charset_type::partition(refs);
            std::ranges::sort(wrap_starts);
        }

        /* determine starts transitions */

        start_tr_map_t start_transitions;
        std::vector to_visit{ std::from_range, sc_transitions };

        if (wraparound_lb_closure.has_value())
        {
            /* insert wraparound edges */
            transition_map_t to_insert;

            for (const auto& edge : wrap_starts)
            {
                for (const tr_index i : wraparound_lb_closure->tr_into)
                    if (auto new_edge{ edge & std::get<normal_tr>(get_tr(i).type).cs }; not new_edge.empty())
                        start_transitions[i].try_emplace(std::move(new_edge));

                for (const tr_index i : wraparound_lb_closure->tr_between)
                    if (auto new_edge{ edge & std::get<lookbehind_1_tr>(get_tr(i).type).cs }; not new_edge.empty() and new_edge != edge)
                        to_insert[std::move(new_edge)].emplace_back(i);
            }

            dedup_transitions_map(to_insert);
            to_visit.append_range(std::move(to_insert));
        }

        while (not to_visit.empty())
        {
            using visit_type = transition_map_t::value_type;
            const visit_type current{ std::move(to_visit.back()) };
            to_visit.pop_back();

            const auto& [edge, idxs]{ current };
            transition_map_t to_insert;

            for (const tr_index lb_tr_index : idxs)
            {
                const auto& lb{ lb_closures.at(lb_tr_index) };

                for (const tr_index i : lb.tr_into)
                    if (auto new_edge{ edge & std::get<normal_tr>(get_tr(i).type).cs }; not new_edge.empty())
                        start_transitions[i].try_emplace(std::move(new_edge));

                for (const tr_index i : lb.tr_between)
                    if (auto new_edge{ edge & std::get<lookbehind_1_tr>(get_tr(i).type).cs }; not new_edge.empty() and new_edge != edge)
                        to_insert[std::move(new_edge)].emplace_back(i);
            }

            dedup_transitions_map(to_insert);
            to_visit.append_range(std::move(to_insert));
        }

        /* partition closure starts */

        using wraparound_index = std::size_t;
        std::flat_map<charset_type, wraparound_index> closure_wraparounds;

        std::flat_map<charset_type, std::vector<state_t>> closure_starts;

        for (const auto& edge : wrap_starts)
            closure_starts[edge].emplace_back(continue_state.value());

        for (const auto& [i, edges] : start_transitions)
        {
            if (edges.empty())
                continue;

            const auto fn = [&](const charset_type& edge) {
                std::size_t cont_index{ std::numeric_limits<wraparound_index>::max() };
                if (const auto it{ std::ranges::lower_bound(wrap_starts, edge) }; it != wrap_starts.end() and edge == *it)
                    cont_index = static_cast<wraparound_index>(std::distance(wrap_starts.begin(), it));
                return std::pair{ std::cref(edge), cont_index };
            };

            const std::vector ref_pairs{ std::from_range, edges.keys() | std::views::transform(fn) };

            for (auto&& [new_edge, vec] : charset_type::partition_ext(ref_pairs))
            {
                std::erase(vec, std::numeric_limits<std::size_t>::max());
                if (vec.size() > 1)
                    throw tnfa_error("tagged_nfa::rewrite_sc_lookbehind: vec.size() > 1");

                if (vec.size() == 1)
                    if (const auto [it, success]{ closure_wraparounds.try_emplace(new_edge, vec.front()) }; not success and it->second != vec.front())
                        throw tnfa_error("tagged_nfa::rewrite_sc_lookbehind: new_edge already exists in closure_wraparounds with different value");

                closure_starts[std::move(new_edge)].emplace_back(transitions_.at(i).dst);
            }
        }

        /* create copies of e-closures leading to lookbehind_1 transitions */

        all_states_map_t all_mapped_states;

        for (auto&& [edge, initial_states] : closure_starts)
        {
            auto& mapped_states{ all_mapped_states[edge] };

            auto pred = [&e = std::as_const(edge)](const tnfa::transition<char_type>& tr) {
                if (std::holds_alternative<tnfa::normal_tr<char_type>>(tr.type))
                    return false;

                if (const auto* const ptr{ std::get_if<lookbehind_1_tr>(&tr.type) }; ptr != nullptr)
                    return (e | ptr->cs) == ptr->cs;

                return true;
            };

            std::vector ec{ epsilon_closure(std::move(initial_states), pred) };

            /* set ec to be keys */
            std::ranges::sort(ec, mapped_states.key_comp());
            const auto [first, last]{ std::ranges::unique(ec) };
            ec.erase(first, last);

            /* create new state for each state in ec; defer making nodes fallback to later */
            std::vector new_states{ std::from_range, ec | std::views::transform([this](auto&&) { return node_create(); }) };
            mapped_states.replace(std::move(ec), std::move(new_states));
        }

        closure_starts.clear();

        /* set continue info and make nodes fallback */

        std::vector<tnfa::continue_at_t> continue_ats;

        for (const auto& edge : wrap_starts)
        {
            const auto& mapped_states{ all_mapped_states.at(edge) };
            const auto mapped_cont{ mapped_states.at(continue_state.value()) };

            if (cont_info_.size() >= std::numeric_limits<tnfa::continue_at_t>::max())
                throw tnfa_error("tagged_nfa::rewrite_sc_lookbehind: maximum size of cont_info_ exceeded");

            continue_ats.emplace_back(std::saturate_cast<tnfa::continue_at_t>(cont_info_.size()));
            cont_info_.emplace_back(mapped_cont);
        }

        if (continue_state.has_value())
        {
            for (const auto& [edge, mapped_states] : all_mapped_states)
            {
                auto it{ closure_wraparounds.find(edge) };

                if (it == closure_wraparounds.end())
                    continue;

                const auto continue_at{ continue_ats.at(it->second) };

                for (const state_t qf : fallback_states)
                {
                    auto it2{ mapped_states.find(qf) };

                    if (it2 == mapped_states.end())
                        continue;

                    const auto& old_n{ nodes_.at(qf) };
                    auto& new_n{ nodes_.at(it2->second) };

                    new_n.is_final = old_n.is_final;
                    new_n.is_fallback = old_n.is_fallback;
                    new_n.final_offset = old_n.final_offset;
                    new_n.continue_at = continue_at;
                }
            }
        }

        /* duplicate transitions inside and leaving copied closures */

        std::vector<tr_index> to_remove{ lb_closures.keys() };

        for (const auto& [edge, mapped_states] : all_mapped_states)
        {
            for (const auto [q, p] : mapped_states)
            {
                if (nodes_.at(q).is_final and not nodes_.at(p).is_final)
                    make_epsilon(p, q); /* a quick hack to make (^\n) work? */

                for (const tr_index i : nodes_.at(q).out_tr)
                {
                    /* reminder: reference may be invalidated after one call to emplace_back (when transitions_ is resized) */
                    const auto& tr{ get_tr(i) };

                    if (const auto* const ptr{ std::get_if<normal_tr>(&tr.type) }; ptr != nullptr)
                    {
                        /* this should be valid since in our tnfa, nodes by construction either only have outgoing
                         * normal transitions, or have outgoing e-transitions (or assertions), but never both */
                        make_epsilon(p, q);
                    }
                    else if (const auto* const ptr{ std::get_if<lookbehind_1_tr>(&tr.type) }; ptr != nullptr)
                    {
                        if (const auto new_edge{ edge & ptr->cs }; not new_edge.empty())
                        {
                            if (const auto it{ mapped_states.find(tr.dst) }; it != mapped_states.end())
                                make_epsilon(p, it->second); /* continue within copied subgraph if possible  */
                            else
                                make_epsilon(p, tr.dst); /* transition from copied subgraph back to main graph */
                        }
                    }
                    else if (not std::holds_alternative<sof_anchor_tr>(tr.type))
                    {
                        /* transition within copied e-closure (if necessary) */
                        if (const auto it{ mapped_states.find(tr.dst) }; it != mapped_states.end())
                            make_copy(p, it->second, tr.type);
                    }
                }
            }
        }

        /* add transitions leading into copied e-closures */

        is_closure_start.resize(transitions_.size(), false);

        for (const auto& [edge, mapped_states] : all_mapped_states)
        {
            for (const auto [q, p] : mapped_states)
            {
                for (const tr_index i : nodes_.at(q).in_tr)
                {
                    if (not is_closure_start.at(i))
                        continue;

                    auto& tr{ transitions_.at(i) };
                    auto* const ptr{ std::get_if<normal_tr>(&tr.type) };

                    if (ptr == nullptr)
                        continue;

                    auto new_edge{ edge & ptr->cs };

                    /* remove edge from existing transition */
                    ptr->cs -= edge;
                    if (ptr->cs.empty())
                        to_remove.emplace_back(i);

                    /* transition from main graph to copied e-closure */
                    if (not new_edge.empty())
                        make_transition(tr.src, p, std::move(new_edge));
                }
            }
        }

        /* remove lookbehind_1 transitions and empty regular transitions */

        std::ranges::sort(to_remove);
        const auto [new_end, old_end]{ std::ranges::unique(to_remove) };
        to_remove.erase(new_end, old_end);

        for (const tr_index i : to_remove)
        {
            auto& tr{ transitions_.at(i) };

            /* delete transition */
            std::erase(nodes_.at(tr.src).out_tr, i);
            std::erase(nodes_.at(tr.dst).in_tr, i);
            tr.unset();
        }
    }

    template<typename CharT>
    constexpr void tagged_nfa<CharT>::rewrite_assertions()
    {
        if (has_sof_anchor_) rewrite_sof_anchors();
        if (has_eof_anchor_) rewrite_eof_anchors();
        if (has_lookbehind_1_) rewrite_sc_lookbehind();
        if (has_lookahead_1_) rewrite_sc_lookahead();

        if (has_sof_anchor_ or has_eof_anchor_ or has_lookbehind_1_ or has_lookahead_1_) remove_dead_and_unreachable_states();
    }

    /* constructor */

    template<typename CharT>
    constexpr tagged_nfa<CharT>::tagged_nfa(const expr_tree<char_type>& ast, fsm_flags flags)
        : capture_info_{ ast.get_capture_info() }, tag_count_{ ast.tag_count() }, flags_{ flags }
    {
        auto& dfn{ nodes_.at(default_final_node) };
        dfn.is_final = true;
        dfn.is_fallback = (flags_.enable_fallback and not flags_.longest_match);

        if (flags_.is_iterator)
            cont_info_.emplace_back(default_start_node);

        thompson(ast);
    }
}

namespace rx::detail::tdfa
{
    template<typename CharT>
    using charset_t = tnfa::charset_t<CharT>;

    template<typename CharT> class factory;
    template<typename CharT> class opt;
    template<typename CharT> class min;

    using reg_t = std::uint_least32_t;
    using continue_at_t = tnfa::continue_at_t;

    struct regop
    {
        struct set
        {
            bool val; /* use false for nil and true for current */

            friend constexpr bool operator==(set, set) noexcept = default;
            friend constexpr std::strong_ordering operator<=>(set, set) noexcept = default;
        };

        struct copy
        {
            reg_t src;

            friend constexpr bool operator==(copy, copy) noexcept = default;
            friend constexpr std::strong_ordering operator<=>(copy, copy) noexcept = default;
        };

        using op_t = std::variant<set, copy>;

        op_t op;
        reg_t dst;

        constexpr regop(reg_t destination, op_t operation)
            : op{ operation }, dst{ destination } {}

        friend constexpr bool operator==(regop, regop) = default;
        friend constexpr std::strong_ordering operator<=>(regop, regop) noexcept = default;
    };

    using regops_t = std::vector<regop>;
    inline constexpr regops_t empty_regops{};

    struct final_node_info
    {
        std::size_t         op_index;
        std::uint_least16_t final_offset;

        friend constexpr bool operator==(const final_node_info&, const final_node_info&) noexcept = default;
        friend constexpr auto operator<=>(const final_node_info&, const final_node_info&) noexcept = default;
    };

    struct fallback_node_info
    {
        std::size_t   op_index;
        continue_at_t continue_at;

        friend constexpr bool operator==(const fallback_node_info&, const fallback_node_info&) noexcept = default;
        friend constexpr auto operator<=>(const fallback_node_info&, const fallback_node_info&) noexcept = default;
    };

    using continue_nodes_t = std::vector<std::size_t>;
    using final_nodes_t    = std::flat_map<std::size_t, final_node_info>;
    using fallback_nodes_t = std::flat_map<std::size_t, fallback_node_info>;
    using final_regs_t     = std::vector<reg_t>;

    inline constexpr continue_at_t no_continue{ std::numeric_limits<continue_at_t>::max() };
    inline constexpr std::size_t   no_transition_regops{ std::numeric_limits<std::size_t>::max() };

    constexpr bool toposort_regops(regops_t::iterator beg, regops_t::iterator end, reg_t regcount);

    template<typename CharT>
    struct transition
    {
        std::size_t next;
        std::size_t op_index; /* use no_transition_regops for no ops */
        charset_t<CharT> cs;
    };

    template<typename CharT>
    struct node
    {
        std::vector<transition<CharT>> tr;
    };
}

namespace rx::detail
{
    template<typename CharT>
    class tagged_dfa
    {
    public:
        using char_type = CharT;

        explicit constexpr tagged_dfa(const tagged_nfa<char_type>& tnfa);
        constexpr void optimise_registers();
        constexpr void minimise_states();

        friend class tdfa::factory<char_type>;
        friend class tdfa::opt<char_type>;
        friend class tdfa::min<char_type>;

        /* observers */

        static constexpr std::size_t match_start{ 0 };

        [[nodiscard]] constexpr const tdfa::node<CharT>& get_node(std::size_t i) const { return nodes_.at(i); }
        [[nodiscard]] constexpr const tdfa::regops_t& get_regops(std::size_t i) const { return (i == tdfa::no_transition_regops) ? tdfa::empty_regops : regops_.at(i); }
        [[nodiscard]] constexpr const tdfa::continue_nodes_t& continue_nodes() const { return continue_nodes_; }
        [[nodiscard]] constexpr const tdfa::final_nodes_t& final_nodes() const { return final_nodes_; }
        [[nodiscard]] constexpr const tdfa::fallback_nodes_t& fallback_nodes() const { return fallback_nodes_; }
        [[nodiscard]] constexpr const tdfa::final_regs_t& final_registers() const { return final_registers_; }
        [[nodiscard]] constexpr std::size_t node_count() const { return nodes_.size(); }
        [[nodiscard]] constexpr std::size_t reg_count() const { return register_count_; }
        [[nodiscard]] constexpr std::size_t tag_count() const { return tag_count_; }
        [[nodiscard]] constexpr const capture_info& get_capture_info() const { return capture_info_; }

        template<typename>
        friend struct tdfa_info;

    private:
        using data_t = std::vector<tdfa::node<char_type>>;
        using regop_data_t = std::vector<tdfa::regops_t>;

        data_t                  nodes_;
        tdfa::continue_nodes_t  continue_nodes_;
        tdfa::final_nodes_t     final_nodes_;
        tdfa::fallback_nodes_t  fallback_nodes_;
        tdfa::final_regs_t      final_registers_;
        regop_data_t            regops_;
        capture_info            capture_info_;
        std::size_t             tag_count_;
        tdfa::reg_t             register_count_{ 0 };
        fsm_flags               flags_;
    };
}

namespace rx::detail::tdfa
{
    /* tnfa -> tdfa conversion */

    using tag_t = int;
    using tag_sequence_t = std::vector<tag_t>;

    using reg_vec = std::vector<reg_t>;

    struct configuration
    {
        std::size_t    tnfa_state{ 0 };
        reg_vec        registers;
        tag_sequence_t tag_seq;

        friend constexpr bool operator==(const configuration&, const configuration&) = default;

        constexpr configuration(std::size_t state, reg_vec reg, tag_sequence_t seq)
            : tnfa_state{ state }, registers{ std::move(reg) }, tag_seq{ std::move(seq) } {}

        constexpr configuration() = default;
    };

    struct closure_entry
    {
        std::size_t    tnfa_state{ 0 };
        reg_vec        registers;
        tag_sequence_t tag_seq;
        tag_sequence_t new_tag_seq;

        [[maybe_unused]] friend constexpr bool operator==(const closure_entry&, const closure_entry&) = default;

        constexpr closure_entry(std::size_t state, reg_vec reg, tag_sequence_t seq = {}, tag_sequence_t newseq = {})
            : tnfa_state{ state }, registers{ std::move(reg) }, tag_seq{ std::move(seq) }, new_tag_seq{ std::move(newseq) } {}

        constexpr closure_entry() = default;

        static constexpr configuration next_config(const closure_entry& ce)
        {
            return { ce.tnfa_state, ce.registers, ce.new_tag_seq };
        }
    };

    using config_set_t = std::vector<configuration>;
    using closure_t = std::vector<closure_entry>;
    using register_map_t = std::flat_map<reg_t, reg_t>;

    template<typename CharT>
    using multistep_closures_t = charset_t<CharT>::template partition_pair_result<closure_entry>;

    template<typename CharT>
    using multistep_closure_t = multistep_closures_t<CharT>::value_type;

    using precedence_t = std::monostate;

    struct node_info
    {
        config_set_t config;
        precedence_t precedence;
        bool         is_fallback{ false };

        friend constexpr bool operator==(const node_info&, const node_info&) = default;
    };

    using state_info_t = std::vector<node_info>;
    using continue_info_t = std::flat_map<std::size_t, std::uint16_t>;

    class tag_op_map
    {
        struct entry
        {
            tag_t       tag;
            regop::op_t op;
            reg_t       reg;
        };

    public:
        constexpr std::pair<reg_t&, bool> lookup(tag_t tag, const regop::op_t& op)
        {
            auto it{ std::ranges::lower_bound(data_, tag, std::less{}, &entry::tag) };

            while (it != std::ranges::end(data_) and it->tag == tag)
            {
                if (it->op == op)
                    return { it->reg, true };
                ++it;
            }

            /* entry not found; insert new entry */
            it = data_.insert(it, entry{ .tag = tag, .op = op, .reg = 0 });
            return { it->reg, false };
        }

    private:
        std::vector<entry> data_;
    };

    template<typename CharT>
    class factory
    {
    public:
        using char_type = CharT;
        using tnfa_t = tagged_nfa<char_type>;
        using tdfa_t = tagged_dfa<char_type>;

        constexpr explicit factory(const tnfa_t& input, tdfa_t& result, std::size_t tag_count);

    private:
        [[nodiscard]] constexpr closure_t e_closure(closure_t&& c) const;
        [[nodiscard]] constexpr std::size_t add_state(tdfa_t& result, const closure_t& c, const precedence_t& p, regops_t& o);
        [[nodiscard]] constexpr multistep_closures_t<char_type> multistep(std::size_t state) const;
        [[nodiscard]] constexpr regops_t transition_regops(closure_t& c, reg_t& regcount, tag_op_map& map) const;
        [[nodiscard]] constexpr regops_t final_regops(const final_regs_t& final_registers, const reg_vec& r, const tag_sequence_t& tag_seq) const;
        [[nodiscard]] constexpr regop::op_t regop_rhs(const std::vector<bool>& hist) const;
        [[nodiscard]] constexpr std::vector<bool> history(const tag_sequence_t& hist, tag_t tag) const;
        [[nodiscard]] constexpr bool mappable(const node_info& state, std::size_t mapped_state, regops_t& o, reg_t regcount) const;

        constexpr void fallback_regops(tdfa_t& result);
        constexpr void backup_regops(tdfa_t& result, std::size_t state, reg_t reg_dst, reg_t reg_src);

        constexpr std::size_t make_initial_state(tdfa_t& result, tnfa::state_t tnfa_state);

        const tnfa_t* tnfa_ptr_;
        state_info_t state_info_;
        continue_info_t cont_info_;
        reg_t tag_count_;
        fsm_flags flags_;
    };

    template<typename CharT>
    constexpr auto factory<CharT>::e_closure(closure_t&& c) const -> closure_t
    {
        static constexpr auto compose = [](const auto& g, const auto& f) {
            return [=]<typename T>(T&& arg) {
                return std::invoke(g, std::invoke(f, std::forward<T>(arg)));
            };
        };

        closure_t new_closure;

        closure_t stack{ std::move(c) };
        std::erase_if(stack, [](const closure_entry& ce) { return not ce.new_tag_seq.empty(); });
        std::ranges::reverse(stack);
        std::vector<bool> visited(tnfa_ptr_->node_count(), false);

        while (not stack.empty())
        {
            if (visited.at(stack.back().tnfa_state))
            {
                stack.pop_back();
                continue;
            }

            new_closure.push_back(std::move(stack.back()));
            stack.pop_back();

            const closure_entry& ce{ new_closure.back() };
            visited.at(ce.tnfa_state) = true;

            using epsilon_t = std::pair<tnfa::state_t, tnfa::epsilon_tr>;
            std::vector et{
                std::from_range,
                tnfa_ptr_->get_node(ce.tnfa_state).out_tr
                | std::views::transform([&](const std::size_t i) { return tnfa_ptr_->get_tr(i); })
                | std::views::filter([](const auto& t) { return std::holds_alternative<tnfa::epsilon_tr>(t.type); })
                | std::views::transform([](const auto& t) -> epsilon_t { return { t.dst, std::get<tnfa::epsilon_tr>(t.type) }; })
            };

            std::ranges::sort(et, std::ranges::greater{}, compose(&tnfa::epsilon_tr::priority, &epsilon_t::second));

            for (const auto& [next, e] : et)
            {
                if (not visited.at(next))
                {
                    auto newer_tag_seq{ ce.new_tag_seq };
                    if (e.tag != 0)
                        newer_tag_seq.push_back(e.tag);
                    stack.emplace_back(next, ce.registers, ce.tag_seq, std::move(newer_tag_seq));
                }
            }
        }

        if (flags_.longest_match)
        {
            /* this version is needed for full matches, but below is needed for laziness in partial matches */
            std::erase_if(new_closure, [this](const closure_entry& ce) -> bool {
                if (tnfa_ptr_->get_node(ce.tnfa_state).is_fallback) return false;
                return 0 != std::ranges::count_if(tnfa_ptr_->get_node(ce.tnfa_state).out_tr,
                                                  [&](const std::size_t i) { return not std::holds_alternative<tnfa::normal_tr<CharT>>(tnfa_ptr_->get_tr(i).type); });
            });
        }
        else
        {
            /* remove all (non-final) states with only e-transitions, and remove all states after first fallback state encountered */
            bool end_found{ false };
            std::erase_if(new_closure, [this, &end_found](const closure_entry& ce) -> bool {
                if (end_found) return true;
                const auto& node{ tnfa_ptr_->get_node(ce.tnfa_state) };
                if (node.is_fallback)
                {
                    end_found = true;
                    return false;
                }
                if (node.is_final) return false;
                return 0 != std::ranges::count_if(tnfa_ptr_->get_node(ce.tnfa_state).out_tr,
                                                  [&](const std::size_t i) { return not std::holds_alternative<tnfa::normal_tr<CharT>>(tnfa_ptr_->get_tr(i).type); });
            });
        }

        return new_closure;
    }

    template<typename CharT>
    constexpr auto factory<CharT>::add_state(tdfa_t& result, const closure_t& c, const precedence_t& p, regops_t& o) -> std::size_t
    {
        node_info current_info{ .config{ std::from_range, c | std::views::transform(closure_entry::next_config) }, .precedence = p };

        const std::size_t new_state{ static_cast<std::size_t>(std::ranges::distance(state_info_.begin(), std::ranges::find(state_info_, current_info))) };

        /* check if state already exists */
        if (new_state < state_info_.size())
            return new_state;

        /* check if state can be mapped to an existing state */
        for (std::size_t mapped_state{ 0 }; mapped_state < state_info_.size(); ++mapped_state)
            if (mappable(current_info, mapped_state, o, result.register_count_))
                return mapped_state;

        /* create new state */
        result.nodes_.emplace_back();
        state_info_.emplace_back(std::move(current_info));

        /* make final regops if state is an accepting state */
        const auto& current_cfg = state_info_.back().config;
        const auto is_final = [this](std::size_t arg) { return tnfa_ptr_->get_node(arg).is_final; };
        const auto it{ std::ranges::find_if(current_cfg, is_final, &configuration::tnfa_state) };
        std::optional<continue_at_t> continue_at;
        if (it != std::ranges::end(current_cfg))
        {
            auto final_ops{ final_regops(result.final_registers_, it->registers, it->tag_seq) };
            const auto& node{ tnfa_ptr_->get_node(it->tnfa_state) };
            const auto final_offset{ node.final_offset };

            if (node.continue_at < tnfa_ptr_->get_cont_info().size())
                continue_at = node.continue_at;

            if (final_ops.empty())
            {
                /* avoid creating empty regop blocks */
                result.final_nodes_.emplace(new_state, final_node_info{ .op_index = no_transition_regops, .final_offset = final_offset });
            }
            else
            {
                result.final_nodes_.emplace(new_state, final_node_info{ .op_index = result.regops_.size(), .final_offset = final_offset });
                result.regops_.emplace_back(std::move(final_ops));
            }
        }

        if (flags_.enable_fallback)
        {
            /* set fallback state status for fallback_regops */
            const auto is_fallback = [&](std::size_t arg) { return tnfa_ptr_->get_node(arg).is_fallback; };
            const auto it2{ std::ranges::find_if(current_cfg, is_fallback, &configuration::tnfa_state) };
            if (it2 != std::ranges::end(current_cfg))
            {
                state_info_.back().is_fallback = true;
                if (continue_at.has_value())
                    cont_info_.emplace_hint(cont_info_.end(), new_state, *continue_at);
            }
        }

        return new_state;
    }

    template<typename CharT>
    constexpr auto factory<CharT>::multistep(std::size_t state) const -> multistep_closures_t<char_type>
    {
        auto configs{ state_info_.at(state).config };

        using elem_t = tnfa::charset_t<char_type>::template ref_pair<closure_entry>;

        std::vector<elem_t> transitions;

        for (const auto& cfg : configs)
        {
            for (std::size_t tr_idx : tnfa_ptr_->get_node(cfg.tnfa_state).out_tr)
            {
                const auto& tr{ tnfa_ptr_->get_tr(tr_idx) };
                if (const auto* const ptr{ std::get_if<tnfa::normal_tr<CharT>>(&tr.type) }; ptr != nullptr)
                    transitions.emplace_back(std::cref(ptr->cs), closure_entry{ tr.dst, cfg.registers, cfg.tag_seq });
            }
        }

        return charset_t<char_type>::partition_ext(transitions);
    }

    template<typename CharT>
    constexpr auto factory<CharT>::transition_regops(closure_t& c, reg_t& regcount, tag_op_map& map) const -> regops_t
    {
        regops_t regops;

        for (closure_entry& ce : c)
        {
            for (tag_t t{ 1 }; std::cmp_less_equal(t, tag_count_); ++t)
            {
                auto h{ history(ce.tag_seq, t) };

                if (not h.empty())
                {
                    regop::op_t op_rhs{ regop_rhs(h) };

                    auto [value_ref, existing]{ map.lookup(t, op_rhs) };

                    if (not existing)
                    {
                        auto i{ regcount++ };
                        value_ref = i;
                        regops.emplace_back(i, op_rhs);
                    }
                    else if (not std::ranges::contains(regops, value_ref, &regop::dst))
                    {
                        regops.emplace_back(value_ref, op_rhs);
                    }

                    ce.registers.at(t - 1) = value_ref;
                }
            }
        }

        return regops;
    }

    template<typename CharT>
    constexpr auto factory<CharT>::final_regops(const final_regs_t& final_registers, const reg_vec& r, const tag_sequence_t& tag_seq) const -> regops_t
    {
        regops_t regops;

        for (tag_t t{ 1 }; std::cmp_less_equal(t, tag_count_); ++t)
        {
            auto h{ history(tag_seq, t) };

            if (not h.empty()) /* assign final registers */
                regops.emplace_back(final_registers.at(t - 1), regop_rhs(h));
            else
                regops.emplace_back(final_registers.at(t - 1), regop::copy{ .src = r.at(t - 1) }); /* copy into final register */
        }

        return regops;
    }

    template<typename CharT>
    constexpr auto factory<CharT>::regop_rhs(const std::vector<bool>& hist) const -> regop::op_t
    {
        return regop::set{ .val = hist.back() };
    }

    template<typename CharT>
    constexpr auto factory<CharT>::history(const tag_sequence_t& h, tag_t tag) const -> std::vector<bool>
    {
        return h | std::views::filter([tag](tag_t i) { return /* std::abs(i) == tag */ (i == -tag) or (i == tag); }) 
                 | std::views::transform([](tag_t i) -> bool { return i >= 0; })
                 | std::ranges::to<std::vector>();
    }

    template<typename CharT>
    constexpr bool factory<CharT>::mappable(const node_info& state, std::size_t mapped_state, regops_t& o, const reg_t regcount) const
    {
        const auto& mapped_state_info{ state_info_.at(mapped_state) };

        /* different precedences also imply differing sets of states (for now at least);
         * we ensure tnfa states have the same tag sequences later                       */

        if (not std::ranges::equal(state.config, mapped_state_info.config, {}, &configuration::tnfa_state, &configuration::tnfa_state))
            return false;

        register_map_t map, rmap;

        for (const auto& ce1 : state.config)
        {
            for (const auto& ce2 : mapped_state_info.config)
            {
                if (ce1.tnfa_state != ce2.tnfa_state)
                    continue;
                if (ce1.tag_seq != ce2.tag_seq)
                    return false; /* see first comment of function */

                for (tag_t t{ 1 }; std::cmp_less_equal(t, tag_count_); ++t)
                {
                    auto h{ history(ce1.tag_seq, t) };

                    if (h.empty())
                    {
                        const reg_t i{ ce1.registers.at(t - 1) };
                        const reg_t j{ ce2.registers.at(t - 1) };

                        if (not map.contains(i) and not rmap.contains(j))
                            map[i] = j, rmap[j] = i;
                        else if (not (map.contains(i) and map.at(i) == j) or not (rmap.contains(j) and rmap.at(j) == i))
                            return false;
                    }
                }
            }
        }

        regops_t o_copy{ o };

        for (auto it{ o_copy.begin() }; it != o_copy.end();)
        {
            const auto i{ it->dst };

            if (not map.contains(i))
            {
                /* skip mapping registers with history */
                it = o_copy.erase(it);
                continue;
            }

            it->dst = map.at(i);
            map.erase(i);
            ++it;
        }

        regops_t o_new;

        for (const auto [j, i] : map)
        {
            if (j == i) continue;
            o_new.emplace_back(i, regop::copy{ .src = j });
        }

        o_new.append_range(o_copy);

        const bool success{ toposort_regops(o_new.begin(), o_new.end(), regcount) };

        if (success)
            o = std::move(o_new);

        return success;
    }

    template<typename CharT>
    constexpr void factory<CharT>::fallback_regops(tdfa_t& result)
    {
        for (const auto [state, fni] : result.final_nodes_)
        {
            /* check if current state is a fallback state */

            if (not state_info_.at(state).is_fallback)
                continue;

            const auto& final_ops{ result.get_regops(fni.op_index) };

            /* determine clobbered registers */

            std::vector<bool> added(result.nodes_.size(), false);
            std::vector<bool> clobbered(result.register_count_, false);

            std::vector<std::pair<std::size_t, std::size_t>> stack;
            stack.emplace_back(state, 0);
            added.at(state) = true;

            while (not stack.empty())
            {
                auto& [s, i]{ stack.back() };

                if (result.final_nodes_.contains(s) or i >= result.nodes_.at(s).tr.size())
                {
                    stack.pop_back();
                }
                else
                {
                    const auto& tr{ result.nodes_.at(s).tr.at(i) };

                    for (const auto& op : result.get_regops(tr.op_index))
                        clobbered.at(op.dst) = true;

                    ++i;
                    if (not added.at(tr.next))
                    {
                        stack.emplace_back(tr.next, 0);
                        added.at(tr.next) = true;
                    }
                }
            }

            /* determine state to restart from in repeated searches */

            auto continuation_index{ tdfa::no_continue };
            if (auto it{ cont_info_.find(state) }; it != cont_info_.end())
                continuation_index = it->second;

            /* insert fallback regops */

            regops_t o;

            for (const auto& f : final_ops)
            {
                if (auto* cpy{ std::get_if<regop::copy>(&f.op) }; cpy != nullptr and clobbered.at(cpy->src))
                    backup_regops(result, state, f.dst, cpy->src);
                else
                    o.emplace_back(f);
            }

            if (o.empty())
            {
                /* avoid creating empty regop blocks */
                result.fallback_nodes_.emplace(state, fallback_node_info{ .op_index = no_transition_regops, .continue_at = continuation_index });
            }
            else
            {
                result.fallback_nodes_.emplace(state, fallback_node_info{ .op_index = result.regops_.size(), .continue_at = continuation_index });
                result.regops_.emplace_back(std::move(o));
            }
        }
    }

    template<typename CharT>
    constexpr void factory<CharT>::backup_regops(tdfa_t& result, std::size_t state, reg_t reg_dst, reg_t reg_src)
    {
        for (auto& tr : result.nodes_.at(state).tr)
        {
            /* not final node => non-accepting path exists from tr.next (end of inputs can be anywhere) */
            if (not result.final_nodes_.contains(tr.next))
            {
                if (tr.op_index == no_transition_regops)
                {
                    tr.op_index = result.regops_.size();
                    result.regops_.emplace_back();
                }

                result.regops_.at(tr.op_index).emplace_back(reg_dst, regop::copy{ reg_src });
            }
        }
    }

    template<typename CharT>
    constexpr std::size_t factory<CharT>::make_initial_state(tdfa_t& result, const tnfa::state_t tnfa_state)
    {
        reg_vec initial_reg(tag_count_);
        std::ranges::iota(initial_reg, 0);

        closure_t initial_cfg;
        initial_cfg.emplace_back(tnfa_state, initial_reg);
        initial_cfg = e_closure(std::move(initial_cfg));
        regops_t regs;
        return add_state(result, initial_cfg, precedence_t{ /* cfg0 */ }, regs);
    }

    template<typename CharT>
    constexpr factory<CharT>::factory(const tnfa_t& input, tdfa_t& result, const std::size_t tag_count)
        : tnfa_ptr_{ &input }, tag_count_{ std::saturate_cast<reg_t>(tag_count) }, flags_{ result.flags_ }
    {
        result.register_count_ = tag_count_ * 2;
        result.final_registers_.resize(tag_count_);
        std::ranges::iota(result.final_registers_, tag_count_);

        const std::size_t initial{ make_initial_state(result, tnfa_ptr_->start_node()) };

        for (const auto& cont : tnfa_ptr_->get_cont_info())
        {
            if (cont.value == tnfa_ptr_->start_node())
                result.continue_nodes_.emplace_back(initial);
            else
                result.continue_nodes_.emplace_back(make_initial_state(result, cont.value));
        }

        for (std::size_t state{ initial }; state < result.nodes_.size(); ++state)
        {
            tag_op_map map;

            for (auto& [cs, cfg] : multistep(state))
            {
                cfg = e_closure(std::move(cfg));
                auto o{ transition_regops(cfg, result.register_count_, map) };
                const auto s{ add_state(result, cfg, precedence_t{ /* cfg */ }, o) };

                /* Add transition to tdfa */
                if (o.empty())
                {
                    /* avoid creating empty regop blocks */
                    result.nodes_.at(state).tr.emplace_back(s, no_transition_regops, std::move(cs));
                }
                else
                {
                    result.nodes_.at(state).tr.emplace_back(s, result.regops_.size(), std::move(cs));
                    result.regops_.emplace_back(std::move(o));
                }
            }
        }

        if (flags_.enable_fallback)
            fallback_regops(result);
    }
}

namespace rx::detail
{
    template<typename CharT>
    constexpr tagged_dfa<CharT>::tagged_dfa(const tagged_nfa<char_type>& tnfa)
        : capture_info_{ tnfa.get_capture_info() }, tag_count_{ tnfa.tag_count() }, flags_{ tnfa.get_flags() }
    {
        tdfa::factory<char_type>{ tnfa, *this, tag_count_ };
    }
}

namespace rx::detail::tdfa
{
    /* regops sorting */

    constexpr bool toposort_regops(const regops_t::iterator beg, const regops_t::iterator end, const reg_t regcount)
    {
        std::vector<std::size_t> indeg(regcount, 0);

        for (auto it{ beg }; it != end; ++it)
        {
            if (const auto* copy{ std::get_if<regop::copy>(&it->op) }; copy != nullptr)
                ++indeg.at(copy->src);
        }

        regops_t o_new;
        regops_t o_copy{ beg, end };
        bool cycle_detected{ false };

        while (not o_copy.empty())
        {
            bool added{ false };

            for (auto it{ o_copy.begin() }; it != o_copy.end();)
            {
                if (indeg.at(it->dst) == 0)
                {
                    if (const auto* copy{ std::get_if<regop::copy>(&it->op) }; copy != nullptr)
                        --indeg.at(copy->src);

                    o_new.emplace_back(*it);
                    it = o_copy.erase(it);
                    added = true;
                }
                else
                {
                    ++it;
                }
            }

            if (not added and not o_copy.empty())
            {
                for (const auto& oc : o_copy)
                {
                    /* ignore copying to self */
                    if (const auto* copy{ std::get_if<regop::copy>(&oc.op) }; copy != nullptr and copy->src != oc.dst)
                    {
                        cycle_detected = true;
                        break;
                    }
                }

                o_new.append_range(o_copy);
                break;
            }
        }

        if (std::cmp_not_equal(std::ranges::distance(beg, end), o_new.size()))
            cycle_detected = true; /* unknown error */

        if (not cycle_detected)
            std::ranges::move(o_copy, beg);

        return not cycle_detected;
    }

    /* tdfa normalisation */

    constexpr regops_t::iterator dedup_regops(regops_t& o, const regops_t::iterator beg, const regops_t::iterator end)
    {
        regops_t::iterator new_end{ end };

        for (auto it{ beg }; it != new_end; ++it)
            new_end = std::remove(it + 1, new_end, *it);

        return o.erase(new_end, end);
    }

    constexpr void normalise_regops(regops_t& o, const reg_t regcount)
    {
        for (auto it{ o.begin() }; it != o.end();)
        {
            auto local_end{ it + 1 };
            if (std::holds_alternative<regop::set>(it->op))
            {
                while (local_end != o.end() and std::holds_alternative<regop::set>(local_end->op))
                    ++local_end;

                local_end = dedup_regops(o, it, local_end);
                std::ranges::sort(it, local_end, std::ranges::less{}, &regop::dst);
                it = local_end;
            }
            else if (std::holds_alternative<regop::copy>(it->op))
            {
                while (local_end != o.end() and std::holds_alternative<regop::copy>(local_end->op))
                    ++local_end;

                local_end = dedup_regops(o, it, local_end);
                toposort_regops(it, local_end, regcount);
                it = local_end;
            }
            else
            {
                std::unreachable();
            }
        }
    }

    /* tdfa optimisation */

    class liveness_matrix
    {
    public:
        explicit constexpr liveness_matrix(std::size_t block_count, reg_t reg_count) : data_(block_count * reg_count, false), reg_count_{ reg_count } {}
        [[nodiscard]] constexpr auto operator[](std::size_t block_idx, reg_t reg) { return data_[(block_idx * reg_count_) + reg]; }
        [[nodiscard]] constexpr auto operator[](std::size_t block_idx, reg_t reg) const { return data_[(block_idx * reg_count_) + reg]; }
        [[nodiscard]] constexpr auto at(std::size_t block_idx, reg_t reg) { return data_.at((block_idx * reg_count_) + reg); }
        [[nodiscard]] constexpr auto at(std::size_t block_idx, reg_t reg) const { return data_.at((block_idx * reg_count_) + reg); }
        [[nodiscard]] constexpr auto row_begin(std::size_t block_idx) { return data_.begin() + static_cast<std::ptrdiff_t>(block_idx * reg_count_); }
        [[nodiscard]] constexpr auto row_end(std::size_t block_idx) { return row_begin(block_idx + 1); }
        [[nodiscard]] constexpr auto row_begin(std::size_t block_idx) const { return data_.begin() + static_cast<std::ptrdiff_t>(block_idx * reg_count_); }
        [[nodiscard]] constexpr auto row_end(std::size_t block_idx) const { return row_begin(block_idx + 1); }
        [[nodiscard]] constexpr reg_t reg_count() const { return reg_count_; }
        [[nodiscard]] constexpr bool block_valid(std::size_t block_idx) const { return (block_idx * reg_count_) < data_.size(); }

    private:
        std::vector<bool> data_;
        reg_t reg_count_;
    };

    class square_matrix
    {
    public:
        explicit constexpr square_matrix(std::size_t reg_count) : data_(reg_count * reg_count, false), reg_count_{ reg_count } {}
        [[nodiscard]] constexpr auto operator[](std::size_t reg1, std::size_t reg2) { return data_[(reg1 * reg_count_) + reg2]; }
        [[nodiscard]] constexpr auto operator[](std::size_t reg1, std::size_t reg2) const { return data_[(reg1 * reg_count_) + reg2]; }
        [[nodiscard]] constexpr auto at(std::size_t reg1, std::size_t reg2) { return data_.at((reg1 * reg_count_) + reg2); }
        [[nodiscard]] constexpr auto at(std::size_t reg1, std::size_t reg2) const { return data_.at((reg1 * reg_count_) + reg2); }
        [[nodiscard]] constexpr std::size_t side_length() const { return reg_count_; }

        constexpr void set_symmetric(std::size_t reg1, std::size_t reg2, bool value)
        {
            at(reg1, reg2) = value;
            at(reg2, reg1) = value;
        }

    private:
        std::vector<bool> data_;
        std::size_t reg_count_;
    };

    template<typename CharT>
    class opt
    {
    public:
        using tdfa_t = tagged_dfa<CharT>;

        constexpr explicit opt(std::size_t i = 2) noexcept : iterations_{ i } {}
        constexpr void operator()(tdfa_t& dfa);

    private:
        using remap_t = std::vector<reg_t>;

        [[nodiscard]] static constexpr remap_t compact_registers(const tdfa_t& dfa);
        [[nodiscard]] static constexpr remap_t allocate_registers(const tdfa_t& dfa, const square_matrix& overlapping_lifetimes);
        [[nodiscard]] constexpr liveness_matrix liveness(const tdfa_t& dfa) const;
        [[nodiscard]] constexpr square_matrix interference(const tdfa_t& dfa, const liveness_matrix& liveness) const;

        static constexpr void rename_registers(tdfa_t& dfa, const remap_t& remap);
        static constexpr void deadcode_elim(tdfa_t& dfa, const liveness_matrix& liveness);
        static constexpr void normalise(tdfa_t& dfa);

        constexpr void make_cfg(const tdfa_t& dfa);

        std::vector<std::vector<std::size_t>> block_graph_;
        std::vector<std::size_t> block_graph_start_;
        std::size_t iterations_;
    };

    template<typename CharT>
    constexpr void opt<CharT>::make_cfg(const tdfa_t& dfa)
    {
        block_graph_start_.clear();
        block_graph_.clear();
        block_graph_.resize(dfa.regops_.size());

        std::vector<std::vector<std::size_t>> successor_blocks(dfa.nodes_.size());
        square_matrix reachable(dfa.nodes_.size());

        for (std::size_t node_idx{ 0 }; node_idx < dfa.nodes_.size(); ++node_idx)
        {
            for (const auto& tr : dfa.nodes_.at(node_idx).tr)
            {
                if (tr.op_index == no_transition_regops)
                    reachable.at(node_idx, tr.next) = true;
                else
                    successor_blocks.at(node_idx).emplace_back(tr.op_index);
            }

            /* add final transitions too */

            if (dfa.final_nodes_.contains(node_idx))
                if (const auto fni{ dfa.final_nodes_.at(node_idx) }; fni.op_index != no_transition_regops)
                    successor_blocks.at(node_idx).emplace_back(fni.op_index);
        }

        /* calculate reachability matrix (initial reachability is added above) */

        for (std::size_t i{ 0 }; i < reachable.side_length(); ++i)
            reachable[i, i] = true;

        for (std::size_t k{ 0 }; k < reachable.side_length(); ++k)
            for (std::size_t i{ 0 }; i < reachable.side_length(); ++i)
                for (std::size_t j{ 0 }; j < reachable.side_length(); ++j)
                    if (not reachable[i, j])
                        reachable[i, j] = reachable[i, k] and reachable[k, j];

        /* construct graph */

        std::vector<std::vector<std::size_t>> nodes_to_edges(dfa.nodes_.size());

        for (std::size_t node_idx{ 0 }; node_idx < dfa.nodes_.size(); ++node_idx)
        {
            std::vector<std::size_t> tmp;

            for (std::size_t i{ 0 }; i < reachable.side_length(); ++i)
                if (reachable.at(node_idx, i))
                    tmp.insert_range(tmp.end(), successor_blocks.at(i));

            std::ranges::sort(tmp);
            auto [beg, end]{ std::ranges::unique(tmp) };
            tmp.erase(beg, end);
            tmp.shrink_to_fit();

            nodes_to_edges.at(node_idx) = std::move(tmp);
        }

        for (std::size_t node_idx{ 0 }; node_idx < dfa.nodes_.size(); ++node_idx)
            for (const auto& tr : dfa.nodes_.at(node_idx).tr)
                if (tr.op_index != no_transition_regops)
                    block_graph_.at(tr.op_index) = nodes_to_edges.at(tr.next);

        block_graph_start_ = std::move(nodes_to_edges.at(dfa.match_start));
    }

    template<typename CharT>
    constexpr auto opt<CharT>::compact_registers(const tdfa_t& dfa) -> remap_t
    {
        std::vector<bool> visited(dfa.register_count_, false);
        remap_t remap_result(dfa.register_count_, std::numeric_limits<reg_t>::max());

        for (auto& block : dfa.regops_)
        {
            for (auto& op : block)
            {
                if (auto* cpy{ std::get_if<regop::copy>(&op.op) }; cpy != nullptr)
                    visited.at(cpy->src) = true;
                visited.at(op.dst) = true;
            }
        }

        reg_t new_reg{ 0 };
        for (std::size_t i{ 0 }; i < dfa.register_count_; ++i)
            if (visited.at(i))
                remap_result.at(i) = new_reg++;

        return remap_result;
    }

    template<typename CharT>
    constexpr void opt<CharT>::rename_registers(tdfa_t& dfa, const remap_t& remap)
    {
        for (auto& block : dfa.regops_)
        {
            for (auto it{ std::ranges::begin(block) }; it != std::ranges::end(block);)
            {
                it->dst = remap.at(it->dst);
                if (auto* cpy{ std::get_if<regop::copy>(&it->op) }; cpy != nullptr)
                {
                    cpy->src = remap.at(cpy->src);

                    if (cpy->src == it->dst)
                        it = block.erase(it); /* remove trivial cycles */
                    else
                        ++it;
                }
                else
                {
                    ++it;
                }
            }
        }

        for (reg_t& reg : dfa.final_registers_)
            reg = remap.at(reg);

        dfa.register_count_ = 1 + std::ranges::max(remap | std::views::filter([](reg_t r) { return r != std::numeric_limits<reg_t>::max(); }));
    }

    template<typename CharT>
    constexpr liveness_matrix opt<CharT>::liveness(const tdfa_t& dfa) const
    {
        /* unfortunately coroutines can't be constexpr */

        class postorder_visitor
        {
        public:
            explicit constexpr postorder_visitor(const std::vector<std::size_t>& block_graph_start, std::size_t block_count)
                : block_added(block_count, false)
            {
                for (const auto i : block_graph_start | std::views::reverse)
                {
                    stack.emplace_back(i, 0);
                    block_added.at(i) = true;
                }
            }

            constexpr std::optional<std::size_t> operator()(const std::vector<std::vector<std::size_t>>& block_graph)
            {
                std::optional<std::size_t> result;

                while (not stack.empty() and not result.has_value())
                {
                    const auto [block_idx, i]{ stack.back() };

                    if (i == block_graph.at(block_idx).size())
                    {
                        result = block_idx;
                        stack.pop_back();
                    }
                    else
                    {
                        const auto next{ block_graph.at(block_idx).at(i) };
                        stack.back().second += 1;
                        if (not block_added.at(next))
                        {
                            stack.emplace_back(next, 0);
                            block_added.at(next) = true;
                        }
                    }
                }

                return result;
            }

        private:
            std::vector<std::pair<std::size_t, std::size_t>> stack;
            std::vector<bool> block_added;
        };

        /* resume liveness implementation */

        liveness_matrix liveness(dfa.regops_.size(), dfa.register_count_);

        /* make registers assigned to in final transitions live */

        for (const auto [_, fni] : dfa.final_nodes_)
            for (const reg_t final_reg : dfa.final_registers_)
                liveness.at(fni.op_index, final_reg) = true;

        for (bool loop{ true }; loop;)
        {
            loop = false;
            postorder_visitor vis{ block_graph_start_, dfa.regops_.size() };

            while (true)
            {
                const auto opt{ vis(block_graph_) };
                if (not opt.has_value())
                    break;

                const auto block_idx{ opt.value() };

                std::vector<bool> current_row_copy(liveness.row_begin(block_idx), liveness.row_end(block_idx));

                for (const std::size_t sblock_idx : block_graph_.at(block_idx))
                {
                    std::vector<bool> successor_row_copy(liveness.row_begin(sblock_idx), liveness.row_end(sblock_idx));

                    for (const auto& op : dfa.regops_.at(sblock_idx) | std::views::reverse) /* todo: in post order? */
                    {
                        if (std::holds_alternative<regop::set>(op.op))
                        {
                            successor_row_copy.at(op.dst) = false;
                        }
                        else if (const auto* cpy{ std::get_if<regop::copy>(&op.op) }; cpy != nullptr)
                        {
                            if (successor_row_copy.at(op.dst))
                            {
                                successor_row_copy.at(op.dst) = false;
                                successor_row_copy.at(cpy->src) = true;
                            }
                        }
                        else
                        {
                            std::unreachable();
                        }
                    }

                    for (std::size_t i{ 0 }; i < current_row_copy.size(); ++i)
                        current_row_copy[i] = current_row_copy[i] or successor_row_copy[i];
                }

                if (not std::equal(current_row_copy.begin(), current_row_copy.end(), liveness.row_begin(block_idx)))
                {
                    std::copy(current_row_copy.begin(), current_row_copy.end(), liveness.row_begin(block_idx));
                    loop = true;
                }
            }
        }

        for (const auto [fallback_state, fbni] : dfa.fallback_nodes_)
        {
            std::vector<bool> current_row;

            if (fbni.op_index == no_transition_regops)
            {
                current_row.assign(liveness.reg_count(), false);

                for (const std::size_t final_reg : dfa.final_registers_)
                    current_row.at(final_reg) = true;
            }
            else
            {
                for (const std::size_t final_reg : dfa.final_registers_)
                    liveness.at(fbni.op_index, final_reg) = true;

                current_row.assign(liveness.row_begin(fbni.op_index), liveness.row_end(fbni.op_index));

                for (const auto& op : dfa.regops_.at(fbni.op_index))
                    current_row.at(op.dst) = false;

                for (const auto& op : dfa.regops_.at(fbni.op_index))
                    if (auto* cpy{ std::get_if<regop::copy>(&op.op) }; cpy != nullptr)
                        current_row.at(cpy->src) = false;
            }

            /* traverse nodes that can possibly fallback to fallback_state */

            std::vector<bool> added(dfa.nodes_.size(), false);
            std::vector<std::pair<std::size_t, std::size_t>> stack;
            stack.emplace_back(fallback_state, 0);
            added.at(fallback_state) = true;

            while (not stack.empty())
            {
                auto& [s, i]{ stack.back() };

                if (dfa.final_nodes_.contains(s) or i >= dfa.nodes_.at(s).tr.size())
                {
                    stack.pop_back();
                }
                else
                {
                    const auto& tr{ dfa.nodes_.at(s).tr.at(i) };

                    /* note: we overapproximate here by assuming the transition function from a state is
                    *       not a total function, making a fallback possible from every non-final state */

                    if (liveness.block_valid(tr.op_index))
                        for (std::size_t j{ 0 }; j < liveness.reg_count(); ++j)
                            liveness[tr.op_index, j] = liveness[tr.op_index, j] or current_row[j];

                    ++i;
                    if (not added.at(tr.next))
                    {
                        stack.emplace_back(tr.next, 0);
                        added.at(tr.next) = true;
                    }
                }
            }
        }

        return liveness;
    }

    template<typename CharT>
    constexpr void opt<CharT>::deadcode_elim(tdfa_t& dfa, const liveness_matrix& liveness)
    {
        for (std::size_t block_idx{ 0 }; block_idx < dfa.regops_.size(); ++block_idx)
        {
            auto& block{ dfa.regops_.at(block_idx) };
            std::vector<bool> current_row_copy(liveness.row_begin(block_idx), liveness.row_end(block_idx));

            for (std::size_t i{ block.size() }; i > 0; --i)
            {
                const auto& op{ block.at(i - 1) };

                if (current_row_copy.at(op.dst))
                {
                    if (std::holds_alternative<regop::set>(op.op))
                        current_row_copy.at(op.dst) = false;
                    else if (const auto* cpy{ std::get_if<regop::copy>(&op.op) }; cpy != nullptr)
                        current_row_copy.at(op.dst) = false, current_row_copy.at(cpy->src) = true;
                    else
                        std::unreachable();
                }
                else
                {
                    /* erase dead operation */
                    block.erase(block.begin() + i - 1);
                }
            }
        }
    }

    template<typename CharT>
    constexpr square_matrix opt<CharT>::interference(const tdfa_t& dfa, const liveness_matrix& liveness) const
    {
        square_matrix overlapping_lifetimes(dfa.register_count_);
        std::vector<std::optional<regop::op_t>> histories(dfa.register_count_);

        for (std::size_t block_idx{ 0 }; block_idx < dfa.regops_.size(); ++block_idx)
        {
            const auto& block{ dfa.regops_.at(block_idx) };

            for (const auto& op : block)
                if (const auto* cpy{ std::get_if<regop::copy>(&op.op) }; cpy != nullptr)
                    histories.at(cpy->src) = op.op;

            for (const auto& op : block)
            {
                if (const auto* set{ std::get_if<regop::set>(&op.op) }; set != nullptr)
                    histories.at(op.dst) = op.op;
                else if (const auto* cpy{ std::get_if<regop::copy>(&op.op) }; cpy != nullptr)
                    histories.at(op.dst) = histories.at(cpy->src);
                else
                    std::unreachable();
            }

            for (const auto& op : block)
            {
                std::vector<bool> current_row_copy(liveness.row_begin(block_idx), liveness.row_end(block_idx));

                if (const auto* set{ std::get_if<regop::set>(&op.op) }; set != nullptr)
                    current_row_copy.at(op.dst) = false;
                else if (const auto* cpy{ std::get_if<regop::copy>(&op.op) }; cpy != nullptr)
                    current_row_copy.at(op.dst) = false, current_row_copy.at(cpy->src) = false;
                else
                    std::unreachable();

                for (const auto& op2 : block)
                    if (histories.at(op2.dst) == histories.at(op.dst))
                        current_row_copy.at(op2.dst) = false;

                for (reg_t i{ 0 }; i < current_row_copy.size(); ++i)
                    if (current_row_copy.at(i))
                        overlapping_lifetimes.set_symmetric(i, op.dst, true);
            }
        }

        return overlapping_lifetimes;
    }

    template<typename CharT>
    constexpr auto opt<CharT>::allocate_registers(const tdfa_t& dfa, const square_matrix& overlapping_lifetimes) -> remap_t
    {
        static constexpr reg_t no_register{ std::numeric_limits<reg_t>::max() };

        remap_t remap_result(dfa.register_count_, no_register);
        remap_t representative_map(dfa.register_count_, no_register);
        std::vector<std::vector<reg_t>> equivalence_classes(dfa.register_count_); 

        for (const auto& block : dfa.regops_)
        {
            for (const auto& op : block)
            {
                if (const auto* cpy{ std::get_if<regop::copy>(&op.op) }; cpy != nullptr and op.dst != cpy->src)
                {
                    reg_t x{ representative_map.at(op.dst) };
                    reg_t y{ representative_map.at(cpy->src) };

                    if (x == no_register and y == no_register)
                    {
                        if (not overlapping_lifetimes.at(op.dst, cpy->src))
                        {
                            representative_map.at(op.dst) = op.dst;
                            representative_map.at(cpy->src) = op.dst;

                            auto& set{ equivalence_classes.at(op.dst) };
                            if (op.dst < cpy->src)
                                set.assign({ op.dst, cpy->src });
                            else if (op.dst > cpy->src)
                                set.assign({ cpy->src, op.dst });
                            else
                                set.assign({ op.dst });
                        }
                    }
                    else if (x != no_register and y == no_register)
                    {
                        if (std::ranges::all_of(equivalence_classes.at(x), [&](reg_t t) { return not overlapping_lifetimes.at(t, cpy->src); }))
                        {
                            representative_map.at(cpy->src) = x;

                            auto& set{ equivalence_classes.at(x) };
                            auto it{ std::ranges::lower_bound(set, cpy->src) };
                            if (it == set.end() or *it != cpy->src)
                                set.insert(it, cpy->src);
                        }
                    }
                    else if (x == no_register and y != no_register)
                    {
                        if (std::ranges::all_of(equivalence_classes.at(y), [&](reg_t t) { return not overlapping_lifetimes.at(t, op.dst); }))
                        {
                            representative_map.at(op.dst) = y;

                            auto& set{ equivalence_classes.at(y) };
                            auto it{ std::ranges::lower_bound(set, op.dst) };
                            if (it == set.end() or *it != op.dst)
                                set.insert(it, op.dst);
                        }
                    }
                }
            }
        }

        /* merge non-interfering equivalence classes */

        for (reg_t x{ 0 }; x < representative_map.size(); ++x)
        {
            if (representative_map.at(x) != x)
                continue;

            for (reg_t y{ 0 }; y < representative_map.size(); ++y)
            {
                if (representative_map.at(y) != y or y <= x)
                    continue;

                bool fail{ false };
                for (const reg_t i : equivalence_classes.at(x))
                {
                    for (const reg_t j : equivalence_classes.at(y))
                    {
                        if (overlapping_lifetimes.at(i, j))
                        {
                            fail = true;
                            break;
                        }
                    }

                    if (fail)
                        break;
                }

                if (not fail)
                {
                    representative_map.at(y) = x;
                    std::vector<reg_t> set_union_result;
                    std::ranges::set_union(equivalence_classes.at(x), equivalence_classes.at(y), std::back_inserter(set_union_result));
                    equivalence_classes.at(x) = std::move(set_union_result);
                    equivalence_classes.at(y) = {};
                }
            }
        }

        /* assign remaining registers to an equivalence class (either new or existing) */

        for (reg_t i{ 0 }; i < representative_map.size(); ++i)
        {
            if (representative_map.at(i) != no_register)
                continue;

            bool success{ false };

            for (reg_t j{ 0 }; j < representative_map.size(); ++j)
            {
                if (representative_map.at(j) != j)
                    continue;

                if (std::ranges::all_of(equivalence_classes.at(j), [&](reg_t t) { return not overlapping_lifetimes.at(i, t); }))
                {
                    representative_map.at(i) = j;

                    auto& set{ equivalence_classes.at(j) };
                    auto it{ std::ranges::lower_bound(set, i) };
                    if (it == set.end() or *it != i)
                        set.insert(it, i);

                    success = true;
                    break;
                }
            }

            if (not success)
            {
                representative_map.at(i) = i;
                equivalence_classes.at(i).assign({ i });
            }
        }

        /* assign new registers */

        reg_t new_register_number{ 0 };

        for (reg_t i{ 0 }; i < representative_map.size(); ++i)
        {
            if (representative_map.at(i) != i)
                continue;

            for (const reg_t j : equivalence_classes.at(i))
                remap_result.at(j) = new_register_number;

            ++new_register_number;
        }

        return remap_result;
    }

    template<typename CharT>
    constexpr void opt<CharT>::normalise(tdfa_t& dfa)
    {
        for (auto& block : dfa.regops_)
            normalise_regops(block, dfa.register_count_);
    }

    template<typename CharT>
    constexpr void opt<CharT>::operator()(tdfa_t& dfa)
    {
        if (dfa.register_count_ == 0)
            return;

        make_cfg(dfa);
        const auto w{ compact_registers(dfa) };
        rename_registers(dfa, w);

        for (std::size_t count{ 0 }; count < iterations_; ++count)
        {
            const auto l{ liveness(dfa) };
            deadcode_elim(dfa, l);
            const auto i{ interference(dfa, l) };
            const auto v{ allocate_registers(dfa, i) };
            rename_registers(dfa, v);
            normalise(dfa);
        }
    }
}

namespace rx::detail
{
    template<typename CharT>
    constexpr void tagged_dfa<CharT>::optimise_registers()
    {
        std::invoke(tdfa::opt<char_type>{}, *this);
    }
}

namespace rx::detail::tdfa
{
    template<typename CharT>
    class min
    {
    public:
        using char_type = CharT;
        using tdfa_t = tagged_dfa<CharT>;

        static constexpr void operator()(tdfa_t& dfa);
        static constexpr void compact_regop_blocks(tdfa_t& dfa);
        static constexpr std::vector<std::vector<std::size_t>> dry_run(const tdfa_t& dfa);

    private:

        using bitset_t = std::vector<bool>;

        using partition_t = std::vector<bitset_t>;

        static constexpr partition_t init_hopcroft_partition(const tdfa_t& dfa);
        static constexpr partition_t hopcroft(const tdfa_t& dfa);
    };

    template<typename CharT>
    constexpr void min<CharT>::compact_regop_blocks(tdfa_t& dfa)
    {
        std::vector<std::size_t> regop_block_map(dfa.regops_.size());
        std::flat_map<regops_t, std::size_t> regop_map;
        typename tdfa_t::regop_data_t new_regops;

        for (std::size_t i{ 0 }; i < dfa.regops_.size(); ++i)
        {
            auto [it, inserted]{ regop_map.try_emplace(dfa.regops_[i], new_regops.size()) };

            if (inserted)
                new_regops.emplace_back(dfa.regops_[i]);

            regop_block_map[i] = it->second;
        }

        /* remap regop block indicies in dfa */

        for (auto& node : dfa.nodes_)
            for (auto& tr : node.tr)
                tr.op_index = (tr.op_index < regop_block_map.size()) ? regop_block_map[tr.op_index] : no_transition_regops;

        for (auto it{ dfa.final_nodes_.begin() }, last{ dfa.final_nodes_.end() }; it != last; ++it)
            it->second.op_index = (it->second.op_index < regop_block_map.size()) ? regop_block_map[it->second.op_index] : no_transition_regops;

        for (auto it{ dfa.fallback_nodes_.begin() }, last{ dfa.fallback_nodes_.end() }; it != last; ++it)
            it->second.op_index = (it->second.op_index < regop_block_map.size()) ? regop_block_map[it->second.op_index] : no_transition_regops;

        dfa.regops_ = std::move(new_regops);
    }

    template<typename CharT>
    constexpr auto min<CharT>::init_hopcroft_partition(const tdfa_t& dfa) -> partition_t
    {
        const std::size_t bitset_size{ dfa.node_count() };

        partition_t partitions;

        /* add single partition for all non-final states */

        partitions.emplace_back(bitset_size, false);
        for (const std::size_t state : dfa.final_nodes().keys())
            partitions.back().at(state) = true;

        partitions.back().flip();

        /* add separate partitions of final states for each different regops block */

        using key_type = std::pair<final_node_info, std::optional<fallback_node_info>>;
        std::flat_map<key_type, bitset_t> final_node_map;

        for (const auto& [state, fni] : dfa.final_nodes_)
        {
            key_type key{ fni, std::nullopt };

            /* assume fallback states are a subset of final states */
            if (const auto it{ dfa.fallback_nodes_.find(state) }; it != dfa.fallback_nodes_.end())
                key.second = it->second;

            auto [it, _]{ final_node_map.try_emplace(std::move(key), bitset_size, false) };
            it->second[state] = true;
        }

        partitions.append_range(final_node_map.values());

        /* we keep partitions sorted with the set containing 0 first */

        std::ranges::sort(partitions, std::ranges::greater{});

        return partitions;
    }

    template<typename CharT>
    constexpr auto min<CharT>::hopcroft(const tdfa_t& dfa) -> partition_t
    {

        const std::size_t bitset_size{ dfa.node_count() };

        /* set initial partitions; let work = partition  */
        partition_t partitions{ init_hopcroft_partition(dfa) };
        partition_t work(partitions);

        while (not work.empty())
        {
            const bitset_t transitions_to{ std::move(work.back()) };

            work.pop_back();

            std::flat_map<std::size_t, std::vector<std::pair<std::reference_wrapper<const charset_t<CharT>>, std::size_t>>> symbol_pairs_map;

            for (std::size_t i{ 0 }; i < dfa.nodes_.size(); ++i)
                for (const auto& tr : dfa.nodes_[i].tr)
                    if (transitions_to[tr.next])
                        symbol_pairs_map[tr.op_index].emplace_back(std::cref(tr.cs), i);

            for (auto smit{ symbol_pairs_map.begin() }, end{ symbol_pairs_map.end() }; smit != end; ++smit)
            {
                for (const auto& states : charset_t<CharT>::partition_contents(smit->second))
                {
                    bitset_t transitions_from(bitset_size, false);

                    for (const std::size_t s : states)
                        transitions_from[s] = true;

                    for (std::size_t p{ 0 }; p < partitions.size(); ++p)
                    {
                        bitset_t intersection(bitset_size, false);
                        bitset_t rel_complement(bitset_size, false);

                        for (std::size_t i{ 0 }; i < bitset_size; ++i)
                        {
                            intersection[i] = partitions[p][i] and transitions_from[i];
                            rel_complement[i] = partitions[p][i] and not transitions_from[i];
                        }

                        const auto i_count{ std::ranges::count(intersection, true) };
                        const auto c_count{ std::ranges::count(rel_complement, true) };

                        using gt = std::ranges::greater;

                        if (i_count > 0 and c_count > 0)
                        {
                            if (std::ranges::contains(work, partitions[p]))
                            {
                                if (const auto it{ std::ranges::lower_bound(work, intersection, gt{}) }; it == work.end() or *it != intersection)
                                    work.emplace(it, intersection);

                                if (const auto it{ std::ranges::lower_bound(work, rel_complement, gt{}) }; it == work.end() or *it != rel_complement)
                                    work.emplace(it, rel_complement);
                            }
                            else if (i_count <= c_count)
                            {
                                if (const auto it{ std::ranges::lower_bound(work, intersection, gt{}) }; it == work.end() or *it != intersection)
                                    work.emplace(it, intersection);
                            }
                            else
                            {
                                if (const auto it{ std::ranges::lower_bound(work, rel_complement, gt{}) }; it == work.end() or *it != rel_complement)
                                    work.emplace(it, rel_complement);
                            }

                            partitions.erase(partitions.begin() + static_cast<std::ptrdiff_t>(p));

                            if (const auto it{ std::ranges::lower_bound(partitions, intersection, gt{}) }; it == partitions.end() or *it != intersection)
                                partitions.emplace(it, std::move(intersection));

                            if (const auto it{ std::ranges::lower_bound(partitions, rel_complement, gt{}) }; it == partitions.end() or *it != rel_complement)
                                partitions.emplace(it, std::move(rel_complement));
                        }
                    }
                }
            }
        }

        /* note: the initial state must be in the first partition */

        /* if we change the type partition_t, we either need to reorder the
           partitions or keep track of the start state in tagged dfas separately */

        return partitions;
    }

    template<typename CharT>
    constexpr void min<CharT>::operator()(tdfa_t& dfa)
    {
        compact_regop_blocks(dfa);
        const partition_t partitions{ hopcroft(dfa) };

        /* create map for node remapping */

        std::vector<std::size_t> state_remap(dfa.node_count(), -1);

        for (std::size_t i{ 0 }; i < partitions.size(); ++i)
        {
            const auto& part{ partitions[i] };
            for (std::size_t j{ 0 }; j < part.size(); ++j)
                if (part[j])
                    state_remap[j] = i;
        }

        /* remap unneeded nodes */

        typename tdfa_t::data_t new_nodes(partitions.size());
        final_nodes_t new_final_nodes;
        fallback_nodes_t new_fallback_nodes;

        bitset_t new_states_visited(partitions.size(), false);

        for (std::size_t i{ 0 }; i < state_remap.size(); ++i)
        {
            std::size_t remapped_state{ state_remap[i] };
            if (not new_states_visited.at(remapped_state))
            {
                new_states_visited.at(remapped_state) = true;

                new_nodes.at(remapped_state) = std::move(dfa.nodes_.at(i));

                for (auto& tr : new_nodes[remapped_state].tr)
                    tr.next = state_remap.at(tr.next);

                if (const auto it{ dfa.final_nodes_.find(i) }; it != dfa.final_nodes_.end())
                    new_final_nodes.try_emplace(remapped_state, it->second);

                if (const auto it{ dfa.fallback_nodes_.find(i) }; it != dfa.fallback_nodes_.end())
                    new_fallback_nodes.try_emplace(remapped_state, it->second);
            }
        }

        dfa.nodes_ = std::move(new_nodes);
        dfa.final_nodes_ = std::move(new_final_nodes);
        dfa.fallback_nodes_ = std::move(new_fallback_nodes);
    }

    template<typename CharT>
    constexpr std::vector<std::vector<std::size_t>> min<CharT>::dry_run(const tdfa_t& dfa)
    {
        /* perform a dry run of hopcroft, but don't make changes to the dfa */
        const partition_t partitions{ hopcroft(dfa) };
        std::vector<std::vector<std::size_t>> result(partitions.size());
        for (std::size_t i{ 0 }; i < partitions.size(); ++i)
            for (std::size_t j{ 0 }; j < partitions[i].size(); ++j)
                if (partitions[i][j])
                    result[i].emplace_back(j);
        return result;
    }
}

namespace rx::detail
{
    template<typename CharT>
    constexpr void tagged_dfa<CharT>::minimise_states()
    {
        std::invoke(tdfa::min<char_type>{}, *this);
    }
}

namespace rx::detail
{
    struct final_capture_info
    {
        explicit consteval final_capture_info(const capture_info& ci)
        {
            using namespace std::placeholders;

            std::vector<capture_info::tag_pair_t> captures_tmp;
            std::vector<capture_info::tag_pair_t> overflow_tmp;

            for (std::size_t i{ 0 }; i < ci.capture_count(); ++i)
            {
                const auto range{ ci.lookup(i) };

                if (std::ranges::size(range) == 1)
                {
                    captures_tmp.emplace_back(*std::ranges::begin(range));
                }
                else
                {
                    const auto begin{ std::saturate_cast<int>(overflow_tmp.size()) };
                    overflow_tmp.append_range(range);
                    const auto end{ std::saturate_cast<int>(overflow_tmp.size()) };
                    captures_tmp.emplace_back(capture_info::pair_entry{ .tag_number = end_of_input_tag, .offset = begin },
                                              capture_info::pair_entry{ .tag_number = start_of_input_tag, .offset = end });
                }
            }

            captures = std::define_static_array(captures_tmp);
            overflow = std::define_static_array(overflow_tmp);
        }

        [[nodiscard]] consteval std::size_t capture_count() const noexcept
        {
            return captures.size();
        }

        [[nodiscard]] consteval bool has_match_start() const
        {
            for (const auto [fst, snd] : captures)
            {
                if (fst.tag_number == end_of_input_tag and snd.tag_number == start_of_input_tag)
                    continue;
                if (fst.tag_number == start_of_input_tag or snd.tag_number == start_of_input_tag)
                    return true;
            }

            return std::ranges::any_of(overflow, [](const capture_info::tag_pair_t& p) {
                return p.first.tag_number == start_of_input_tag or p.second.tag_number == start_of_input_tag;
            });
        }

        /* data members (public so that final_capture_info is structural) */
        static_span<const capture_info::tag_pair_t> captures; /* use invalid tag pairs to point towards overflow being used */
        static_span<const capture_info::tag_pair_t> overflow;
    };

    struct register_operation
    {
        tdfa::reg_t dst;
        tdfa::reg_t cpy_src;
        bool        set_val;
        bool        is_copy; /* true if cpy, false if set */ 
    };

    template<typename CharT>
    struct static_transition
    {
        std::size_t next;
        std::size_t op_index;
        static_span<const std::pair<CharT, CharT>> cs;
    };

    template<typename CharT>
    struct tdfa_info
    {
        using char_type = CharT;

    private:
        static consteval auto make_static_transition(const tdfa::transition<char_type>& tr)
        {
            return static_transition{ tr.next, tr.op_index, static_span{ std::define_static_array(tr.cs.get_intervals()) } };
        }

        static consteval auto make_node_transitions(const tdfa::node<char_type>& n)
        {
            return static_span<const static_transition<char_type>>{ std::define_static_array(n.tr | std::views::transform(make_static_transition)) };
        }

        static consteval auto make_register_operations(const tdfa::regops_t& o) -> static_span<const register_operation>
        {
            return std::define_static_array(o | std::views::transform(
                [](const tdfa::regop& op) consteval -> register_operation {
                    if (const auto* set{ std::get_if<tdfa::regop::set>(&op.op) }; set != nullptr)
                        return { .dst = op.dst, .cpy_src = 0, .set_val = set->val, .is_copy = false };
                    else if (const auto* cpy{ std::get_if<tdfa::regop::copy>(&op.op) }; cpy != nullptr)
                        return { .dst = op.dst, .cpy_src = cpy->src, .set_val = false, .is_copy = true };
                    else
                        std::unreachable();
                }
            ));
        }

    public:
        explicit consteval tdfa_info(const tagged_dfa<char_type>& dfa)
            : nodes{ std::define_static_array(dfa.nodes_ | std::views::transform(make_node_transitions)) },
              regops{ std::define_static_array(dfa.regops_ | std::views::transform(make_register_operations)) },
              continue_nodes{ std::define_static_array(dfa.continue_nodes()) },
              final_nodes{ std::define_static_array(dfa.final_nodes().keys()) },
              final_node_regops{ std::define_static_array(dfa.final_nodes().values()) },
              fallback_nodes{ std::define_static_array(dfa.fallback_nodes().keys()) },
              fallback_node_regops{ std::define_static_array(dfa.fallback_nodes().values()) },
              final_registers{ std::define_static_array(dfa.final_registers()) },
              register_count{ dfa.reg_count() },
              match_start{ dfa.match_start },
              captures{ dfa.get_capture_info() } {}

        /* data members (public so that tdfa_info is structural) */
        static_span<const static_span<const static_transition<char_type>>> nodes;
        static_span<const static_span<const register_operation>> regops;
        static_span<const std::size_t> continue_nodes;
        static_span<const std::size_t> final_nodes;
        static_span<const tdfa::final_node_info> final_node_regops;
        static_span<const std::size_t> fallback_nodes;
        static_span<const tdfa::fallback_node_info> fallback_node_regops;
        static_span<const tdfa::reg_t> final_registers;

        std::size_t register_count{ 0 };
        std::size_t match_start{ 0 };
        final_capture_info captures;
    };

    template<typename CharT, std::size_t N>
    consteval tdfa_info<CharT> compile_pattern(string_literal<CharT, N> pattern, fsm_flags f)
    {
        /* set parser flags as appropriate */
        parser_flags p{};
        if (f.no_captures) p.enable_captures = false;
        if (f.return_bool) p.enable_start_tag = false;

        /* parse pattern string into tree */
        expr_tree ast{ pattern.view(), p };
        if (f.is_search) ast.insert_search_prefix();
        ast.optimise_tags();

        /* convert to tnfa */
        tagged_nfa nfa{ ast, f };
        nfa.rewrite_assertions();

        /* convert to tdfa */
        tagged_dfa dfa{ nfa };
        dfa.optimise_registers();
        dfa.minimise_states();

        return tdfa_info{ dfa };
    }

    template<string_literal Pattern, fsm_flags Flags>
    struct compiled_dfa
    {
        static constexpr auto value{ compile_pattern(Pattern, Flags) };
    };
}

namespace rx::detail
{
    template<string_literal Pattern, fsm_flags Flags>
    struct p1306_matcher;
}

namespace rx
{
    template<std::bidirectional_iterator I, string_literal Pattern, detail::fsm_flags Flags>
    requires std::default_initializable<I> and std::copyable<I> 
    class static_regex_match_result
    {
        using factory = detail::submatch_factory<I>;
        using dfa_t   = detail::compiled_dfa<Pattern, Flags>;

        template<bool Const>
        class proxy_iterator;

    public:
        using size_type              = std::size_t;
        using char_type              = std::remove_cv_t<std::iter_value_t<I>>;
        using submatch_type          = submatch<I>;
        using iterator               = proxy_iterator<false>;
        using reverse_iterator       = std::reverse_iterator<iterator>;
        using const_iterator         = proxy_iterator<true>;
        using const_reverse_iterator = std::reverse_iterator<const_iterator>;

        static constexpr size_type submatch_count{ dfa_t::value.captures.capture_count() };

        constexpr static_regex_match_result() noexcept
        {
            if constexpr (has_registers and not has_enabled)
                reg_.fill(I{});
            if constexpr (has_enabled)
                enabled_.fill(false);
        }

        /* observers */

        [[nodiscard]] constexpr bool has_value() const
        {
            if constexpr (std::contiguous_iterator<I>)
                return std::to_address(match_end_) != std::to_address(I{});
            else
                return match_success_;
        }

        [[nodiscard]] constexpr explicit(false) operator bool() const
        {
            return this->has_value();
        }

        [[nodiscard]] constexpr size_type size() const
        {
            return (this->has_value()) ? submatch_count : 0;
        }

        /* array-like access */

        [[nodiscard]] constexpr submatch_type operator[](size_type n) const noexcept
        {
            template for (constexpr size_type N : std::views::iota(0uz, submatch_count))
            {
                if (n == N)
                    return this->get<N>();
            }
            std::unreachable();
        }

        [[nodiscard]] constexpr submatch_type at(size_type i) const
        {
            this->range_check(i);
            return this->operator[](i);
        }

        /* iterator support */

        [[nodiscard]] constexpr iterator begin() const
        {
            return this->has_value()
                   ? iterator{ this, 0 }
                   : this->end();
        }

        [[nodiscard]] constexpr iterator end() const
        {
            return { this, this->size() };
        }

        [[nodiscard]] constexpr reverse_iterator rbegin() const
        {
            return std::make_reverse_iterator(this->end());
        }

        [[nodiscard]] constexpr reverse_iterator rend() const
        {
            return std::make_reverse_iterator(this->begin());
        }

        [[nodiscard]] constexpr const_iterator cbegin() const
        {
            return this->has_value()
                   ? const_iterator{ this, 0 }
                   : this->end();
        }

        [[nodiscard]] constexpr const_iterator cend() const
        {
            return { this, this->size() };
        }

        [[nodiscard]] constexpr const_reverse_iterator crbegin() const
        {
            return std::make_reverse_iterator(this->cend());
        }

        [[nodiscard]] constexpr const_reverse_iterator crend() const
        {
            return std::make_reverse_iterator(this->cbegin());
        }

        /* tuple support */

        template<size_type N>
        requires (N < submatch_count)
        [[nodiscard]] constexpr submatch_type get() const noexcept
        {
            using namespace detail;

            static constexpr auto current{ dfa_t::value.captures.captures[N] };

            if (not this->has_value())
                return {};

            if constexpr (current.first.tag_number == end_of_input_tag and current.second.tag_number == start_of_input_tag)
            {

                throw std::logic_error("Branch reset not implemented");
            }
            else
            {
                if constexpr (current.first.tag_number == current.second.tag_number)
                {
                    if (this->tag_enabled<current.first.tag_number>())
                    {
                        return factory::make_submatch(
                            std::next(this->get_tag<current.first.tag_number>(), current.first.offset),
                            std::next(this->get_tag<current.second.tag_number>(), current.second.offset)
                        );
                    }
                }
                else
                {
                    if (this->tag_enabled<current.first.tag_number>() and this->tag_enabled<current.second.tag_number>())
                    {
                        return factory::make_submatch(
                            std::next(this->get_tag<current.first.tag_number>(), current.first.offset),
                            std::next(this->get_tag<current.second.tag_number>(), current.second.offset)
                        );
                    }
                }
            }

            return {};
        }

        friend struct detail::p1306_matcher<Pattern, Flags>;

        template<std::ranges::bidirectional_range V, typename Regex>
        requires std::ranges::view<V>
        friend class regex_match_view;

    private:
        /* implementation helpers */

        static constexpr bool has_registers{ dfa_t::value.register_count != 0 };
        static constexpr bool has_success{ not std::contiguous_iterator<I> };
        static constexpr bool has_enabled{ has_registers and has_success };
        static constexpr bool has_match_start{ dfa_t::value.captures.has_match_start() };

        explicit constexpr static_regex_match_result(I start)
            : match_start_{ start }
        {
            if constexpr (has_registers and not has_enabled)
                reg_.fill(I{});
            if constexpr (has_enabled)
                enabled_.fill(false);
        }

        template<detail::tag_number_t N>
        [[nodiscard]] constexpr bool tag_enabled() const
        {
            if constexpr (N == detail::start_of_input_tag or N == detail::end_of_input_tag)
                return true;
            else if constexpr (std::contiguous_iterator<I>)
                return std::to_address(reg_[dfa_t::value.final_registers[N]]) != std::to_address(I{});
            else
                return enabled_[dfa_t::value.final_registers[N]];
        }

        template<detail::tag_number_t N>
        [[nodiscard]] constexpr I get_tag() const
        {
            if constexpr (N == detail::start_of_input_tag)
                return match_start_;
            else if constexpr (N == detail::end_of_input_tag)
                return match_end_;
            else
                return reg_[dfa_t::value.final_registers[N]];
        }

        constexpr void range_check(size_type n) const
        {
            if (n >= this->size())
                throw std::out_of_range("static_regex_match_result::range_check: n >= this->size()");
        }

        /* data members and protected trivial accessors */

        using registers_type   = detail::maybe_type_t<has_registers, std::array<I, dfa_t::value.register_count>>;
        using enabled_type     = detail::maybe_type_t<has_enabled, std::array<bool, dfa_t::value.register_count>>;
        using match_start_type = detail::maybe_type_t<has_match_start, I>;
        using continue_type    = detail::maybe_type_t<Flags.is_iterator, detail::tdfa::continue_at_t>;
        using success_type     = detail::maybe_type_t<has_success, bool>;

        [[no_unique_address]] registers_type reg_{};
        [[no_unique_address]] match_start_type match_start_{};
        I match_end_{};
        [[no_unique_address]] enabled_type enabled_{};
        [[no_unique_address]] continue_type continue_at_{ detail::tdfa::no_continue };
        [[no_unique_address]] success_type match_success_{ false };
    };

    /* iterator implementation */

    template<std::bidirectional_iterator I, string_literal Pattern, detail::fsm_flags Flags>
    requires std::default_initializable<I> and std::copyable<I>
    template<bool Const>
    class static_regex_match_result<I, Pattern, Flags>::proxy_iterator
    {
        friend class static_regex_match_result;

        using Parent      = static_regex_match_result;

    public:
        using iterator_concept  = std::random_access_iterator_tag;
        using iterator_category = std::input_iterator_tag;
#if __cpp_lib_ranges_as_const >= 202311L
        using value_type        = submatch<std::conditional_t<Const, std::const_iterator<I>, I>>;
#else
        using value_type        = submatch<I>; /* semantically incorrect workaround */
#endif
        using difference_type   = std::ptrdiff_t;

        proxy_iterator() = default;

        constexpr proxy_iterator(const Parent* ptr, std::size_t pos)
            : ptr_{ ptr }, pos_{ pos } {}

        constexpr explicit(false) proxy_iterator(proxy_iterator<not Const> i) requires Const
            : ptr_{ i.ptr_ }, pos_{ i.pos_ } {}

        constexpr value_type operator*() const
        {
            return (*ptr_)[pos_];
        }

        constexpr value_type operator[](difference_type n) const
        {
            return (*ptr_)[pos_ + n];
        }

        constexpr proxy_iterator& operator++()
        {
            ++pos_;
            return *this;
        }

        constexpr proxy_iterator operator++(int)
        {
            auto tmp{ *this };
            ++*this;
            return tmp;
        }

        constexpr proxy_iterator& operator--()
        {
            --pos_;
            return *this;
        }

        constexpr proxy_iterator operator--(int)
        {
            auto tmp{ *this };
            --*this;
            return tmp;
        }

        constexpr proxy_iterator& operator+=(difference_type n)
        {
            pos_ += n;
            return *this;
        }

        constexpr proxy_iterator& operator-=(difference_type n)
        {
            pos_ -= n;
            return *this;
        }

        constexpr friend bool operator==(const proxy_iterator&, const proxy_iterator&) = default;

        constexpr friend auto operator<=>(const proxy_iterator&, const proxy_iterator&) = default;

        constexpr friend proxy_iterator operator+(const proxy_iterator& i, difference_type n)
        {
            return { i.ptr_, i.pos_ + n };
        }

        constexpr friend proxy_iterator operator+(difference_type n, const proxy_iterator& i)
        {
            return { i.ptr_, n + i.pos_ };
        }

        constexpr friend proxy_iterator operator-(const proxy_iterator& i, difference_type n)
        {
            return { i.ptr_, i.pos_ - n };
        }

        constexpr friend difference_type operator-(const proxy_iterator& x, const proxy_iterator& y)
        {
            return y.pos_ - x.pos_;
        }

    private:
        const Parent* ptr_{ nullptr };
        std::size_t pos_{ 0 };
    };
}

/* structured binding support for static_regex_match_result */

template<std::bidirectional_iterator Iter, rx::string_literal Pattern, rx::detail::fsm_flags Flags>
struct std::tuple_size<rx::static_regex_match_result<Iter, Pattern, Flags>>
    : integral_constant<std::size_t, rx::static_regex_match_result<Iter, Pattern, Flags>::submatch_count> {};

template<std::size_t N, std::bidirectional_iterator Iter, rx::string_literal Pattern, rx::detail::fsm_flags Flags>
requires (N < rx::static_regex_match_result<Iter, Pattern, Flags>::submatch_count)
struct std::tuple_element<N, rx::static_regex_match_result<Iter, Pattern, Flags>>
{
    using type = rx::static_regex_match_result<Iter, Pattern, Flags>::submatch_type;
};

namespace rx::detail
{
    /* p1306 matcher implementation */

    template<typename CharT, CharT Lower, CharT Upper>
    [[gnu::always_inline]] constexpr bool tr_possible_interval_impl(CharT c)
    {
        if constexpr (Lower == Upper)
            return (c == Lower);
        else
            return (Lower <= c and c <= Upper);
    }

    template<typename CharT, std::pair<CharT, CharT>... Intervals>
    [[gnu::always_inline]] constexpr bool tr_possible_impl(CharT c)
    {
        return (tr_possible_interval_impl<CharT, Intervals.first, Intervals.second>(c) or ...);
    }

    template<typename CharT>
    consteval auto tr_possible_make_refl(static_transition<CharT> tr)
    {
        std::vector<std::meta::info> result{ ^^CharT };
        result.reserve(1 + tr.cs.size());
        for (const auto& pair : tr.cs)
            result.emplace_back(std::meta::reflect_constant(pair));
        return result;
    }

    template<static_transition Tr, typename CharT>
    [[gnu::always_inline]] constexpr bool tr_possible(CharT c)
    {
        constexpr auto func{ std::meta::substitute(^^tr_possible_impl, tr_possible_make_refl(Tr)) };
        return [: func :](c);
    }

    template<string_literal Pattern, fsm_flags Flags>
    struct p1306_matcher
    {
        using dfa_t = compiled_dfa<Pattern, Flags>;
        using char_type = decltype(Pattern)::char_type;

        template<typename I>
        using result_type = static_regex_match_result<I, Pattern, Flags>;

        template<typename I>
        struct result
        {
            template<typename... Args>
            constexpr explicit result(Args... args) noexcept(noexcept(result_type{ args... }))
                : value(std::forward<Args>(args)...) {}

            [[no_unique_address]] result_type<I> value;
        };

    private:
        static constexpr std::size_t fallback_disabled{ std::numeric_limits<std::size_t>::max() };

        template<std::size_t Blk, std::bidirectional_iterator I>
        static constexpr void register_operations(I it, result<I>& res)
        {
            if constexpr (Blk != tdfa::no_transition_regops)
            {
                template for (constexpr register_operation op : dfa_t::value.regops.at(Blk))
                {
                    if constexpr (op.is_copy)
                        res.value.reg_[op.dst] = res.value.reg_[op.cpy_src];
                    else if constexpr (op.set_val)
                        res.value.reg_[op.dst] = it;
                    else if constexpr (std::contiguous_iterator<I>)
                        res.value.reg_[op.dst] = I{};

                    if constexpr (not std::contiguous_iterator<I>)
                    {
                        res.value.enabled_[op.dst] = (op.is_copy or op.set_val);
                    }
                }
            }
        }

        template<std::bidirectional_iterator I, std::sentinel_for<I> S>
        static constexpr void fallback(I /* it */, result<I>& res, const S /* last */, std::size_t fallback_state, I fallback_it)
        requires (Flags.enable_fallback)
        {
            if (fallback_state == fallback_disabled)
                return;

            template for (constexpr std::size_t i : std::views::iota(0uz, dfa_t::value.fallback_nodes.size()))
            {
                static constexpr auto state{ dfa_t::value.fallback_nodes[i] };
                if (fallback_state == state)
                {
                    static constexpr auto key{ std::ranges::lower_bound(dfa_t::value.final_nodes, state) };
                    static_assert(key != dfa_t::value.final_nodes.end() and *key == state);
                    static constexpr auto fni{ dfa_t::value.final_node_regops.at(key - dfa_t::value.final_nodes.begin()) };
                    static constexpr auto fbni{ dfa_t::value.fallback_node_regops.at(i) };

                    register_operations<fbni.op_index>(fallback_it, res);
                    res.value.match_end_ = std::ranges::prev(fallback_it, fni.final_offset);

                    if constexpr (not std::contiguous_iterator<I>)
                        res.value.match_success_ = true;

                    if constexpr (Flags.is_iterator)
                    {
                        if constexpr (fbni.continue_at != tdfa::no_continue)
                            res.value.continue_at_ = fbni.continue_at;
                    }

                    return;
                }
            }
        }

        template<std::size_t DFAState, std::bidirectional_iterator I, std::sentinel_for<I> S>
        static constexpr void state(I it, result<I>& res, const S last, std::size_t fallback_state, I fallback_it)
        {
            if constexpr (Flags.enable_fallback and std::ranges::binary_search(dfa_t::value.fallback_nodes, DFAState))
            {
                fallback_state = DFAState;
                fallback_it = it;
            }

            if (it == last)
            {
                if constexpr (constexpr auto key{ std::ranges::lower_bound(dfa_t::value.final_nodes, DFAState) };
                              key != dfa_t::value.final_nodes.end() and *key == DFAState)
                {
                    static constexpr auto fni{ dfa_t::value.final_node_regops.at(key - dfa_t::value.final_nodes.begin()) };

                    register_operations<fni.op_index>(it, res);
                    res.value.match_end_ = std::ranges::prev(it, fni.final_offset);

                    if constexpr (not std::contiguous_iterator<I>)
                        res.value.match_success_ = true;

                    return;
                }
            }
            else
            {
                template for (constexpr static_transition<char_type> tr : dfa_t::value.nodes.at(DFAState))
                {
                    if (tr_possible<tr>(*it))
                    {
                        register_operations<tr.op_index>(it, res);
                        [[clang::musttail]] return state<tr.next>(++it, res, last, fallback_state, fallback_it);
                    }
                }
            }

            if constexpr (Flags.enable_fallback)
                [[clang::musttail]] return fallback(it, res, last, fallback_state, fallback_it);
        }

    public:
        template<std::bidirectional_iterator I, std::sentinel_for<I> S>
        requires std::is_nothrow_convertible_v<std::iter_value_t<I>, char_type>
        [[nodiscard]] static constexpr auto operator()(const I first, const S last)
        {
            result<I> res{ first };
            state<dfa_t::value.match_start>(first, res, last, fallback_disabled, first);
            return res.value;
        }

        template<std::bidirectional_iterator I, std::sentinel_for<I> S>
        requires std::is_nothrow_convertible_v<std::iter_value_t<I>, char_type>
        [[nodiscard]] static constexpr auto operator()(const I first, const S last, const tdfa::continue_at_t continue_at) requires (Flags.is_iterator)
        {
            result<I> res{ first };

            template for (constexpr std::size_t i : std::views::iota(0uz, dfa_t::value.continue_nodes.size()))
            {
                if (i == continue_at)
                    state<dfa_t::value.continue_nodes[i]>(first, res, last, fallback_disabled, first);
            }

            return res.value;
        }
    };

    template<string_literal Pattern, fsm_flags Flags>
    requires (Flags.return_bool)
    struct p1306_matcher<Pattern, Flags>
    {
        using dfa_t = compiled_dfa<Pattern, Flags>;
        using char_type = decltype(Pattern)::char_type;

    private:
        static constexpr std::size_t fallback_disabled{ std::numeric_limits<std::size_t>::max() };

        template<std::bidirectional_iterator I, std::sentinel_for<I> S>
        static constexpr bool fallback(I /* it */, const S /* last */, std::size_t fallback_state)
        requires (Flags.enable_fallback)
        {
            if (fallback_state == fallback_disabled)
                return false;

            template for (constexpr std::size_t i : std::views::iota(0uz, dfa_t::value.fallback_nodes.size()))
            {
                if (fallback_state == dfa_t::value.fallback_nodes[i])
                    return true;
            }

            return false;
        }

        template<std::size_t DFAState, std::bidirectional_iterator I, std::sentinel_for<I> S>
        static constexpr bool state(I it, const S last, std::size_t fallback_state)
        {
            if constexpr (Flags.enable_fallback and std::ranges::binary_search(dfa_t::value.fallback_nodes, DFAState))
                fallback_state = DFAState;

            if (it == last)
            {
                if constexpr (constexpr auto key{ std::ranges::lower_bound(dfa_t::value.final_nodes, DFAState) };
                              key != dfa_t::value.final_nodes.end() and *key == DFAState)
                    return true;
            }
            else
            {
                template for (constexpr static_transition<char_type> tr : dfa_t::value.nodes.at(DFAState))
                {
                    if (tr_possible<tr>(*it))
                        [[clang::musttail]] return state<tr.next>(++it, last, fallback_state);
                }
            }

            if constexpr (Flags.enable_fallback)
                [[clang::musttail]] return fallback(it, last, fallback_state);

            return false;
        }

    public:
        template<std::bidirectional_iterator I, std::sentinel_for<I> S>
        requires std::is_nothrow_convertible_v<std::iter_value_t<I>, char_type>
        [[nodiscard]] static constexpr bool operator()(const I first, const S last)
        {
            return state<dfa_t::value.match_start>(first, last, fallback_disabled);
        }

        template<std::bidirectional_iterator I, std::sentinel_for<I> S>
        requires std::is_nothrow_convertible_v<std::iter_value_t<I>, char_type>
        static constexpr bool operator()(const I first, const S last, const tdfa::continue_at_t continue_at) = delete;
    };
}

namespace rx
{
    template<string_literal Pattern>
    requires std::same_as<char, typename decltype(Pattern)::char_type> /* temporary: remove later */
    struct static_regex
    {
        using char_type = decltype(Pattern)::char_type;

        template<std::bidirectional_iterator I, std::sentinel_for<I> S>
        requires std::same_as<std::iter_value_t<I>, char_type>
        [[nodiscard]] static constexpr auto match(const I first, const S last)
        {
            using namespace detail::default_fsm_flags;
            detail::p1306_matcher<Pattern, full_match> matcher;
            return matcher(first, last);
        }

        template<std::ranges::bidirectional_range R>
        requires std::same_as<std::ranges::range_value_t<R>, char_type> and std::ranges::borrowed_range<R>
        [[nodiscard]] static constexpr auto match(R&& r)
        {
            return match(std::ranges::begin(r), std::ranges::end(r));
        }

        template<typename CharT>
        requires std::same_as<CharT, char_type>
        [[nodiscard]] static constexpr auto match(const CharT* cstr)
        {
            return match(cstr, detail::cstr_sentinel);
        }

        template<std::bidirectional_iterator I, std::sentinel_for<I> S>
        requires std::same_as<std::iter_value_t<I>, char_type>
        [[nodiscard]] static constexpr auto prefix_match(const I first, const S last)
        {
            using namespace detail::default_fsm_flags;
            detail::p1306_matcher<Pattern, partial_match> matcher;
            return matcher(first, last);
        }

        template<std::ranges::bidirectional_range R>
        requires std::same_as<std::ranges::range_value_t<R>, char_type> and std::ranges::borrowed_range<R>
        [[nodiscard]] static constexpr auto prefix_match(R&& r)
        {
            return prefix_match(std::ranges::begin(r), std::ranges::end(r));
        }

        template<typename CharT>
        requires std::same_as<CharT, char_type>
        [[nodiscard]] static constexpr auto prefix_match(const CharT* cstr)
        {
            return prefix_match(cstr, detail::cstr_sentinel);
        }

        template<std::bidirectional_iterator I, std::sentinel_for<I> S>
        requires std::same_as<std::iter_value_t<I>, char_type>
        [[nodiscard]] static constexpr auto search(const I first, const S last)
        {
            using namespace detail::default_fsm_flags;
            detail::p1306_matcher<Pattern, search_single> matcher;
            return matcher(first, last);
        }

        template<std::ranges::bidirectional_range R>
        requires std::same_as<std::ranges::range_value_t<R>, char_type> and std::ranges::borrowed_range<R>
        [[nodiscard]] static constexpr auto search(R&& r)
        {
            return search(std::ranges::begin(r), std::ranges::end(r));
        }

        template<typename CharT>
        requires std::same_as<CharT, char_type>
        [[nodiscard]] static constexpr auto search(const CharT* cstr)
        {
            return search(cstr, detail::cstr_sentinel);
        }

        template<std::bidirectional_iterator I, std::sentinel_for<I> S>
        requires std::same_as<std::iter_value_t<I>, char_type>
        [[nodiscard]] static constexpr bool is_match(const I first, const S last)
        {
            using namespace detail::default_fsm_flags;
            detail::p1306_matcher<Pattern, full_match | return_bool_modifier> matcher;
            return matcher(first, last);
        }

        template<std::ranges::bidirectional_range R>
        requires std::same_as<std::ranges::range_value_t<R>, char_type>
        [[nodiscard]] static constexpr bool is_match(R&& r)
        {
            return is_match(std::ranges::begin(r), std::ranges::end(r));
        }

        template<typename CharT>
        requires std::same_as<CharT, char_type>
        [[nodiscard]] static constexpr bool is_match(const CharT* cstr)
        {
            return is_match(cstr, detail::cstr_sentinel);
        }

        template<std::bidirectional_iterator I, std::sentinel_for<I> S>
        requires std::same_as<std::iter_value_t<I>, char_type>
        [[nodiscard]] static constexpr bool starts_with_match(const I first, const S last)
        {
            using namespace detail::default_fsm_flags;
            detail::p1306_matcher<Pattern, partial_match | return_bool_modifier> matcher;
            return matcher(first, last);
        }

        template<std::ranges::bidirectional_range R>
        requires std::same_as<std::ranges::range_value_t<R>, char_type>
        [[nodiscard]] static constexpr bool starts_with_match(R&& r)
        {
            return starts_with_match(std::ranges::begin(r), std::ranges::end(r));
        }

        template<typename CharT>
        requires std::same_as<CharT, char_type>
        [[nodiscard]] static constexpr bool starts_with_match(const CharT* cstr)
        {
            return starts_with_match(cstr, detail::cstr_sentinel);
        }

        template<std::bidirectional_iterator I, std::sentinel_for<I> S>
        requires std::same_as<std::iter_value_t<I>, char_type>
        [[nodiscard]] static constexpr bool contains_match(const I first, const S last)
        {
            using namespace detail::default_fsm_flags;
            detail::p1306_matcher<Pattern, search_single | return_bool_modifier> matcher;
            return matcher(first, last);
        }

        template<std::ranges::bidirectional_range R>
        requires std::same_as<std::ranges::range_value_t<R>, char_type>
        [[nodiscard]] static constexpr bool contains_match(R&& r)
        {
            return contains_match(std::ranges::begin(r), std::ranges::end(r));
        }

        template<typename CharT>
        requires std::same_as<CharT, char_type>
        [[nodiscard]] static constexpr bool contains_match(const CharT* cstr)
        {
            return contains_match(cstr, detail::cstr_sentinel);
        }
    };

    template<std::ranges::bidirectional_range V, typename Regex>
    requires std::ranges::view<V>
    class regex_match_view
    {
        static_assert("regex_match_view: invalid range");
    };

    template<std::ranges::bidirectional_range V, string_literal Pattern>
    requires std::ranges::view<V>
    class regex_match_view<V, static_regex<Pattern>> : std::ranges::view_interface<regex_match_view<V, static_regex<Pattern>>>
    {
        template<bool Const>
        struct iterator;

        struct sentinel {};

    public:
        regex_match_view() requires std::default_initializable<V> = default;
        constexpr explicit regex_match_view(V base, static_regex<Pattern> /* regex */) : base_{ std::move(base) } {}

        [[nodiscard]] constexpr V base() const& requires std::copy_constructible<V> { return base_; }
        [[nodiscard]] constexpr V base() && { return std::move(base_); }

        [[nodiscard]] constexpr iterator<false> begin()
        {
            return iterator<false>{ std::ranges::begin(base_), std::ranges::end(base_) };
        }

        [[nodiscard]] constexpr iterator<true> begin() const
        requires std::ranges::bidirectional_range<const V>
        {
            return iterator<true>{ std::ranges::begin(base_), std::ranges::end(base_) };
        }

        [[nodiscard]] constexpr sentinel end()
        {
            return {};
        }

        [[nodiscard]] constexpr sentinel end() const
        requires std::ranges::bidirectional_range<const V>
        {
            return {};
        }

    private:
        V base_{};
    };

    template<std::ranges::bidirectional_range V, string_literal Pattern>
    requires std::ranges::view<V>
    template<bool Const>
    struct regex_match_view<V, static_regex<Pattern>>::iterator
    {
    private:
        using Parent  = detail::maybe_const_t<Const, regex_match_view>;
        using Base    = detail::maybe_const_t<Const, V>;
        using Matcher = detail::p1306_matcher<Pattern, detail::default_fsm_flags::search_all>;

    public:
        using iterator_category = std::input_iterator_tag;
        using iterator_concept  = std::input_iterator_tag;
        using value_type        = static_regex_match_result<std::ranges::iterator_t<Base>, Pattern, detail::default_fsm_flags::search_all>;
        using difference_type   = std::ranges::range_difference_t<Base>;

        iterator() requires std::default_initializable<std::ranges::iterator_t<Base>> = default;

        constexpr iterator(std::ranges::iterator_t<Base> current, std::ranges::sentinel_t<Base> end)
            : current_{ std::move(current) }, end_{ std::move(end) }, result_{ matcher_(current_, end_) } {}

        constexpr explicit(false) iterator(iterator<not Const> i)
        requires Const and std::convertible_to<std::ranges::iterator_t<V>, std::ranges::iterator_t<Base>>
            : current_{ std::move(i.current_) }, end_{ std::move(i.end_) }, result_{ std::move(i.result_) } {}

        constexpr const std::ranges::iterator_t<Base>& base() const& noexcept 
        {
            return current_;
        }

        constexpr std::ranges::iterator_t<Base>& base() && 
        {
            return std::move(current_);
        }

        constexpr const value_type& operator*() const
        {
            return result_;
        }

        constexpr const value_type* operator->() const
        {
            return &result_;
        }

        constexpr iterator& operator++()
        {
            if (not result_.has_value())
                return *this;

            if (current_ == result_.match_end_)
            {
                if (current_ == end_)
                {
                    result_ = value_type{};
                    return *this;
                }
                else
                {
                    ++current_;
                }
            }
            else
            {
                current_ = result_.match_end_;
            }

            result_ = matcher_(current_, end_, result_.continue_at_);
            return *this;
        }

        constexpr void operator++(int)
        {
            ++*this;
        }

        friend constexpr bool operator==(const iterator& x, const iterator& y)
        requires std::equality_comparable<std::ranges::iterator_t<Base>>
        {
            return x.current_ == y.current_;
        }

        friend constexpr auto operator<=>(const iterator& x, const iterator& y)
        requires std::three_way_comparable<std::ranges::iterator_t<Base>>
        {
            return x.current_ <=> y.current_;
        }

        friend constexpr bool operator==(const iterator& x, sentinel /* y */)
        {
            return not x.result_.has_value();
        }

        template<std::ranges::input_range W, int...>
        requires std::ranges::view<W> 
        friend class submatches_view;

    private:
        std::ranges::iterator_t<Base> current_{};
        [[no_unique_address]] std::ranges::sentinel_t<Base> end_{};
        [[no_unique_address]] Matcher matcher_; 
        value_type result_;
    };

    template<typename Range, string_literal Pattern>
    regex_match_view(Range&&, static_regex<Pattern>) -> regex_match_view<std::views::all_t<Range>, static_regex<Pattern>>;

    namespace detail
    {
        template<typename R>
        concept static_regex_match_view_like = template_instantiation_of<std::ranges::range_value_t<R>, ^^static_regex_match_result>;
    }

    template<std::ranges::input_range V, int... Submatches>
    requires std::ranges::view<V>
    class submatches_view
    {
        static_assert("submatches_view: invalid range");
    };

    template<std::ranges::input_range V, int... Submatches>
    requires std::ranges::view<V> and detail::static_regex_match_view_like<V>
    class submatches_view<V, Submatches...> : std::ranges::view_interface<submatches_view<V, Submatches...>>
    {
        using MatchResult = std::ranges::range_value_t<V>;

        template<int Submatch>
        static constexpr bool submatch_is_valid{ ((-1 == Submatch) or (Submatch < MatchResult::submatch_count)) };

        static_assert(sizeof...(Submatches) > 0);
        static_assert((submatch_is_valid<Submatches> and ...));

        template<bool Const>
        struct iterator;

        struct sentinel {};

    public:
        submatches_view() requires std::default_initializable<V> = default;
        constexpr explicit submatches_view(V base, std::integer_sequence<int, Submatches...> /* submatches */) : base_{ std::move(base) } {}

        [[nodiscard]] constexpr V base() const& requires std::copy_constructible<V> { return base_; }
        [[nodiscard]] constexpr V base() && { return std::move(base_); }

        [[nodiscard]] constexpr iterator<false> begin()
        {
            return iterator<false>{ std::ranges::begin(base_), std::ranges::end(base_) };
        }

        [[nodiscard]] constexpr iterator<true> begin() const
        requires std::ranges::bidirectional_range<const V>
        {
            return iterator<true>{ std::ranges::begin(base_), std::ranges::end(base_) };
        }

        [[nodiscard]] constexpr sentinel end()
        {
            return {};
        }

        [[nodiscard]] constexpr sentinel end() const
        requires std::ranges::bidirectional_range<const V>
        {
            return {};
        }

    private:
        V base_{};
    };

    template<std::ranges::input_range V, int... Submatches>
    requires std::ranges::view<V> and detail::static_regex_match_view_like<V>
    template<bool Const>
    struct submatches_view<V, Submatches...>::iterator
    {
    private:
        using Parent   = detail::maybe_const_t<Const, submatches_view>;
        using Base     = detail::maybe_const_t<Const, V>;
        using BaseBase = typename std::ranges::iterator_t<Base>::Base;

    public:
        using iterator_category = std::input_iterator_tag;
        using iterator_concept  = std::input_iterator_tag;
        using value_type        = submatch<std::ranges::iterator_t<BaseBase>>;
        using difference_type   = std::ranges::range_difference_t<Base>;

        iterator()
        requires std::default_initializable<std::ranges::iterator_t<Base>>
                 and std::default_initializable<std::ranges::iterator_t<BaseBase>> = default;

        constexpr iterator(std::ranges::iterator_t<Base> current, std::ranges::sentinel_t<Base> end)
            : current_{ std::move(current) }, end_{ std::move(end) },
              suffix_start_{ std::as_const(current_).base() }
        {
            this->stash_result();
        }

        constexpr explicit(false) iterator(iterator<not Const> i)
        requires Const and std::convertible_to<std::ranges::iterator_t<V>, std::ranges::iterator_t<Base>>
            : current_{ std::move(i.current_) }, end_{ std::move(i.end_) }, index_{ i.index_ },
              result_{ std::move(i.result_) }, suffix_start_{ std::move(i.suffix_start_)} {}

        constexpr const std::ranges::iterator_t<Base>& base() const& noexcept 
        {
            return current_;
        }

        constexpr std::ranges::iterator_t<Base>& base() && 
        {
            return std::move(current_);
        }

        constexpr const value_type& operator*() const
        {
            return result_;
        }

        constexpr const value_type* operator->() const
        {
            return &result_;
        }

        constexpr iterator& operator++()
        {
            ++index_;

            if (index_ == submatches.size() and current_ != end_)
            {
                ++current_;
                index_ = 0;

                if constexpr (has_suffix_iterator)
                    suffix_start_ = current_.current_;
            }

            if constexpr (has_suffix_iterator)
            {
                if (current_ == end_)
                {
                    while (index_ < submatches.size() and submatches[index_] != -1)
                        ++index_;

                    if (index_ == submatches.size())
                        return *this;
                }
            }

            this->stash_result();
            return *this;
        }

        constexpr void operator++(int)
        {
            ++*this;
        }

        friend constexpr bool operator==(const iterator& x, const iterator& y)
        requires std::equality_comparable<std::ranges::iterator_t<Base>>
        {
            return x.current_ == y.current_;
        }

        friend constexpr auto operator<=>(const iterator& x, const iterator& y)
        requires std::three_way_comparable<std::ranges::iterator_t<Base>>
        {
            return x.current_ <=> y.current_;
        }

        friend constexpr bool operator==(const iterator& x, sentinel /* y */)
        {
            if constexpr (has_suffix_iterator)
                return not x.current_->has_value() and x.index_ == submatches.size();
            else
                return not x.current_->has_value();     
        }

    private:
        static constexpr bool has_suffix_iterator{ ((Submatches == -1) or ...) };
        static constexpr std::array submatches{ Submatches... };

        using suffix_start_t = detail::maybe_type_t<has_suffix_iterator, std::ranges::iterator_t<BaseBase>>;

        constexpr void stash_result()
        {
            if constexpr (has_suffix_iterator)
            {
                if (submatches[index_] == -1)
                {
                    using sf = detail::submatch_factory<std::ranges::iterator_t<BaseBase>>;

                    if (current_->has_value())
                    {
                        result_ = sf::make_submatch(suffix_start_, current_->template get<0>().begin());
                    }
                    else
                    {
                        if constexpr (std::ranges::common_range<BaseBase>)
                        {
                            result_ = sf::make_submatch(suffix_start_, current_.end_);
                        }
                        else if constexpr (std::ranges::random_access_range<BaseBase> and std::ranges::sized_range<BaseBase>)
                        {
                            result_ = sf::make_submatch(suffix_start_, suffix_start_ + (current_.end_ - suffix_start_));
                        }
                        else
                        {
                            std::ranges::iterator_t<BaseBase> suffix_end{ suffix_start_ };
                            while (suffix_end != current_.end_)
                                ++suffix_end;

                            result_ = sf::make_submatch(suffix_start_, suffix_end);
                        }

                        if (result_.begin() == result_.end())
                            index_ = submatches.size();
                    }

                    return;
                }
            }

            if (current_->has_value())
                result_ = current_->operator[](submatches[index_]);
        }

        std::ranges::iterator_t<Base> current_{};
        [[no_unique_address]] std::ranges::sentinel_t<Base> end_{};
        std::size_t index_{ 0 };
        value_type  result_;

        [[no_unique_address]] suffix_start_t suffix_start_;
    };

    template<std::ranges::input_range V>
    requires std::ranges::view<V> and detail::static_regex_match_view_like<V>
    class submatches_view<V> : std::ranges::view_interface<submatches_view<V>>
    {
        using MatchResult = std::ranges::range_value_t<V>;
        static constexpr int submatch_limit{ MatchResult::submatch_count };

        template<bool Const>
        struct iterator;

        struct sentinel {};

    public:
        submatches_view() requires std::default_initializable<V> = default;

        template<std::ranges::input_range R>
        requires std::same_as<std::ranges::range_value_t<R>, int>
        constexpr explicit submatches_view(V base, R&& submatches)
            : base_{ std::move(base) }, submatches_{ std::from_range, std::forward<R>(submatches) }
        {
            if (submatches_.empty())
                throw std::invalid_argument("submatches_view::submatches_view: no submatches specified");

            if (not std::ranges::all_of(submatches_, [](int s) { return (-1 == s) or (s < submatch_limit); }))
                throw std::out_of_range("submatches_view::submatches_view: invalid submatch index");
        }

        [[nodiscard]] constexpr V base() const& requires std::copy_constructible<V> { return base_; }
        [[nodiscard]] constexpr V base() && { return std::move(base_); }

        [[nodiscard]] constexpr iterator<false> begin()
        {
            return iterator<false>{ this, std::ranges::begin(base_), std::ranges::end(base_) };
        }

        [[nodiscard]] constexpr iterator<true> begin() const
        requires std::ranges::bidirectional_range<const V>
        {
            return iterator<true>{ this, std::ranges::begin(base_), std::ranges::end(base_) };
        }

        [[nodiscard]] constexpr sentinel end()
        {
            return {};
        }

        [[nodiscard]] constexpr sentinel end() const
        requires std::ranges::bidirectional_range<const V>
        {
            return {};
        }

    private:
        V base_{};
        std::vector<int> submatches_{ 0 };
    };

    template<std::ranges::input_range V>
    requires std::ranges::view<V> and detail::static_regex_match_view_like<V>
    template<bool Const>
    struct submatches_view<V>::iterator
    {
    private:
        using Parent   = detail::maybe_const_t<Const, submatches_view>;
        using Base     = detail::maybe_const_t<Const, V>;
        using BaseBase = typename std::ranges::iterator_t<Base>::Base;

    public:
        using iterator_category = std::input_iterator_tag;
        using iterator_concept  = std::input_iterator_tag;
        using value_type        = submatch<std::ranges::iterator_t<BaseBase>>;
        using difference_type   = std::ranges::range_difference_t<Base>;

        iterator()
        requires std::default_initializable<std::ranges::iterator_t<Base>>
                 and std::default_initializable<std::ranges::iterator_t<BaseBase>> = default;

        constexpr iterator(Parent* parent, std::ranges::iterator_t<Base> current, std::ranges::sentinel_t<Base> end)
            : parent_{ parent }, current_{ std::move(current) }, end_{ std::move(end) },
              suffix_start_{ std::as_const(current_).base() }
        {
            this->stash_result();
        }

        constexpr explicit(false) iterator(iterator<not Const> i)
        requires Const and std::convertible_to<std::ranges::iterator_t<V>, std::ranges::iterator_t<Base>>
            : current_{ std::move(i.current_) }, end_{ std::move(i.end_) }, index_{ i.index_ },
              result_{ std::move(i.result_) }, suffix_start_{ std::move(i.suffix_start_)} {}

        constexpr const std::ranges::iterator_t<Base>& base() const& noexcept 
        {
            return current_;
        }

        constexpr std::ranges::iterator_t<Base>& base() && 
        {
            return std::move(current_);
        }

        constexpr const value_type& operator*() const
        {
            return result_;
        }

        constexpr const value_type* operator->() const
        {
            return &result_;
        }

        constexpr iterator& operator++()
        {
            ++index_;

            if (index_ == parent_->submatches_.size() and current_ != end_)
            {
                ++current_;
                index_ = 0;
                suffix_start_ = current_.current_;
            }

            if (current_ == end_)
            {
                while (index_ < parent_->submatches_.size() and parent_->submatches_.at(index_) != -1)
                    ++index_;

                if (index_ == parent_->submatches_.size())
                    return *this;
            }

            this->stash_result();
            return *this;
        }

        constexpr void operator++(int)
        {
            ++*this;
        }

        friend constexpr bool operator==(const iterator& x, const iterator& y)
        requires std::equality_comparable<std::ranges::iterator_t<Base>>
        {
            return x.current_ == y.current_;
        }

        friend constexpr auto operator<=>(const iterator& x, const iterator& y)
        requires std::three_way_comparable<std::ranges::iterator_t<Base>>
        {
            return x.current_ <=> y.current_;
        }

        friend constexpr bool operator==(const iterator& x, sentinel /* y */)
        {
            return not x.current_->has_value() and x.index_ == x.parent_->submatches_.size();
        }

    private:
        constexpr void stash_result()
        {
            if (parent_->submatches_.at(index_) == -1)
            {
                using sf = detail::submatch_factory<std::ranges::iterator_t<BaseBase>>;

                if (current_->has_value())
                {
                    result_ = sf::make_submatch(suffix_start_, current_->template get<0>().begin());
                }
                else
                {
                    if constexpr (std::ranges::common_range<BaseBase>)
                    {
                        result_ = sf::make_submatch(suffix_start_, current_.end_);
                    }
                    else if constexpr (std::ranges::random_access_range<BaseBase> and std::ranges::sized_range<BaseBase>)
                    {
                        result_ = sf::make_submatch(suffix_start_, suffix_start_ + (current_.end_ - suffix_start_));
                    }
                    else
                    {
                        std::ranges::iterator_t<BaseBase> suffix_end{ suffix_start_ };
                        while (suffix_end != current_.end_)
                            ++suffix_end;

                        result_ = sf::make_submatch(suffix_start_, suffix_end);
                    }

                    if (result_.begin() == result_.end())
                        index_ = parent_->submatches_.size();
                }

                return;
            }

            if (current_->has_value())
                result_ = current_->operator[](parent_->submatches_.at(index_));
        }

        Parent* parent_;
        std::ranges::iterator_t<Base> current_{};
        [[no_unique_address]] std::ranges::sentinel_t<Base> end_{};
        std::size_t index_{ 0 };
        value_type  result_;

        std::ranges::iterator_t<BaseBase> suffix_start_;
    };

    template<typename Range, int... Submatches>
    submatches_view(Range&&, std::integer_sequence<int, Submatches...>) -> submatches_view<std::views::all_t<Range>, Submatches...>;

    template<typename Range, typename Submatches>
    submatches_view(Range&&, Submatches&&) -> submatches_view<std::views::all_t<Range>>;

    namespace detail
    {
        template<typename Regex>
        concept static_regex_like = template_instantiation_of<Regex, ^^static_regex>;
    }

    namespace views
    {
        namespace detail
        {
            template<typename Range, typename Regex>
            concept can_regex_match_view = requires
            {
                regex_match_view(std::declval<Range>(), std::declval<Regex>());
            };

            template<typename Range, typename T>
            concept can_submatches_view = requires
            {
                submatches_view(std::declval<Range>(), std::declval<T>());
            };

            template<typename Regex>
            struct static_regex_adaptor_closure : std::ranges::range_adaptor_closure<static_regex_adaptor_closure<Regex>>
            {
                template<std::ranges::viewable_range Range>
                requires detail::can_regex_match_view<Range, Regex>
                [[nodiscard]] constexpr auto operator()(Range&& r) const
                {
                    return regex_match_view(std::forward<Range>(r), Regex{});
                }
            };

            struct regex_match_adaptor
            {
                template<std::ranges::viewable_range Range, typename Regex>
                requires detail::can_regex_match_view<Range, Regex>
                [[nodiscard]] constexpr auto operator()(Range&& r, Regex&& x) const
                {
                    return regex_match_view(std::forward<Range>(r), std::forward<Regex>(x));
                }

                template<typename Regex>
                requires rx::detail::static_regex_like<Regex>
                [[nodiscard]] consteval auto operator()(Regex /* x */) const
                {
                    return static_regex_adaptor_closure<Regex>();
                }
            };

            template<int... Submatches>
            struct static_submatches_adaptor_closure : std::ranges::range_adaptor_closure<static_submatches_adaptor_closure<Submatches...>>
            {
                template<std::ranges::viewable_range Range>
                requires detail::can_submatches_view<Range, std::integer_sequence<int, Submatches...>>
                [[nodiscard]] constexpr auto operator()(Range&& r) const
                {
                    return submatches_view(std::forward<Range>(r), std::integer_sequence<int, Submatches...>{});
                }
            };

            template<typename T>
            struct dynamic_submatches_adaptor_closure : std::ranges::range_adaptor_closure<dynamic_submatches_adaptor_closure<T>>
            {
                template<std::ranges::viewable_range Range>
                requires detail::can_submatches_view<Range, std::views::all_t<T>>
                [[nodiscard]] constexpr auto operator()(Range&& r) const
                {
                    return submatches_view(std::forward<Range>(r), sub_);
                }

                constexpr explicit dynamic_submatches_adaptor_closure(T submatches) :
                    sub_{ std::forward<T>(submatches) } {}

            private:
                std::views::all_t<T> sub_;
            };

            struct submatches_adaptor
            {
                template<std::ranges::viewable_range Range, typename Submatches>
                requires detail::can_submatches_view<Range, Submatches>
                [[nodiscard]] constexpr auto operator()(Range&& r, Submatches&& x) const
                {
                    return submatches_view(std::forward<Range>(r), std::forward<Submatches>(x));
                }

                template<typename Submatches>
                [[nodiscard]] constexpr auto operator()(Submatches&& submatches) const
                {
                    return dynamic_submatches_adaptor_closure{ std::forward<Submatches>(submatches) };
                }

                template<int... Submatches>
                [[nodiscard]] consteval auto operator()(std::integer_sequence<int, Submatches...>) const
                {
                    return static_submatches_adaptor_closure<Submatches...>();
                }

                template<int... Submatches>
                [[nodiscard]] consteval auto operator()(std::integral_constant<int, Submatches>...) const
                requires (sizeof...(Submatches) == 1)
                {
                    return static_submatches_adaptor_closure<Submatches...>();
                }
            };
        }

        inline constexpr detail::regex_match_adaptor regex_match;
        inline constexpr detail::submatches_adaptor submatches;
    }

    namespace literals
    {
        template<string_literal Pattern>
        consteval static_regex<Pattern> operator ""_rx()
        {
            return {};
        }
    }
}

