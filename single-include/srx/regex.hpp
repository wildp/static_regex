// THIS FILE IS AUTOMATICALLY AMALGAMATED FROM "include/srx/regex.hpp"
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
#include <bitset>
#include <cmath>
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
#include <simd>
#include <span>
#include <stdexcept>
#include <string>
#include <string_view>
#include <tuple>
#include <type_traits>
#include <utility>
#include <variant>
#include <vector>


namespace srx {

template<typename CharT, std::size_t N>
struct string_literal
{
    static_assert(N != 0);
    using value_type = CharT;

    consteval string_literal(const value_type (&str)[N])
    {
        std::ranges::copy_n(str, N, value_);
    }

    [[nodiscard]] constexpr std::basic_string_view<value_type> view() const
    {
        return { value_, N - 1 };
    }

    value_type value_[N]{};
};

}

namespace srx {
namespace detail {

template<class... Ts>
struct overloads : Ts... { using Ts::operator()...; };

template<typename T, typename... Ts>
concept one_of = (std::same_as<T, Ts> or ...);

template<typename T, typename U>
concept not_same_as = (not std::same_as<T, U>);

template<typename CharT>
concept character = one_of<CharT, char, wchar_t, char8_t, char16_t, char32_t>;

consteval bool is_template_instantiation_of_impl(std::meta::info type, std::meta::info templ)
{
    if (not is_type(type) or not is_class_template(templ))
        return false;

    if (not has_template_arguments(dealias(type)))
        return false;

    return template_of(dealias(type)) == templ;
}

template<typename T, template<typename...> typename Template>
concept template_instantiation_of = is_template_instantiation_of_impl(^^T, ^^Template);

consteval bool in_variant_impl(std::meta::info type, std::meta::info variant)
{
    if (not is_template_instantiation_of_impl(dealias(variant), ^^std::variant))
        throw std::invalid_argument("in_variant_impl: variant is not an instantiation of std::variant");

    return std::ranges::contains(template_arguments_of(dealias(variant)), dealias(type), std::meta::dealias);
}

template<typename T, typename Variant>
concept in_variant = in_variant_impl(^^T, ^^Variant);

consteval std::size_t index_in_variant(std::meta::info type, std::meta::info variant)
{
    if (not is_template_instantiation_of_impl(dealias(variant), ^^std::variant))
        throw std::invalid_argument("index_in_variant: variant is not an instantiation of std::variant");

    const auto vartypes = template_arguments_of(dealias(variant));
    const auto it = std::ranges::find(vartypes, dealias(type), std::meta::dealias);

    if (it == vartypes.end())
        throw std::invalid_argument("index_in_variant: type is not an alternative in variant");

    return static_cast<std::size_t>(std::ranges::distance(vartypes.begin(), it));
}

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

template<typename>
struct sequence_helper
{
    static constexpr bool empty{ true };
};

template<typename T, T Head, T... Tail>
struct sequence_helper<std::integer_sequence<T, Head, Tail...>>
{
    static constexpr bool empty{ false };
    static constexpr T head{ Head };
    using tail = std::integer_sequence<T, Tail...>;
};

template<typename T>
concept integer_sequence_like = is_template_instantiation_of_impl(^^T, ^^std::integer_sequence);

namespace hash {

inline constexpr std::size_t fnv_offset_basis{ 0xcbf29ce484222325 };
inline constexpr std::size_t fnv_prime{ 0xcbf29ce484222325 };

consteval std::size_t init() { return fnv_offset_basis; }

template<typename T>
concept memberwise_hashable = std::is_class_v<T> and not std::ranges::range<T>;

template<std::integral T>
constexpr void append(std::size_t& hash, const T& value);

template<std::ranges::range T>
constexpr void append(std::size_t& hash, T&& value);

template<memberwise_hashable T>
constexpr void append(std::size_t& hash, const T& value);

template<std::integral T>
constexpr void append(std::size_t& hash, const T& value)
{
    for (unsigned char byte : std::bit_cast<std::array<unsigned char, sizeof(T)>>(value))
    {
        hash ^= byte;
        hash *= fnv_prime;
    }
}

template<std::ranges::range T>
constexpr void append(std::size_t& hash, T&& value)
{
    for (auto&& elem : value)
        append(hash, elem);
}

template<memberwise_hashable T>
constexpr void append(std::size_t& hash, const T& value)
{
    template for (constexpr auto member : define_static_array(nonstatic_data_members_of(dealias(^^T), std::meta::access_context::unchecked())))
        append(hash, value.[: member
                            :]);
}

}

inline constexpr std::size_t no_tag{ std::numeric_limits<std::size_t>::max() };

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
    constexpr explicit(false) terminal_object(Ts&&...) {}

    template<not_same_as<terminal_object> T>
    constexpr terminal_object& operator=(T&&) { return *this; }
};

template<bool Enabled, typename T>
using maybe_type_t = std::conditional_t<Enabled, T, terminal_object>;

template<bool Const, typename T>
using maybe_const_t = std::conditional_t<Const, const T, T>;

}
}

namespace srx {
namespace detail {

struct fsm_flags
{
    bool is_search       : 1;
    bool longest_match   : 1;
    bool enable_fallback : 1;
    bool is_iterator     : 1;
    bool no_captures     : 1;
    bool return_bool     : 1;
    bool maybe_no_empty  : 1;
    bool adapted_search  : 1;
};

namespace default_fsm_flags {

inline constexpr fsm_flags full_match{
    .is_search       = false,
    .longest_match   = true,
    .enable_fallback = false,
    .is_iterator     = false,
    .no_captures     = false,
    .return_bool     = false,
    .maybe_no_empty  = false,
    .adapted_search  = false,
};

inline constexpr fsm_flags partial_match{
    .is_search       = false,
    .longest_match   = false,
    .enable_fallback = true,
    .is_iterator     = false,
    .no_captures     = false,
    .return_bool     = false,
    .maybe_no_empty  = false,
    .adapted_search  = false,
};

inline constexpr fsm_flags search_single{
    .is_search       = true,
    .longest_match   = false,
    .enable_fallback = true,
    .is_iterator     = false,
    .no_captures     = false,
    .return_bool     = false,
    .maybe_no_empty  = false,
    .adapted_search  = false,
};

inline constexpr fsm_flags search_all{
    .is_search       = true,
    .longest_match   = false,
    .enable_fallback = true,
    .is_iterator     = true,
    .no_captures     = false,
    .return_bool     = false,
    .maybe_no_empty  = true,
    .adapted_search  = false,
};

inline constexpr fsm_flags match_sequential{
    .is_search       = false,
    .longest_match   = false,
    .enable_fallback = true,
    .is_iterator     = true,
    .no_captures     = false,
    .return_bool     = false,
    .maybe_no_empty  = true,
    .adapted_search  = false,
};

}

/* flag adaptors */

constexpr fsm_flags adapt_searcher_flags_to_matcher(fsm_flags f)
{
    f.is_search = false;
    f.adapted_search = true;
    return f;
}

constexpr fsm_flags adapt_flags_return_bool(fsm_flags f)
{
    f.no_captures = true;
    f.return_bool = true;
    return f;
}

constexpr fsm_flags adapt_flags_non_capturing(fsm_flags f)
{
    f.no_captures = true;
    return f;
}

/* packed flag representation */

using ff = unsigned long long;

constexpr ff pack_flags(fsm_flags f)
{
    static constexpr auto nsdms{ define_static_array(nonstatic_data_members_of(^^fsm_flags, std::meta::access_context::unprivileged())) };
    static_assert(nsdms.size() <= std::numeric_limits<ff>::digits);

    std::bitset<nsdms.size()> bitset;

    template for (constexpr std::size_t i : std::views::iota(0uz, nsdms.size()))
        bitset[i] = f.[: nsdms[i] :];

    return bitset.to_ullong();
}

constexpr fsm_flags unpack_flags(ff f)
{
    static constexpr auto nsdms{ define_static_array(nonstatic_data_members_of(^^fsm_flags, std::meta::access_context::unprivileged())) };
    static_assert(nsdms.size() <= std::numeric_limits<ff>::digits);

    std::bitset<nsdms.size()> bitset{ f };
    fsm_flags result{};

    template for (constexpr std::size_t i : std::views::iota(0uz, nsdms.size()))
        result.[: nsdms[i] :] = bitset[i];

    return result;
}

}
}

namespace srx {

class regex_error : public std::runtime_error
{
public:
    using runtime_error::runtime_error;
};

class pattern_error : public regex_error
{
public:
    using regex_error::regex_error;
};

class parser_error : public regex_error
{
public:
    using regex_error::regex_error;
};

class tree_error : public regex_error
{
public:
    using regex_error::regex_error;
};

class tnfa_error : public regex_error
{
public:
    using regex_error::regex_error;
};

}

#ifdef SRX_USE_BOOST_DYNAMIC_BITSET
    #include <boost/dynamic_bitset.hpp>
#endif

namespace srx {
namespace detail {

#ifndef SRX_USE_BOOST_DYNAMIC_BITSET
class vec_bool_adaptor
{
public:
    vec_bool_adaptor() = default;

    constexpr vec_bool_adaptor(std::size_t sz, bool x = false)
        : data_(sz, x) {}

    [[nodiscard]] constexpr auto size() const { return data_.size(); }
    [[nodiscard]] constexpr auto operator[](std::size_t n) { return data_[n]; }
    [[nodiscard]] constexpr auto operator[](std::size_t n) const { return data_[n]; }
    [[nodiscard]] constexpr auto at(std::size_t n) { return data_.at(n); }
    [[nodiscard]] constexpr auto at(std::size_t n) const { return data_.at(n); }
    [[nodiscard]] constexpr auto begin() { return data_.begin(); }
    [[nodiscard]] constexpr auto begin() const { return data_.begin(); }
    [[nodiscard]] constexpr auto end() { return data_.end(); }
    [[nodiscard]] constexpr auto end() const { return data_.end(); }
    [[nodiscard]] constexpr auto cbegin() const { return data_.cbegin(); }
    [[nodiscard]] constexpr auto cend() const { return data_.cend(); }
    [[nodiscard]] constexpr auto rbegin() { return data_.rbegin(); }
    [[nodiscard]] constexpr auto rbegin() const { return data_.rbegin(); }
    [[nodiscard]] constexpr auto rend() { return data_.rend(); }
    [[nodiscard]] constexpr auto rend() const { return data_.rend(); }
    [[nodiscard]] constexpr auto crbegin() const { return data_.crbegin(); }
    [[nodiscard]] constexpr auto crend() const { return data_.crend(); }

    [[nodiscard]] constexpr std::size_t count() const
    {
        std::size_t result{ 0 };
        for (auto a : data_)
            result += a;
        return result;
    }

    constexpr void resize(std::size_t sz, bool x = false)
    {
        data_.resize(sz, x);
    }

    constexpr void push_back(bool x)
    {
        data_.push_back(x);
    }

    constexpr void reset()
    {
        for (auto a : data_)
            a = false;
    }

    constexpr void flip()
    {
        for (auto a : data_)
            a.flip();
    }

    constexpr vec_bool_adaptor& operator&=(const vec_bool_adaptor& other)
    {
        auto zv = std::views::zip(data_, other.data_);
        for (auto [a, b] : zv)
            a = (a and b);
        return *this;
    }

    constexpr vec_bool_adaptor& operator|=(const vec_bool_adaptor& other)
    {
        auto zv = std::views::zip(data_, other.data_);
        for (auto [a, b] : zv)
            a = (a or b);
        return *this;
    }

    constexpr vec_bool_adaptor& operator^=(const vec_bool_adaptor& other)
    {
        auto zv = std::views::zip(data_, other.data_);
        for (auto [a, b] : zv)
            a = (a != b);
        return *this;
    }

    constexpr vec_bool_adaptor& operator-=(const vec_bool_adaptor& other)
    {
        auto zv = std::views::zip(data_, other.data_);
        for (auto [a, b] : zv)
            a = (a and not b);
        return *this;
    }

    [[nodiscard]] constexpr vec_bool_adaptor operator~() const
    {
        vec_bool_adaptor result;
        result.data_.reserve(data_.size());
        for (auto a : data_)
            result.data_.push_back(not a);
        return result;
    }

    [[nodiscard]] friend constexpr vec_bool_adaptor operator&(const vec_bool_adaptor& x, const vec_bool_adaptor& y)
    {
        auto zv = std::views::zip(x.data_, y.data_);
        vec_bool_adaptor result;
        result.data_.reserve(zv.size());
        for (auto [a, b] : zv)
            result.data_.push_back(a and b);
        return result;
    }

    [[nodiscard]] friend constexpr vec_bool_adaptor operator|(const vec_bool_adaptor& x, const vec_bool_adaptor& y)
    {
        auto zv = std::views::zip(x.data_, y.data_);
        vec_bool_adaptor result;
        result.data_.reserve(zv.size());
        for (auto [a, b] : zv)
            result.data_.push_back(a or b);
        return result;
    }

    [[nodiscard]] friend constexpr vec_bool_adaptor operator^(const vec_bool_adaptor& x, const vec_bool_adaptor& y)
    {
        auto zv = std::views::zip(x.data_, y.data_);
        vec_bool_adaptor result;
        result.data_.reserve(zv.size());
        for (auto [a, b] : zv)
            result.data_.push_back(a != b);
        return result;
    }

    [[nodiscard]] friend constexpr vec_bool_adaptor operator-(const vec_bool_adaptor& x, const vec_bool_adaptor& y)
    {
        auto zv = std::views::zip(x.data_, y.data_);
        vec_bool_adaptor result;
        result.data_.reserve(zv.size());
        for (auto [a, b] : zv)
            result.data_.push_back(a and not b);
        return result;
    }

    [[nodiscard]] friend constexpr bool operator==(const vec_bool_adaptor& x, const vec_bool_adaptor& y)
    {
        return x.data_ == y.data_;
    }

    [[nodiscard]] friend constexpr auto operator<=>(const vec_bool_adaptor& x, const vec_bool_adaptor& y)
    {
        return x.data_ <=> y.data_;
    }

private:
    std::vector<bool> data_;
};
#endif

#ifdef SRX_USE_BOOST_DYNAMIC_BITSET
using bitset_t = boost::dynamic_bitset<std::size_t>;
#else
using bitset_t = vec_bool_adaptor;
#endif

}
}

namespace srx {
namespace detail {

template<typename CharT>
class bitcharset
{
    using integer_type = std::uint64_t;
    static constexpr std::size_t byte_bits{ std::numeric_limits<unsigned char>::digits };
    static constexpr std::size_t integer_bits{ std::numeric_limits<integer_type>::digits };
    static constexpr std::size_t total_size{ (0b1uz << (sizeof(CharT) * byte_bits)) };
    static constexpr std::size_t array_size{ total_size / integer_bits };
    static constexpr std::size_t min_offset{ (std::is_signed_v<CharT>) ? (array_size / 2) : 0z };
    static constexpr auto offset_max = static_cast<int>(integer_bits);

    static constexpr std::size_t acceptable_numbers_of_bits_in_a_byte{ 8 };
    static_assert(byte_bits == acceptable_numbers_of_bits_in_a_byte);
    static_assert(sizeof(CharT) < sizeof(int));

public:
    using char_type = CharT;
    using char_interval = std::pair<char_type, char_type>;

    bitcharset() = default;

    template<typename... Args>
        requires (sizeof...(Args) >= 1) and ((std::convertible_to<Args, char_type> or std::convertible_to<Args, char_interval>) and ...)
    constexpr explicit bitcharset(Args... args) noexcept
    {
        ([&]{ insert(args); }(), ...);
    }

    /* observers */

    [[nodiscard]] constexpr bool empty() const noexcept
    {
        integer_type result{ 0uz };
        for (std::size_t i{ 0 }; i < array_size; ++i)
            result |= data_[i];
        return (result == 0);
    }

    [[nodiscard]] constexpr bool full() const noexcept
    {
        std::size_t result{ 0uz };
        for (std::size_t i{ 0 }; i < array_size; ++i)
            result |= ~data_[i];
        return (result == 0);
    }

    [[nodiscard]] constexpr std::size_t count() const noexcept
    {
        std::size_t result{ 0 };
        for (std::size_t i{ 0 }; i < array_size; ++i)
            result += std::popcount(data_[i]);
        return result;
    }

    [[nodiscard]] constexpr std::size_t interval_count() const noexcept
    {
        std::size_t interval_count{ 0 };
        bool carry{ false };

        for (std::size_t i{ 0 }; i < array_size; ++i)
        {
            integer_type tmp{ data_[(i + min_offset) % array_size] };
            int offset{ 0 };

            while (true)
            {
                const int zeros{ std::countr_zero(tmp) };

                offset += zeros;

                if (offset >= offset_max)
                {
                    carry = false;
                    break;
                }

                tmp >>= zeros;

                const int ones{ std::countr_one(tmp) }; /* note: ones >= 1 is always true */

                if (not (zeros == 0 and carry))
                    ++interval_count;

                carry = false;
                offset += ones;

                if (offset >= offset_max)
                {
                    carry = true;
                    break;
                }

                tmp >>= ones;
            }
        }

        return interval_count;
    }

    [[nodiscard]] constexpr int score_intervals() const noexcept
    {
        int score{ 0 };
        bool carry{ false };

        for (std::size_t i{ 0 }; i < array_size; ++i)
        {
            integer_type tmp{ data_[(i + min_offset) % array_size] };
            int offset{ 0 };

            while (true)
            {
                const int zeros{ std::countr_zero(tmp) };

                offset += zeros;

                if (offset >= offset_max)
                {
                    carry = false;
                    break;
                }

                tmp >>= zeros;

                const int ones{ std::countr_one(tmp) }; /* note: ones >= 1 is always true */

                if (not (zeros == 0 and carry))
                {
                    if (ones == 1)
                        score += 1;
                    else
                        score += 2;
                }

                carry = false;
                offset += ones;

                if (offset >= offset_max)
                {
                    carry = true;
                    break;
                }

                tmp >>= ones;
            }
        }

        if (contains(std::numeric_limits<char_type>::min()) and contains(std::numeric_limits<char_type>::min() + 1))
            --score;

        if (contains(std::numeric_limits<char_type>::max()) and contains(std::numeric_limits<char_type>::max() - 1))
            --score;

        return score;
    }

    [[nodiscard]] constexpr bool contains(char_type c) const noexcept
    {
        /* widen to accommodate signed chars */
        const int input{ static_cast<std::make_unsigned_t<char_type>>(c) };

        return data_[input / integer_bits] & (0b1uz << (input % integer_bits));
    }

    [[nodiscard]] constexpr std::vector<char_interval> get_intervals() const
    {
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
                const auto prev_pos = position;

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
        const std::size_t beg{ static_cast<std::make_unsigned_t<char>>(first) };
        const std::size_t end{ static_cast<std::make_unsigned_t<char>>(last) + 1u };

        const std::size_t select1{ beg / integer_bits };
        const std::size_t select2{ end / integer_bits };

        std::array<std::size_t, array_size> mask1{};
        mask1[select1] = (0b1uz << (beg % integer_bits)) - 1;
        std::ranges::fill_n(mask1.begin(), select1, ~0uz);

        if constexpr (std::is_signed_v<char_type>)
        {
            if (std::signbit(first) != std::signbit(last))
                for (std::size_t i{ 0 }; i < array_size; ++i)
                    mask1[i] = ~mask1[i];
        }

        std::array<std::size_t, array_size> mask2{};
        mask2[select2] ^= (0b1uz << (end % integer_bits)) - 1;
        std::ranges::fill_n(mask2.begin(), select2, ~0uz);

        for (std::size_t i{ 0 }; i < array_size; ++i)
            data_[i] |= mask1[i] ^ mask2[i];
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
        static constexpr auto index = uppercase_beg / integer_bits;
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
    constexpr void insert(char_interval ci) noexcept { return insert(ci.first, ci.second); }

    std::array<integer_type, array_size> data_{};
};

template<typename CharT>
constexpr auto bitcharset<CharT>::partition(const std::vector<ref>& input) -> partition_result
{
    std::vector<bitcharset> partitions;

    if (input.size() <= 1 )
    {
        if (not input.empty())
            partitions.emplace_back(input.back().get());
        return partitions;
    }

    /* note: partitions.size() >= 1 is always true, since for each iteration we insert
     *       at least one element into next_gen per element in partitions, since at
     *       most one of partitions[i] & val and partitions[i] & ~val will be empty */
    partitions.emplace_back();
    partitions.back().negate();

    for (const bitcharset& val : input)
    {
        std::vector<bitcharset> next_gen;
        const auto partition_size = partitions.size();
        next_gen.reserve(partition_size * 2);

        for (std::size_t i{ 0 }; i < partition_size; ++i)
            if (auto cs = partitions[i] & val; not cs.empty())
                next_gen.emplace_back(cs);

        const auto complement = ~val;

        for (std::size_t i{ 0 }; i + 1 < partition_size; ++i)
            if (auto cs = partitions[i] & complement; not cs.empty())
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
    using part_pair = std::pair<bitcharset, bitset_t>;

    partition_pair_result<T> result;

    if (input.size() <= 1)
    {
        if (not input.empty())
            result.emplace_back(input.back().first, std::vector<T>{ input.back().second });
        return result;
    }

    std::vector<part_pair> partitions(1);
    partitions.back().first.negate();

    for (const auto& [val, _] : input)
    {
        std::vector<part_pair> next_gen;
        next_gen.reserve(partitions.size() * 2);

        const auto complement = ~val.get();

        for (const auto& [v, from] : partitions)
        {
            if (auto cs = v & complement; not cs.empty())
            {
                next_gen.emplace_back(cs, from);
                next_gen.back().second.push_back(false);
            }
        }

        for (const auto& [v, from] : partitions)
        {
            if (auto cs = v & val.get(); not cs.empty())
            {
                next_gen.emplace_back(cs, from);
                next_gen.back().second.push_back(true);
            }
        }

        partitions = std::move(next_gen);
    }

    const bitset_t empty(input.size(), false);

    for (const auto& [v, from] : partitions)
    {
        if (from == empty)
            continue;

        result.emplace_back(v, std::vector<T>{});
        for (std::size_t i{ 0 }, i_end{ input.size() }; i < i_end; ++i)
            if (from.at(i))
                result.back().second.emplace_back(input[i].second);
    }

    return result;
}

template<typename CharT>
template<typename T>
constexpr auto bitcharset<CharT>::partition_contents(const std::vector<ref_pair<T>>& input) -> partition_contents_result<T>
{
    using part_pair = std::pair<bitcharset, bitset_t>;

    partition_contents_result<T> result;

    if (input.size() <= 1)
    {
        if (not input.empty())
            return result.emplace_back(std::vector<T>{ input.back().second });
        return result;
    }

    std::vector<part_pair> partitions(1);
    partitions.back().first.negate();

    for (const auto& [val, _] : input)
    {
        std::vector<part_pair> next_gen;
        next_gen.reserve(partitions.size() * 2);

        const auto complement = ~val.get();

        for (const auto& [v, from] : partitions)
        {
            if (auto cs = v & complement; not cs.empty())
            {
                next_gen.emplace_back(cs, from);
                next_gen.back().second.push_back(false);
            }
        }

        for (const auto& [v, from] : partitions)
        {
            if (auto cs = v & val.get(); not cs.empty())
            {
                next_gen.emplace_back(cs, from);
                next_gen.back().second.push_back(true);
            }
        }

        partitions = std::move(next_gen);
    }

    const bitset_t empty(input.size(), false);

    for (const auto& [v, from] : partitions)
    {
        if (from == empty)
            continue;

        result.emplace_back();
        for (std::size_t i{ 0 }, i_end{ input.size() }; i < i_end; ++i)
            if (from.at(i))
                result.back().emplace_back(input[i].second);
    }

    return result;
}

}
}

namespace srx {
namespace detail {

template<typename T, typename CharT>
concept charset_interval_range = std::ranges::contiguous_range<T>
                                 and std::same_as<std::ranges::range_value_t<T>, std::pair<CharT, CharT>>;

template<typename CharT>
class charset
{
public:
    using char_type = CharT;
    using char_interval = std::pair<char_type, char_type>;

    charset() = default;

    template<typename... Args>
        requires (sizeof...(Args) >= 1) and ((std::convertible_to<Args, char_type> or std::convertible_to<Args, char_interval>) and ...)
    constexpr explicit charset(Args... args)
    {
        ([&]{ insert(args); }(), ...);
    }

    /* observers */

    [[nodiscard]] constexpr bool empty() const noexcept
    {
        return data_.empty();
    }

    [[nodiscard]] constexpr bool full() const noexcept
    {
        return data_.size() == 1
               and data_[0].first == std::numeric_limits<CharT>::min()
               and data_[0].second == std::numeric_limits<CharT>::max();
    }

    [[nodiscard]] constexpr std::size_t count() const noexcept
    {
        std::size_t result{ 0 };
        for (const auto& [first, second] : data_)
            result += (second + 1 - first);
        return result;
    }

    [[nodiscard]] constexpr std::size_t interval_count() const noexcept
    {
        return data_.size();
    }

    [[nodiscard]] constexpr int score_intervals() const noexcept
    {
        int score{ 0 };

        for (const auto& [beg, end] : data_)
        {
            if (beg == end)
                score += 1;
            else
                score += 2;
        }

        if (not data_.empty())
        {
            if (const auto& [beg, end] = data_.front(); beg == std::numeric_limits<char_type>::min() and end != std::numeric_limits<char_type>::min())
                --score;
            if (const auto& [beg, end] = data_.back(); end == std::numeric_limits<char_type>::max() and beg != std::numeric_limits<char_type>::max())
                --score;
        }

        return score;
    }

    [[nodiscard]] constexpr bool should_invert() const noexcept
    {
        /* an extremely simple heuristic (this could probably be improved?) */
        return data_.size() > 0
               and data_.front().first == std::numeric_limits<CharT>::min()
               and data_.back().second == std::numeric_limits<CharT>::max();
    }

    [[nodiscard]] constexpr bool contains(char_type c) const
    {
        const auto it = std::ranges::lower_bound(data_, c, {}, &char_interval::second);
        if (it == data_.end())
            return false;
        return c >= it->first;
    }

    [[nodiscard]] constexpr const std::vector<char_interval>& get_intervals() const& noexcept
    {
        return data_;
    }

    [[nodiscard]] constexpr std::vector<char_interval> get_intervals() && noexcept
    {
        return std::move(data_);
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
    using partition_entry = std::pair<char_interval, bitset_t>;
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
    static constexpr auto part_make_map(const partitioned_intervals& part) -> std::flat_map<bitset_t, charset>;

    constexpr void insert(char_interval ci) { return insert(ci.first, ci.second); }

    underlying_t data_;
};

template<typename CharT>
constexpr auto charset<CharT>::insert_single(const char_type c) -> underlying_t::iterator
{
    if (data_.empty())
        return data_.emplace(data_.end(), c, c);

    /* form a closed interval */
    auto lower_bound = data_.begin();
    auto upper_bound = std::ranges::prev(data_.end());

    while (true)
    {
        auto midpoint = lower_bound + ((upper_bound - lower_bound) / 2);

        if (midpoint->first != std::numeric_limits<char_type>::min() and c + 1 == midpoint->first)
        {
            /* extend range of pair by 1 */
            midpoint->first -= 1;

            /* attempt to merge with (mid - 1) */
            if (midpoint != data_.begin())
            {
                if (auto prev = std::ranges::prev(midpoint); midpoint->first - prev->second <= 1)
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
            if (auto next = std::ranges::next(midpoint); next != data_.end() and next->first - midpoint->second <= 1)
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
        auto next = std::ranges::next(inserted);

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

    auto lit = std::ranges::cbegin(lhs);
    auto rit = std::ranges::cbegin(rhs);
    const auto lend = std::ranges::cend(lhs);
    const auto rend = std::ranges::cend(rhs);

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

    for (auto it = result.begin(); it != result.end();)
    {
        const auto erase_begin = std::ranges::next(it);
        auto next = erase_begin;

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
    underlying_t result;

    if (lhs.empty())
    {
        result.assign_range(rhs);
        return result;
    }
    if (rhs.empty())
    {
        result.assign_range(lhs);
        return result;
    }

    auto lit = std::ranges::cbegin(lhs);
    auto rit = std::ranges::cbegin(rhs);
    const auto lend = std::ranges::cend(lhs);
    const auto rend = std::ranges::cend(rhs);

    std::optional<char_type> tmp;

    while (lit != lend and rit != rend)
    {
        const auto [min_first, max_first] = std::minmax(lit->first, rit->first);
        const auto min_first_or_tmp = tmp.value_or(min_first);

        const auto [min_second, max_second] = std::minmax(lit->second, rit->second);
        auto& smaller_it = (lit->second < rit->second) ? lit : rit;

        if (min_second < max_first)
        {
            if (not result.empty() and result.back().second + 1 == min_first_or_tmp)
                result.back().second = min_second;
            else
                result.emplace_back(min_first_or_tmp, min_second);

            tmp.reset();
            ++smaller_it;
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
                ++smaller_it;
            }
            else /* (min_second == max_second) */
            {
                tmp.reset();
                ++lit;
                ++rit;
            }
        }
    }

    if (lit != lend or rit != rend)
    {
        auto it = (lit != lend) ? lit : rit;
        const auto end = (lit != lend) ? lend : rend;
        const auto min_first_or_tmp = tmp.value_or(it->first);

        if (not result.empty() and result.back().second + 1 == min_first_or_tmp)
            result.back().second = it->second;
        else
            result.emplace_back(min_first_or_tmp, it->second);

        ++it;

        while (it != end)
        {
            result.emplace_back(it->first, it->second);
            ++it;
        }
    }

    return result;
}

template<typename CharT>
constexpr auto charset<CharT>::make_relative_complement(const charset_interval_range<char_type> auto& lhs, const charset_interval_range<char_type> auto& rhs) -> underlying_t
{
    underlying_t result;

    if (rhs.empty())
    {
        result.assign_range(lhs);
        return result;
    }

    auto lit = lhs.cbegin();
    auto rit = rhs.cbegin();
    const auto lend = lhs.cend();
    const auto rend = rhs.cend();

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
    if (auto it = part.begin() + current_idx + 1; it + 1 != part.end())
    {
        if (*it >= *(it + 1))
        {
            /* re-sort lookahead */
            const auto pos = std::ranges::lower_bound(it + 1, part.end(), it->first, {}, &partition_entry::first);
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
    if (auto it = part.begin() + current_idx + 1; it + 1 != part.end())
    {
        if (*it >= *(it + 1))
        {
            /* re-sort lookahead */
            const auto pos = std::ranges::lower_bound(it + 1, part.end(), it->first, {}, &partition_entry::first);
            if (pos == part.end() or *pos != *it)
            {
                const auto rot = std::ranges::rotate(it, it + 1, pos);

                /* insert new pair */
                const auto pos2 = std::ranges::upper_bound(std::ranges::begin(rot) + 1, part.end(), to_insert);
                if (pos2 == part.end() or *pos2 != to_insert)
                    part.emplace(pos2, std::move(to_insert));
            }
            else
            {
                /* remove lookahead as a duplicate */
                if (to_insert >= *(it + 1))
                {
                    const auto pos2 = std::ranges::lower_bound(it + 1, part.end(), to_insert.first, {}, &partition_entry::first);
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
            const auto pos2 = std::ranges::lower_bound(it + 1, part.end(), to_insert.first, {}, &partition_entry::first);
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

    for (auto it = part.begin(); it != part.end(); ++it)
    {
        const auto duplicate_begin = std::ranges::next(it);
        auto duplicate_it = duplicate_begin;

        for (; duplicate_it != part.end() and duplicate_it->first == it->first; ++duplicate_it)
            for (std::size_t j{ 0 }, j_end{ std::min(it->second.size(), duplicate_it->second.size()) }; j < j_end; ++j)
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
        auto& [current, current_mask] = part[i];
        auto& [lookahead, lookahead_mask] = part[i + 1];

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
                current_mask |= lookahead_mask;
                part_sort_lookahead(part, i);
            }
            else
            {
                /* ---------
                 * ---------
                 */

                current_mask |= lookahead_mask;
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
                lookahead_mask |= current_mask;
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
                lookahead_mask |= current_mask;
                part_sort_lookahead_and_insert(part, i, std::move(to_insert));
            }
            else
            {
                /* --------------
                 *    --------
                 */

                partition_entry to_insert{ { lookahead.second + 1, current.second }, current_mask };
                current.second = lookahead.first - 1;
                lookahead_mask |= current_mask;
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
constexpr auto charset<CharT>::part_make_map(const partitioned_intervals& part) -> std::flat_map<bitset_t, charset>
{
    std::flat_map<bitset_t, charset> map;

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

    for (std::size_t i{ 0 }, i_end{ input.size() }; i < i_end; ++i)
    {
        bitset_t mask(input.size(), false);
#ifdef SRX_USE_BOOST_DYNAMIC_BITSET
        mask[i] = true;
#else
        mask[input.size() - i - 1] = true;
#endif
        for (const auto& pair : input[i].get().data_)
            part.emplace_back(pair, mask);
    }

    part_sort_and_dedup(part);
    part_merge_intervals(part);
    const auto map = part_make_map(part);

    return map.values();
}

template<typename CharT>
template<typename T>
constexpr auto charset<CharT>::partition_ext(const std::vector<ref_pair<T>>& input) -> partition_pair_result<T>
{
    partition_pair_result<T> result;

    if (input.size() <= 1)
    {
        if (not input.empty())
            result.emplace_back(input.back().first, std::vector<T>{ input.back().second });
        return result;
    }

    partitioned_intervals part;

    for (std::size_t i{ 0 }, i_end{ input.size() }; i < i_end; ++i)
    {
        bitset_t mask(input.size(), false);
#ifdef SRX_USE_BOOST_DYNAMIC_BITSET
        mask[i] = true;
#else
        mask[input.size() - i - 1] = true;
#endif
        for (const auto& pair : input[i].first.get().data_)
            part.emplace_back(pair, mask);
    }

    part_sort_and_dedup(part);
    part_merge_intervals(part);
    auto map = part_make_map(part);

    for (auto it = map.begin(), end{ map.end() }; it != end; ++it)
    {
        result.emplace_back(std::move(it->second), std::vector<T>{});
        for (std::size_t i{ 0 }, i_end{ input.size() }; i < i_end; ++i)
#ifdef SRX_USE_BOOST_DYNAMIC_BITSET
            if (it->first.at(i))
#else
            if (it->first.at(input.size() - i - 1))
#endif
                result.back().second.emplace_back(input[i].second);
    }

    return result;
}

template<typename CharT>
template<typename T>
constexpr auto charset<CharT>::partition_contents(const std::vector<ref_pair<T>>& input) -> partition_contents_result<T>
{
    partition_contents_result<T> result;

    if (input.size() <= 1)
    {
        if (not input.empty())
            return result.emplace_back(std::vector<T>{ input.back().second });
        return result;
    }

    if (input.empty())
        return {};
    else if (input.size() == 1)
        return { { input.back().second } };

    partitioned_intervals part;

    for (std::size_t i{ 0 }, i_end{ input.size() }; i < i_end; ++i)
    {
        bitset_t mask(input.size(), false);
#ifdef SRX_USE_BOOST_DYNAMIC_BITSET
        mask[i] = true;
#else
        mask[input.size() - i - 1] = true;
#endif
        for (const auto& pair : input[i].first.get().data_)
            part.emplace_back(pair, mask);
    }

    part_sort_and_dedup(part);
    part_merge_intervals(part);
    const auto map = part_make_map(part);

    for (auto it = map.cbegin(), end{ map.cend() }; it != end; ++it)
    {
        result.emplace_back();
        for (std::size_t i{ 0 }, i_end{ input.size() }; i < i_end; ++i)
#ifdef SRX_USE_BOOST_DYNAMIC_BITSET
            if (it->first.at(i))
#else
            if (it->first.at(input.size() - i - 1))
#endif
                result.back().emplace_back(input[i].second);
    }

    return result;
}

}
}

namespace srx {
namespace detail {

template<typename T>
class static_span
{
public:
    /* types */
    using element_type           = std::add_const_t<T>;
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
    consteval static_span() noexcept = default;

    template<std::size_t N = std::dynamic_extent>
    consteval static_span(std::span<element_type, N> span) noexcept
        : data_{ span.data() }, size_{ span.size() } {}

    template<typename R>
        requires (std::same_as<std::remove_cvref_t<std::ranges::range_value_t<R>>, value_type> and not std::same_as<R, static_span>)
    consteval static_span(R&& r)
        : static_span(std::define_static_array(std::forward<R>(r))) {}

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
static_span(R&&) -> static_span<std::ranges::range_value_t<R>>;

template<typename Key, typename T, typename Compare = std::less<std::remove_cv_t<Key>>>
class static_map
{
public:
    /* types */
    using key_type               = Key;
    using mapped_type            = T;
    using element_type           = const std::pair<key_type, mapped_type>;
    using value_type             = std::remove_cv_t<element_type>;
    using key_compare            = Compare;
    using const_reference        = const value_type&;
    using reference              = const_reference;
    using size_type              = size_t;
    using difference_type        = ptrdiff_t;
    using const_iterator         = const value_type*;
    using iterator               = const_iterator;
    using reverse_iterator       = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    /* constructors and assignment */
    consteval static_map() noexcept : static_map(key_compare()) {}
    consteval explicit static_map(const key_compare& comp) noexcept : data_{} , compare_{ comp } {}

    template<typename K, typename V, typename Cmp, typename KeyCont, typename MappedCont>
    consteval explicit static_map(const std::flat_map<K, V, Cmp, KeyCont, MappedCont>& map)
        : data_{ std::vector<value_type>(std::from_range, map) }, compare_{ map.key_comp() } {}

    /* size and other observers */
    [[nodiscard]] constexpr bool empty() const noexcept { return data_.empty(); }
    [[nodiscard]] constexpr size_type size() const noexcept { return data_.size(); }
    [[nodiscard]] constexpr key_compare key_comp() const { return compare_; }

    /* iterators */
    [[nodiscard]] constexpr const_iterator begin() const noexcept { return data_.begin(); }
    [[nodiscard]] constexpr const_iterator end() const noexcept { return data_.end(); }
    [[nodiscard]] constexpr const_iterator cbegin() const noexcept { return this->begin(); }
    [[nodiscard]] constexpr const_iterator cend() const noexcept { return this->end(); }
    [[nodiscard]] constexpr const_reverse_iterator rbegin() const noexcept { return std::make_reverse_iterator(this->begin()); }
    [[nodiscard]] constexpr const_reverse_iterator rend() const noexcept { return std::make_reverse_iterator(this->end()); }
    [[nodiscard]] constexpr const_reverse_iterator crbegin() const noexcept { return this->rbegin(); }
    [[nodiscard]] constexpr const_reverse_iterator crend() const noexcept { return this->rend(); }

    /* element access */
    [[nodiscard]] constexpr const mapped_type& at(const key_type& x) const
    {
        const auto it = std::ranges::lower_bound(data_, x, compare_, &element_type::first);
        if (it == data_.end() or compare_(it->first, x) or compare_(x, it->first))
            throw std::out_of_range("static_map::at");
        return it->second;
    }

    [[nodiscard]] constexpr const mapped_type* at_if(const key_type& x) const &
    {
        const auto it = std::ranges::lower_bound(data_, x, compare_, &element_type::first);
        if (it == data_.end() or compare_(it->first, x) or compare_(x, it->first))
            return nullptr;
        return &(it->second);
    }

    constexpr const mapped_type* at_if(const key_type& x) && = delete;

    /* map operations */
    [[nodiscard]] constexpr const_iterator find(const key_type& x) const
    {
        const auto it = std::ranges::lower_bound(data_, x, compare_, &element_type::first);
        if (it == data_.end() or compare_(it->first, x) or compare_(x, it->first))
            return data_.end();
        return it;
    }

    [[nodiscard]] constexpr size_type count(const key_type& x) const
    {
        return this->contains(x);
    }

    [[nodiscard]] constexpr bool contains(const key_type& x) const
    {
        return std::ranges::binary_search(data_, x, compare_, &element_type::first);
    }

    [[nodiscard]] constexpr const_iterator lower_bound(const key_type& x) const
    {
        return std::ranges::lower_bound(data_, x, compare_, &element_type::first);
    }

    [[nodiscard]] constexpr const_iterator upper_bound(const key_type& x) const
    {
        return std::ranges::upper_bound(data_, x, compare_, &element_type::first);
    }

    [[nodiscard]] constexpr std::pair<const_iterator, const_iterator> equal_range(const key_type& x) const
    {
        return std::ranges::equal_range(data_, x, compare_, &element_type::first);
    }

    static_span<element_type> data_;
    [[no_unique_address]] key_compare compare_;
};

template<typename K, typename V, typename C, typename KeyCont, typename MappedCont>
static_map(const std::flat_map<const K, V, C, KeyCont, MappedCont>& mapped_cont) -> static_map<const K, V, C>;

}
}

namespace srx {
namespace detail {

template<typename CharT>
class static_charset;

template<typename T, typename CharT>
concept charset_like = one_of<T, charset<CharT>, static_charset<CharT>>;

template<typename T>
concept unconstrained_charset_like = template_instantiation_of<T, charset> or template_instantiation_of<T, static_charset>;

template<typename CharT>
class static_charset
{
public:
    using charset_type = charset<CharT>;
    using char_type = charset_type::char_type;
    using char_interval = charset_type::char_interval;

    consteval static_charset() noexcept = default;

    consteval explicit static_charset(const charset_type& cs)
        : data_{ cs.data_ } {}

    consteval explicit static_charset(const bitcharset<CharT>& bcs)
        requires (sizeof(CharT) == 1)
        : data_{ bcs.get_intervals() } {}

    template<typename... Args>
        requires (sizeof...(Args) >= 1) and ((std::convertible_to<Args, char_type> or std::convertible_to<Args, char_interval>) and ...)
    constexpr explicit static_charset(Args... args)
    {
        charset_type tmp;
        ([&]{ tmp.insert(args); }(), ...);
        data_ = static_span{ tmp.data_ };
    }

    constexpr auto make_unsigned() const noexcept requires std::signed_integral<CharT>
    {
        using uchar_type = std::make_unsigned_t<char_type>;
        charset<uchar_type> result;

        static constexpr uchar_type umin{ std::numeric_limits<uchar_type>::min() };
        static constexpr uchar_type umax{ std::numeric_limits<uchar_type>::max() };
        static constexpr char_type threshold{ static_cast<char_type>(umin) };

        for (const auto& [first, last] : data_)
        {
            if (first == last)
            {
                result.insert(static_cast<uchar_type>(first));
            }
            else if (first < threshold and last >= threshold)
            {
                result.insert(static_cast<uchar_type>(first), umax);
                result.insert(umin, static_cast<uchar_type>(last));
            }
            else
            {
                result.insert(static_cast<uchar_type>(first), static_cast<uchar_type>(last));
            }
        }

        return result;
    }

    /* observers */

    [[nodiscard]] constexpr bool empty() const noexcept
    {
        return data_.empty();
    }

    [[nodiscard]] constexpr bool full() const noexcept
    {
        return data_.size() == 1
               and data_[0].first == std::numeric_limits<CharT>::min()
               and data_[0].second == std::numeric_limits<CharT>::max();
    }

    [[nodiscard]] constexpr std::size_t count() const noexcept
    {
        std::size_t result{ 0 };
        for (const auto& [first, second] : data_)
            result += (second + 1 - first);
        return result;
    }

    [[nodiscard]] constexpr std::size_t interval_count() const noexcept
    {
        return data_.size();
    }

    [[nodiscard]] constexpr int score_intervals() const noexcept
    {
        int score{ 0 };

        for (const auto& [beg, end] : data_)
        {
            if (beg == end)
                score += 1;
            else
                score += 2;
        }

        if (not data_.empty())
        {
            if (const auto& [beg, end] = data_.front(); beg == std::numeric_limits<char_type>::min() and end != std::numeric_limits<char_type>::min())
                --score;
            if (const auto& [beg, end] = data_.back(); end == std::numeric_limits<char_type>::max() and beg != std::numeric_limits<char_type>::max())
                --score;
        }

        return score;
    }

    [[nodiscard]] constexpr bool should_invert() const noexcept
    {
        /* an extremely simple heuristic (this could probably be improved?) */
        return data_.size() > 0
               and data_.front().first == std::numeric_limits<CharT>::min()
               and data_.back().second == std::numeric_limits<CharT>::max();
    }

    [[nodiscard]] constexpr bool contains(char_type c) const
    {
        const auto it = std::ranges::lower_bound(data_, c, {}, &char_interval::second);
        if (it == data_.end())
            return false;
        return c >= it->first;
    }

    [[nodiscard]] constexpr const static_span<char_interval>& get_intervals() const noexcept
    {
        return data_;
    }

    /* operators */

    constexpr explicit(false) operator charset_type() const
    {
        return charset_type{ typename charset_type::underlying_t(std::from_range, data_) };
    }

    constexpr charset_type operator~() const
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

    static_span<char_interval> data_;
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
}

namespace srx {
namespace detail {

enum class named_character_class : unsigned char
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

    /* for internal use */
    octdigits,
    nucs_char,
};

/* class definitions */

struct negated_cc_tag_t {};
inline constexpr negated_cc_tag_t negated_cc_tag;

template<bool IsNarrow>
class char_class_impl
{
public:
    using char_type = std::conditional_t<IsNarrow, char, char32_t>;
    using underlying_type = std::conditional_t<IsNarrow, bitcharset<char_type>, charset<char_type>>;

    char_class_impl() = default;
    constexpr explicit char_class_impl(negated_cc_tag_t) noexcept(IsNarrow) { negate(); }
    constexpr explicit char_class_impl(named_character_class ncc) noexcept(IsNarrow) { insert(ncc); }
    constexpr explicit char_class_impl(named_character_class ncc, negated_cc_tag_t) noexcept(IsNarrow) : char_class_impl(ncc) { negate(); }
    constexpr explicit char_class_impl(char_type c) noexcept(IsNarrow) { insert(c); }
    constexpr explicit char_class_impl(char_type c, negated_cc_tag_t) noexcept(IsNarrow) : char_class_impl(c) { negate(); }

    template<std::same_as<char_type>... Ts>
        requires (sizeof...(Ts) > 1)
    constexpr explicit char_class_impl(Ts... c) noexcept(IsNarrow) { ([&]{ insert(c); }() , ...); }

    template<std::same_as<char_type>... Ts>
        requires (sizeof...(Ts) > 1)
    constexpr explicit char_class_impl(Ts... c, negated_cc_tag_t) noexcept(IsNarrow) : char_class_impl(c...) { negate(); }

    constexpr void insert(char_type c) noexcept(IsNarrow) { data_.insert(c); }
    constexpr void insert(char_type first, char_type last) noexcept(IsNarrow) { data_.insert(first, last); }
    constexpr void insert(named_character_class ncc) noexcept(IsNarrow);
    constexpr void insert(named_character_class ncc, negated_cc_tag_t) noexcept(IsNarrow);
    constexpr void insert(const char_class_impl& cc) noexcept(IsNarrow) { data_ |= cc.data_; };

    [[nodiscard]] constexpr bool empty() const noexcept { return data_.empty(); }
    [[nodiscard]] constexpr std::size_t count() const noexcept { return data_.count(); }
    [[nodiscard]] constexpr auto intervals() const noexcept(not IsNarrow) { return data_.get_intervals(); }
    [[nodiscard]] constexpr const auto& get() const noexcept { return data_; }

    constexpr void negate() noexcept(IsNarrow) { data_.negate(); }
    constexpr void make_caseless() noexcept(IsNarrow);

    static consteval bool is_narrow() noexcept { return IsNarrow; }

    friend constexpr bool operator==(const char_class_impl& x, const char_class_impl& y) = default;

private:
    using static_charset = nontransient_constexpr_version_of_t<underlying_type>;
    using p = static_charset::char_interval;

    static constexpr static_charset alnum{ p{ '0', '9' }, p{ 'A', 'Z' }, p{ 'a', 'z' } };
    static constexpr static_charset alpha{ p{ 'A', 'Z' }, p{ 'a', 'z' } };
    static constexpr static_charset ascii{ p{ 0x00, 0x7F } };
    static constexpr static_charset blank{ '\t', ' ' };
    static constexpr static_charset cntrl{ p{ 0x00, 0x1F }, 0x7F };
    static constexpr static_charset digit{ p{ '0', '9' } };
    static constexpr static_charset graph{ p{ '!', '~' } };
    static constexpr static_charset lower{ p{ 'a', 'z' } };
    static constexpr static_charset print{ p{ ' ', '~' } };
    static constexpr static_charset punct{ p{ '!', '/' }, p{ ':', '@' }, p{ '[', '`' }, p{ '{', '~' } };
    static constexpr static_charset space{ '\v', '\t', '\n', '\f', '\r', ' ' };
    static constexpr static_charset perls{ '\t', '\n', '\f', '\r', ' ' };
    static constexpr static_charset upper{ p{ 'A', 'Z' } };
    static constexpr static_charset word{ p{ '0', '9' }, p{ 'A', 'Z' }, p{ 'a', 'z' }, '_' };
    static constexpr static_charset xdigit{ p{ '0', '9' }, p{ 'A', 'F' }, p{ 'a', 'f' } };
    static constexpr static_charset octal{ p{ '0', '7' } };
    static constexpr static_charset nucsc{ p{ '0', '9' }, p{ 'A', 'Z' }, ' ', '-' };

    underlying_type data_;
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

    /* for internal use */
    case named_character_class::octdigits: data_ |= octal; break;
    case named_character_class::nucs_char: data_ |= nucsc; break;
    }
}

template<bool IsNarrow>
constexpr void char_class_impl<IsNarrow>::insert(named_character_class ncc, negated_cc_tag_t) noexcept(IsNarrow)
{
    switch (ncc)
    {
    case named_character_class::alphanumeric:     data_ |= ~alnum;  break;
    case named_character_class::alphabetic:       data_ |= ~alpha;  break;
    case named_character_class::ascii:            data_ |= ~ascii;  break;
    case named_character_class::blank:            data_ |= ~blank;  break;
    case named_character_class::control:          data_ |= ~cntrl;  break;
    case named_character_class::digits:           data_ |= ~digit;  break;
    case named_character_class::graphical:        data_ |= ~graph;  break;
    case named_character_class::lowercase:        data_ |= ~lower;  break;
    case named_character_class::printable:        data_ |= ~print;  break;
    case named_character_class::punctuation:      data_ |= ~punct;  break;
    case named_character_class::posix_whitespace: data_ |= ~space;  break;
    case named_character_class::perl_whitespace:  data_ |= ~perls;  break;
    case named_character_class::uppercase:        data_ |= ~upper;  break;
    case named_character_class::word:             data_ |= ~word;   break;
    case named_character_class::hexdigits:        data_ |= ~xdigit; break;

    /* for internal use */
    case named_character_class::octdigits: data_ |= ~octal; break;
    case named_character_class::nucs_char: data_ |= ~nucsc; break;
    }
}

}
}

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
    non_capturing,
    branch_reset,
    atomic,
    positive_lookahead,
    negative_lookahead,
    positive_lookbehind,
    negative_lookbehind,
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

        if (elem.mode == gm::branch_reset)
            target.number_end = std::max(target.number_end, elem.number_end);
        else
            target.number_end = elem.number_end;

        if (elem.mode == gm::normal)
            return elem.number;
        else
            return {};
    }

    constexpr void set_flags(capture_flags input_flags)
    {
        auto& target = elems_.empty() ? base_ : elems_.back();
        template for (constexpr auto flag : define_static_array(nonstatic_data_members_of(^^capture_flags, std::meta::access_context::unchecked())))
            if (input_flags.[: flag
                             :] != cf::inherit)
                target.flags.[: flag :] = input_flags.[: flag :];
}

private:
    template<std::meta::info CaptureFlagReflection>
    [[nodiscard]] constexpr bool get() const
    {
        for (const auto& elem : elems_ | std::views::reverse)
            if (elem.flags.[: CaptureFlagReflection
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

}
}
}

/* Note: We assume the literal character encoding is a superset of ASCII */

namespace srx {
namespace detail {

/* RE2 limits counted repetitions to 1000 - we do the same here */
inline constexpr int counted_repetition_limit{ 1000 };

enum class assert_type : unsigned char
{
    text_start,
    text_end,
    line_start,
    line_end,
    ascii_word_boundary,
    not_ascii_word_boundary,
};

namespace parser {

template<typename CharT>
class ll1;

}

/* token definitions */

namespace tok {

struct end_of_input {};
struct vert {};
struct dot {};
struct hat {};
struct dollar {};
struct rparen {};
struct star {};
struct plus {};
struct quest {};

struct repeat_n_m
{
    int min;
    int max; /* use max=min for {min} or max<min for {min,} */

    friend constexpr bool operator==(const repeat_n_m& x, const repeat_n_m& y) = default;
};

struct assertion
{
    assert_type type;

    friend constexpr bool operator==(const assertion& x, const assertion& y) = default;
};

struct set_flags
{
    parser::capture_flags flags{};
};

template<typename CharT>
struct lparen
{
    parser::capture_flags flags{};
    parser::group_modes mode{ parser::group_modes::normal };
    bool is_named{ false };
    std::basic_string_view<CharT> name{};

    lparen() = default;

    constexpr explicit lparen(std::basic_string_view<CharT> sv)
        : is_named{ true }, name{ sv } {}

    constexpr explicit lparen(parser::group_modes mode)
        : mode{ mode } {}

    constexpr explicit lparen(parser::capture_flags flags)
        : flags{ flags }, mode{ parser::group_modes::non_capturing } {}
};

template<typename CharT>
struct char_class
{
    using impl_type = char_class_impl<std::same_as<char, CharT>>;
    using underlying_char_type = impl_type::char_type;

    impl_type data;

    template<typename... Args>
    constexpr explicit char_class(Args&&... args) : data{ std::forward<Args>(args)... } {}

    friend constexpr bool operator==(const char_class& x, const char_class& y) = default;
};

template<typename CharT>
struct char_str
{
    std::basic_string<CharT> data;

    char_str() = default;

    constexpr explicit char_str(CharT c) : data{ c } {}

    constexpr explicit char_str(char c)
        requires (not std::same_as<CharT, char>)
        : data{ static_cast<CharT>(c) } {};

    template<std::input_iterator I, std::sentinel_for<I> S>
        requires std::convertible_to<std::iter_value_t<I>, CharT>
    constexpr explicit char_str(I first, S last) : data(first, last) {}

    constexpr explicit char_str(std::basic_string_view<CharT> str) : data{ str } {}

    constexpr explicit char_str(std::basic_string_view<char> str)
        requires (not std::same_as<CharT, char>)
        : data{ std::from_range, str | std::views::transform([](char c){ return static_cast<CharT>(c); })} {}

    [[nodiscard]] constexpr std::optional<typename char_class<CharT>::underlying_char_type> get_if_single()
    {
        if (data.size() == 1)
            return data.front();
        return {};
    }

    friend constexpr bool operator==(const char_str& x, const char_str& y) = default;
};

struct backref
{
    unsigned int number;

    friend constexpr bool operator==(const backref& x, const backref& y) = default;
};

}

template<typename CharT>
using token_type = std::variant<tok::end_of_input, tok::dot, tok::hat, tok::dollar,
                                tok::lparen<CharT>, tok::rparen, tok::vert,
                                tok::star, tok::plus, tok::quest, tok::repeat_n_m,
                                tok::char_str<CharT>, tok::char_class<CharT>,
                                tok::backref, tok::assertion, tok::set_flags>;

/* lexer concept definitions */

template<typename T>
concept lexer_like = requires (T l) {
    typename T::token_t;
    { l.empty() } -> std::same_as<bool>;
    { l.nexttok() } -> std::same_as<typename T::token_t>;
};

template<typename T>
concept extended_lexer_like = lexer_like<T> and requires (T l) {
    { l.set_extended() } -> std::same_as<void>;
    { l.set_extended_more() } -> std::same_as<void>;
    { l.reset_extended() } -> std::same_as<void>;
};

namespace lexer {

template<typename CharT>
class generated
{
public:
    static_assert(character<CharT>);
    using token_t = token_type<CharT>;

    constexpr explicit generated(std::basic_string_view<CharT> sv) : it_{ sv.begin() }, end_{ sv.end() } {}
    constexpr explicit generated(const CharT* cstr) : generated(std::basic_string_view<CharT>{ cstr }) {}
    [[nodiscard]] constexpr token_t nexttok();
    [[nodiscard]] constexpr bool empty() { return it_ == end_; }

    constexpr void set_extended() { extended_mode_ = 1; }
    constexpr void set_extended_more() { extended_mode_ = 2; }
    constexpr void reset_extended() { extended_mode_ = 0; }

private:
    using it_type = std::basic_string_view<CharT>::const_iterator;

    it_type it_;
    it_type end_;
    bool literal_string_mode_{ false };
    unsigned char extended_mode_{ 0 };
};

template<typename CharT>
class lexer
{
public:
    static_assert(character<CharT>);
    using token_t = token_type<CharT>;

    constexpr explicit lexer(std::basic_string_view<CharT> sv) : it_{ sv.begin() }, end_{ sv.end() } {}
    [[nodiscard]] constexpr token_t nexttok();
    [[nodiscard]] constexpr bool empty() { return it_ == end_; }

    constexpr void set_extended() { extended_mode_ = 1; }
    constexpr void set_extended_more() { extended_mode_ = 2; }
    constexpr void reset_extended() { extended_mode_ = 0; }

private:
    using it_type = std::basic_string_view<CharT>::const_iterator;

    constexpr std::size_t            parse_hex(std::size_t fixed_amt);
    constexpr std::size_t            parse_remaining_oct(std::size_t init);
    constexpr std::size_t            parse_arbitrary_oct();
    constexpr tok::backref           parse_bref();
    constexpr tok::repeat_n_m        parse_repeat();
    constexpr token_t                parse_lparen();
    constexpr token_t                parse_bref_or_octal(CharT init);
    constexpr token_t                parse_literal_string();
    constexpr tok::char_class<CharT> parse_char_class();
    constexpr named_character_class  parse_posix_char_class(it_type first, it_type last, bool& ncc_negated);

    it_type it_;
    it_type end_;
    bool literal_string_mode_{ false };
    unsigned char extended_mode_{ 0 };
};

/* lexer implementation */

template<typename CharT>
constexpr lexer<CharT>::token_t lexer<CharT>::nexttok()
{
    using namespace tok;
    using char_str = char_str<CharT>;
    using char_class = char_class<CharT>;

    if (literal_string_mode_)
        return parse_literal_string();

    while (true)
    {
        if (it_ == end_)
            return end_of_input{};

        const auto current = it_;

        switch (*it_++)
        {
        case '(': return parse_lparen();
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

            const auto escaped = *it_++;

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

            case 'o': return char_str{ static_cast<CharT>(parse_arbitrary_oct()) };
            case 'x': return char_str{ static_cast<CharT>(parse_hex(0)) };
            case 'u': return char_str{ static_cast<CharT>(parse_hex(4)) };
            case 'U': return char_str{ static_cast<CharT>(parse_hex(8)) };

            /* perl character classes */

            case 'd': return char_class{ ncc::digits };
            case 'D': return char_class{ ncc::digits, negated_cc_tag };
            case 's': return char_class{ ncc::perl_whitespace };
            case 'S': return char_class{ ncc::perl_whitespace, negated_cc_tag };
            case 'w': return char_class{ ncc::word };
            case 'W': return char_class{ ncc::word, negated_cc_tag };

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
            case '9': return backref{ static_cast<unsigned int>(escaped - '0') };

            case 'g': return parse_bref();

            /* assertions */

            case 'A': return assertion{ assert_type::text_start };
            case 'b': return assertion{ assert_type::ascii_word_boundary };
            case 'B': return assertion{ assert_type::not_ascii_word_boundary };
            case 'G': throw parser_error("Assertion (\\G) is not implemented");
            case 'Z': throw parser_error("End of text or newlines followed by end of text (\\Z) is not implemented");
            case 'z': return assertion{ assert_type::text_end };

            /* literal string */

            case 'Q': literal_string_mode_ = true; return parse_literal_string();

            default:
                if (('A' <= escaped and escaped <= 'Z') or ('a' <= escaped and escaped <= 'z'))
                    throw pattern_error("Invalid control character");
                else
                    return char_str{ escaped }; /* TODO: extract multibyte character */
            }
        }

        case '{': return parse_repeat();
        case '[': return parse_char_class();

        case '\f':
        case '\t':
        case '\n':
        case '\r':
        case '\v':
        case ' ':
            if (extended_mode_ >= 1)
                break;
        [[fallthrough]];

        default:
            return char_str{ *current }; /* TODO: extract multibyte character */
        }
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

    const auto lookahead = *it_;

    if (lookahead == '{')
    {
        /* arbitrary number of digits contained in {} */

        std::size_t digits{ 0 };
        ++it_;

        while (true)
        {
            if (it_ == end_)
                throw pattern_error("EOF in escape sequence");;

            const auto c = *it_;
            ++it_;

            if (c == '}')
                break;
            else if ('0' <= c and c <= '9')
                result = (result * hexadecimal_base) + (c - '0');
            else if ('A' <= c and c <= 'F')
                result = (result * hexadecimal_base) + decimal_base + (c - 'A');
            else if ('a' <= c and c <= 'f')
                result = (result * hexadecimal_base) + decimal_base + (c - 'a');
            else
                throw pattern_error("Invalid character in hexadecimal escape sequence");

            ++digits;
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

            const auto c = *it_;

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

            const auto c = *it_;

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

        const auto c = *it_;

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

    const auto lookahead = *it_;

    if (lookahead != '{')
        throw pattern_error("Invalid escape sequence");

    /* arbitrary number of digits contained in {} */

    std::size_t digits{ 0 };
    ++it_;

    while (true)
    {
        if (it_ == end_)
            throw pattern_error("EOF in escape sequence");

        const auto c = *it_;
        ++it_;

        if (c == '}')
            break;
        else if ('0' <= c and c <= '7')
            result = (result * octal_base) + (c - '0');
        else
            throw pattern_error("Invalid character in octal escape sequence");

        ++digits;
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

    auto next = *it_++;

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
    static constexpr int base{ 10 };

    repeat_n_m rep{ .min = -1, .max = -1 };

    bool parse_min{ true };
    bool parse_max{ true };

    while (parse_min)
    {
        if (it_ == end_)
            throw pattern_error("Repeater is incomplete");

        auto c = *it_++;

        if ('0' <= c and c <= '9')
        {
            if (rep.min == -1)
                rep.min = c - '0';
            else
                rep.min = std::saturating_add(std::saturating_mul(rep.min, base), static_cast<int>(c - '0'));
        }
        else if (c == ',')
        {
            parse_min = false;
        }
        else if (c == '}')
        {
            if (rep.min == -1)
                throw pattern_error("Repeater is empty");

            parse_min = false;

            /* skip parsing max */
            rep.max = rep.min;
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

        auto c = *it_++;

        if ('0' <= c and c <= '9')
        {
            if (rep.max == -1)
                rep.max = c - '0';
            else
                rep.max = std::saturating_add(std::saturating_mul(rep.max, base), static_cast<int>(c - '0'));
        }
        else if (c == '}')
        {
            if (rep.max != -1 and rep.max < rep.min)
                throw pattern_error("Invalid repeater (max is less than min)");

            parse_max = false;
        }
        else
        {
            throw pattern_error("Invalid character in repeater");
        }
    }

    if (rep.min > counted_repetition_limit)
    {
        if (rep.min == rep.max)
            throw pattern_error("Finite number of counted repetitions exceeds limit");
        else
            throw pattern_error("Lower bound of counted repetitions exceeds limit");
    }
    else if (rep.max > counted_repetition_limit)
    {
        throw pattern_error("Finite upper bound of counted repetitions exceeds limit");
    }
    else if (rep.max < 0)
    {
        /* normalise for merging purposes */
        rep.max = rep.min - 1;
    }

    return rep;
}

template<typename CharT>
constexpr lexer<CharT>::token_t lexer<CharT>::parse_lparen()
{
    if (it_ == end_ or *it_ != '?')
        return tok::lparen<CharT>{};

    if (++it_ == end_)
        throw pattern_error("Invalid Pattern");;

    using cf = parser::capture_flags::flag_value;
    using gm = parser::group_modes;

    switch (*it_)
    {
    case '#':
        ++it_;
        while (it_ != end_)
            if (*it_++ == ')')
                break;
        return nexttok();

    case '|':
        ++it_;
        return tok::lparen<CharT>{ gm::branch_reset };

    case '>':
        ++it_;
        return tok::lparen<CharT>{ gm::atomic };

    case 'P':
    case '<':
    case '\'':
        ++it_;
        throw pattern_error("Named capture groups are unsupported");

    default: /* parse options */
    {
        parser::capture_flags flags{};
        bool flag_value{ true };

        while (true)
        {
            if (it_ == end_)
                throw pattern_error("Invalid Pattern");

            switch (*it_++)
            {
            case 'i':
                flags.caseless = (flag_value) ? cf::enabled : cf::disabled;
                break;
            case 'm':
                flags.multiline = (flag_value) ? cf::enabled : cf::disabled;
                break;
            case 'n':
                flags.noautocap = (flag_value) ? cf::enabled : cf::disabled;
                break;
            case 's':
                flags.dotall = (flag_value) ? cf::enabled : cf::disabled;
                break;
            case 'U':
                flags.ungreedy = (flag_value) ? cf::enabled : cf::disabled;
                break;
            case 'x':
                flags.extended = (flag_value) ? cf::enabled : cf::disabled;
                if (it_ != end_ and *it_ == 'x')
                    if (++it_, flag_value)
                        flags.extended = cf::enabled_more;
                break;
            case '-':
                if (not flag_value)
                    throw pattern_error("Capturing group arguments can only contain one hyphen");
                flag_value = false;
                break;

            case ':':
                return tok::lparen<CharT>{ flags };

            case ')':
                return tok::set_flags{ flags };

            default:
                throw pattern_error("Invalid capturing group");
            }
        }
        break;
    }
    }
}

template<typename CharT>
constexpr lexer<CharT>::token_t lexer<CharT>::parse_bref_or_octal(const CharT init)
{
    using namespace tok;
    static constexpr unsigned int base{ 010 };

    backref bref{ static_cast<unsigned int>(init - '0') };
    std::size_t result{ bref.number };

    for (int i{ 0 }; i < 2; ++i)
    {
        if (it_ == end_)
            break;

        const auto lookahead = *it_;

        if (not ('0' <= lookahead and lookahead <= '7'))
            break;

        bref.number = 0;

        result *= base;
        result += lookahead - '0';
        ++it_;
    }

    if (bref.number == 0)
        return char_str<CharT>{ static_cast<CharT>(result) };
    else
        return bref;
}

template<typename CharT>
constexpr lexer<CharT>::token_t lexer<CharT>::parse_literal_string()
{
    using namespace std::string_view_literals;
    static constexpr auto proj = [](CharT c){ return static_cast<char>(c); };

    if (it_ == end_)
        throw pattern_error("Reached end of input in literal text");

    if (std::ranges::starts_with(std::ranges::subrange{ it_, end_ }, "\\E"sv, {}, proj))
    {
        std::ranges::advance(it_, 2);
        literal_string_mode_ = false;
        return nexttok();
    }

    return tok::char_str<CharT>{ *it_++ }; /* TODO: extract multibyte character */
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

    char_class result{};
    std::size_t count{ 0 };

    std::optional<underlying_char_t> c;
    bool is_range{ false };

    for (bool loop{ true }; loop;)
    {
        bool ncc_negated{ false };

        if (it_ == end_)
            throw pattern_error("EOF in character class");

        const auto current = it_;

        std::optional<ncc> selected_cc;
        std::optional<underlying_char_t> next;

        switch (*it_++)
        {
        case ']':
            if (count == 0)
                next = ']';
            else
                loop = false;
            break;

        case '-':
            if (c.has_value() and not is_range)
                is_range = true;
            else
                next = '-';
            break;

        case '\\':
        {
            if (it_ == end_)
                throw pattern_error("Pattern cannot end with '\\'");

            const auto escaped = *it_++;

            switch (escaped)
            {
            /* standard escape sequences */

            case 'a': next = '\a'; break;
            case 'b': next = '\b'; break;
            case 'f': next = '\f'; break;
            case 't': next = '\t'; break;
            case 'n': next = '\n'; break;
            case 'r': next = '\r'; break;
            case 'v': next = '\v'; break;

            /* numeric escape sequences */

            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7': next = parse_remaining_oct(escaped - '0'); break;

            case 'o': next = parse_arbitrary_oct(); break;
            case 'x': next = parse_hex(0); break;
            case 'u': next = parse_hex(4); break;
            case 'U': next = parse_hex(8); break;

            /* perl character classes */

            case 'D': ncc_negated = true; [[fallthrough]];
            case 'd': selected_cc = ncc::digits; break;
            case 'S': ncc_negated = true; [[fallthrough]];
            case 's': selected_cc = ncc::perl_whitespace; break;
            case 'W': ncc_negated = true; [[fallthrough]];
            case 'w': selected_cc = ncc::word; break;

            default:
                if (('A' <= escaped and escaped <= 'Z') or ('a' <= escaped and escaped <= 'z') or escaped == '8' or escaped == '9')
                    throw pattern_error("Invalid control character");
                else
                    next = escaped; /* TODO: extract multibyte character */
                break;
            }
            break;
        }

        case '[':
            if (it_ != end_ and *it_ == ':')
            {
                using namespace std::string_view_literals;

                static constexpr auto proj = [](CharT c){ return static_cast<char>(c); };
                const auto& [first, last] = std::ranges::search(std::ranges::subrange{ it_, end_ }, ":]"sv, {}, proj);

                if (first != end_)
                {
                    selected_cc = parse_posix_char_class(it_ + 1, first, ncc_negated);
                    it_ = last;
                }
            }
            if (not selected_cc)
                next  = '[';
            break;

        case '\f':
        case '\t':
        case '\n':
        case '\r':
        case '\v':
        case ' ':
            if (extended_mode_ >= 2)
                continue;
            [[fallthrough]];

        default:
            next = *current; /* TODO: extract multibyte character */
            break;
        }

        ++count;

        if (selected_cc)
        {
            if (is_range)
                throw pattern_error("Invalid range in character class");

            if (c)
            {
                /* insert c */
                result.data.insert(*c);
                c.reset();
            }

            /* insert char class */
            if (ncc_negated)
                result.data.insert(*selected_cc, negated_cc_tag);
            else
                result.data.insert(*selected_cc);

            selected_cc.reset();
        }
        else if (next)
        {
            if (not c)
            {
                /* delay insert */
                c = next;
            }
            else if (is_range)
            {
                if (*c > *next)
                    throw pattern_error("Invalid character class range");

                /* insert c - nc */
                result.data.insert(*c, *next);
                is_range = false;
                c.reset();
            }
            else
            {
                /* insert c */
                result.data.insert(*c);
                c = next;
            }
        }
        else if (not loop)
        {
            if (c)
                result.data.insert(*c);

            if (c and is_range)
                result.data.insert(underlying_char_t{ '-' });
        }
    }

    if (is_negated)
        result.data.negate();

    return result;
}

template<typename CharT>
constexpr named_character_class lexer<CharT>::parse_posix_char_class(it_type first, it_type last, bool& ncc_negated)
{
    using namespace std::string_view_literals;
    using ncc = named_character_class;

    static constexpr auto proj = [](CharT c){ return static_cast<char>(c); };
    std::string str{ std::from_range, std::ranges::subrange{ first, last } | std::views::transform(proj) };
    std::string_view sv{ str };

    if (sv.starts_with('^'))
    {
        sv.remove_prefix(1);
        ncc_negated = true;
    }

    if (sv == "alnum"sv)
        return ncc::alphanumeric;
    else if (sv == "alpha"sv)
        return ncc::alphabetic;
    else if (sv == "ascii"sv)
        return ncc::ascii;
    else if (sv == "blank"sv)
        return ncc::blank;
    else if (sv == "cntrl"sv)
        return ncc::control;
    else if (sv == "digit"sv)
        return ncc::digits;
    else if (sv == "graph"sv)
        return ncc::graphical;
    else if (sv == "lower"sv)
        return ncc::lowercase;
    else if (sv == "print"sv)
        return ncc::printable;
    else if (sv == "punct"sv)
        return ncc::punctuation;
    else if (sv == "space"sv)
        return ncc::posix_whitespace;
    else if (sv == "upper"sv)
        return ncc::uppercase;
    else if (sv == "word"sv)
        return ncc::word;
    else if (sv == "xdigit"sv)
        return ncc::hexdigits;
    else
        throw pattern_error("Invalid POSIX Character Class");
}

}
}
}

namespace srx {
namespace detail {

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
    using staging_info_t = std::flat_map<tag_number_t, tag_number_t>;

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
            throw std::invalid_argument("capture_info::insert: cannot insert capture with number 0");

        const auto key_it = std::ranges::upper_bound(keys_, cap);
        const auto value_it = values_.begin() + (key_it - keys_.begin());

        keys_.emplace(key_it, cap);
        values_.emplace(value_it, pair_entry{ .tag_number = lhs, .offset = 0 }, pair_entry{ .tag_number = rhs, .offset = 0 });
    }

    constexpr void set_match_start_tag(tag_number_t value)
    {
        values_.at(0).first.tag_number = value;
    }

    [[nodiscard]] constexpr capture_number_t capture_count() const
    {
        auto key_copy = keys_;
        auto [last, _] = std::ranges::unique(key_copy);
        return std::saturating_cast<capture_number_t>(last - key_copy.begin());
    }

    [[nodiscard]] constexpr std::pair<bool, bool> capture_side(tag_number_t tag) const
    {
        static constexpr auto compose = [](const auto& g, const auto& f){
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
        auto [key_beg, key_end] = std::ranges::equal_range(keys_, cap);

        return std::ranges::subrange{
            values_.begin() + (key_beg - keys_.begin()),
            values_.begin() + (key_end - keys_.begin())
        };
    }

    [[nodiscard]] constexpr tag_remapper_t remap_tags(const std::flat_map<tag_number_t, pair_entry>& map)
    {
        static constexpr auto compose = [](const auto& g, const auto& f){
            return [=]<typename T>(T&& arg) {
                return std::invoke(g, std::invoke(f, std::forward<T>(arg)));
            };
        };

        for (auto& val : values_)
        {
            if (auto it = map.find(val.first.tag_number); it != map.end())
            {
                val.first.offset += it->second.offset;
                val.first.tag_number = it->second.tag_number;
            }

            if (auto it = map.find(val.second.tag_number); it != map.end())
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
        auto [tmp_beg, tmp_end] = std::ranges::unique(set);
        set.erase(tmp_beg, tmp_end);
        std::erase_if(set, [](tag_number_t n){ return n < 0; });

        for (tag_number_t i{ 0 }; std::cmp_less(i, set.size()); ++i)
            remapper[set.at(i)] = i;

        for (auto& val : values_)
        {
            if (auto it = remapper.find(val.first.tag_number); it != remapper.end())
                val.first.tag_number = it->second;

            if (auto it = remapper.find(val.second.tag_number); it != remapper.end())
                val.second.tag_number = it->second;
        }

        return remapper;
    }

    [[nodiscard]] constexpr tag_remapper_t eliminate_branch_reset()
    {
        tag_remapper_t result;

        for (std::size_t i{ 1 }, i_end{ keys_.size() }; i < i_end; ++i)
        {
            const auto capnum = keys_.at(i - 1);

            if (keys_.at(i) != capnum)
                continue;

            if (values_[i - 1].first.offset != 0 or values_[i - 1].first.offset != 0)
                throw std::logic_error("capture_info::get_multitags: tags already optimised");

            const auto first_target = values_[i - 1].first.tag_number;
            const auto second_target = values_[i - 1].second.tag_number;

            result.emplace(values_[i].first.tag_number, first_target);
            result.emplace(values_[i].second.tag_number, second_target);

            while (i < i_end and keys_.at(i) == capnum)
            {
                result.emplace(values_[i].first.tag_number, first_target);
                result.emplace(values_[i].second.tag_number, second_target);

                if (values_[i].first.offset != 0 or values_[i].first.offset != 0)
                    throw std::logic_error("capture_info::get_multitags: tags already optimised");

                values_[i].first.tag_number = first_target;
                values_[i].second.tag_number = second_target;

                ++i;
            };
        }

        auto zv = std::views::zip(keys_, values_);
        auto [it, _] = std::ranges::unique(zv, {}, [](const auto& v) -> decltype(auto) { return get<0>(v); });
        auto dist = std::ranges::distance(std::ranges::begin(zv), it);
        keys_.erase(keys_.begin() + dist, keys_.end());
        values_.erase(values_.begin() + dist, values_.end());

        return result;
    }

    [[nodiscard]] constexpr staging_info_t get_staged_tags() const
    {
        staging_info_t result;

        for (const auto& [lhs , rhs] : values_)
            if (lhs.offset == 0 and lhs.tag_number != start_of_input_tag)
                result.emplace(lhs.tag_number, rhs.tag_number);

        return result;
    }

    [[nodiscard]] constexpr const auto& get_values_branchfree() const
    {
        if (keys_.size() != capture_count())
            throw std::logic_error("capture_info::get_values_branchfree: capture info contains branch reset");
        return values_;
    }

private:
    std::vector<capture_number_t> keys_;
    std::vector<tag_pair_t> values_;
};

}
}

namespace srx {
namespace detail {
namespace parser {

template<typename CharT>
class ll1;

}

/* types */

enum class repeater_mode : unsigned char
{
    greedy = 0,
    lazy,
    possessive
};

enum class special_group_mode : unsigned char
{
    atomic_group = 0,
    positive_lookahead,
    negative_lookahead,
    positive_lookbehind,
    negative_lookbehind,

    /* backtracking control */
    backtrack_accept,
    backtrack_fail,
    backtrack_mark,
    backtrack_commit,
    backtrack_prune,
    backtrack_skip,
    backtrack_then
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

namespace ast_entry {

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

struct repeat
{
    std::size_t idx;
    tok::repeat_n_m reps;
    repeater_mode mode; /* default = greedy */
};

struct special
{
    std::size_t idx;         /* use idx=-1 to ignore when mode is backtrack_.*  */
    special_group_mode mode; /* for backtrack_.*, treat idx as the name of a mark when not ignored */
};

}

/* ast definition */

template<typename T, typename CharT>
concept expr_tree_lexer = lexer_like<T> and std::same_as<typename T::token_t, token_type<CharT>>;

template<typename CharT>
class expr_tree
{
public:
    using char_type = CharT;
    using sv_type = std::basic_string_view<char_type>;
    using default_lexer = lexer::lexer<char_type>;

    using assertion     = tok::assertion;
    using char_str      = tok::char_str<char_type>;
    using char_class    = tok::char_class<char_type>;
    using backref       = tok::backref;
    using alt           = ast_entry::alt;
    using concat        = ast_entry::concat;
    using tag           = ast_entry::tag;
    using repeat        = ast_entry::repeat;
    using special       = ast_entry::special;

    using type = std::variant<assertion, char_str, char_class, backref, alt, concat, tag, repeat>;

    template<expr_tree_lexer<char_type> L>
    constexpr explicit expr_tree(L&& lex, parser_flags flags = {});

    template<std::ranges::range R>
        requires expr_tree_lexer<std::ranges::range_value_t<R>, char_type>
    constexpr explicit expr_tree(R&& lex_range, parser_flags flags = {});

    constexpr explicit expr_tree(sv_type sv, parser_flags flags = {})
        : expr_tree(default_lexer{ sv }, flags) {}

    friend class parser::ll1<char_type>;

    [[nodiscard]] constexpr const type& get_expr(std::size_t i) const { return expressions_.at(i); }
    [[nodiscard]] constexpr std::size_t root_idx() const noexcept { return root_idx_; }
    [[nodiscard]] constexpr std::size_t tag_count() const noexcept { return tag_count_; }
    [[nodiscard]] constexpr const auto& get_all_exprs() const noexcept { return expressions_; }
    [[nodiscard]] constexpr const auto& get_capture_info() const noexcept { return capture_info_; }

    [[nodiscard]] constexpr std::pair<std::size_t, std::size_t> min_max_length() const;
    [[nodiscard]] constexpr bool empty_match_possible() const;
    [[nodiscard]] constexpr bool is_alt_mode() const { return enable_alt_mode_; }

    constexpr void make_tag_vec(std::vector<std::vector<int>>& tag_vec) const;
    constexpr void optimise_tags();
    constexpr void insert_search_prefix();
    constexpr void simplify_counted_repeat();
    constexpr void optimise_exact_repeat();
    constexpr auto tag_to_register();

private:
    template<typename T>
    static constexpr std::size_t ast_index{ index_in_variant(^^T, ^^type) };

    [[nodiscard]] constexpr std::vector<std::optional<std::size_t>> make_const_len_vec();
    [[nodiscard]] constexpr std::vector<std::size_t> subtrees_equivalent(std::size_t x, std::size_t y) const;

    std::size_t root_idx_{ 0 };
    std::vector<type> expressions_{};
    capture_info capture_info_;
    tag_number_t tag_count_{ 0 };
    parser_flags flags_;
    bool enable_alt_mode_{ false };
};

template<typename CharT>
expr_tree(const CharT*) -> expr_tree<CharT>;

template<typename CharT>
expr_tree(const CharT*, parser_flags) -> expr_tree<CharT>;

}
}

namespace srx {
namespace detail {

/* observer for non empty match */

template<typename CharT>
constexpr std::pair<std::size_t, std::size_t> expr_tree<CharT>::min_max_length() const
{
    using min_max_t = std::pair<std::size_t, std::size_t>;

    static constexpr auto no_upper_bound = std::numeric_limits<min_max_t::second_type>::max();
    std::vector<min_max_t> lengths(expressions_.size());

    using stack_elem_t = std::pair<std::size_t, std::size_t>;

    std::vector<stack_elem_t> stack;
    stack.emplace_back(root_idx(), false);

    while (not stack.empty())
    {
        auto& [idx, pos] = stack.back();
        const auto& entry = expressions_.at(idx);

        switch (entry.index())
        {
        case ast_index<assertion>:
        case ast_index<tag>:
            lengths.at(idx) = { 0, 0 };
            stack.pop_back();
            break;

        case ast_index<char_str>:
            lengths.at(idx) = [](const auto c) -> min_max_t { return { c, c }; }(get<char_str>(entry).data.size());
            stack.pop_back();
            break;

        case ast_index<char_class>:
            lengths.at(idx) = { 1, 1 }; /* TODO: determine maximum character length for utf8 and utf16 */
            stack.pop_back();
            break;

        case ast_index<backref>:
            lengths.at(idx) = min_max_t{ 0, no_upper_bound }; /* syntactic approximation */
            stack.pop_back();
            break;

        case ast_index<concat>:
        {
            const auto& cat = get<concat>(entry);

            if (pos == cat.idxs.size())
            {
                auto tmp = cat.idxs | std::views::transform([&](std::size_t i){ return lengths.at(i); });
                lengths.at(idx) = std::ranges::fold_left(tmp, min_max_t{ 0, 0 }, [](const min_max_t& x, const min_max_t& y) -> min_max_t {
                    return { std::saturating_add(x.first, y.first), std::saturating_add(x.second, y.second) };
                });
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
            const auto& atl = get<alt>(entry);

            if (pos == atl.idxs.size())
            {
                auto tmp = atl.idxs | std::views::transform([&](std::size_t i){ return lengths.at(i); });

                if (std::ranges::empty(tmp))
                    lengths.at(idx) = { 0, no_upper_bound };
                else
                    lengths.at(idx) = { std::ranges::min(tmp, {}, &min_max_t::first).first, std::ranges::max(tmp, {}, &min_max_t::second).second };

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
            const auto& rep = get<repeat>(entry);

            if (pos == 1)
            {
                const auto& [lmin, lmax] = lengths.at(rep.idx);
                const auto& [rmin, rmax] = rep.reps;

                if (rmin > rmax) /* unbounded repetition */
                    lengths.at(idx) = { std::saturating_mul<min_max_t::first_type>(lmin, rmin), no_upper_bound };
                else
                    lengths.at(idx) = { std::saturating_mul<min_max_t::first_type>(lmin, rmin), std::saturating_mul<min_max_t::second_type>(lmax, rmax) };

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

    return lengths.at(root_idx_);
}

template<typename CharT>
constexpr bool expr_tree<CharT>::empty_match_possible() const
{
    return min_max_length().first == 0;
}

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
        auto& [idx, pos] = stack.back();
        const auto& entry = expressions_.at(idx);

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
            const auto& cat = get<concat>(entry);

            if (cat.idxs.empty())
            {
                stack.pop_back();
            }
            else if (pos == cat.idxs.size())
            {
                auto& vec = tag_vec.at(idx);

                for (const std::size_t i : cat.idxs)
                    std::ranges::copy(tag_vec.at(i), std::back_inserter(vec));

                std::ranges::sort(vec);
                auto [_, last] = std::ranges::unique(vec);
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
            const auto& atl = get<alt>(entry);

            if (pos == atl.idxs.size())
            {
                auto& vec = tag_vec.at(idx);

                for (const std::size_t i : atl.idxs)
                    std::ranges::copy(tag_vec.at(i), std::back_inserter(vec));

                std::ranges::sort(vec);
                auto [_, last] = std::ranges::unique(vec);
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
            const auto& rep = get<repeat>(entry);

            if (pos == 1)
            {
                auto& vec = tag_vec.at(idx);
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
            const auto& tag_entry = get<tag>(entry);

            auto& vec = tag_vec.at(idx);
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
        auto& [idx, pos] = stack.back();
        const auto& entry = expressions_.at(idx);

        switch (entry.index())
        {
        case ast_index<assertion>:
        case ast_index<tag>:
            const_len.at(idx) = 0;
            stack.pop_back();
            break;

        case ast_index<char_str>:
            const_len.at(idx) = get<char_str>(entry).data.size();
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
            const auto& cat = get<concat>(entry);

            if (pos == cat.idxs.size())
            {
                auto tmp = cat.idxs
                           | std::views::transform([&](std::size_t i){ return const_len.at(i); })
                           | std::ranges::to<std::vector>();

                if (std::ranges::all_of(tmp, [](const opt_t& o){ return o.has_value(); }))
                    const_len.at(idx) = std::ranges::fold_left(tmp | std::views::transform([](const opt_t& o){ return *o; }),
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
            const auto& atl = get<alt>(entry);

            if (pos == atl.idxs.size())
            {
                auto tmp = atl.idxs
                           | std::views::transform([&](std::size_t i){ return const_len.at(i); })
                           | std::ranges::to<std::vector>();

                auto first = *std::ranges::begin(tmp);

                if (std::ranges::size(tmp) > 0 and std::ranges::all_of(tmp, [&](const opt_t& o){ return o == first; }))
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
            const auto& rep = get<repeat>(entry);

            if (pos == 1)
            {
                if (rep.reps.min == rep.reps.max and const_len.at(rep.idx))
                    const_len.at(idx) = *const_len.at(rep.idx) * rep.reps.min;

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
    const auto const_len = make_const_len_vec();

    std::flat_map<tag_number_t, capture_info::pair_entry> tag_remap;

    for (std::size_t i{ 0 }, i_end{ expressions_.size() }; i < i_end; ++i)
    {
        if (not holds_alternative<concat>(expressions_.at(i)))
            continue;

        std::optional<capture_info::pair_entry> current;
        auto& target = get<concat>(expressions_.at(i)).idxs;

        if (i == root_idx())
            current = { .tag_number = end_of_input_tag, .offset = 0 };

        for (std::size_t j{ target.size() }; j > 0; --j)
        {
            const std::size_t idx{ target.at(j - 1) };

            if (auto* tn = get_if<tag>(&expressions_.at(idx)); tn != nullptr)
            {
                if (current.has_value())
                {
                    /* remap tag */

                    auto [_, success] = tag_remap.try_emplace(tn->number, *current);
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

    /* re-map start of input tag if possible */

    if (const auto& opt = const_len.at(root_idx_); opt.has_value())
        tag_remap.try_emplace(start_of_input_tag, capture_info::pair_entry{ .tag_number = end_of_input_tag, .offset = -(static_cast<int>(*opt)) });

    /* re-number map and re-number tags in capture_info */

    const auto remapper = capture_info_.remap_tags(tag_remap);

    /* re-number tags in ast */

    for (auto& expr : expressions_)
    {
        if (auto* tn = get_if<tag>(&expr); tn != nullptr)
            if (auto it = remapper.find(tn->number); it != remapper.end())
                tn->number = it->second;
    }

    if (remapper.size() > std::numeric_limits<tag_number_t>::max())
        throw tree_error("Tag number exceeded");

    tag_count_ = std::saturating_cast<tag_number_t>(remapper.size());
}

/* convert pattern for use in regex search */

template<typename CharT>
constexpr void expr_tree<CharT>::insert_search_prefix()
{
    using tok::repeat_n_m;

    /* make true wildcard */
    const std::size_t wildcard_idx{ expressions_.size() };
    expressions_.emplace_back(std::in_place_type<char_class>, char_class{ negated_cc_tag });

    /* make lazy repeater of wildcard */
    const std::size_t repeater_idx{ expressions_.size() };
    expressions_.emplace_back(std::in_place_type<repeat>, wildcard_idx, repeat_n_m{ .min = 0, .max = -1 }, repeater_mode::lazy);

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
    if (type& ast{ expressions_.at(root_idx_) }; holds_alternative<concat>(ast))
    {
        /* root idx is already concat, so we can avoid creating a new concat as root */
        auto& target = get<concat>(ast).idxs;
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

/* convert pattern for use in naive matcher */

template<typename CharT>
constexpr void expr_tree<CharT>::simplify_counted_repeat()
{
    using tok::repeat_n_m;

    for (std::size_t i{ 0 }, i_end{ expressions_.size() }; i < i_end; ++i)
    {
        if (not holds_alternative<repeat>(expressions_[i]))
            continue;

        repeat rep{ get<repeat>(expressions_[i]) };
        auto& [min, max] = rep.reps;

        /* here we simplify `a{n,m}` into 3 primitives: `a{n}`, `a*`, and `a?` */

        if (min == 0)
        {
            if (max > 1)
            {
                std::size_t cat{ rep.idx };

                for (auto i = min + 1; i < max; ++i)
                {
                    const std::size_t quest{ expressions_.size() };
                    expressions_.emplace_back(std::in_place_type<repeat>, cat, repeat_n_m{ .min = 0, .max = 1 }, rep.mode);

                    cat = expressions_.size();
                    expressions_.emplace_back(std::in_place_type<concat>, std::vector{ rep.idx, quest });
                }

                expressions_[i].template emplace<repeat>(cat, repeat_n_m{ .min = 0, .max = 1 }, rep.mode);
            }
        }
        else if (min < max)
        {
            /* simplify `a{n,m}`, where 0 < n < m */

            const std::size_t fixed_rep{ expressions_.size() };
            expressions_.emplace_back(std::in_place_type<repeat>, rep.idx, repeat_n_m{ .min = min, .max = min }, rep.mode);

            std::size_t quest{ expressions_.size() };
            expressions_.emplace_back(std::in_place_type<repeat>, rep.idx, repeat_n_m{ .min = 0, .max = 1 }, rep.mode);

            for (auto i = min + 1; i < max; ++i)
            {
                const std::size_t cat{ expressions_.size() };
                expressions_.emplace_back(std::in_place_type<concat>, std::vector{ rep.idx, quest });

                quest = expressions_.size();
                expressions_.emplace_back(std::in_place_type<repeat>, cat, repeat_n_m{ .min = 0, .max = 1 }, rep.mode);
            }

            expressions_[i].template emplace<concat>(std::vector{ fixed_rep, quest });
        }
        else if (min > max)
        {
            /* simplify `a{n,}` and `a+`, where n > 0 */

            const std::size_t fixed_rep{ expressions_.size() };
            expressions_.emplace_back(std::in_place_type<repeat>, rep.idx, repeat_n_m{ .min = min, .max = min }, rep.mode);

            const std::size_t star{ expressions_.size() };
            expressions_.emplace_back(std::in_place_type<repeat>, rep.idx, repeat_n_m{ .min = 0, .max = -1 }, rep.mode);

            expressions_[i].template emplace<concat>(std::vector{ fixed_rep, star });
        }
    }
}

template<typename CharT>
constexpr void expr_tree<CharT>::optimise_exact_repeat()
{
    for (std::size_t i{ 0 }, i_end{ expressions_.size() }; i < i_end; ++i)
    {
        if (not holds_alternative<repeat>(expressions_[i]))
            continue;

        repeat rep{ get<repeat>(expressions_[i]) };

        if (rep.reps.min != rep.reps.max or not holds_alternative<char_str>(expressions_.at(rep.idx)))
            continue;

        auto& cs = expressions_[i].template emplace<char_str>();

        const auto& old = get<char_str>(expressions_[rep.idx]);
        cs.data.reserve(rep.reps.min * old.data.size());
        for (int j{ 0 }; j < rep.reps.min; ++j)
            cs.data += old.data;
    }
}

template<typename CharT>
constexpr auto expr_tree<CharT>::tag_to_register()
{
    const auto branch_remapper = capture_info_.eliminate_branch_reset();

    for (auto& expr : expressions_)
    {
        if (auto* tn = get_if<tag>(&expr); tn != nullptr)
            if (auto it = branch_remapper.find(tn->number); it != branch_remapper.end())
                tn->number = it->second;
    }

    std::vector<bool> ignore(tag_count_, false);

    /* the section below has been copied from optimise_tags, except:
     * - branch reset tags are ignored,
     * - the start of input is not remapped
     */

    for (const auto tag : branch_remapper.values())
        ignore.at(tag) = true;

    const auto const_len = make_const_len_vec();

    std::flat_map<tag_number_t, capture_info::pair_entry> tag_remap;

    for (std::size_t i{ 0 }, i_end{ expressions_.size() }; i < i_end; ++i)
    {
        if (not holds_alternative<concat>(expressions_.at(i)))
            continue;

        std::optional<capture_info::pair_entry> current;
        auto& target = get<concat>(expressions_.at(i)).idxs;

        if (i == root_idx())
            current = { .tag_number = end_of_input_tag, .offset = 0 };

        for (std::size_t j{ target.size() }; j > 0; --j)
        {
            const std::size_t idx{ target.at(j - 1) };

            if (auto* tn = get_if<tag>(&expressions_.at(idx)); tn != nullptr and not ignore.at(tn->number))
            {
                if (current.has_value())
                {
                    /* remap tag */

                    auto [_, success] = tag_remap.try_emplace(tn->number, *current);
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

    const auto remapper = capture_info_.remap_tags(tag_remap);

    /* re-number tags in ast */

    for (auto& expr : expressions_)
    {
        if (auto* tn = get_if<tag>(&expr); tn != nullptr)
            if (auto it = remapper.find(tn->number); it != remapper.end())
                tn->number = it->second;
    }

    if (remapper.size() > std::numeric_limits<tag_number_t>::max())
        throw tree_error("Tag number exceeded");

    tag_count_ = std::saturating_cast<tag_number_t>(remapper.size());

    /* --- end copied section ---  */

    /* this returns an over-approximation of the tags which need staging;
     * only lhs tag for capturing groups which contain a
     * backreference to themselves need to be staged                      */
    return capture_info_.get_staged_tags();
}

/* helper for parser */

template<typename CharT>
constexpr std::vector<std::size_t> expr_tree<CharT>::subtrees_equivalent(std::size_t x, std::size_t y) const
{
    std::vector<std::size_t> result;

    /* always same size */
    std::vector lhs{ x };
    std::vector rhs{ y };

    std::vector<std::ptrdiff_t> overlap_idxs{};

    for (std::size_t i{ 0 }; i < lhs.size(); ++i)
    {
        const auto index1 = lhs.at(i);
        const auto index2 = rhs.at(i);

        if (index1 == index2)
        {
            overlap_idxs.emplace_back(i);
            continue;
        }

        const auto& ref1 = expressions_.at(index1);
        const auto& ref2 = expressions_.at(index2);

        if (ref1.index() != ref2.index())
            return result;

        switch (ref1.index())
        {
        case ast_index<assertion>:
            if (get<assertion>(ref1) != get<assertion>(ref2))
                return result; /* empty */
            break;

        case ast_index<tag>:
            if (get<tag>(ref1).number != get<tag>(ref2).number)
                return result; /* empty */
            break;

        case ast_index<char_str>:
            if (get<char_str>(ref1) != get<char_str>(ref2))
                return result; /* empty */
            break;

        case ast_index<char_class>:
            if (get<char_class>(ref1) != get<char_class>(ref2))
                return result; /* empty */
            break;

        case ast_index<backref>:
         if (get<backref>(ref1) != get<backref>(ref2))
                return result; /* empty */
            break;

        case ast_index<concat>:
        {
            const auto& cat1 = get<concat>(ref1);
            const auto& cat2 = get<concat>(ref2);

            if (cat1.idxs.size() != cat2.idxs.size())
                return result; /* empty */

            lhs.append_range(cat1.idxs);
            rhs.append_range(cat2.idxs);
            break;
        }

        case ast_index<alt>:
        {
            const auto& alt1 = get<alt>(ref1);
            const auto& alt2 = get<alt>(ref2);

            if (alt1.idxs.size() != alt2.idxs.size())
                return result; /* empty */

            lhs.append_range(alt1.idxs);
            rhs.append_range(alt2.idxs);
            break;
        }

        case ast_index<repeat>:
        {
            const auto& rep1 = get<repeat>(ref1);
            const auto& rep2 = get<repeat>(ref2);

            if (rep1.reps != rep2.reps or rep1.mode != rep2.mode)
                return result; /* empty */

            lhs.emplace_back(rep1.idx);
            rhs.emplace_back(rep2.idx);
            break;
        }

        default:
            throw tree_error("Invalid tree");
        }
    }

    if (overlap_idxs.empty())
    {
        result = std::move(rhs);
    }
    else
    {
        std::ptrdiff_t pos{ 0 };
        overlap_idxs.emplace_back(rhs.size());

        for (const auto skipped_idx : overlap_idxs)
        {
            /* assume overlap_idxs is sorted and unique */
            result.insert(result.end(), rhs.cbegin() + pos, rhs.cbegin() + skipped_idx);
            pos = skipped_idx + 1;
        }
    }

    return result;
}

}
}

namespace srx {
namespace detail {
namespace parser {

enum class nonterminal : unsigned char { S, E, E_, F, F_, G, R, R_, H, P, V };

enum class semantic_action : unsigned char
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
    set_flags,
    next_alt,
};

/* parser class definition */

template<typename CharT>
class ll1
{
public:
    using char_type = CharT;
    using ast_t = expr_tree<char_type>;
    using token_t = token_type<char_type>;
    using sv_type = std::basic_string_view<char_type>;

    template<expr_tree_lexer<char_type> L>
    constexpr ll1(ast_t& ast, L&& lex)
        : ref_{ ast }
    {
        ast.root_idx_ = parse(std::forward<L>(lex));
    }

    template<std::ranges::range R>
        requires expr_tree_lexer<std::ranges::range_value_t<R>, char_type>
    constexpr ll1(ast_t& ast, R&& lex_range)
        : ref_{ ast }
    {
        ast.root_idx_ = new_expression<alt>();

        for (auto&& lex : lex_range)
        {
            const std::size_t subtree_root{ parse(std::forward<std::remove_cvref_t<decltype(lex)>>(lex)) };
            auto& root = std::get<alt>(get_expr(ast.root_idx_));
            root.idxs.emplace_back(subtree_root);
        }
    }

private:
    [[nodiscard]] constexpr std::size_t parse(expr_tree_lexer<char_type> auto lex);

    using assertion  = ast_t::assertion;
    using alt        = ast_t::alt;
    using concat     = ast_t::concat;
    using tag        = ast_t::tag;
    using backref    = ast_t::backref;
    using repeat     = ast_t::repeat;
    using char_str   = ast_t::char_str;
    using char_class = ast_t::char_class;
    using type       = ast_t::type;

    using repeat_n_m = tok::repeat_n_m;
    using set_flags  = tok::set_flags;
    using lparen     = tok::lparen<CharT>;

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
    [[nodiscard]] constexpr std::size_t sa_make_repeat(std::size_t child_idx, repeat_n_m rep, repeater_mode mode);
    [[nodiscard]] constexpr repeater_mode sa_rep_greedy() const;
    [[nodiscard]] constexpr repeater_mode sa_rep_lazy() const;
    [[nodiscard]] constexpr repeater_mode sa_rep_possessive() const;
    [[nodiscard]] constexpr std::size_t sa_cap_pop(std::size_t child_idx);
    [[nodiscard]] constexpr std::size_t sa_cap_push(const lparen& flag_info);
    [[nodiscard]] constexpr std::size_t sa_set_flags(const set_flags& flag_info);
    [[nodiscard]] constexpr std::size_t sa_next_alternative();

    [[nodiscard]] constexpr parser_flags& flags() { return ref_.get().flags_; }
    [[nodiscard]] constexpr const parser_flags& flags() const { return ref_.get().flags_; }
    [[nodiscard]] constexpr type& get_expr(std::size_t idx) { return ref_.get().expressions_.at(idx); }
    [[nodiscard]] constexpr capture_info& get_capture_info() { return ref_.get().capture_info_; }
    [[nodiscard]] constexpr tag_number_t& tag_count() { return ref_.get().tag_count_; }

    template<typename T>
    static constexpr std::size_t tok_index{ index_in_variant(^^T, ^^token_t) };
    static constexpr std::size_t tok_count{ template_arguments_of(dealias(^^token_t)).size() };

    static consteval std::size_t se(nonterminal nt)
    {
        /* return adjusted index corresponding to stack element */
        return tok_count + static_cast<std::size_t>(nt);
    }

    static consteval std::size_t se(semantic_action sa)
    {
        /* return adjusted index corresponding to stack element */
        static constexpr auto offset{ tok_count + enumerators_of(^^nonterminal).size() };
        return offset +  static_cast<std::size_t>(sa);
    }

    template<in_variant<type> T, typename... Args>
    [[nodiscard]] constexpr std::size_t new_expression(Args... args)
    {
        auto& exprs = ref_.get().expressions_;

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

    constexpr bool subtrees_equivalent(std::size_t lhs_idx, std::size_t& rhs_idx)
    {
        const auto result = ref_.get().subtrees_equivalent(lhs_idx, rhs_idx);

        if (result.empty())
            return false;

        /* if subtrees are equivalent, rhs_idx is no longer valid */
        rhs_idx = lhs_idx;
        overwritable_.append_range(result);
        return true;
    }

    std::reference_wrapper<ast_t> ref_;
    std::vector<std::size_t> overwritable_;
    capture_stack capstack_;
};

/* helper classes for parser */

class ll1_stack
{
public:
    using elem_t = std::size_t;

    constexpr void pop() { data_.pop_back(); }
    constexpr void push(const std::vector<elem_t>& elems) { data_.append_range(elems | std::views::reverse); }
    [[nodiscard]] constexpr auto& root() { return data_.back(); }
    [[nodiscard]] constexpr const auto& root() const { return data_.back(); }

    [[nodiscard]] constexpr auto begin() const noexcept { return data_.cbegin(); }
    [[nodiscard]] constexpr auto end() const noexcept { return data_.cend(); }
    [[nodiscard]] constexpr auto rbegin() const noexcept { return data_.crbegin(); }
    [[nodiscard]] constexpr auto rend() const noexcept { return data_.crend(); }
    [[nodiscard]] constexpr auto cbegin() const noexcept { return data_.cbegin(); }
    [[nodiscard]] constexpr auto cend() const noexcept { return data_.cend(); }
    [[nodiscard]] constexpr auto crbegin() const noexcept { return data_.crbegin(); }
    [[nodiscard]] constexpr auto crend() const noexcept { return data_.crend(); }
    [[nodiscard]] constexpr bool empty() const noexcept { return data_.empty(); }

    constexpr ll1_stack(std::size_t start) : data_{ start } {};
    ll1_stack() = delete;

private:
    std::vector<elem_t> data_;
};

template<typename CharT>
class semantic_stack
{
public:
    using char_type = CharT;
    using terminal = ll1<char_type>::token_t;
    using elem_t = std::variant<std::size_t, terminal, repeater_mode>;

    [[nodiscard]] constexpr elem_t pop() { elem_t tmp{ std::move(data_.back()) }; data_.pop_back(); return tmp; }
    constexpr void push(elem_t&& elems) { data_.push_back(std::move(elems)); }

    [[nodiscard]] constexpr auto begin() const noexcept { return data_.cbegin(); }
    [[nodiscard]] constexpr auto end() const noexcept { return data_.cend(); }
    [[nodiscard]] constexpr auto rbegin() const noexcept { return data_.crbegin(); }
    [[nodiscard]] constexpr auto rend() const noexcept { return data_.crend(); }
    [[nodiscard]] constexpr auto cbegin() const noexcept { return data_.cbegin(); }
    [[nodiscard]] constexpr auto cend() const noexcept { return data_.cend(); }
    [[nodiscard]] constexpr auto crbegin() const noexcept { return data_.crbegin(); }
    [[nodiscard]] constexpr auto crend() const noexcept { return data_.crend(); }
    [[nodiscard]] constexpr bool empty() const noexcept { return data_.empty(); }

private:
    std::vector<elem_t> data_{};
};

/* parser implementation */

template<typename CharT>
constexpr std::size_t ll1<CharT>::parse(expr_tree_lexer<char_type> auto lex)
{
    using terminal = token_t;

    if (lex.empty()) /* ensure expressions is non-empty */
        return new_expression<char_str>(/* empty string */);

    ll1_stack stack{ se(nonterminal::S) };
    semantic_stack<char_type> semstack;

    auto token = lex.nexttok();

    for (bool loop{ true }; loop;)
    {
        if (stack.empty())
            throw pattern_error("Invalid pattern");

        const auto top = stack.root();
        stack.pop();

        using nt = nonterminal;
        using sa = semantic_action;
        using namespace tok;

        switch (top)
        {
        case tok_index<end_of_input>:
        case tok_index<dot>:
        case tok_index<hat>:
        case tok_index<dollar>:
        case tok_index<lparen>:
        case tok_index<rparen>:
        case tok_index<vert>:
        case tok_index<star>:
        case tok_index<plus>:
        case tok_index<quest>:
        case tok_index<repeat_n_m>:
        case tok_index<char_str>:
        case tok_index<char_class>:
        case tok_index<backref>:
        case tok_index<assertion>:
        case tok_index<set_flags>:
            if (top == tok_index<tok::end_of_input>)
            {
                /* parsing is done */
                loop = false;
            }
            else if (top == token.index())
            {
                /* match */
                semstack.push(std::move(token));
                token = lex.nexttok();
            }
            else
            {
                /* no match */
                throw pattern_error("Parse Error: Token does not match");
            }
            break;

        /* predict */

        case se(nt::S):
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
            case tok_index<set_flags>:
                stack.push({ se(nt::E), tok_index<end_of_input> });
                break;
            default:
                throw pattern_error("Invalid pattern");
            }
            break;
        case se(nt::E):
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
            case tok_index<set_flags>:
                stack.push({ se(nt::F), se(nt::E_) });
                break;
            default:
                throw pattern_error("Invalid pattern");
            }
            break;
        case se(nt::E_):
            switch (token.index())
            {
            case tok_index<end_of_input>:
            case tok_index<rparen>:
                /* epsilon */
                break;
            case tok_index<vert>:
                stack.push({ se(nt::V), se(nt::E), se(sa::make_alt) });
                break;
            default:
                throw pattern_error("Invalid pattern");
            }
            break;
        case se(nt::F):
            switch (token.index())
            {
            case tok_index<end_of_input>:
            case tok_index<rparen>:
            case tok_index<vert>:
                stack.push({ /* epsilon */ se(sa::make_empty) });
                break;
            case tok_index<dot>:
            case tok_index<hat>:
            case tok_index<dollar>:
            case tok_index<assertion>:
            case tok_index<lparen>:
            case tok_index<char_str>:
            case tok_index<char_class>:
            case tok_index<backref>:
            case tok_index<set_flags>:
                stack.push({ se(nt::G), se(nt::F_) });
                break;
            default:
                throw pattern_error("Invalid pattern");
            }
            break;
        case se(nt::F_):
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
            case tok_index<set_flags>:
                stack.push({ se(nt::G), se(nt::F_), se(sa::make_concat) });
                break;
            default:
                throw pattern_error("Invalid pattern");
            }
            break;
        case se(nt::G):
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
            case tok_index<set_flags>:
                stack.push({ se(nt::H), se(nt::R) });
                break;
            default:
                throw pattern_error("Invalid pattern");
            }
            break;
        case se(nt::R):
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
            case tok_index<set_flags>:
                /* epsilon */
                break;
            case tok_index<star>:
                stack.push({ tok_index<star>, se(nt::R_), se(sa::make_star) });
                break;
            case tok_index<plus>:
                stack.push({ tok_index<plus>, se(nt::R_), se(sa::make_plus) });
                break;
            case tok_index<quest>:
                stack.push({ tok_index<quest>, se(nt::R_), se(sa::make_quest) });
                break;
            case tok_index<repeat_n_m>:
                stack.push({ tok_index<repeat_n_m>, se(nt::R_), se(sa::make_repeat) });
                break;
            }
            break;
        case se(nt::R_):
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
            case tok_index<set_flags>:
                stack.push({ /* epsilon, */ se(sa::rep_greedy) });
                break;
            case tok_index<quest>:
                stack.push({ tok_index<quest>, se(sa::rep_lazy) });
                break;
            case tok_index<plus>:
                stack.push({ tok_index<plus>, se(sa::rep_possessive) });
                break;
            default:
                throw pattern_error("Invalid pattern");
            }
            break;
        case se(nt::H):
            switch (token.index())
            {
            case tok_index<dot>:
                stack.push({ tok_index<dot>, se(sa::make_dot) });
                break;
            case tok_index<hat>:
                stack.push({ tok_index<hat>, se(sa::make_hat) });
                break;
            case tok_index<dollar>:
                stack.push({ tok_index<dollar>, se(sa::make_dollar) });
                break;
            case tok_index<assertion>:
                stack.push({ tok_index<assertion>, se(sa::make_assert) });
                break;
            case tok_index<lparen>:
                stack.push({ se(nt::P), se(nt::E), tok_index<rparen>, se(sa::cap_pop) });
                break;
            case tok_index<char_str>:
                stack.push({ tok_index<char_str>, se(sa::make_char_lit) });
                break;
            case tok_index<char_class>:
                stack.push({ tok_index<char_class>, se(sa::make_char_class) });
                break;
            case tok_index<backref>:
                stack.push({ tok_index<backref>, se(sa::make_bref) });
                break;
            case tok_index<set_flags>:
                stack.push({ tok_index<set_flags>, se(sa::set_flags) });
                break;
            default:
                throw pattern_error("Invalid pattern");
            }
            break;
        case se(nt::P):
            switch (token.index())
            {
            case tok_index<lparen>:
                stack.push({ tok_index<lparen>, se(sa::cap_push) });
                break;
            default:
                throw pattern_error("Invalid pattern");
            }
            break;
        case se(nt::V):
            switch (token.index())
            {
            case tok_index<vert>:
                stack.push({ tok_index<vert>, se(sa::next_alt) });
                break;
            default:
                throw pattern_error("Invalid pattern");
            }
            break;

        /* semantic actions */

        case se(sa::make_empty):
        {
            semstack.push(sa_make_empty());
            break;
        }
        case se(sa::make_dot):
        {
            std::ignore = semstack.pop(); /* pop tok::dot */
            semstack.push(sa_make_dot());
            break;
        }
        case se(sa::make_hat):
        {
            std::ignore = semstack.pop(); /* pop tok::hat */
            semstack.push(sa_make_hat());
            break;
        }
        case se(sa::make_dollar):
        {
            std::ignore = semstack.pop(); /* pop tok::dollar */
            semstack.push(sa_make_dollar());
            break;
        }
        case se(sa::make_assert):
        {
            assertion as{ get<assertion>(get<terminal>(semstack.pop())) }; /* pop tok::assertion */
            semstack.push(sa_make_assert(std::move(as)));
            break;
        }
        case se(sa::make_char_lit):
        {
            char_str lit{ get<char_str>(get<terminal>(semstack.pop())) }; /* pop tok::char_str */
            semstack.push(sa_make_char_lit(std::move(lit)));
            break;
        }
        case se(sa::make_char_class):
        {
            char_class cc{ get<char_class>(get<terminal>(semstack.pop())) }; /* pop tok::char_str */
            semstack.push(sa_make_char_class(std::move(cc)));
            break;
        }
        case se(sa::make_alt):
        {
            const auto rhs_idx = get<std::size_t>(semstack.pop());
            std::ignore = semstack.pop(); /* pop (placeholder) */
            const auto lhs_idx = get<std::size_t>(semstack.pop());
            semstack.push(sa_make_alt(lhs_idx, rhs_idx));
            break;
        }
        case se(sa::make_concat):
        {
            const auto rhs_idx = get<std::size_t>(semstack.pop());
            const auto lhs_idx = get<std::size_t>(semstack.pop());
            semstack.push(sa_make_concat(lhs_idx, rhs_idx));
            break;
        }
        case se(sa::make_bref):
        {
            const auto bref = get<tok::backref>(get<terminal>(semstack.pop())); /* pop tok::backref */
            semstack.push(sa_make_bref(bref));
            break;
        }
        case se(sa::make_star):
        {
            const auto mode = get<repeater_mode>(semstack.pop());
            std::ignore = semstack.pop(); /* pop tok::star */
            const auto child_idx = get<std::size_t>(semstack.pop());
            semstack.push(sa_make_star(child_idx, mode));
            break;
        }
        case se(sa::make_plus):
        {
            const auto mode = get<repeater_mode>(semstack.pop());
            std::ignore = semstack.pop(); /* pop tok::plus */
            const auto child_idx = get<std::size_t>(semstack.pop());
            semstack.push(sa_make_plus(child_idx, mode));
            break;
        }
        case se(sa::make_quest):
        {
            const auto mode = get<repeater_mode>(semstack.pop());
            std::ignore = semstack.pop(); /* pop tok::quest */
            const auto child_idx = get<std::size_t>(semstack.pop());
            semstack.push(sa_make_quest(child_idx, mode));
            break;
        }
        case se(sa::make_repeat):
        {
            const auto mode = get<repeater_mode>(semstack.pop());
            const auto rep = get<repeat_n_m>(get<terminal>(semstack.pop()));
            const auto child_idx = get<std::size_t>(semstack.pop());
            semstack.push(sa_make_repeat(child_idx, rep, mode));
            break;
        }
        case se(sa::rep_greedy):
        {
            semstack.push(sa_rep_greedy());
            break;
        }
        case se(sa::rep_lazy):
        {
            std::ignore = semstack.pop(); /* pop tok::quest */
            semstack.push(sa_rep_lazy());
            break;
        }
        case se(sa::rep_possessive):
        {
            std::ignore = semstack.pop(); /* pop tok::plus */
            semstack.push(sa_rep_possessive());
            break;
        }
        case se(sa::cap_push):
        {
            const auto lpn = get<lparen>(get<terminal>(semstack.pop()));
            semstack.push(sa_cap_push(lpn));
            break;
        }
        case se(sa::cap_pop):
        {
            std::ignore = semstack.pop(); /* pop tok::rparen */
            const auto child_idx = get<std::size_t>(semstack.pop());
            std::ignore = semstack.pop(); /* pop dummy integer */
            semstack.push(sa_cap_pop(child_idx));
            break;
        }
        case se(sa::set_flags):
        {
            const auto set = get<set_flags>(get<terminal>(semstack.pop()));
            semstack.push(sa_set_flags(set));
            break;
        }
        case se(sa::next_alt):
        {
            std::ignore = semstack.pop(); /* pop tok::vert */
            semstack.push(sa_next_alternative());
            break;
        }

        /* no match */

        default:
            throw pattern_error("Parse Error: Invalid stack value");
        }
    }

    if (not holds_alternative<tok::end_of_input>(token))
        throw pattern_error("Parse Error: Incomplete parse");

    if (semstack.empty())
        throw pattern_error("Parse Error: Empty semstack");

    return get<std::size_t>(semstack.pop());
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
    static constexpr auto newline = [] consteval {
        if constexpr (char_class::impl_type::is_narrow())
            return '\n';
        else
            return U'\n';
    }();

    /* depending on flags, insert true wildcard instead of [^\n] */
    if (capstack_.dotall())
        return new_expression<char_class>(char_class{ negated_cc_tag });
    else
        return new_expression<char_class>(char_class{ newline, negated_cc_tag });
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

        if (auto c = lit.get_if_single())
        {
            /* single character (therefore easier to implement) */

            if (is_alphabetic(*c))
            {
                const auto new_idx = new_expression<char_class>();
                auto& target = get<char_class>(get_expr(new_idx)).data;

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

            auto lit_it = lit.data.begin();
            const auto lit_end = lit.data.end();

            if (std::ranges::any_of(lit_it, lit_end, is_alphabetic))
            {
                /* create new concat to insert caseless string into */
                const auto cat_idx = new_expression<concat>();

                while (lit_it != lit_end)
                {
                    const auto c = *lit_it++;

                    if (is_alphabetic(c))
                    {
                        /* insert character class of [cC] into cat */
                        const auto new_idx = new_expression<char_class>();
                        auto& target = get<char_class>(get_expr(new_idx)).data;
                        get<concat>(get_expr(cat_idx)).idxs.push_back(new_idx);

                        target.insert(c);
                        target.make_caseless();
                    }
                    else
                    {
                        /* insert character string into cat */
                        const auto new_idx = new_expression<char_str>();
                        auto& target = get<char_str>(get_expr(new_idx)).data;

                        target.push_back(c);

                        while (lit_it != lit_end and is_alphabetic(*lit_it))
                            target.push_back(*lit_it++);

                        get<concat>(get_expr(cat_idx)).idxs.push_back(new_idx);
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
    if (type& ast{ get_expr(rhs_idx) }; holds_alternative<alt>(ast))
    {
        auto& ast_alt = get<alt>(ast);

        if (flags().enable_alttocc and not ast_alt.idxs.empty())
        {
            /* attempt to replace a|b with [ab] */
            type& rhs{ get_expr(ast_alt.idxs.front()) };
            type& lhs{ get_expr(lhs_idx) };

            if (holds_alternative<char_class>(rhs))
            {
                auto& target = get<char_class>(rhs).data;

                if (holds_alternative<char_class>(lhs))
                {
                    /* merge char classes */
                    auto& other = get<char_class>(lhs).data;
                    target.insert(other);
                    overwritable_.push_back(lhs_idx);
                    return rhs_idx;
                }
                else if (holds_alternative<char_str>(lhs))
                {
                    if (auto to_insert = get<char_str>(lhs).get_if_single())
                    {
                        /* insert char into char class */
                        target.insert(*to_insert);
                        overwritable_.push_back(lhs_idx);
                        return rhs_idx;
                    }
                }
            }
            else if (holds_alternative<char_str>(rhs))
            {
                const auto saved_idx = ast_alt.idxs.front();

                if (auto to_insert = get<char_str>(rhs).get_if_single())
                {
                    if (holds_alternative<char_class>(lhs))
                    {
                        /* replace rhs with lhs in alt, and insert char into char class */
                        auto& target = get<char_class>(lhs).data;
                        ast_alt.idxs.front() = lhs_idx;
                        target.insert(*to_insert);
                        overwritable_.push_back(saved_idx);
                        return rhs_idx;
                    }
                    else if (holds_alternative<char_str>(lhs))
                    {
                        if (auto other_insert = get<char_str>(lhs).get_if_single())
                        {
                            /* replace rhs with new char class in alt */
                            const auto new_idx = new_expression<char_class>();
                            auto& target = get<char_class>(get_expr(new_idx)).data;

                            /* calling new_expression invalidates references, so we must re-get ast_alt */
                            auto& ast_alt2 = get<alt>(get_expr(rhs_idx));
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

            if (holds_alternative<char_class>(rhs))
            {
                auto& target = get<char_class>(rhs).data;

                if (holds_alternative<char_class>(lhs))
                {
                    /* merge char classes */
                    auto& other = get<char_class>(lhs).data;
                    target.insert(other);
                    overwritable_.push_back(lhs_idx);
                    return rhs_idx;
                }
                else if (holds_alternative<char_str>(lhs))
                {
                    if (auto to_insert = get<char_str>(lhs).get_if_single())
                    {
                        /* insert char into char class */
                        target.insert(*to_insert);
                        overwritable_.push_back(lhs_idx);
                        return rhs_idx;
                    }
                }
            }
            else if (holds_alternative<char_str>(rhs))
            {
                if (auto to_insert = get<char_str>(rhs).get_if_single())
                {
                    if (holds_alternative<char_class>(lhs))
                    {
                        /* insert (rhs) char into (lhs) char class */
                        auto& target = get<char_class>(lhs).data;
                        target.insert(*to_insert);
                        overwritable_.push_back(rhs_idx);
                        return lhs_idx;
                    }
                    else if (holds_alternative<char_str>(lhs))
                    {
                        if (auto other_insert = get<char_str>(lhs).get_if_single())
                        {
                            /* create new char class */
                            const auto new_idx = new_expression<char_class>();
                            auto& target = get<char_class>(get_expr(new_idx)).data;

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
    static constexpr auto merge_char_str = [](type& lhs, type& rhs){
        auto& target = get<char_str>(rhs).data;
        auto& lhs_str = get<char_str>(lhs).data;
        lhs_str.append(target);
        std::ranges::swap(lhs_str, target);
    };

    auto merge_repeats = [this](type& lhs, type& rhs){
        auto& target = get<repeat>(rhs);
        auto& lhs_rep = get<repeat>(lhs);

        if (lhs_rep.mode != target.mode and (lhs_rep.reps.min != lhs_rep.reps.max or target.reps.min == target.reps.max))
            return false;

        if (not subtrees_equivalent(lhs_rep.idx, target.idx))
            return false;

        if (target.reps.min == target.reps.max)
            target.mode = lhs_rep.mode;

        const bool unbounded_max{ target.reps.max < target.reps.min or lhs_rep.reps.max < lhs_rep.reps.min };
        target.reps.min += lhs_rep.reps.min;
        target.reps.max = unbounded_max ? (target.reps.min - 1) : (target.reps.max + lhs_rep.reps.max);
        return true;
    };

    if (type& ast{ get_expr(lhs_idx) }; holds_alternative<concat>(ast))
    {
        /* append rhs into existing concat */
        /* this case _should_ only arise from captures, so we can skip merging strings */
        auto& target = get<concat>(ast).idxs;

        type& rhs{ get_expr(rhs_idx) };

        if (holds_alternative<concat>(rhs))
        {
            /* append contents of rhs concat to lhs concat  */
            auto& src = get<concat>(rhs).idxs;
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
    else if (type& ast{ get_expr(rhs_idx) }; holds_alternative<concat>(ast))
    {
        /* insert lhs into existing concat */
        auto& ast_concat = get<concat>(ast);

        if (not ast_concat.idxs.empty())
        {
            type& rhs{ get_expr(ast_concat.idxs.front()) };
            type& lhs{ get_expr(lhs_idx) };

            if (holds_alternative<char_str>(rhs) and holds_alternative<char_str>(lhs))
            {
                /* merge lhs string with first entry of rhs (also a string) */
                merge_char_str(lhs, rhs);
                overwritable_.push_back(lhs_idx);
                return rhs_idx;

            }
            else if (holds_alternative<repeat>(rhs) and holds_alternative<repeat>(lhs))
            {
                /* lhs and rhs are both repeats: attempt to merge if contents are identical */
                if (merge_repeats(lhs, rhs))
                {
                    overwritable_.push_back(lhs_idx);
                    return rhs_idx;
                }
            }
        }

        /* insert lhs into existing concat */
        ast_concat.idxs.insert(ast_concat.idxs.begin(), lhs_idx);
        return rhs_idx;
    }
    else
    {
        type& rhs{ get_expr(rhs_idx) };
        type& lhs{ get_expr(lhs_idx) };

        if (holds_alternative<char_str>(rhs) and holds_alternative<char_str>(lhs))
        {
            /* lhs and rhs are both strings: merge strings into one instead of creating concat  */
            merge_char_str(lhs, rhs);
            overwritable_.push_back(lhs_idx);
            return rhs_idx;
        }
        else if (holds_alternative<repeat>(rhs) and holds_alternative<repeat>(lhs))
        {
            /* lhs and rhs are both repeats: attempt to merge if contents are identical */
            if (merge_repeats(lhs, rhs))
            {
                overwritable_.push_back(lhs_idx);
                return rhs_idx;
            }
        }

        /* create new concat */
        return new_expression<concat>(std::vector{ lhs_idx, rhs_idx });
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
    return new_expression<repeat>(child_idx, repeat_n_m{ .min = 0, .max = -1 }, mode);
}

template<typename CharT>
constexpr std::size_t ll1<CharT>::sa_make_plus(const std::size_t child_idx, const repeater_mode mode)
{
    return new_expression<repeat>(child_idx, repeat_n_m{ .min = 1, .max = 0 }, mode);
}

template<typename CharT>
constexpr std::size_t ll1<CharT>::sa_make_quest(const std::size_t child_idx, const repeater_mode mode)
{
    return new_expression<repeat>(child_idx, repeat_n_m{ .min = 0, .max = 1 }, mode);
}

template<typename CharT>
constexpr std::size_t ll1<CharT>::sa_make_repeat(const std::size_t child_idx, const repeat_n_m rep, const repeater_mode mode)
{
    return new_expression<repeat>(child_idx, rep, mode);
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
    const auto cap_number = capstack_.pop();

    if (cap_number.has_value())
    {
        type& ast{ get_expr(child_idx) };

        if (char_str* lit{ get_if<char_str>(&ast) }; lit != nullptr and lit->data.empty())
        {
            /* empty capturing group; only insert one tag */

            const tag_number_t tag_num{ tag_count()++ };
            get_capture_info().insert(*cap_number, tag_num, tag_num);

            if (tag_num < 0)
                throw tree_error("Capture limit exceed");

            overwritable_.push_back(child_idx);
            return new_expression<tag>(tag_num);
        }
        else if (holds_alternative<concat>(ast))
        {
            /* insert tags on either end of existing concat */
            const tag_number_t lhs_tag{ tag_count()++ };
            const tag_number_t rhs_tag{ tag_count()++ };
            get_capture_info().insert(*cap_number, lhs_tag, rhs_tag);

            if (lhs_tag < 0 or rhs_tag < 0)
                throw tree_error("Capture limit exceed");

            const auto lhs_tag_entry = new_expression<tag>(lhs_tag);
            const auto rhs_tag_entry = new_expression<tag>(rhs_tag);

            /* calling new_expression invalidates references, so we must re-get ast for target */
            auto& target = get<concat>(get_expr(child_idx)).idxs;
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

            const auto lhs_tag_entry = new_expression<tag>(lhs_tag);
            const auto rhs_tag_entry = new_expression<tag>(rhs_tag);
            return new_expression<concat>(std::vector{ lhs_tag_entry, child_idx, rhs_tag_entry });
        }
    }
    else
    {
        return child_idx; /* non capturing group */
    }
}

template<typename CharT>
constexpr std::size_t ll1<CharT>::sa_cap_push(const tok::lparen<CharT>& flag_info)
{
    if (not flags().enable_branchreset and flag_info.mode == group_modes::branch_reset)
        throw parser_error("Branch resetting in captures is not enabled");

    if (flag_info.mode == group_modes::atomic)
        throw pattern_error("Atomic capture groups are unsupported");
    else if (flag_info.mode == group_modes::positive_lookahead)
        throw pattern_error("Positive lookahead is unsupported");
    else if (flag_info.mode == group_modes::negative_lookahead)
        throw pattern_error("Negative lookahead is unsupported");
    else if (flag_info.mode == group_modes::positive_lookbehind)
        throw pattern_error("Positive lookbehind is unsupported");
    else if (flag_info.mode == group_modes::negative_lookbehind)
        throw pattern_error("Negative lookbehind is unsupported");
    else if (flag_info.is_named)
        throw pattern_error("Named capture groups are unsupported");

    if (flag_info.flags.noautocap != capture_flags::flag_value::inherit)
        throw pattern_error("Capturing group flag 'n' is unsupported");
    if (flag_info.flags.extended != capture_flags::flag_value::inherit)
        throw pattern_error("Capturing group flag 'x' is unsupported");

    if (flags().enable_captures)
    {
        if (capstack_.push(flag_info.flags, flag_info.mode)) /* i.e. unsigned integer overflow has occurred */
            throw pattern_error("Capturing group limit exceeded");
    }
    else
    {
        capstack_.push_non_capturing(flag_info.flags, flag_info.mode);
    }

    return -1uz; /* dummy return value */
}

template<typename CharT>
constexpr std::size_t ll1<CharT>::sa_set_flags(const set_flags& flag_info)
{
    capstack_.set_flags(flag_info.flags);
    return new_expression<char_str>(/* empty string */);
}

template<typename CharT>
constexpr std::size_t ll1<CharT>::sa_next_alternative()
{
    /* allow branch reset to work properly */
    if (flags().enable_branchreset)
        capstack_.branch_reset_if_enabled();
    return -1; /* dummy value */
}

}

/* constructors for tree */

template<typename CharT>
template<expr_tree_lexer<CharT> L>
constexpr expr_tree<CharT>::expr_tree(L&& lex, parser_flags flags)
    : flags_{ flags }
{
    parser::ll1<char_type> ll1_parser(*this, std::forward<L>(lex));
}

template<typename CharT>
template<std::ranges::range R>
    requires expr_tree_lexer<std::ranges::range_value_t<R>, CharT>
constexpr expr_tree<CharT>::expr_tree(R&& lex_range, parser_flags flags)
     : flags_{ flags }, enable_alt_mode_{ true }
{
    parser::ll1<char_type> ll1_parser(*this, std::forward<R>(lex_range));
}

}
}

namespace srx {
namespace detail {
namespace tnfa {

using state_t = std::size_t;
using tr_index = std::size_t;
using continue_at_t = std::uint_least16_t;

template<typename CharT>
using charset_t = std::conditional_t<sizeof(CharT) == 1, bitcharset<CharT>, charset<CharT>>;

struct stack_elem
{
    tnfa::state_t q0, qf;
    std::size_t idx;
};

namespace assert_category {

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
    bool is_fallback{ false };              /* must equal false if not is_final */

    std::uint_least16_t final_offset{ 0 };  /* only meaningful if is_final */
    continue_at_t       continue_at{ 0 };   /* only meaningful if is_final */
    std::uint_least16_t final_alt{ 0 };     /* only meaningful if is_final */
};

template<typename CharT>
struct continue_info
{
    using sub_e_closure = std::optional<std::vector<state_t>>;

    state_t value;

    charset_t<CharT> cs;
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
        return holds_alternative<epsilon_tr>(tr.type);
    }
};

}

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

    constexpr explicit tagged_nfa(const expr_tree<char_type>& ast, fsm_flags flags);
    constexpr void rewrite_assertions();
    constexpr void add_non_empty_match_pathway();

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
    [[nodiscard]] constexpr const auto& get_additional_cont() const { return additional_cont_; }

private:
    using ast_t = expr_tree<char_type>;
    using tag_vec_t = std::vector<std::vector<int>>;
    using transition_info = tnfa::transition<char_type>::transition_type;

    template<typename T>
    static constexpr state_t ast_index{ index_in_variant(^^T, ^^typename ast_t::type) };

    /* fundamental helpers */

    constexpr state_t node_create();
    constexpr state_t node_copy(state_t q);
    constexpr void node_copy_to(state_t q_old, state_t q_new);

    template<typename... Args>
    constexpr void transition_create(state_t q0, state_t qf, Args&&... args);

    /* transition creation */

    constexpr void make_epsilon(state_t q0, state_t qf, int priority = 0, int tag = 0);

    constexpr void make_transition(state_t q0, state_t qf, char_type c);

    template<std::convertible_to<charset_type> CharSet>
    constexpr void make_transition(state_t q0, state_t qf, CharSet&& cs);

    template<typename T>
        requires one_of<T, tnfa::assert_category::eof_tag_t, tnfa::assert_category::sof_tag_t>
    constexpr void make_assert(state_t q0, state_t qf, T category);

    template<typename T, std::convertible_to<charset_type> CharSet>
        requires one_of<T, tnfa::assert_category::lookahead1_tag_t, tnfa::assert_category::lookbehind1_tag_t>
    constexpr void make_assert(state_t q0, state_t qf, T category, CharSet&& cs);

    constexpr void make_copy(state_t q0, state_t qf, const transition_info& type);

    constexpr void make_ntags(state_t q0, state_t qf, const std::vector<int>& ntags);

    constexpr void thompson(const expr_tree<char_type>& ast, const tag_vec_t& tag_vec, const tnfa::stack_elem& initial);

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

    std::vector<tnfa::node>                     nodes_{ 2 };
    std::vector<tnfa::transition<char_type>>    transitions_;
    capture_info                                capture_info_;
    std::size_t                                 tag_count_;
    state_t                                     start_node_{ default_start_node };
    std::vector<tnfa::continue_info<char_type>> cont_info_;
    std::vector<state_t>                        additional_cont_;
    std::vector<state_t>                        final_nodes_{ default_final_node };

    fsm_flags flags_;

    bool has_eof_anchor_   : 1 { false };
    bool has_sof_anchor_   : 1 { false };
    bool has_lookahead_1_  : 1 { false };
    bool has_lookbehind_1_ : 1 { false };
};

}
}

namespace srx {
namespace detail {

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
    auto& new_n = nodes_.emplace_back();
    const auto& old_n = nodes_.at(q);

    new_n.is_final     = old_n.is_final;
    new_n.is_fallback  = old_n.is_fallback;
    new_n.final_offset = old_n.final_offset;
    new_n.continue_at  = old_n.continue_at;
    new_n.final_alt    = old_n.final_alt;

    if (new_n.is_final)
        final_nodes_.emplace_back(p);

    return p;
}

template<typename CharT>
constexpr void tagged_nfa<CharT>::node_copy_to(const state_t q_old, const state_t q_new)
{
    auto& new_n = nodes_.at(q_new);
    const auto& old_n = nodes_.at(q_old);

    if (old_n.is_final and not new_n.is_final)
        final_nodes_.emplace_back(q_new);

    new_n.is_final     = old_n.is_final;
    new_n.is_fallback  = old_n.is_fallback;
    new_n.final_offset = old_n.final_offset;
    new_n.continue_at  = old_n.continue_at;
    new_n.final_alt    = old_n.final_alt;
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
template<std::convertible_to<typename tagged_nfa<CharT>::charset_type> CharSet>
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
template<typename T, std::convertible_to<typename tagged_nfa<CharT>::charset_type> CharSet>
    requires one_of<T, tnfa::assert_category::lookahead1_tag_t, tnfa::assert_category::lookbehind1_tag_t>
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
constexpr void tagged_nfa<CharT>::thompson(const expr_tree<char_type>& ast, const tag_vec_t& tag_vec, const tnfa::stack_elem& initial)
{
    std::vector stack{ initial };

    while (not stack.empty())
    {
        auto [q0, qf, idx] = stack.back();
        stack.pop_back();

        const auto& entry = ast.get_expr(idx);

        switch (entry.index())
        {
        case ast_index<typename ast_t::char_str>:
            if (const auto& str = get<typename ast_t::char_str>(entry); not str.data.empty())
            {
                for (const auto c : str.data | std::views::take(str.data.size() - 1))
                {
                    auto qi = node_create();
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
            const auto& cla = get<typename ast_t::char_class>(entry);

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
            if (const auto& cat = get<typename ast_t::concat>(entry); not cat.idxs.empty())
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
            if (const auto& alt = get<typename ast_t::alt>(entry); not alt.idxs.empty())
            {
                if (tag_vec.empty() or tag_vec.at(idx).empty())
                {
                    /* generate naive tag-free nfa */
                    for (std::size_t i{ 0 }, i_end{ alt.idxs.size() }; i < i_end; ++i)
                    {
                        const state_t qi{ node_create() };
                        make_epsilon(q0, qi, i);
                        stack.emplace_back(qi, qf, alt.idxs.at(i));
                    }
                }
                else
                {

                    /* generate tag-aware nfa */
                    for (std::size_t i{ 0 }, i_end{ alt.idxs.size() }; i < i_end; ++i)
                    {
                        if (i + 1 == i_end)
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
                            for (std::size_t j{ i + 1 }; j < i_end; ++j)
                                std::ranges::copy(tag_vec.at(alt.idxs.at(j)), std::back_inserter(remaining_ntags));

                            std::ranges::sort(remaining_ntags);
                            auto [_, last] = std::ranges::unique(remaining_ntags);
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
            const auto& rep = get<typename ast_t::repeat>(entry);

            if (rep.mode == repeater_mode::possessive)
                throw tnfa_error("Possessive repetition is unimplemented");

            /* reminder: max < min denotes infinity */
            int max{ (rep.reps.max < rep.reps.min) ? rep.reps.min - 1 : rep.reps.max };
            int min{ rep.reps.min };
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
                else if (min == 0 and max == 0)
                {
                    make_epsilon(q0, qf);
                    break;
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
            const auto& tag_entry = get<typename ast_t::tag>(entry);
            make_epsilon(q0, qf, 0, tag_entry.number + 1);
            break;
        }

        case ast_index<typename ast_t::assertion>:
        {
            const typename ast_t::assertion& assertion{ get<typename ast_t::assertion>(entry) };

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

            case assert_type::ascii_word_boundary:
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

            case assert_type::not_ascii_word_boundary:
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

    bitset_t visited(node_count(), false);
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
            const auto& tr = get_tr(i);
            const auto val = std::invoke(tr_proj, tr);

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
    initial_nodes.append_range(cont_info_ | std::views::transform(&tnfa::continue_info<CharT>::value));
    initial_nodes.append_range(additional_cont_);

    const auto reachable_nodes = epsilon_closure<false>(std::move(initial_nodes), tnfa::reachable_predicate{});

    /* determine live states */

    const auto live_nodes = backwards_epsilon_closure<false>(final_nodes_, tnfa::reachable_predicate{});

    /* remove transitions containing dead and unreachable nodes */

    const auto live_and_reachable = live_nodes & reachable_nodes;

    bitset_t removed_transitions(transitions_.size(), false);

    for (tr_index i{ 0 }, i_end{ transitions_.size() }; i < i_end; ++i)
    {
        auto& tr = transitions_[i];

        if (tr.src == std::numeric_limits<state_t>::max() or tr.dst == std::numeric_limits<state_t>::max())
            continue;

        if (live_and_reachable.at(tr.src) and live_and_reachable.at(tr.dst))
            continue;

        removed_transitions[i] = true;
        tr.unset();
    }

    /* remove dead and unreachable nodes and transitions from nodes */

    const auto pred = [&rt = std::as_const(removed_transitions)](const std::size_t i){ return rt.at(i); };

    for (state_t q{ 0 }, q_end{ nodes_.size() }; q < q_end; ++q)
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
        return not holds_alternative<normal_tr>(tr.type) and not holds_alternative<lookbehind_1_tr>(tr.type);
    };

    const auto ec = epsilon_closure({ start_node() }, pred);

    /* create a copy of the start node's e-closure */

    std::flat_map<std::size_t, std::size_t> mapped_states;

    for (const state_t q : ec)
    {
        if (const auto& node = get_node(q); node.is_final)
            mapped_states[q] = q; /* do not create duplicate final nodes */
        else
            mapped_states[q] = node_copy(q);
    }

    /* duplicate transitions */

    for (const auto [q, p] : mapped_states)
    {
        bool normal_tr_flag{ false };

        /* q == p is only possible when there are no transitions from q */
        for (const tr_index i : nodes_.at(q).out_tr)
        {
            /* reminder: reference may be invalidated after one call to emplace_back (when transitions_ is resized) */
            const auto& tr = get_tr(i);

            if (holds_alternative<normal_tr>(tr.type))
            {
                /* this should be valid since in our tnfa, nodes by construction either only have outgoing
                 * normal transitions, or have outgoing e-transitions (or assertions), but never both */

                if (not normal_tr_flag)
                {
                    /* transition from copied e-closure to main graph (at most once) */
                    make_epsilon(p, q);
                    normal_tr_flag = true;
                }
            }
            else if (holds_alternative<sof_anchor_tr>(tr.type))
            {
                /* replace sof anchor with e-transition in copied subgraph */
                make_epsilon(p, mapped_states.at(tr.dst));
            }
            else if (not holds_alternative<lookbehind_1_tr>(tr.type))
            {
                /* transition within copied e-closure */
                make_copy(p, mapped_states.at(tr.dst), tr.type);
            }
        }
    }

    /* remove all sof anchors */

    for (tr_index i{ 0 }; i < transitions_.size(); ++i)
    {
        auto& tr = transitions_[i];

        if (not holds_alternative<sof_anchor_tr>(tr.type))
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
    static constexpr auto pred = [](const tnfa::transition<char_type>& tr) static {
        return not holds_alternative<normal_tr>(tr.type) and not holds_alternative<lookahead_1_tr>(tr.type);
    };

    const auto bec = backwards_epsilon_closure<false>(final_nodes_, pred);

    std::vector<state_t> initial;
    std::vector<tr_index> to_revisit;

    for (tr_index i{ 0 }; i < transitions_.size(); ++i)
    {
        const auto& tr = transitions_[i];

        if (not holds_alternative<eof_anchor_tr>(tr.type))
            continue;

        to_revisit.emplace_back(i);

        if (bec.at(tr.dst))
            initial.emplace_back(tr.dst);
    }

    const auto ec = epsilon_closure(std::move(initial), pred);

    /* create copy of the e-closures  */

    std::flat_map<state_t, state_t> mapped_states;

    for (const state_t q : ec)
    {
        if (const auto& node = get_node(q); node.is_final)
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
            const auto& tr = get_tr(i);

            if (holds_alternative<normal_tr>(tr.type))
            {
                /* do not insert transition */
            }
            else if (holds_alternative<eof_anchor_tr>(tr.type))
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
        auto& tr = transitions_.at(i);

        if (auto it = mapped_states.find(tr.dst); it != mapped_states.end())
        {
            /* remap rhs and replace with e-transition */
            const auto [q, p] = *it;
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
        const auto& tr = transitions_[i];
        if (const auto* ptr = get_if<lookahead_1_tr>(&tr.type); ptr != nullptr)
            sc_transitions[ptr->cs].emplace_back(i);
    }

    /* create copy of the e-closures of destinations of lookahead_1 transitions  */

    all_states_map_t all_mapped_states;

    std::vector<std::pair<charset_type, std::vector<state_t>>> outer_visit;

    for (const auto& [edge, tr_vec] : sc_transitions)
    {
        outer_visit.emplace_back(
            edge,
            tr_vec
            | std::views::transform([this](const tr_index i){ return transitions_.at(i).dst; })
            | std::ranges::to<std::vector>()
        );
    }

    while (not outer_visit.empty())
    {
        const auto edge = std::move(outer_visit.back().first);
        std::vector tmp{ std::move(outer_visit.back().second) };
        outer_visit.pop_back();

        /* perform a modified epsilon closure */

        auto pred = [&](const tnfa::transition<char_type>& tr){
            /* skip normal transitions and eof anchors */
            if (holds_alternative<normal_tr>(tr.type) or holds_alternative<eof_anchor_tr>(tr.type))
                return false;

            if (const auto* const ptr{ get_if<lookahead_1_tr>(&tr.type) }; ptr != nullptr)
            {
                if (auto new_edge = edge & ptr->cs; not new_edge.empty() and new_edge != ptr->cs)
                {
                    /* intersection with lookahead_1 transition requires a cloned subgraph */
                    outer_visit.emplace_back(std::move(new_edge), std::vector{ tr.dst });
                }

                return false;
            }

            return true;
        };

        const auto ec = epsilon_closure(std::move(tmp), pred);

        /* duplicate all nodes in each subgraph at most once */
        auto& mapped_states = all_mapped_states[edge];
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
            if (const auto& node = nodes_.at(q); node.is_final and node.is_fallback)
            {
                /* reminder: reference 'node' may be invalidated after calling node_create */

                if (not offset_end.has_value())
                {
                    /* create new offset copy of end node */
                    const state_t oq{ node_copy(q) };
                    nodes_[oq].final_offset = 1;
                    offset_end = oq;
                }

                make_transition(p, *offset_end, edge);
            }

            for (const tr_index i : nodes_.at(q).out_tr)
            {
                /* reminder: reference may be invalidated after one call to emplace_back (when transitions_ is resized) */
                const auto& tr = get_tr(i);

                if (const auto* const ptr{ get_if<normal_tr>(&tr.type) }; ptr != nullptr)
                {
                    /* conditionally transition from copied e-closure to main graph */
                    if (auto new_edge = edge & ptr->cs; not new_edge.empty())
                        make_transition(p, tr.dst, std::move(new_edge));
                }
                else if (const auto* const ptr{ get_if<lookahead_1_tr>(&tr.type) }; ptr != nullptr)
                {
                    /* conditionally e-transition between copied subgraphs */
                    if (auto new_edge = edge & ptr->cs; not new_edge.empty())
                        make_epsilon(p, all_mapped_states.at(std::move(new_edge)).at(tr.dst));
                }
                else if (not holds_alternative<eof_anchor_tr>(tr.type))
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
        const auto& mapped_states = all_mapped_states.at(edge);
        for (const tr_index i : tr_vec)
        {
            auto& tr = transitions_.at(i);

            /* assign the lowest priority to avoid clashes with eof_anchor */
            using priority_t = decltype(epsilon_tr::priority);
            const auto p = std::saturating_cast<priority_t>(std::saturating_sub(nodes_.at(tr.src).out_tr.size(), 1uz));

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
    for (const state_t q : final_nodes_)
        if (const auto& node = nodes_[q]; node.is_final and node.is_fallback and not node.in_tr.empty())
            fallback_states.emplace_back(q);

    /* setup for finding transitions leading into backwards e-closures */

    struct lb_info
    {
        std::vector<tr_index> tr_into;
        std::vector<tr_index> tr_between;
    };

    static constexpr auto search_predicate = [](lb_info& lb, const charset_type& edge, const tnfa::transition<char_type>& tr, const tr_index i) static {
        /* assign results here instead of using the normal return value */
        if (const auto* const ptr{ get_if<normal_tr>(&tr.type) }; ptr != nullptr)
        {
            if (const auto new_edge = edge & ptr->cs; not new_edge.empty())
                lb.tr_into.emplace_back(i);

            return false;
        }

        /* skip sof anchors */
        if (holds_alternative<sof_anchor_tr>(tr.type))
            return false;

        /* intersection with lookbehind_1 transition requires cloned subgraph */
        if (const auto* const ptr{ get_if<lookbehind_1_tr>(&tr.type) }; ptr != nullptr)
        {
            if (const auto new_edge = edge & ptr->cs; not new_edge.empty() and new_edge != ptr->cs)
                lb.tr_between.emplace_back(i);

            return false;
        }

        return true;
    };

    static constexpr auto dedup_transitions_map = [](transition_map_t& map) static {
        for (auto it = map.begin(), end{ map.end() }; it != end; ++it)
        {
            auto& vec = it->second;
            std::ranges::sort(vec);
            const auto [first, last] = std::ranges::unique(vec);
            vec.erase(first, last);
        }
    };

    /* find lookbehind transitions and group by edge */

    transition_map_t sc_transitions;
    std::flat_map<tr_index, lb_info> lb_closures;

    std::vector<tr_index> wraparounds; /* sorted; i.e. flat_set */
    charset_type wraparound_union;

    for (tr_index i{ 0 }, i_end{ transitions_.size() }; i < i_end; ++i)
    {
        const auto& tr = transitions_[i];
        const auto* ptr = get_if<lookbehind_1_tr>(&tr.type);

        if (ptr == nullptr)
            continue;

        auto [it, _] = lb_closures.try_emplace(i);
        auto& lb = it->second;
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

    bitset_t is_closure_start(transitions_.size(), false);

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
        const auto fn = [this](const tr_index i){
            const auto& tr = get_tr(i);
            return std::cref(get<lookbehind_1_tr>(tr.type).cs);
        };

        const auto refs = wraparounds | std::views::transform(fn) | std::ranges::to<std::vector>();
        wrap_starts = charset_type::partition(refs);
        std::ranges::sort(wrap_starts);
    }

    /* determine starts transitions */

    start_tr_map_t start_transitions;
    std::vector to_visit(std::from_range, sc_transitions);

    if (wraparound_lb_closure.has_value())
    {
        /* insert wraparound edges */
        transition_map_t to_insert;

        for (const auto& edge : wrap_starts)
        {
            for (const tr_index i : wraparound_lb_closure->tr_into)
                if (auto new_edge = edge & get<normal_tr>(get_tr(i).type).cs; not new_edge.empty())
                    start_transitions[i].try_emplace(std::move(new_edge));

            for (const tr_index i : wraparound_lb_closure->tr_between)
                if (auto new_edge = edge & get<lookbehind_1_tr>(get_tr(i).type).cs; not new_edge.empty() and new_edge != edge)
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

        const auto& [edge, idxs] = current;
        transition_map_t to_insert;

        for (const tr_index lb_tr_index : idxs)
        {
            const auto& lb = lb_closures.at(lb_tr_index);

            for (const tr_index i : lb.tr_into)
                if (auto new_edge = edge & get<normal_tr>(get_tr(i).type).cs; not new_edge.empty())
                    start_transitions[i].try_emplace(std::move(new_edge));

            for (const tr_index i : lb.tr_between)
                if (auto new_edge = edge & get<lookbehind_1_tr>(get_tr(i).type).cs; not new_edge.empty() and new_edge != edge)
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

        const auto fn = [&](const charset_type& edge){
            std::size_t cont_index{ std::numeric_limits<wraparound_index>::max() };

            for (std::size_t i{ 0 }, i_max{ wrap_starts.size() }; i < i_max; ++i)
            {
                if (const auto& ws = wrap_starts[i]; (edge | ws) == ws)
                {
                    cont_index = i;
                    break;
                }
            }

            return std::pair{ std::cref(edge), cont_index };
        };

        const auto ref_pairs = edges.keys() | std::views::transform(fn) | std::ranges::to<std::vector>();

        for (auto&& [new_edge, vec] : charset_type::partition_ext(ref_pairs))
        {
            std::erase(vec, std::numeric_limits<std::size_t>::max());
            if (vec.size() > 1)
                throw tnfa_error("tagged_nfa::rewrite_sc_lookbehind: vec.size() > 1");

            if (vec.size() == 1)
                if (const auto [it, success] = closure_wraparounds.try_emplace(new_edge, vec.front()); not success and it->second != vec.front())
                    throw tnfa_error("tagged_nfa::rewrite_sc_lookbehind: new_edge already exists in closure_wraparounds with different value");

            closure_starts[std::move(new_edge)].emplace_back(transitions_.at(i).dst);
        }
    }

    /* create copies of e-closures leading to lookbehind_1 transitions */

    all_states_map_t all_mapped_states;

    for (auto&& [edge, initial_states] : closure_starts)
    {
        auto& mapped_states = all_mapped_states[edge];

        auto pred = [&e = std::as_const(edge)](const tnfa::transition<char_type>& tr){
            if (holds_alternative<tnfa::normal_tr<char_type>>(tr.type))
                return false;

            if (const auto* const ptr{ get_if<lookbehind_1_tr>(&tr.type) }; ptr != nullptr)
                return (e | ptr->cs) == ptr->cs;

            return true;
        };

        std::vector ec{ epsilon_closure(std::move(initial_states), pred) };

        /* set ec to be keys */
        std::ranges::sort(ec, mapped_states.key_comp());
        const auto [first, last] = std::ranges::unique(ec);
        ec.erase(first, last);

        /* create new state for each state in ec; defer making nodes fallback to later */
        auto new_states = ec | std::views::transform([this](auto&&){ return node_create(); }) | std::ranges::to<std::vector>();
        mapped_states.replace(std::move(ec), std::move(new_states));
    }

    closure_starts.clear();

    /* set continue info and make nodes fallback */

    std::vector<tnfa::continue_at_t> continue_ats;

    for (const auto& edge : wrap_starts)
    {
        const auto& mapped_states = all_mapped_states.at(edge);
        const auto mapped_cont = mapped_states.at(continue_state.value());

        if (cont_info_.size() >= std::numeric_limits<tnfa::continue_at_t>::max())
            throw tnfa_error("tagged_nfa::rewrite_sc_lookbehind: maximum size of cont_info_ exceeded");

        continue_ats.emplace_back(std::saturating_cast<tnfa::continue_at_t>(cont_info_.size()));
        cont_info_.emplace_back(mapped_cont, edge);
        cont_info_.at(0).cs -= edge; /* note: cont_info should be non-empty to begin with */
    }

    if (continue_state.has_value())
    {
        for (const auto& [edge, mapped_states] : all_mapped_states)
        {
            auto it = closure_wraparounds.find(edge);

            if (it == closure_wraparounds.end())
                continue;

            const auto continue_at = continue_ats.at(it->second);

            for (const state_t qf : fallback_states)
            {
                auto it2 = mapped_states.find(qf);

                if (it2 == mapped_states.end())
                    continue;

                node_copy_to(qf, it2->second);
                nodes_[it2->second].continue_at = continue_at;
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

            bool normal_tr_flag{ false };

            for (const tr_index i : nodes_.at(q).out_tr)
            {
                /* reminder: reference may be invalidated after one call to emplace_back (when transitions_ is resized) */
                const auto& tr = get_tr(i);

                if (const auto* const ptr{ get_if<normal_tr>(&tr.type) }; ptr != nullptr)
                {
                    /* this should be valid since in our tnfa, nodes by construction either only have outgoing
                        * normal transitions, or have outgoing e-transitions (or assertions), but never both */

                    if (not normal_tr_flag)
                    {
                        /* transition from copied e-closure to main graph (at most once) */
                        make_epsilon(p, q);
                        normal_tr_flag = true;
                    }
                }
                else if (const auto* const ptr{ get_if<lookbehind_1_tr>(&tr.type) }; ptr != nullptr)
                {
                    if (const auto new_edge = edge & ptr->cs; not new_edge.empty())
                    {
                        if (const auto it = mapped_states.find(tr.dst); it != mapped_states.end())
                            make_epsilon(p, it->second); /* continue within copied subgraph if possible  */
                        else
                            make_epsilon(p, tr.dst); /* transition from copied subgraph back to main graph */
                    }
                }
                else if (not holds_alternative<sof_anchor_tr>(tr.type))
                {
                    /* transition within copied e-closure (if necessary) */
                    if (const auto it = mapped_states.find(tr.dst); it != mapped_states.end())
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

                auto& tr = transitions_.at(i);
                auto* const ptr{ get_if<normal_tr>(&tr.type) };

                if (ptr == nullptr)
                    continue;

                auto new_edge = edge & ptr->cs;

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
    const auto [new_end, old_end] = std::ranges::unique(to_remove);
    to_remove.erase(new_end, old_end);

    for (const tr_index i : to_remove)
    {
        auto& tr = transitions_.at(i);

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

/* introduce additional set of start states to avoid matching empty states */

template<typename CharT>
constexpr void tagged_nfa<CharT>::add_non_empty_match_pathway()
{
    /* NOTE: this function must be called before after assertions have been rewritten */

    std::vector<state_t> final_nodes{};
    std::vector<bool> is_trailing_state(nodes_.size(), false);

    for (state_t q{ 0 }, q_end{ nodes_.size() }; q < q_end; ++q)
    {
        if (nodes_[q].is_final)
        {
            if (nodes_[q].final_offset == 0)
            {
                final_nodes.emplace_back(q);
            }
            else
            {
                std::vector<state_t> to_insert{ q };

                for (std::size_t o{ 0 }, o_max{ nodes_[q].final_offset }; o < o_max; ++o)
                {
                    const auto ec = epsilon_closure(std::move(to_insert), tnfa::e_closure_predicate{});
                    to_insert.clear();

                    for (const state_t q : ec)
                    {
                        is_trailing_state.at(q) = true;
                        for (const tr_index i : nodes_.at(q).in_tr)
                            if (const auto& tr = get_tr(i); holds_alternative<normal_tr>(tr.type))
                                to_insert.emplace_back(tr.src);
                    }
                }

                final_nodes.append_range(to_insert);
            }
        }
    }

    const auto bec = backwards_epsilon_closure<false>(std::move(final_nodes), tnfa::e_closure_predicate{});

    /* determine start and continue nodes that can result in an empty match */

    std::vector<state_t> to_remap;

    if (bec.at(start_node_))
        to_remap.emplace_back(start_node_);

    for (const auto& cont : cont_info_)
        if (bec.at(cont.value))
            to_remap.emplace_back(cont.value);

    if (to_remap.empty())
        return;

    /* create a copy of the start and continue nodes' e-closures */

    std::flat_map<std::size_t, std::size_t> mapped_states;

    std::vector ec{ epsilon_closure(std::move(to_remap), tnfa::e_closure_predicate{}) };
    std::ranges::sort(ec);

    for (const state_t q : ec)
        mapped_states.emplace_hint(mapped_states.end(), q, node_create());

    /* duplicate transitions */

    for (const auto [q, p] : mapped_states)
    {
        for (const tr_index i : nodes_.at(q).out_tr)
        {
            /* reminder: reference may be invalidated after one call to emplace_back (when transitions_ is resized) */
            const auto& tr = get_tr(i);

            if (holds_alternative<normal_tr>(tr.type))
            {
                /* conditionally transition from copied e-closure to main graph */
                if (not is_trailing_state.at(tr.dst))
                    make_copy(p, tr.dst, tr.type);
            }
            else
            {
                /* transition within copied subgraph */
                make_copy(p, mapped_states.at(tr.dst), tr.type);
            }
        }
    }

    /* copy continue states to additional continue states */

    for (const auto& cont : cont_info_)
    {
        if (const auto it = mapped_states.find(cont.value); it != mapped_states.end())
            additional_cont_.emplace_back((*it).second);
        else
            additional_cont_.emplace_back(cont.value);
    }

    if (const auto it = mapped_states.find(start_node_); it != mapped_states.end())
        additional_cont_.emplace_back((*it).second);
    else
        additional_cont_.emplace_back(start_node_);

    remove_dead_and_unreachable_states();
}

/* constructor */

template<typename CharT>
constexpr tagged_nfa<CharT>::tagged_nfa(const expr_tree<char_type>& ast, fsm_flags flags)
    : capture_info_{ ast.get_capture_info() }, tag_count_{ ast.tag_count() }, flags_{ flags }
{
    using tnfa::stack_elem;

    auto& dfn = nodes_.at(default_final_node);
    dfn.is_final = true;
    dfn.is_fallback = (flags_.enable_fallback and not flags_.longest_match);

    if (flags_.is_iterator or flags_.adapted_search)
        cont_info_.emplace_back(default_start_node, ~charset_type{});

    std::vector<std::vector<int>> tag_vec{};
    if (tag_count_ > 1)
        ast.make_tag_vec(tag_vec);

    if (not ast.is_alt_mode())
    {
        thompson(ast, tag_vec, stack_elem{ .q0 = default_start_node, .qf = default_final_node, .idx = ast.root_idx() });
        return;
    }

    /* lexer mode implementation */

    const std::size_t root_idx{ ast.root_idx() };
    const auto& root = std::get<typename ast_t::alt>(ast.get_expr(root_idx));
    const std::size_t branch_count{ root.idxs.size() };

    if (branch_count > 1)
    {
        const tnfa::node dfn_copy{ dfn };
        std::ranges::fill_n(std::back_inserter(nodes_), branch_count - 1, dfn_copy);
        std::ranges::copy(std::views::iota(1uz, branch_count), std::back_inserter(final_nodes_));
        std::uint_least32_t i{ 0 };
        for (auto& new_final_node : nodes_ | std::views::drop(2))
            new_final_node.final_alt = ++i;
    }

    for (std::size_t i{ 0 }; i < branch_count; ++i)
    {
        state_t q0{ node_create() };
        state_t qf{ default_final_node + i };
        make_epsilon(default_start_node, q0, i);

        /* generate tag-aware nfa if necessary */
        if (not (tag_vec.empty() or tag_vec.at(root_idx).empty()))
        {
            /* insert ntags beforehand */
            std::vector<int> ntags_before;
            for (std::size_t j{ 0 }; j < i; ++j)
                std::ranges::copy(tag_vec.at(root.idxs[j]), std::back_inserter(ntags_before));

            std::ranges::sort(ntags_before);
            auto [_, before_last] = std::ranges::unique(ntags_before);
            ntags_before.erase(before_last, ntags_before.end());

            if (not ntags_before.empty())
            {
                const state_t q1{ node_create() };
                make_ntags(q0, q1, ntags_before);
                q0 = q1;
            }

            /* insert ntags after */
            std::vector<int> ntags_after;
            for (std::size_t j{ i + 1 }; j < branch_count; ++j)
                std::ranges::copy(tag_vec.at(root.idxs[j]), std::back_inserter(ntags_after));

            std::ranges::sort(ntags_after);
            auto [_, after_last] = std::ranges::unique(ntags_after);
            ntags_after.erase(after_last, ntags_after.end());

            if (not ntags_before.empty())
            {
                const state_t qe{ node_create() };
                make_ntags(qe, qf, ntags_after);
                qf = qe;
            }
        }

        thompson(ast, tag_vec, stack_elem{ .q0 = q0, .qf = qf, .idx = root.idxs[i] });
    }
}

}
}

namespace srx {
namespace detail {
namespace tdfa {

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
    std::uint_least16_t offset;
    std::uint_least32_t alternative;

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
inline constexpr std::size_t   default_transition_is_not_state{ std::numeric_limits<std::size_t>::max() - 1 };

template<typename CharT>
struct transition
{
    std::size_t next;
    std::size_t op_index; /* use no_transition_regops for no ops */
    charset_t<CharT> cs;

    friend constexpr bool operator==(const transition&, const transition&) = default;
};

struct default_transition
{
    std::size_t next;
    std::size_t op_index; /* use no_transition_regops for no ops, and default_transition_is_not_state for jumps */

    friend constexpr bool operator==(const default_transition&, const default_transition&) = default;
};

template<typename CharT>
struct node
{
    std::vector<transition<CharT>> tr;
    std::optional<default_transition> default_tr;

    friend constexpr bool operator==(const node&, const node&) = default;
};

}

template<typename CharT>
class tagged_dfa
{
public:
    using char_type = CharT;

    constexpr explicit tagged_dfa(const tagged_nfa<char_type>& tnfa);
    constexpr void optimise_registers();
    constexpr void compact_regop_blocks();
    constexpr void minimise_states();
    constexpr void minimise_transition_edges();
    constexpr void make_default_transitions();
    constexpr void make_shared_transitions();
    constexpr void de_default_edges();

    friend class tdfa::factory<char_type>;
    friend class tdfa::opt<char_type>;
    friend class tdfa::min<char_type>;

    /* observers */

    static constexpr std::size_t match_start{ 0 };

    [[nodiscard]] constexpr const tdfa::node<CharT>& get_node(std::size_t i) const { return nodes_.at(i); }
    [[nodiscard]] constexpr const tdfa::regops_t& get_regops(std::size_t i) const { return (i == tdfa::no_transition_regops) ? tdfa::empty_regops : regops_.at(i); }
    [[nodiscard]] constexpr const tdfa::continue_nodes_t& continue_nodes() const { return continue_nodes_; }
    [[nodiscard]] constexpr const tdfa::continue_nodes_t& additional_continue_nodes() const { return additional_continue_nodes_; }
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
    tdfa::continue_nodes_t  additional_continue_nodes_;
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
}

namespace srx {
namespace detail {
namespace tdfa {

constexpr bool toposort_regops(regops_t::iterator beg, regops_t::iterator end, reg_t regcount);

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

struct node_info
{
    config_set_t config;
    bool         is_fallback{ false };

    friend constexpr bool operator==(const node_info&, const node_info&) = default;
};

using state_info_t = std::vector<node_info>;
using continue_info_t = std::flat_map<std::size_t, tdfa::continue_at_t>;

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
        auto it = std::ranges::lower_bound(data_, tag, std::less{}, &entry::tag);

        for (const auto end = data_.end(); it != end and it->tag == tag; ++it)
            if (it->op == op)
                return { it->reg, true };

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
    using epsilon_tr_t = std::pair<tnfa::state_t, tnfa::epsilon_tr>;
    using normal_tr_t  = std::pair<tnfa::state_t, std::reference_wrapper<const tnfa::charset_t<char_type>>>;
    using epsilon_tr_info_t = std::vector<std::vector<epsilon_tr_t>>;
    using normal_tr_info_t  = std::vector<std::vector<normal_tr_t>>;

    [[nodiscard]] constexpr closure_t e_closure(closure_t&& c) const;
    [[nodiscard]] constexpr std::size_t add_state(tdfa_t& result, const closure_t& c, regops_t& o);
    [[nodiscard]] constexpr multistep_closures_t<char_type> multistep(std::size_t state) const;
    [[nodiscard]] constexpr regops_t transition_regops(closure_t& c, reg_t& regcount, tag_op_map& map) const;
    [[nodiscard]] constexpr regops_t final_regops(const final_regs_t& final_registers, const reg_vec& r, const tag_sequence_t& tag_seq) const;
    [[nodiscard]] constexpr regop::op_t regop_rhs(const std::vector<bool>& hist) const;
    [[nodiscard]] constexpr std::vector<bool> history(const tag_sequence_t& hist, tag_t tag) const;
    [[nodiscard]] constexpr bool has_history(const tag_sequence_t& hist, tag_t tag) const;
    [[nodiscard]] constexpr bool mappable(const node_info& state, std::size_t mapped_state, regops_t& o, reg_t regcount) const;

    constexpr void fallback_regops(tdfa_t& result);
    constexpr void backup_regops(tdfa_t& result, std::size_t state, reg_t reg_dst, reg_t reg_src);

    constexpr std::size_t make_initial_state(tdfa_t& result, tnfa::state_t tnfa_state);

    constexpr void factory_init();

    static constexpr std::size_t hash_state(const node_info& state);
    static constexpr std::size_t hash_mappability(const node_info& state);

    const tnfa_t* tnfa_ptr_;
    state_info_t state_info_;
    continue_info_t cont_info_;
    epsilon_tr_info_t epsilon_transitions_;
    normal_tr_info_t normal_transitions_;
    reg_t tag_count_;
    fsm_flags flags_;

    std::vector<std::size_t> state_hashes_keys_;
    std::vector<std::size_t> state_hashes_values_;
    std::vector<std::size_t> mappable_candidate_keys_;
    std::vector<std::size_t> mappable_candidate_values_;
};

template<typename CharT>
constexpr std::size_t factory<CharT>::hash_state(const node_info& state)
{
    std::size_t hash{ hash::init() };
    hash::append(hash, state);
    return hash;
}

template<typename CharT>
constexpr std::size_t factory<CharT>::hash_mappability(const node_info& state)
{
    std::size_t hash{ hash::init() };
    for (const auto& cfg: state.config)
        hash::append(hash, cfg.tnfa_state);
    return hash;
}

template<typename CharT>
constexpr auto factory<CharT>::e_closure(closure_t&& c) const -> closure_t
{
    closure_t new_closure;

    closure_t stack{ std::move(c) };
    std::erase_if(stack, [](const closure_entry& ce){ return not ce.new_tag_seq.empty(); });
    std::ranges::reverse(stack);
    bitset_t visited(tnfa_ptr_->node_count(), false);

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

        for (const auto& [next, e] : epsilon_transitions_.at(ce.tnfa_state))
        {
            if (visited.at(next))
                continue;

            auto newer_tag_seq = ce.new_tag_seq;
            if (e.tag != 0)
                newer_tag_seq.push_back(e.tag);
            stack.emplace_back(next, ce.registers, ce.tag_seq, std::move(newer_tag_seq));
        }
    }

    if (flags_.longest_match)
    {
        /* this version is needed for full matches, but below is needed for laziness in prefix matches */
        std::erase_if(new_closure, [this](const closure_entry& ce) -> bool {
            const auto& node = tnfa_ptr_->get_node(ce.tnfa_state);
            if (node.is_final or node.is_fallback)
                return false;
            return normal_transitions_.at(ce.tnfa_state).empty();
        });
    }
    else
    {
        /* remove all (non-final) states with only e-transitions, and remove all states after first fallback state encountered */
        bool end_found{ false };
        std::erase_if(new_closure, [this, &end_found](const closure_entry& ce) -> bool {
            if (end_found) return true;
            const auto& node = tnfa_ptr_->get_node(ce.tnfa_state);
            if (node.is_fallback)
            {
                end_found = true;
                return false;
            }
            if (node.is_final) return false;
            return normal_transitions_.at(ce.tnfa_state).empty();
        });
    }

    return new_closure;
}

template<typename CharT>
constexpr auto factory<CharT>::add_state(tdfa_t& result, const closure_t& c, regops_t& o) -> std::size_t
{
    static constexpr std::size_t map_usage_threshold{ 128 };
    static constexpr auto key_proj = [](const auto& v) -> decltype(auto) { return get<0>(v); };

    node_info current_info{ .config{ c | std::views::transform(closure_entry::next_config) | std::ranges::to<std::vector>() } };
    std::size_t new_state{ state_info_.size() };

    if (new_state < map_usage_threshold) [[likely]]
    {
        /* check if state already exists */
        const std::size_t sh_key{ hash_state(current_info) };
        for (std::size_t existing_state{ 0 }, size{ state_hashes_keys_.size() }; existing_state < size; ++existing_state)
            if (sh_key == state_hashes_keys_[existing_state]) [[unlikely]]
                if (mappable(current_info, existing_state, o, result.register_count_))
                    return existing_state;

        /* check if state can be mapped to an existing state */
        const std::size_t mc_key{ hash_mappability(current_info) };
        for (std::size_t mapped_state{ 0 }, size{ mappable_candidate_keys_.size() }; mapped_state < size; ++mapped_state)
            if (mc_key == mappable_candidate_keys_[mapped_state]) [[unlikely]]
                if (mappable(current_info, mapped_state, o, result.register_count_))
                    return mapped_state;

        /* create new state */
        result.nodes_.emplace_back();
        state_info_.emplace_back(std::move(current_info));

        state_hashes_keys_.emplace_back(sh_key);
        mappable_candidate_keys_.emplace_back(mc_key);

        /* switch to using maps at threshold */
        if (state_info_.size() == map_usage_threshold)
        {
            state_hashes_values_.assign_range(std::views::iota(0uz, new_state));
            mappable_candidate_values_.assign_range(std::views::iota(0uz, new_state));

            std::ranges::sort(std::views::zip(state_hashes_keys_, state_hashes_values_), {}, key_proj);
            std::ranges::sort(std::views::zip(mappable_candidate_keys_, mappable_candidate_values_), {}, key_proj);
        }
    }
    else
    {
        /* check if state already exists */
        auto sh_zv = std::views::zip(state_hashes_keys_, state_hashes_values_);
        const std::size_t sh_key{ hash_state(current_info) };
        const auto sh_eq_range = std::ranges::equal_range(sh_zv, sh_key, {}, key_proj);

        for (const auto& [_, state] : sh_eq_range)
            if (current_info == state_info_.at(state))
                return state;

        /* check if state can be mapped to an existing state */
        auto mc_zv = std::views::zip(mappable_candidate_keys_, mappable_candidate_values_);
        const std::size_t mc_key{ hash_mappability(current_info) };
        const auto mc_eq_range = std::ranges::equal_range(mc_zv, mc_key, {}, key_proj);

        for (const auto& [_, mapped_state] : mc_eq_range)
            if (mappable(current_info, mapped_state, o, result.register_count_))
                return mapped_state;

        /* create new state */
        result.nodes_.emplace_back();
        state_info_.emplace_back(std::move(current_info));

        const auto sh_offset = std::ranges::distance(std::ranges::begin(sh_zv), std::ranges::end(sh_eq_range));
        state_hashes_keys_.emplace(state_hashes_keys_.cbegin() + sh_offset, sh_key);
        state_hashes_values_.emplace(state_hashes_values_.cbegin() + sh_offset, new_state);

        const auto mc_offset = std::ranges::distance(std::ranges::begin(mc_zv), std::ranges::end(mc_eq_range));
        mappable_candidate_keys_.emplace(mappable_candidate_keys_.cbegin() + mc_offset, mc_key);
        mappable_candidate_values_.emplace(mappable_candidate_values_.cbegin() + mc_offset, new_state);
    }

    /* make final regops if state is an accepting state */
    const auto& current_cfg = state_info_.back().config;
    const auto is_final = [this](std::size_t arg){ return tnfa_ptr_->get_node(arg).is_final; };
    const auto it = std::ranges::find_if(current_cfg, is_final, &configuration::tnfa_state);
    std::optional<continue_at_t> continue_at;
    if (it != current_cfg.end())
    {
        auto final_ops = final_regops(result.final_registers_, it->registers, it->tag_seq);
        const auto& node = tnfa_ptr_->get_node(it->tnfa_state);
        const auto offset = node.final_offset;
        const auto alt = node.final_alt;

        if (node.continue_at < tnfa_ptr_->get_cont_info().size())
            continue_at = node.continue_at;

        if (final_ops.empty())
        {
            /* avoid creating empty regop blocks */
            result.final_nodes_.emplace(new_state, final_node_info{ .op_index = no_transition_regops, .offset = offset, .alternative = alt });
        }
        else
        {
            result.final_nodes_.emplace(new_state, final_node_info{ .op_index = result.regops_.size(), .offset = offset, .alternative = alt });
            result.regops_.emplace_back(std::move(final_ops));
        }
    }

    if (flags_.enable_fallback)
    {
        /* set fallback state status for fallback_regops */
        const auto is_fallback = [&](std::size_t arg){ return tnfa_ptr_->get_node(arg).is_fallback; };
        const auto it2 = std::ranges::find_if(current_cfg, is_fallback, &configuration::tnfa_state);
        if (it2 != current_cfg.end())
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
    using elem_t = tnfa::charset_t<char_type>::template ref_pair<closure_entry>;

    std::vector<elem_t> transitions;

    for (const auto& cfg : state_info_.at(state).config)
        for (const auto& [dst, cs] : normal_transitions_.at(cfg.tnfa_state))
            transitions.emplace_back(cs, closure_entry{ dst, cfg.registers, cfg.tag_seq });

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
            auto h = history(ce.tag_seq, t);

            if (not h.empty())
            {
                regop::op_t op_rhs{ regop_rhs(h) };

                auto [value_ref, existing] = map.lookup(t, op_rhs);

                if (not existing)
                {
                    auto i = regcount++;
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
        auto h = history(tag_seq, t);

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
    std::vector<bool> result;
    for (const tag_t x : h)
        if (tag == (x < 0 ? -x : x))
            result.push_back(x >= 0);
    return result;
}

template<typename CharT>
constexpr bool factory<CharT>::has_history(const tag_sequence_t& h, tag_t tag) const
{
    return std::ranges::contains(h, tag, [](auto x){ return x < 0 ? -x : x; });
}

template<typename CharT>
constexpr bool factory<CharT>::mappable(const node_info& state, std::size_t mapped_state, regops_t& o, const reg_t regcount) const
{
    const auto& mapped_state_info = state_info_.at(mapped_state);

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
                if (has_history(ce1.tag_seq, t))
                    continue;

                const reg_t i{ ce1.registers.at(t - 1) };
                const reg_t j{ ce2.registers.at(t - 1) };

                auto it = map.lower_bound(i);
                auto jt = rmap.lower_bound(j);

                const bool not_i_match{ it == map.end() or (*it).first != i };
                const bool not_j_match{ jt == rmap.end() or (*jt).first != j };

                if (not_i_match and not_j_match)
                {
                    if (it != map.end()) ++it;
                    if (jt != rmap.end()) ++jt;

                    map.emplace_hint(it, i, j);
                    rmap.emplace_hint(jt, j, i);
                }
                else if (not_i_match or not_j_match or (*it).second != j or (*jt).second != i)
                {
                    return false;
                }
            }
        }
    }

    regops_t o_copy{ o };

    for (auto it = o_copy.begin(); it != o_copy.end();)
    {
        const auto i = it->dst;

        const auto map_it = map.find(i);
        const auto map_end = map.end();

        if (map_it == map_end)
        {
            /* skip mapping registers with history */
            it = o_copy.erase(it);
            continue;
        }

        it->dst = (*map_it).second;
        map.erase(map_it);
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

        const auto& final_ops = result.get_regops(fni.op_index);

        /* determine clobbered registers */

        bitset_t added(result.nodes_.size(), false);
        bitset_t clobbered(result.register_count_, false);

        std::vector<std::pair<std::size_t, std::size_t>> stack;
        stack.emplace_back(state, 0);
        added.at(state) = true;

        while (not stack.empty())
        {
            auto& [s, i] = stack.back();

            if (result.final_nodes_.contains(s) or i >= result.nodes_.at(s).tr.size())
            {
                stack.pop_back();
            }
            else
            {
                const auto& tr = result.nodes_.at(s).tr.at(i);

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

        auto continuation_index = tdfa::no_continue;
        if (auto it = cont_info_.find(state); it != cont_info_.end())
            continuation_index = it->second;

        /* insert fallback regops */

        regops_t o;

        for (const auto& f : final_ops)
        {
            if (auto* cpy = get_if<regop::copy>(&f.op); cpy != nullptr and clobbered.at(cpy->src))
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
    return add_state(result, initial_cfg, regs);
}

template<typename CharT>
constexpr void factory<CharT>::factory_init()
{
    static constexpr auto compose = [](const auto& g, const auto& f){
        return [=]<typename T>(T&& arg) {
            return std::invoke(g, std::invoke(f, std::forward<T>(arg)));
        };
    };

    const std::size_t node_count{ tnfa_ptr_->node_count() };
    epsilon_transitions_.resize(node_count);
    normal_transitions_.resize(node_count);

    for (std::size_t q{ 0 }; q < node_count; ++q)
    {
        auto& current_etr = epsilon_transitions_.at(q);
        auto& current_tr = normal_transitions_.at(q);

        for (const auto tr_idx : tnfa_ptr_->get_node(q).out_tr)
        {
            const auto& tr = tnfa_ptr_->get_tr(tr_idx);

            if (const auto* const ptr{ get_if<tnfa::normal_tr<char_type>>(&tr.type) }; ptr != nullptr)
            {
                current_tr.emplace_back(tr.dst, std::cref(ptr->cs));
            }
            else if (const auto* const ptr{ get_if<tnfa::epsilon_tr>(&tr.type) }; ptr != nullptr)
            {
                current_etr.emplace_back(tr.dst, *ptr);
            }
            else
            {
                throw tnfa_error("Input contains un-rewritten assertions");
            }
        }

        std::ranges::sort(current_etr, std::ranges::greater{}, compose(&tnfa::epsilon_tr::priority, &epsilon_tr_t::second));
    }
}

template<typename CharT>
constexpr factory<CharT>::factory(const tnfa_t& input, tdfa_t& result, const std::size_t tag_count)
    : tnfa_ptr_{ &input }, tag_count_{ std::saturating_cast<reg_t>(tag_count) }, flags_{ result.flags_ }
{
    factory_init();

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

    if (const auto& ac = tnfa_ptr_->get_additional_cont(); not ac.empty())
    {
        const auto& ci = tnfa_ptr_->get_cont_info();

        for (std::size_t i{ 0 }, i_max{ ci.size() }; i < i_max; ++i)
        {
            if (ac.at(i) == ci.at(i).value)
                result.additional_continue_nodes_.emplace_back(result.continue_nodes_.at(i));
            else
                result.additional_continue_nodes_.emplace_back(make_initial_state(result, ac.at(i)));
        }

        if (const auto acb = ac.back(); acb == tnfa_ptr_->start_node())
            result.additional_continue_nodes_.emplace_back(initial);
        else
            result.additional_continue_nodes_.emplace_back(make_initial_state(result, ac.back()));
    }

    for (std::size_t state{ initial }; state < result.nodes_.size(); ++state)
    {
        tag_op_map map;

        for (auto& [cs, cfg] : multistep(state))
        {
            cfg = e_closure(std::move(cfg));
            auto o = transition_regops(cfg, result.register_count_, map);
            const auto s = add_state(result, cfg, o);

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

/* regops sorting */

constexpr bool toposort_regops(const regops_t::iterator beg, const regops_t::iterator end, const reg_t regcount)
{
    static constexpr reg_t regop_threshold{ 64 };

    if (regcount < regop_threshold) [[likely]]
    {
        std::vector<unsigned int> indeg(regcount, 0);

        for (auto it = beg; it != end; ++it)
        {
            if (const auto* copy = get_if<regop::copy>(&it->op); copy != nullptr)
                ++indeg.at(copy->src);
        }

        regops_t o_new;
        regops_t o_copy{ beg, end };
        bool cycle_detected{ false };

        while (not o_copy.empty())
        {
            bool added{ false };

            for (auto it = o_copy.begin(); it != o_copy.end();)
            {
                if (indeg.at(it->dst) == 0)
                {
                    if (const auto* copy = get_if<regop::copy>(&it->op); copy != nullptr)
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
                    if (const auto* copy = get_if<regop::copy>(&oc.op); copy != nullptr and copy->src != oc.dst)
                    {
                        cycle_detected = true;
                        break;
                    }
                }

                o_new.append_range(o_copy);
                break;
            }
        }

        if (std::cmp_not_equal(end - beg, o_new.size()))
            cycle_detected = true; /* unknown error */

        if (not cycle_detected)
            std::ranges::move(o_copy, beg);

        return not cycle_detected;
    }
    else
    {
        std::flat_map<reg_t, unsigned int> indeg;

        for (auto it = beg; it != end; ++it)
        {
            if (const auto* copy = get_if<regop::copy>(&it->op); copy != nullptr)
                ++indeg[copy->src];
        }

        regops_t o_new;
        regops_t o_copy{ beg, end };
        bool cycle_detected{ false };

        while (not o_copy.empty())
        {
            bool added{ false };

            for (auto it = o_copy.begin(); it != o_copy.end();)
            {
                if (indeg[it->dst] == 0)
                {
                    if (const auto* copy = get_if<regop::copy>(&it->op); copy != nullptr)
                        --indeg[copy->src];

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
                    if (const auto* copy = get_if<regop::copy>(&oc.op); copy != nullptr and copy->src != oc.dst)
                    {
                        cycle_detected = true;
                        break;
                    }
                }

                o_new.append_range(o_copy);
                break;
            }
        }

        if (std::cmp_not_equal(end - beg, o_new.size()))
            cycle_detected = true; /* unknown error */

        if (not cycle_detected)
            std::ranges::move(o_copy, beg);

        return not cycle_detected;
    }
}

/* tdfa normalisation */

constexpr regops_t::iterator dedup_regops(regops_t& o, const regops_t::iterator beg, const regops_t::iterator end)
{
    regops_t::iterator new_end{ end };

    for (auto it = beg; it != new_end; ++it)
        new_end = std::remove(it + 1, new_end, *it);

    return o.erase(new_end, end);
}

constexpr void normalise_regops(regops_t& o, const reg_t regcount)
{
    for (auto it = o.begin(); it != o.end();)
    {
        auto local_end = it + 1;
        if (holds_alternative<regop::set>(it->op))
        {
            while (local_end != o.end() and holds_alternative<regop::set>(local_end->op))
                ++local_end;

            local_end = dedup_regops(o, it, local_end);
            std::ranges::sort(it, local_end, std::ranges::less{}, &regop::dst);
            it = local_end;
        }
        else if (holds_alternative<regop::copy>(it->op))
        {
            while (local_end != o.end() and holds_alternative<regop::copy>(local_end->op))
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
    constexpr explicit liveness_matrix(std::size_t block_count, reg_t reg_count) : data_(block_count, bitset_t(reg_count, false)) {}
    [[nodiscard]] constexpr auto operator[](std::size_t block_idx, reg_t reg) { return data_[block_idx][reg]; }
    [[nodiscard]] constexpr auto operator[](std::size_t block_idx, reg_t reg) const { return data_[block_idx][reg]; }
    [[nodiscard]] constexpr auto at(std::size_t block_idx, reg_t reg) { return data_.at(block_idx).at(reg); }
    [[nodiscard]] constexpr auto at(std::size_t block_idx, reg_t reg) const { return data_.at(block_idx).at(reg); }
    [[nodiscard]] constexpr auto& row(std::size_t block_idx) { return data_.at(block_idx); }
    [[nodiscard]] constexpr const auto& row(std::size_t block_idx) const { return data_.at(block_idx); }
    [[nodiscard]] constexpr bool block_valid(std::size_t block_idx) const { return block_idx < data_.size(); }

private:
    std::vector<bitset_t> data_;
};

class square_matrix
{
public:
    constexpr explicit square_matrix(std::size_t reg_count) : data_(reg_count * reg_count, false), reg_count_{ reg_count } {}
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
    bitset_t data_;
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

    for (std::size_t node_idx{ 0 }, node_count{ dfa.nodes_.size() }; node_idx < node_count; ++node_idx)
    {
        for (const auto& tr : dfa.nodes_[node_idx].tr)
        {
            if (tr.op_index == no_transition_regops)
                reachable.at(node_idx, tr.next) = true;
            else
                successor_blocks.at(node_idx).emplace_back(tr.op_index);
        }

        /* add final transitions too */

        if (dfa.final_nodes_.contains(node_idx))
            if (const auto fni = dfa.final_nodes_.at(node_idx); fni.op_index != no_transition_regops)
                successor_blocks.at(node_idx).emplace_back(fni.op_index);
    }

    /* calculate reachability matrix (initial reachability is added above) */

    const auto side_length = reachable.side_length();
    for (std::size_t i{ 0 }; i < side_length; ++i)
        reachable[i, i] = true;

    for (std::size_t k{ 0 }; k < side_length; ++k)
        for (std::size_t i{ 0 }; i < side_length; ++i)
            for (std::size_t j{ 0 }; j < side_length; ++j)
                if (not reachable[i, j])
                    reachable[i, j] = reachable[i, k] and reachable[k, j];

    /* construct graph */

    std::vector<std::vector<std::size_t>> nodes_to_edges(dfa.nodes_.size());

    for (std::size_t node_idx{ 0 }, node_count{ dfa.nodes_.size() }; node_idx < node_count; ++node_idx)
    {
        std::vector<std::size_t> tmp;

        for (std::size_t i{ 0 }; i < reachable.side_length(); ++i)
            if (reachable.at(node_idx, i))
                tmp.insert_range(tmp.end(), successor_blocks.at(i));

        std::ranges::sort(tmp);
        auto [beg, end] = std::ranges::unique(tmp);
        tmp.erase(beg, end);
        tmp.shrink_to_fit();

        nodes_to_edges.at(node_idx) = std::move(tmp);
    }

    for (std::size_t node_idx{ 0 }; node_idx < dfa.nodes_.size(); ++node_idx)
        for (const auto& tr : dfa.nodes_.at(node_idx).tr)
            if (tr.op_index != no_transition_regops)
                block_graph_.at(tr.op_index) = nodes_to_edges.at(tr.next);

    block_graph_start_ = std::move(nodes_to_edges.at(dfa.match_start));

    /* add additional start nodes to block_graph_start */
    std::vector to_visit(dfa.continue_nodes_);
    to_visit.append_range(dfa.additional_continue_nodes_);
    std::ranges::sort(to_visit);
    auto [efirst, elast] = std::ranges::unique(to_visit);
    to_visit.erase(efirst, elast);
    std::erase(to_visit, dfa.match_start);

    for (const std::size_t c : to_visit)
        block_graph_start_.append_range(std::move(nodes_to_edges.at(c)));
}

template<typename CharT>
constexpr auto opt<CharT>::compact_registers(const tdfa_t& dfa) -> remap_t
{
    bitset_t visited(dfa.register_count_, false);
    remap_t remap_result(dfa.register_count_, std::numeric_limits<reg_t>::max());

    for (auto& block : dfa.regops_)
    {
        for (auto& op : block)
        {
            if (auto* cpy = get_if<regop::copy>(&op.op); cpy != nullptr)
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
        for (auto it = block.begin(); it != block.end();)
        {
            it->dst = remap.at(it->dst);
            if (auto* cpy = get_if<regop::copy>(&it->op); cpy != nullptr)
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

    dfa.register_count_ = 1 + std::ranges::max(remap | std::views::filter([](reg_t r){ return r != std::numeric_limits<reg_t>::max(); }));
}

template<typename CharT>
constexpr liveness_matrix opt<CharT>::liveness(const tdfa_t& dfa) const
{
    /* unfortunately coroutines can't be constexpr yet */

    class postorder_visitor
    {
    public:
        constexpr explicit postorder_visitor(const std::vector<std::size_t>& block_graph_start, std::size_t block_count)
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
                const auto [block_idx, i] = stack.back();

                if (i == block_graph.at(block_idx).size())
                {
                    result = block_idx;
                    stack.pop_back();
                }
                else
                {
                    const auto next = block_graph.at(block_idx).at(i);
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
        bitset_t block_added;
    };

    /* resume liveness implementation */

    const std::size_t block_count{ dfa.regops_.size() };
    const std::size_t reg_count{ dfa.register_count_ };
    liveness_matrix liveness(block_count, reg_count);

    /* make registers assigned to in final transitions live */

    for (const auto [_, fni] : dfa.final_nodes_)
        for (const reg_t final_reg : dfa.final_registers_)
            liveness.at(fni.op_index, final_reg) = true;

    bitset_t current_row_copy;
    bitset_t successor_row_copy;

    for (bool loop{ true }; loop;)
    {
        loop = false;
        postorder_visitor vis{ block_graph_start_, block_count };

        while (true)
        {
            const auto opt = vis(block_graph_);
            if (not opt.has_value())
                break;

            const auto block_idx = opt.value();

            current_row_copy = liveness.row(block_idx);

            for (const std::size_t sblock_idx : block_graph_.at(block_idx))
            {
                successor_row_copy = liveness.row(sblock_idx);

                for (const auto& op : dfa.regops_.at(sblock_idx) | std::views::reverse) /* todo: in post order? */
                {
                    if (holds_alternative<regop::set>(op.op))
                    {
                        successor_row_copy.at(op.dst) = false;
                    }
                    else if (const auto* cpy = get_if<regop::copy>(&op.op); cpy != nullptr)
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

                current_row_copy |= successor_row_copy;
            }

            if (current_row_copy != liveness.row(block_idx))
            {
                liveness.row(block_idx) = std::move(current_row_copy);
                loop = true;
            }
        }
    }

    bitset_t current_row(reg_count, false);
    bitset_t added(dfa.nodes_.size(), false);

    for (const auto [fallback_state, fbni] : dfa.fallback_nodes_)
    {
        if (fbni.op_index == no_transition_regops)
        {
            current_row.reset();

            for (const std::size_t final_reg : dfa.final_registers_)
                current_row.at(final_reg) = true;
        }
        else
        {
            for (const std::size_t final_reg : dfa.final_registers_)
                liveness.at(fbni.op_index, final_reg) = true;

            current_row = liveness.row(fbni.op_index);

            for (const auto& op : dfa.regops_.at(fbni.op_index))
                current_row.at(op.dst) = false;

            for (const auto& op : dfa.regops_.at(fbni.op_index))
                if (auto* cpy = get_if<regop::copy>(&op.op); cpy != nullptr)
                    current_row.at(cpy->src) = false;
        }

        /* traverse nodes that can possibly fallback to fallback_state */

        added.reset();
        std::vector<std::pair<std::size_t, std::size_t>> stack;
        stack.emplace_back(fallback_state, 0);
        added.at(fallback_state) = true;

        while (not stack.empty())
        {
            auto& [s, i] = stack.back();

            if (dfa.final_nodes_.contains(s) or i >= dfa.nodes_.at(s).tr.size())
            {
                stack.pop_back();
            }
            else
            {
                const auto& tr = dfa.nodes_.at(s).tr.at(i);

                /* note: we overapproximate here by assuming the transition function from a state is
                *       not a total function, making a fallback possible from every non-final state */

                if (liveness.block_valid(tr.op_index))
                    liveness.row(tr.op_index) |= current_row;

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
    for (std::size_t block_idx{ 0 }, block_count{ dfa.regops_.size() }; block_idx < block_count; ++block_idx)
    {
        auto& block = dfa.regops_.at(block_idx);
        bitset_t current_row_copy{ liveness.row(block_idx) };

        for (std::size_t i{ block.size() }; i > 0; --i)
        {
            const auto& op = block.at(i - 1);

            if (current_row_copy.at(op.dst))
            {
                if (holds_alternative<regop::set>(op.op))
                    current_row_copy.at(op.dst) = false;
                else if (const auto* cpy = get_if<regop::copy>(&op.op); cpy != nullptr)
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

    for (std::size_t block_idx{ 0 }, block_count{ dfa.regops_.size() }; block_idx < block_count; ++block_idx)
    {
        const auto& block = dfa.regops_.at(block_idx);

        for (const auto& op : block)
            if (const auto* cpy = get_if<regop::copy>(&op.op); cpy != nullptr)
                histories.at(cpy->src) = op.op;

        for (const auto& op : block)
        {
            if (const auto* set = get_if<regop::set>(&op.op); set != nullptr)
                histories.at(op.dst) = op.op;
            else if (const auto* cpy = get_if<regop::copy>(&op.op); cpy != nullptr)
                histories.at(op.dst) = histories.at(cpy->src);
            else
                std::unreachable();
        }

        for (const auto& op : block)
        {
            bitset_t current_row_copy{ liveness.row(block_idx) };

            if (const auto* set = get_if<regop::set>(&op.op); set != nullptr)
                current_row_copy.at(op.dst) = false;
            else if (const auto* cpy = get_if<regop::copy>(&op.op); cpy != nullptr)
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
            if (const auto* cpy = get_if<regop::copy>(&op.op); cpy != nullptr and op.dst != cpy->src)
            {
                reg_t x{ representative_map.at(op.dst) };
                reg_t y{ representative_map.at(cpy->src) };

                if (x == no_register and y == no_register)
                {
                    if (not overlapping_lifetimes.at(op.dst, cpy->src))
                    {
                        representative_map.at(op.dst) = op.dst;
                        representative_map.at(cpy->src) = op.dst;

                        auto& set = equivalence_classes.at(op.dst);
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
                    if (std::ranges::all_of(equivalence_classes.at(x), [&](reg_t t){ return not overlapping_lifetimes.at(t, cpy->src); }))
                    {
                        representative_map.at(cpy->src) = x;

                        auto& set = equivalence_classes.at(x);
                        auto it = std::ranges::lower_bound(set, cpy->src);
                        if (it == set.end() or *it != cpy->src)
                            set.insert(it, cpy->src);
                    }
                }
                else if (x == no_register and y != no_register)
                {
                    if (std::ranges::all_of(equivalence_classes.at(y), [&](reg_t t){ return not overlapping_lifetimes.at(t, op.dst); }))
                    {
                        representative_map.at(op.dst) = y;

                        auto& set = equivalence_classes.at(y);
                        auto it = std::ranges::lower_bound(set, op.dst);
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

            if (std::ranges::all_of(equivalence_classes.at(j), [&](reg_t t){ return not overlapping_lifetimes.at(i, t); }))
            {
                representative_map.at(i) = j;

                auto& set = equivalence_classes.at(j);
                auto it = std::ranges::lower_bound(set, i);
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
    const auto w = compact_registers(dfa);
    rename_registers(dfa, w);

    for (std::size_t count{ 0 }; count < iterations_; ++count)
    {
        const auto l = liveness(dfa);
        deadcode_elim(dfa, l);
        const auto i = interference(dfa, l);
        const auto v = allocate_registers(dfa, i);
        rename_registers(dfa, v);
        normalise(dfa);
    }
}

/* tdfa minimisation */

template<typename CharT>
class min
{
public:
    using char_type = CharT;
    using tdfa_t = tagged_dfa<CharT>;

    static constexpr void operator()(tdfa_t& dfa);
    static constexpr std::vector<std::vector<std::size_t>> dry_run(const tdfa_t& dfa);

private:
    using partition_t = std::vector<bitset_t>;

    static constexpr partition_t init_hopcroft_partition(const tdfa_t& dfa);
    static constexpr partition_t hopcroft(const tdfa_t& dfa);
};

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
        if (const auto it = dfa.fallback_nodes_.find(state); it != dfa.fallback_nodes_.end())
            key.second = it->second;

        auto [it, _] = final_node_map.try_emplace(std::move(key), bitset_size, false);
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

        for (std::size_t i{ 0 }, i_end{ dfa.nodes_.size() }; i < i_end; ++i)
            for (const auto& tr : dfa.nodes_[i].tr)
                if (transitions_to[tr.next])
                    symbol_pairs_map[tr.op_index].emplace_back(std::cref(tr.cs), i);

        for (auto smit = symbol_pairs_map.begin(), end{ symbol_pairs_map.end() }; smit != end; ++smit)
        {
            for (const auto& states : charset_t<CharT>::partition_contents(smit->second))
            {
                bitset_t transitions_from(bitset_size, false);

                for (const std::size_t s : states)
                    transitions_from[s] = true;

                for (std::size_t p{ 0 }; p < partitions.size(); ++p)
                {
                    bitset_t intersection{ partitions[p] & transitions_from };
                    bitset_t rel_complement{ partitions[p] - transitions_from };

                    const auto i_count = intersection.count();
                    const auto c_count = rel_complement.count();

                    using gt = std::ranges::greater;

                    if (i_count > 0 and c_count > 0)
                    {
                        if (std::ranges::contains(work, partitions[p]))
                        {
                            if (const auto it = std::ranges::lower_bound(work, intersection, gt{}); it == work.end() or *it != intersection)
                                work.emplace(it, intersection);

                            if (const auto it = std::ranges::lower_bound(work, rel_complement, gt{}); it == work.end() or *it != rel_complement)
                                work.emplace(it, rel_complement);
                        }
                        else if (i_count <= c_count)
                        {
                            if (const auto it = std::ranges::lower_bound(work, intersection, gt{}); it == work.end() or *it != intersection)
                                work.emplace(it, intersection);
                        }
                        else
                        {
                            if (const auto it = std::ranges::lower_bound(work, rel_complement, gt{}); it == work.end() or *it != rel_complement)
                                work.emplace(it, rel_complement);
                        }

                        partitions.erase(partitions.begin() + static_cast<std::ptrdiff_t>(p));

                        if (const auto it = std::ranges::lower_bound(partitions, intersection, gt{}); it == partitions.end() or *it != intersection)
                            partitions.emplace(it, std::move(intersection));

                        if (const auto it = std::ranges::lower_bound(partitions, rel_complement, gt{}); it == partitions.end() or *it != rel_complement)
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
    const partition_t partitions{ hopcroft(dfa) };

    /* create map for node remapping */

    std::vector<std::size_t> state_remap(dfa.node_count(), -1);

    for (std::size_t i{ 0 }, i_end{ partitions.size() }; i < i_end; ++i)
    {
        const auto& part = partitions[i];
        for (std::size_t j{ 0 }, j_end{ part.size() }; j < j_end; ++j)
            if (part[j])
                state_remap[j] = i;
    }

    /* remap unneeded nodes */

    typename tdfa_t::data_t new_nodes(partitions.size());
    final_nodes_t new_final_nodes;
    fallback_nodes_t new_fallback_nodes;

    bitset_t new_states_visited(partitions.size(), false);

    for (std::size_t i{ 0 }, i_end{ state_remap.size() }; i < i_end; ++i)
    {
        std::size_t remapped_state{ state_remap[i] };
        if (not new_states_visited.at(remapped_state))
        {
            new_states_visited.at(remapped_state) = true;

            new_nodes.at(remapped_state) = std::move(dfa.nodes_.at(i));

            for (auto& tr : new_nodes[remapped_state].tr)
                tr.next = state_remap.at(tr.next);

            if (const auto it = dfa.final_nodes_.find(i); it != dfa.final_nodes_.end())
                new_final_nodes.try_emplace(remapped_state, it->second);

            if (const auto it = dfa.fallback_nodes_.find(i); it != dfa.fallback_nodes_.end())
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
    for (std::size_t i{ 0 }, i_end{ partitions.size() }; i < i_end; ++i)
        for (std::size_t j{ 0 }, j_end{ partitions[i].size() }; j < j_end; ++j)
            if (partitions[i][j])
                result[i].emplace_back(j);
    return result;
}

template<std::input_iterator I, std::sentinel_for<I> S, typename T>
constexpr std::size_t hash_node(I first, const S last, const std::optional<T>& opt)
{
    std::size_t hash{ hash::init() };
    for (; first != last; ++first)
        hash::append(hash, *first);
    if (opt.has_value())
        hash::append(hash, *opt);
    return hash;
}

}

/* tdfa constructor and member functions */

template<typename CharT>
constexpr tagged_dfa<CharT>::tagged_dfa(const tagged_nfa<char_type>& tnfa)
    : capture_info_{ tnfa.get_capture_info() }, tag_count_{ tnfa.tag_count() }, flags_{ tnfa.get_flags() }
{
    tdfa::factory<char_type>{ tnfa, *this, tag_count_ };
}

template<typename CharT>
constexpr void tagged_dfa<CharT>::optimise_registers()
{
    tdfa::opt<char_type> regoptimise;
    regoptimise(*this);
    compact_regop_blocks();
}

template<typename CharT>
constexpr void tagged_dfa<CharT>::compact_regop_blocks()
{
    std::vector<std::size_t> regop_block_map(regops_.size());
    std::flat_map<tdfa::regops_t, std::size_t> regop_map;
    regop_data_t new_regops;

    for (std::size_t i{ 0 }, i_end{ regops_.size() }; i < i_end; ++i)
    {
        auto [it, inserted] = regop_map.try_emplace(regops_[i], new_regops.size());

        if (inserted)
            new_regops.emplace_back(regops_[i]);

        regop_block_map[i] = it->second;
    }

    /* remap regop block indicies in dfa */

    for (auto& node : nodes_)
        for (auto& tr : node.tr)
            tr.op_index = (tr.op_index < regop_block_map.size()) ? regop_block_map[tr.op_index] : tdfa::no_transition_regops;

    for (auto it = final_nodes_.begin(), last{ final_nodes_.end() }; it != last; ++it)
        it->second.op_index = (it->second.op_index < regop_block_map.size()) ? regop_block_map[it->second.op_index] : tdfa::no_transition_regops;

    for (auto it = fallback_nodes_.begin(), last{ fallback_nodes_.end() }; it != last; ++it)
        it->second.op_index = (it->second.op_index < regop_block_map.size()) ? regop_block_map[it->second.op_index] : tdfa::no_transition_regops;

    regops_ = std::move(new_regops);
}

template<typename CharT>
constexpr void tagged_dfa<CharT>::minimise_states()
{
    tdfa::min<char_type> minimise{};
    minimise(*this);
}

template<typename CharT>
constexpr void tagged_dfa<CharT>::minimise_transition_edges()
{
    /* mutually exclusive with make_default_transitions */

    /* Note: this function relaxes the requirement for a character to appear at most once in any
             transition edge, and requires the transitions to be checked in the provided order */

    /* DO NOT USE THIS WITH A TABLE OR SWITCH BASED MATCHER */

    using tr_type = tdfa::transition<char_type>;

    for (auto& node : nodes_)
    {
        if (node.tr.empty())
            continue;

        const auto sizes = node.tr | std::views::transform([](auto& t){ return t.cs.count(); }) | std::ranges::to<std::vector>();
        const std::size_t largest_index{ static_cast<std::size_t>(std::ranges::max_element(sizes) - sizes.begin()) };

        auto scored_pairs = std::views::zip(std::views::iota(0uz),
                                            node.tr
                                            | std::views::transform([](const auto& t){ return t.cs.score_intervals(); }))
                            | std::views::filter([largest_index](const auto& x){ return get<0>(x) != largest_index; })
                            | std::ranges::to<std::vector>();

        std::ranges::sort(scored_pairs, {}, [](const auto& x){ return get<1>(x); });
        scored_pairs.emplace_back(largest_index, 0 /* unimportant */);

        std::vector<tr_type> new_tr;
        std::vector<tdfa::charset_t<char_type>> dont_cares;
        tdfa::charset_t<char_type> acc;

        for (const auto& [i, _] : scored_pairs)
        {
            auto& tr = node.tr.at(i);
            dont_cares.emplace_back(acc);
            acc |= tr.cs;
            new_tr.emplace_back(std::move(tr));
        }

        if (acc.full())
        {
            const auto& largest = new_tr.back();
            node.default_tr = { .next = largest.next, .op_index = largest.op_index };
            new_tr.pop_back();
        }

        /* fill gaps where possible */

        for (const auto& [tr_ref, dont_cares] : std::views::zip(std::ranges::ref_view(new_tr), dont_cares))
        {
            tr_type& tr{ tr_ref };

            using interval_t = tdfa::charset_t<char_type>::char_interval;
            std::vector<interval_t> to_insert;

            std::ranges::set_intersection((~tr.cs).get_intervals(), dont_cares.get_intervals(), std::back_inserter(to_insert));

            for (const auto& [beg, end] : to_insert)
            {
                if (beg == end)
                    tr.cs.insert(beg);
                else
                    tr.cs.insert(beg, end);
            }
        }

        node.tr = std::move(new_tr);
    }
}

template<typename CharT>
constexpr void tagged_dfa<CharT>::make_default_transitions()
{
    /* mutually exclusive with minimise_transition_edges */

    using tr_type = tdfa::transition<char_type>;

    for (auto& node : nodes_)
    {
        if (node.tr.empty())
            continue;

        const auto sizes = node.tr | std::views::transform([](auto& t){ return t.cs.count(); }) | std::ranges::to<std::vector>();
        const auto largest_index = static_cast<std::size_t>(std::ranges::max_element(sizes) - sizes.begin());

        auto& largest = node.tr[largest_index];
        tdfa::charset_t<char_type> largest_cs{ largest.cs };
        std::vector<tr_type> new_tr;

        for (std::size_t i{ 0 }, i_end{ node.tr.size() }; i < i_end; ++i)
        {
            if (i == largest_index)
                continue;

            largest_cs |= node.tr[i].cs;
            new_tr.emplace_back(std::move(node.tr[i]));
        }

        if (largest_cs.full())
            node.default_tr = { .next = largest.next, .op_index = largest.op_index };
        else
            new_tr.emplace_back(std::move(largest));

        node.tr = std::move(new_tr);
    }
}

template<typename CharT>
constexpr void tagged_dfa<CharT>::make_shared_transitions()
{
    using node_type = tdfa::node<char_type>;

    auto keys = nodes_
                | std::views::transform([](const node_type& n){
                      return tdfa::hash_node(n.tr.begin(), n.tr.end(), n.default_tr);
                  })
                | std::ranges::to<std::vector>();

    auto values = std::views::iota(0uz, nodes_.size())
                  | std::ranges::to<std::vector>();

    static constexpr auto key_proj = [](const auto& v) -> decltype(auto) { return get<0>(v); };
    std::ranges::sort(std::views::zip(keys, values), {}, key_proj);

    data_t new_nodes{};
    new_nodes.reserve(nodes_.size());

    for (std::size_t current_index{ 0 }, node_count{ nodes_.size() }; current_index < node_count; ++current_index)
    {
        const auto& current = nodes_[current_index];

        const auto beg = current.tr.begin();
        const auto end = current.tr.end();

        bool inserted{ false };

        const auto zv = std::views::zip(keys, values);

        for (auto it = beg; it != end; ++it)
        {
            const std::size_t hash{ tdfa::hash_node(it, end, current.default_tr) };

            for (auto [fst, snd] = std::ranges::equal_range(zv, hash, {}, key_proj); fst != snd; ++fst)
            {
                auto [_, index] = *fst;

                if (index == current_index)
                    break; /* prevent replacement with self */

                if (const auto& other = nodes_.at(index);
                    not (std::ranges::equal(it, end, other.tr.begin(), other.tr.end())
                    and current.default_tr == other.default_tr))
                {
                    continue;
                }

                new_nodes.emplace_back(
                    std::vector<tdfa::transition<char_type>>(beg, it),
                    tdfa::default_transition{ .next = index, .op_index = tdfa::default_transition_is_not_state }
                );

                inserted = true;
                break;
            }

            if (inserted)
                break;
        }

        if (inserted)
            continue;

        new_nodes.emplace_back(current);
    }

    nodes_ = std::move(new_nodes);
}

template<typename CharT>
constexpr void tagged_dfa<CharT>::de_default_edges()
{
    for (auto& node : nodes_)
    {
        if (node.default_tr.has_value())
        {
            node.tr.emplace_back(node.default_tr->next, node.default_tr->op_index, ~(tdfa::charset_t<char_type>{}));
            node.default_tr.reset();
        }
    }
}

}
}

namespace srx {
namespace detail {

struct final_capture_info
{
    explicit consteval final_capture_info(const capture_info& ci)
        : captures{ ci.get_values_branchfree() } {}

    consteval final_capture_info() = default;

    [[nodiscard]] consteval std::size_t capture_count() const noexcept
    {
        return captures.size();
    }

    [[nodiscard]] consteval bool has_match_start() const
    {
        static constexpr auto pred = [](const capture_info::tag_pair_t& pair){
            const auto& [fst, snd] = pair;
            return (fst.tag_number == start_of_input_tag or snd.tag_number == start_of_input_tag);
        };

        return std::ranges::any_of(captures, pred);
    }

    /* data members (public so that final_capture_info is structural) */
    static_span<capture_info::tag_pair_t> captures;
};

struct static_match_result_info
{
    final_capture_info fci;
    static_span<tdfa::reg_t> final_registers;
    std::size_t register_count{ 0 };
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
    static_charset<CharT> cs;
};

template<typename CharT>
struct tdfa_info
{
    using char_type = CharT;

private:
    static consteval auto make_static_transition(const tdfa::transition<char_type>& tr)
    {
        return static_transition{ tr.next, tr.op_index, static_charset{ tr.cs } };
    }

    static consteval auto make_static_outer_transition(const std::pair<std::size_t, tnfa::charset_t<char_type>>& otr)
    {
        return static_transition{ otr.first, tdfa::no_transition_regops, static_charset{ otr.second } };
    }

    static consteval auto make_node_transitions(const tdfa::node<char_type>& n)
    {
        return static_span{ n.tr | std::views::transform(make_static_transition) };
    }

    static consteval auto make_register_operations(const tdfa::regops_t& o)
    {
        return static_span{ o | std::views::transform(
            [](const tdfa::regop& op) consteval -> register_operation {
                if (const auto* set = get_if<tdfa::regop::set>(&op.op); set != nullptr)
                    return { .dst = op.dst, .cpy_src = 0, .set_val = set->val, .is_copy = false };
                else if (const auto* cpy = get_if<tdfa::regop::copy>(&op.op); cpy != nullptr)
                    return { .dst = op.dst, .cpy_src = cpy->src, .set_val = false, .is_copy = true };
                else
                    std::unreachable();
            })
        };
    }

    static consteval auto make_continue_info(const tagged_dfa<char_type>& dfa, const tagged_nfa<char_type>& nfa)
    {
        /* adapted from tagged_dfa::minimise_transition_edges */
        using tr_type = std::pair<std::size_t, tdfa::charset_t<char_type>>;

        std::vector<tr_type> vec;

        for (const auto& [nfa_cont, dfa_cont] : std::views::zip(nfa.get_cont_info(), dfa.continue_nodes()))
            vec.emplace_back(dfa_cont, std::cref(nfa_cont.cs));

        if (not vec.empty())
        {
            auto scored_pairs = std::views::zip(std::views::iota(0uz),
                                                vec | std::views::transform([](const auto& t){ return t.second.score_intervals(); }))
                                                    | std::ranges::to<std::vector>();

            std::ranges::sort(scored_pairs, {}, [](const auto& x){ return get<1>(x); });

            std::vector<tr_type> new_tr;
            std::vector<tdfa::charset_t<char_type>> dont_cares;
            tdfa::charset_t<char_type> acc;

            for (const auto& [i, _] : scored_pairs)
            {
                auto& tr = vec.at(i);
                dont_cares.emplace_back(acc);
                acc |= tr.second;
                new_tr.emplace_back(std::move(tr));
            }

            if (acc.full())
            {
                new_tr.back().second = std::move(acc);
                dont_cares.back() = tdfa::charset_t<char_type>{};
            }

            /* fill gaps where possible */

            for (const auto& [tr_ref, dont_cares] : std::views::zip(std::ranges::ref_view(new_tr), dont_cares))
            {
                tr_type& tr{ tr_ref };

                if (dont_cares.empty())
                    continue;

                using interval_t = tdfa::charset_t<char_type>::char_interval;
                std::vector<interval_t> to_insert;

                std::ranges::set_intersection((~tr.second).get_intervals(), dont_cares.get_intervals(), std::back_inserter(to_insert));

                for (const auto [beg, end] : to_insert)
                {
                    if (beg == end)
                        tr.second.insert(beg);
                    else
                        tr.second.insert(beg, end);
                }
            }

            vec = std::move(new_tr);
        }

        return static_span{ vec | std::views::transform(make_static_outer_transition) };
    }

public:
    explicit consteval tdfa_info(const tagged_dfa<char_type>& dfa, const tagged_nfa<char_type>& nfa,
                                 const std::pair<std::size_t, std::size_t>& mml, fsm_flags f, bool alt_mode)
        : nodes{ dfa.nodes_ | std::views::transform(make_node_transitions) }
        , regops{ dfa.regops_ | std::views::transform(make_register_operations) }
        , continue_nodes{ dfa.continue_nodes() }
        , additional_continue_nodes{ dfa.additional_continue_nodes() }
        , final_nodes{ dfa.final_nodes() }
        , fallback_nodes{ dfa.fallback_nodes() }
        , final_registers{ dfa.final_registers() }
        , register_count{ dfa.reg_count() }
        , match_start{ dfa.match_start }
        , captures{ dfa.get_capture_info() }
        , outer_transitions{ make_continue_info(dfa, nfa) }
        , min_max_lengths{ mml }
        , flags{ f }
        , alt_mode{ alt_mode }{}

    [[nodiscard]] consteval static_match_result_info make_match_result_info() const
    {
        return { .fci = captures, .final_registers = final_registers, .register_count = register_count };
    }

    [[nodiscard]] consteval bool has_continue() const
    {
        return flags.is_iterator and not (continue_nodes.size() == 1 and continue_nodes[0] == match_start);
    }

    /* data members (public so that tdfa_info is structural) */
    static_span<static_span<static_transition<char_type>>> nodes;
    static_span<static_span<register_operation>> regops;
    static_span<std::size_t> continue_nodes;
    static_span<std::size_t> additional_continue_nodes;
    static_map<std::size_t, tdfa::final_node_info> final_nodes;
    static_map<std::size_t, tdfa::fallback_node_info> fallback_nodes;
    static_span<tdfa::reg_t> final_registers;

    std::size_t register_count{ 0 };
    std::size_t match_start{ 0 };
    final_capture_info captures;

    static_span<static_transition<char_type>> outer_transitions;
    std::pair<std::size_t, std::size_t> min_max_lengths;

    fsm_flags flags;
    bool alt_mode;
};

template<typename CharT>
struct lexer_info
{
    using char_type = CharT;

    lexer_info() = delete;

    template<std::ranges::range R>
        requires std::same_as<std::ranges::range_value_t<R>, std::meta::info>
    consteval lexer_info(tdfa_info<CharT>&& dfa, R&& actions, std::meta::info eof_act, std::meta::info err_act, std::meta::info ret_ty)
        : dfa{ std::move(dfa) }
        , actions{ std::forward<R>(actions) }
        , eof_action{ eof_act }
        , error_action{ err_act }
        , return_type{ ret_ty } {}

    /* data members (public so that tdfa_info is structural) */
    tdfa_info<CharT> dfa;
    static_span<std::meta::info> actions;
    std::meta::info eof_action;
    std::meta::info error_action;
    std::meta::info return_type;
};

template<typename CharT>
consteval tdfa_info<CharT> compile_pattern(std::basic_string_view<CharT> pattern, fsm_flags f)
{
    /* set parser flags as appropriate */
    parser_flags p{};
    if (f.no_captures)
        p.enable_captures = false;
    if (f.return_bool)
        p.enable_start_tag = false;

    /* parse pattern string into tree */
    expr_tree ast{ pattern, p };
    if (f.is_search)
        ast.insert_search_prefix();
    ast.optimise_tags();
    const auto mml = ast.min_max_length();

    /* convert to tnfa */
    tagged_nfa nfa{ ast, f };
    nfa.rewrite_assertions();
    if (f.maybe_no_empty and mml.first == 0)
        nfa.add_non_empty_match_pathway();

    /* convert to tdfa */
    tagged_dfa dfa{ nfa };
    dfa.optimise_registers();

    /* optimise transition edges and their order to produce fewest comparisons */
    /* (if using tables, do `dfa.make_default_tr_if_possible()` instead) */
    dfa.minimise_transition_edges();
    dfa.de_default_edges();

    return tdfa_info{ dfa, nfa, mml, f, ast.is_alt_mode() };
}

template<std::meta::info P, ff F>
inline constexpr auto re = compile_pattern(std::basic_string_view{ [: P :] }, unpack_flags(F));

struct match_non_empty_t {};
inline constexpr match_non_empty_t match_non_empty;

}
}

namespace srx {
namespace detail {

template<std::bidirectional_iterator Iter>
class submatch_factory;

}

template<std::bidirectional_iterator I>
class submatch
{
public:
    using iterator               = I;
    using reverse_iterator       = std::reverse_iterator<iterator>;
    using const_iterator         = std::const_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;
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
        return static_cast<size_type>(std::ranges::distance(first_, last_));
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

    /* structured binding support */

    template<std::size_t N>
        requires (N < 2)
    [[nodiscard]] friend constexpr const auto& get(const submatch& s) noexcept
    {
        if constexpr (N == 0)
            return s.first_;
        if constexpr (N == 1)
            return s.last_;
    }

    template<std::size_t N>
        requires (N < 2)
    [[nodiscard]] friend constexpr auto&& get(submatch&& s) noexcept
    {
        if constexpr (N == 0)
            return std::move(s).first_;
        if constexpr (N == 1)
            return std::move(s).last_;
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

    friend constexpr void swap(submatch& x, submatch& y) noexcept(std::is_nothrow_swappable_v<I>)
    {
        std::ranges::swap(x.first_, y.first_);
        std::ranges::swap(x.last_, y.last_);
    }

    template<std::bidirectional_iterator OtherI>
    friend class submatch;

private:
    static constexpr bool use_bool{ not std::contiguous_iterator<I> };
    using maybe_bool = detail::maybe_type_t<use_bool, bool>;

    constexpr submatch(I first, I last) noexcept(std::is_nothrow_move_constructible_v<I>)
        : first_{ std::move(first) }, last_{ std::move(last) }, match_{ true } {}

    friend class detail::submatch_factory<I>;

    I first_{};
    I last_{};

    [[no_unique_address]] maybe_bool match_{ false };
};

}

/* structured binding support for submatch */

template<std::bidirectional_iterator I>
struct std::tuple_size<srx::submatch<I>> : integral_constant<std::size_t, 2> {};

template<std::size_t N, std::bidirectional_iterator I>
    requires (N < 2)
struct std::tuple_element<N, srx::submatch<I>>
{
    using type = srx::submatch<I>::iterator;
};

/* formatting support for submatch */

template<std::bidirectional_iterator I>
inline constexpr auto std::format_kind<srx::submatch<I>> = std::range_format::string;

static_assert(std::formattable<srx::submatch<const char*>, char>);

/* range correctness for submatch */

template<std::bidirectional_iterator I>
constexpr bool std::ranges::disable_sized_range<srx::submatch<I>> = not std::sized_sentinel_for<I, I>;

/* submatch factory implementation */

namespace srx {
namespace detail {

template<std::bidirectional_iterator I>
class submatch_factory
{
public:
    [[nodiscard]] static constexpr submatch<I> make_submatch(I first, I last)
        noexcept(std::is_nothrow_move_constructible_v<I>)
    {
        return { first, last };
    }

    template<std::sentinel_for<I> S>
        requires (not std::same_as<I, S>)
    [[nodiscard]] static constexpr submatch<I> make_submatch(I first, S last)
    {
        return { first, std::ranges::next(first, last) };
    }
};

}
}

namespace srx {
namespace detail {

template<std::meta::info Info>
struct p1306dfa;

template<srx::string_literal Pattern>
struct naive_matcher;

}

template<std::bidirectional_iterator I, srx::detail::static_match_result_info Captures>
class static_match_results
{
    using factory = detail::submatch_factory<I>;

    template<bool Const>
    class proxy_iterator;

public:
    using size_type              = std::size_t;
    using char_type              = std::remove_cv_t<std::iter_value_t<I>>;
    using submatch_type          = submatch<I>;
    using iterator               = proxy_iterator<std::same_as<I, std::const_iterator<I>>>;
    using reverse_iterator       = std::reverse_iterator<iterator>;
    using const_iterator         = proxy_iterator<true>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    static constexpr size_type submatch_count{ Captures.fci.capture_count() };

    constexpr static_match_results() noexcept(std::is_nothrow_default_constructible_v<I>)
    {
        if constexpr (has_registers and not has_enabled)
            reg_.fill(I{});
    }

    /* observers */

    [[nodiscard]] constexpr bool has_value() const noexcept
    {
        if constexpr (std::contiguous_iterator<I>)
            return std::to_address(match_end_) != std::to_address(I{});
        else
            return match_success_;
    }

    [[nodiscard]] constexpr explicit(false) operator bool() const noexcept
    {
        return this->has_value();
    }

    [[nodiscard]] constexpr size_type size() const noexcept
    {
        return (this->has_value()) ? submatch_count : 0;
    }

    /* array-like access */

    [[nodiscard]] constexpr submatch_type operator[](size_type n) const noexcept
    {
        template for (constexpr size_type N : std::views::iota(0uz, submatch_count))
            if (n == N)
                return get<N>(*this);
        std::unreachable();
    }

    [[nodiscard]] constexpr submatch_type at(size_type i) const
    {
        this->range_check(i);
        return this->operator[](i);
    }

    /* iterator support */

    [[nodiscard]] constexpr iterator begin() const noexcept
    {
        return { this, 0 };
    }

    [[nodiscard]] constexpr iterator end() const noexcept
    {
        return { this, this->size() };
    }

    [[nodiscard]] constexpr reverse_iterator rbegin() const noexcept
    {
        return std::make_reverse_iterator(this->end());
    }

    [[nodiscard]] constexpr reverse_iterator rend() const noexcept
    {
        return std::make_reverse_iterator(this->begin());
    }

    [[nodiscard]] constexpr const_iterator cbegin() const noexcept
    {
        return this->begin();
    }

    [[nodiscard]] constexpr const_iterator cend() const noexcept
    {
        return this->end();
    }

    [[nodiscard]] constexpr const_reverse_iterator crbegin() const noexcept
    {
        return std::make_reverse_iterator(this->cend());
    }

    [[nodiscard]] constexpr const_reverse_iterator crend() const noexcept
    {
        return std::make_reverse_iterator(this->cbegin());
    }

    /* tuple support */

    template<size_type N>
        requires (N < submatch_count)
    [[nodiscard]] friend constexpr submatch_type get(const static_match_results& r) noexcept
    {
        if (r.has_value())
            return r.template force_get<N>();
        return {};
    }

    template<std::meta::info Info>
    friend struct detail::p1306dfa;

    template<srx::string_literal Pattern>
    friend struct detail::naive_matcher;

private:
    /* implementation helpers */

    static constexpr bool has_registers{ Captures.register_count != 0 };
    static constexpr bool has_success{ not std::contiguous_iterator<I> };
    static constexpr bool has_enabled{ has_registers and has_success };
    static constexpr bool has_match_start{ Captures.fci.has_match_start() };

    constexpr explicit static_match_results(I start)
        noexcept(std::is_nothrow_default_constructible_v<I> and std::is_nothrow_move_constructible_v<I>)
        : match_start_{ std::move(start) }
    {
        if constexpr (has_registers and not has_enabled)
            reg_.fill(I{});
    }

    constexpr void clear_match() noexcept(std::is_nothrow_default_constructible_v<I>)
    {
        if constexpr (std::contiguous_iterator<I>)
            match_end_ = I{};
        else
            match_success_ = false;
    }

    constexpr void reset(I new_start)
    {
        clear_match();
        if constexpr (has_match_start)
            match_start_ = std::move(new_start);
        if constexpr (has_registers and not has_enabled)
            reg_.fill(I{});
        if constexpr (has_enabled)
            enabled_.fill(false);
    }

    template<detail::tag_number_t N>
    [[nodiscard]] constexpr bool tag_enabled() const noexcept
    {
        if constexpr (N == detail::start_of_input_tag or N == detail::end_of_input_tag)
            return true;
        else if constexpr (std::contiguous_iterator<I>)
            return std::to_address(reg_[Captures.final_registers[N]]) != std::to_address(I{});
        else
            return enabled_[Captures.final_registers[N]];
    }

    template<detail::tag_number_t N>
    [[nodiscard]] constexpr I get_tag() const noexcept
    {
        if constexpr (N == detail::start_of_input_tag)
            return match_start_;
        else if constexpr (N == detail::end_of_input_tag)
            return match_end_;
        else
            return reg_[Captures.final_registers[N]];
    }

    template<size_type N>
        requires (N < submatch_count)
    [[nodiscard]] constexpr submatch_type force_get() const noexcept
    {
        static constexpr auto current = Captures.fci.captures[N];

        if constexpr (current.first.tag_number == current.second.tag_number)
        {
            if (tag_enabled<current.first.tag_number>())
            {
                return factory::make_submatch(
                    std::ranges::next(get_tag<current.first.tag_number>(), current.first.offset),
                    std::ranges::next(get_tag<current.second.tag_number>(), current.second.offset)
                );
            }
        }
        else
        {
            if (tag_enabled<current.first.tag_number>() and tag_enabled<current.second.tag_number>())
            {
                return factory::make_submatch(
                    std::ranges::next(get_tag<current.first.tag_number>(), current.first.offset),
                    std::ranges::next(get_tag<current.second.tag_number>(), current.second.offset)
                );
            }
        }

        return {};
    }

    constexpr void range_check(size_type n) const
    {
        if (n >= this->size())
            throw std::out_of_range("static_match_results::range_check: n >= this->size()");
    }

    /* data members and protected trivial accessors */

    using registers_type   = detail::maybe_type_t<has_registers, std::array<I, Captures.register_count>>;
    using enabled_type     = detail::maybe_type_t<has_enabled, std::array<bool, Captures.register_count>>;
    using match_start_type = detail::maybe_type_t<has_match_start, I>;
    using success_type     = detail::maybe_type_t<has_success, bool>;

    [[no_unique_address]] registers_type reg_;
    [[no_unique_address]] match_start_type match_start_{};
    I match_end_{};
    [[no_unique_address]] enabled_type enabled_{};
    [[no_unique_address]] success_type match_success_{ false };
};

/* iterator implementation */

template<std::bidirectional_iterator I, srx::detail::static_match_result_info Captures>
template<bool Const>
class static_match_results<I, Captures>::proxy_iterator
{
public:
    using iterator_concept  = std::random_access_iterator_tag;
    using iterator_category = std::input_iterator_tag;
    using value_type        = submatch<std::conditional_t<Const, std::const_iterator<I>, I>>;
    using difference_type   = std::ptrdiff_t;

    proxy_iterator() = default;

    constexpr proxy_iterator(const static_match_results* ptr, size_type pos) noexcept
        : ptr_{ ptr }, pos_{ pos } {}

    constexpr explicit(false) proxy_iterator(proxy_iterator<not Const> i) noexcept requires Const
        : ptr_{ i.ptr_ }, pos_{ i.pos_ } {}

    constexpr value_type operator*() const noexcept
    {
        return (*ptr_)[pos_];
    }

    constexpr value_type operator[](difference_type n) const noexcept
    {
        return (*ptr_)[pos_ + n];
    }

    constexpr proxy_iterator& operator++() noexcept
    {
        ++pos_;
        return *this;
    }

    constexpr proxy_iterator operator++(int) noexcept
    {
        auto tmp = *this;
        ++*this;
        return tmp;
    }

    constexpr proxy_iterator& operator--() noexcept
    {
        --pos_;
        return *this;
    }

    constexpr proxy_iterator operator--(int) noexcept
    {
        auto tmp = *this;
        --*this;
        return tmp;
    }

    constexpr proxy_iterator& operator+=(difference_type n) noexcept
    {
        pos_ += n;
        return *this;
    }

    constexpr proxy_iterator& operator-=(difference_type n) noexcept
    {
        pos_ -= n;
        return *this;
    }

    friend constexpr bool operator==(const proxy_iterator&, const proxy_iterator&) = default;

    friend constexpr auto operator<=>(const proxy_iterator&, const proxy_iterator&) = default;

    friend constexpr proxy_iterator operator+(const proxy_iterator& i, difference_type n) noexcept
    {
        return { i.ptr_, i.pos_ + n };
    }

    friend constexpr proxy_iterator operator+(difference_type n, const proxy_iterator& i) noexcept
    {
        return { i.ptr_, n + i.pos_ };
    }

    friend constexpr proxy_iterator operator-(const proxy_iterator& i, difference_type n) noexcept
    {
        return { i.ptr_, i.pos_ - n };
    }

    friend constexpr difference_type operator-(const proxy_iterator& x, const proxy_iterator& y) noexcept
    {
        return x.pos_ - y.pos_;
    }

    friend class proxy_iterator<not Const>;

private:
    const static_match_results* ptr_{ nullptr };
    size_type pos_{ 0 };
};

}

/* structured binding support for static_match_results */

template<std::bidirectional_iterator Iter, srx::detail::static_match_result_info Captures>
struct std::tuple_size<srx::static_match_results<Iter, Captures>>
    : integral_constant<std::size_t, srx::static_match_results<Iter, Captures>::submatch_count> {};

template<std::size_t N, std::bidirectional_iterator Iter, srx::detail::static_match_result_info Captures>
    requires (N < srx::static_match_results<Iter, Captures>::submatch_count)
struct std::tuple_element<N, srx::static_match_results<Iter, Captures>>
{
    using type = srx::static_match_results<Iter, Captures>::submatch_type;
};

namespace srx {
namespace detail {

template<typename CharT>
struct tree_info
{
    using old_type = expr_tree<CharT>::type;

    struct transition
    {
        static constexpr bool is_narrow{ std::same_as<CharT, char> };
        using underlying_char_type = std::conditional_t<is_narrow, char, char32_t>;

        consteval explicit(false) transition(const char_class_impl<is_narrow>& cci)
            : cs{ cci.intervals() } {}

        static_span<std::pair<underlying_char_type, underlying_char_type>> cs;
    };

    template<typename U>
    struct nttp_of;

    consteval
    {
        for (const auto expr_type : template_arguments_of(dealias(^^old_type)))
        {
            if (is_aggregate_type(expr_type) and is_trivially_copyable_type(expr_type))
                continue;

            auto spec = nonstatic_data_members_of(expr_type, std::meta::access_context::current());

            for (auto& info : spec)
            {
                std::meta::data_member_options options{ .name = identifier_of(info) };
                const auto type = type_of(info);

                if (has_template_arguments(type) and (template_of(type) == ^^std::vector or template_of(type) == ^^std::basic_string))
                    info = data_member_spec(substitute(^^static_span, { template_arguments_of(type)[0] }), options);
                else if (has_template_arguments(type) and template_of(type) == ^^char_class_impl)
                    info = data_member_spec(^^transition, options);
                else if (is_integral_type(type) or is_enum_type(type) or (is_aggregate_type(type) and is_trivially_copyable_type(type)))
                    info = data_member_spec(type, options);
                else
                    throw regex_error("unsupported variant type ");
            }

            define_aggregate(substitute(^^nttp_of, { dealias(expr_type) }), spec);
        }
    }

    struct type
    {
        struct empty {};

        union value_t;

        consteval
        {
            auto mems = template_arguments_of(dealias(^^old_type));
            for (auto& member : mems)
            {
                std::string_view name;

                if (has_template_arguments(dealias(member)))
                    name = identifier_of(template_of(dealias(member)));
                else
                    name = identifier_of(member);

                member = dealias(member);

                if (not (is_aggregate_type(member) and is_trivially_copyable_type(member)))
                    member = substitute(^^nttp_of, { member });

                member = data_member_spec(member, { .name = name });
            }
            define_aggregate(^^value_t, mems);
        }

        static constexpr auto nsdms = define_static_array(nonstatic_data_members_of(^^value_t, std::meta::access_context::unchecked()));

        using index_t = unsigned char;

        value_t value;
        index_t index;
    };

private:
    static consteval type to_static_expression(const old_type& expr)
    {
        constexpr auto original_type = dealias(^^old_type);

        /* splicing designated initializers is unsupported in C++26; if it was, we could do something like this: */

        /* we do this instead as a temporary workaround */

        using index_t = type::index_t;

        return expr.visit(overloads{
            [](const typename expr_tree<CharT>::assertion& e) -> type
            {
                const auto& [...mems] = e;
                return { .value{ .assertion{ mems... } }, .index = std::saturating_cast<index_t>(index_in_variant(^^typename expr_tree<CharT>::assertion, original_type)) };
            },
            [](const typename expr_tree<CharT>::char_str& e) -> type
            {
                const auto& [...mems] = e;
                return { .value{ .char_str{ mems... } }, .index = std::saturating_cast<index_t>(index_in_variant(^^typename expr_tree<CharT>::char_str, original_type)) };
            },
            [](const typename expr_tree<CharT>::char_class& e) -> type
            {
                const auto& [...mems] = e;
                return { .value{ .char_class{ mems... } }, .index = std::saturating_cast<index_t>(index_in_variant(^^typename expr_tree<CharT>::char_class, original_type)) };
            },
            [](const typename expr_tree<CharT>::backref& e) -> type
            {
                const auto& [...mems] = e;
                return { .value{ .backref{ mems... } }, .index = std::saturating_cast<index_t>(index_in_variant(^^typename expr_tree<CharT>::backref, original_type)) };
            },
            [](const typename expr_tree<CharT>::alt& e) -> type
            {
                const auto& [...mems] = e;
                return { .value{ .alt{ mems... } }, .index = std::saturating_cast<index_t>(index_in_variant(^^typename expr_tree<CharT>::alt, original_type)) };
            },
            [](const typename expr_tree<CharT>::concat& e) -> type
            {
                const auto& [...mems] = e;
                return { .value{ .concat{ mems... } }, .index = std::saturating_cast<index_t>(index_in_variant(^^typename expr_tree<CharT>::concat, original_type)) };
            },
            [](const typename expr_tree<CharT>::tag& e) -> type
            {
                const auto& [...mems] = e;
                return { .value{ .tag{  mems... } }, .index = std::saturating_cast<index_t>(index_in_variant(^^typename expr_tree<CharT>::tag, original_type)) };
            },
            [](const typename expr_tree<CharT>::repeat& e) -> type
            {
                const auto& [...mems] = e;
                return { .value{ .repeat{ mems... } }, .index = std::saturating_cast<index_t>(index_in_variant(^^typename expr_tree<CharT>::repeat, original_type)) };
            }
        });
    }

public:
    consteval tree_info(const expr_tree<CharT>& ast, const capture_info::staging_info_t& tag_info)
        : root_idx{ ast.root_idx() }
        , tag_count{ ast.tag_count() }
        , exprs{ ast.get_all_exprs() | std::views::transform(to_static_expression) }
        , staging{ tag_info }
        , fci{ ast.get_capture_info() }
        , empty_match_possible{ ast.empty_match_possible() } {}

    [[nodiscard]] consteval static_match_result_info make_match_result_info() const
    {
        static_span regs{ std::views::iota(0u, static_cast<tdfa::reg_t>(tag_count)) };
        return { .fci = fci, .final_registers = regs, .register_count = tag_count };
    }

    std::size_t root_idx;
    std::size_t tag_count;
    static_span<type> exprs;
    static_map<tag_number_t, tag_number_t> staging;

    final_capture_info fci;

    bool empty_match_possible;
};

template<typename CharT>
consteval tree_info<CharT> parse_pattern(std::basic_string_view<CharT> pattern)
{
    /* set parser flags as appropriate */
    parser_flags p{
        .enable_possessive = true,
        .enable_backrefs = true,
        .enable_branchreset = true
    };

    /* parse pattern string into tree */
    expr_tree ast{ pattern, p };
    ast.simplify_counted_repeat();
    ast.optimise_exact_repeat();
    auto tag_info = ast.tag_to_register();

    return tree_info{ ast, tag_info };
}

template<static_span Intervals, typename CharT>
[[gnu::always_inline]] constexpr bool cs_possible(CharT c)
{
    template for (constexpr auto pair : Intervals)
        if (pair.first <= c and c <= pair.second)
            return true;

    return false;
}

template<string_literal Pattern>
struct naive_matcher
{
    using char_type = decltype(Pattern)::value_type;

private:
    using ast_t = expr_tree<char_type>;
    using info_t = tree_info<char_type>;

    template<typename T>
    static constexpr std::size_t ast_index{ index_in_variant(^^T, ^^typename expr_tree<char_type>::type) };

    static constexpr info_t ast{ parse_pattern(Pattern.view()) };

    static constexpr std::size_t require_full_match{ std::numeric_limits<std::size_t>::max() };
    static constexpr std::size_t require_non_empty_match{ std::numeric_limits<std::size_t>::max() - 1 };

    template<typename I>
    using result = static_match_results<I, ast.make_match_result_info()>;

    template<typename I>
    struct result_helper
    {
        std::array<I, ast.staging.size()> reg;
    };

    template<typename I>
    using staging_info = maybe_type_t<not ast.staging.empty(), result_helper<I>>;

    static consteval std::size_t get_staging_index(tag_number_t tagnum)
    {
        return static_cast<std::size_t>(std::ranges::distance(ast.staging.begin(), ast.staging.find(tagnum)));
    }

    template<typename I>
    [[gnu::always_inline]] static constexpr void apply_final_staging_info(result<I>& res, staging_info<I>& si)
    {
        if constexpr (not ast.staging.empty())
        {
            static constexpr auto staged = [] consteval {
                std::vector<tag_number_t> vec;
                for (const auto& [k, v] : ast.staging)
                    if (v == end_of_input_tag)
                        vec.emplace_back(k);
                std::ranges::sort(vec);
                auto [it, _] = std::ranges::unique(vec);
                vec.erase(it, vec.end());
                return define_static_array(vec);
            }();

            template for (constexpr auto tagnum : staged)
                res.reg_[tagnum] = si.reg[get_staging_index(tagnum)];

            if constexpr (not std::contiguous_iterator<I>)
                template for (constexpr auto tagnum : staged)
                    res.enabled_[tagnum] = true;
        }
    }

    template<bool Fwd, std::size_t... Ints>
    struct state {};

    template<bool Fwd>
    struct state<Fwd>
    {
        template<std::bidirectional_iterator I, std::sentinel_for<I> S>
        [[gnu::always_inline]] static constexpr bool operator()(result<I>& /* res */, staging_info<I>& /* si */, const I /* first */, const S /* last */, I& /* it */)
        {
            return true;
        }
    };

    template<bool Fwd>
    struct state<Fwd, require_full_match>
    {
        template<std::bidirectional_iterator I, std::sentinel_for<I> S>
        [[gnu::always_inline]] static constexpr bool operator()(result<I>& /* res */, staging_info<I>& /* si */, const I /* first */, const S last, I& it)
        {
            return (it == last);
        }
    };

    template<bool Fwd>
    struct state<Fwd, require_non_empty_match>
    {
        template<std::bidirectional_iterator I, std::sentinel_for<I> S>
        [[gnu::always_inline]] static constexpr bool operator()(result<I>& res , staging_info<I>& /* si */, const I /* first */, const S /* last */, I& it)
        {
            return (it != res.match_start_);
        }
    };

    template<bool Fwd, std::size_t Expr, std::size_t... Cont>
        requires (Expr < ast.exprs.size() and ast.exprs[Expr].index == ast_index<typename ast_t::char_str>)
    struct state<Fwd, Expr, Cont...>
    {
        static constexpr auto str = ast.exprs[Expr].value.[: info_t::type::nsdms[ast_index<typename ast_t::char_str>] :];

        template<std::bidirectional_iterator I, std::sentinel_for<I> S>
        [[gnu::always_inline]] static constexpr bool operator()(result<I>& res, staging_info<I>& si, const I first, const S last, I& it)
        {
            if constexpr (Fwd)
            {
                if constexpr (std::random_access_iterator<I> and std::sized_sentinel_for<S, I>)
                {
                    if (static_cast<std::size_t>(std::ranges::distance(it, last)) < str.data.size())
                        return false;

                    int mismatch{ false };  /* encourage vectorisation */
                    for (std::size_t i{ 0 }, i_end{ str.data.size() }; i < i_end; ++i)
                        mismatch |= (it[i] != str.data[i]);

                    if (mismatch)
                        return false;

                    std::ranges::advance(it, static_cast<std::ptrdiff_t>(str.data.size()));
                }
                else
                {
                    template for (constexpr char_type c : str.data)
                    {
                        if (it == last or *it != c)
                            return false;

                        ++it;
                    }
                }
            }
            else
            {
                if constexpr (std::random_access_iterator<I> and std::sized_sentinel_for<S, I>)
                {
                    if (static_cast<std::size_t>(std::ranges::distance(first, it)) < str.data.size())
                        return false;

                    std::ranges::advance(it, -static_cast<std::ptrdiff_t>(str.data.size()));

                    int mismatch{ false };  /* encourage vectorisation */
                    for (std::size_t i{ 0 }, i_end{ str.data.size() }; i < i_end; ++i)
                        mismatch |= (it[i] != str.data[i]);

                    if (mismatch)
                        return false;
                }
                else
                {
                    template for (constexpr char_type c : str.data | std::views::reverse)
                    {
                        if (it == first)
                            return false;

                        --it;

                        if (*it != c)
                            return false;
                    }
                }
            }

            [[clang::musttail]] return state<Fwd, Cont...>::operator()(res, si, first, last, it);
        }
    };

    template<bool Fwd, std::size_t Expr, std::size_t... Cont>
        requires (Expr < ast.exprs.size() and ast.exprs[Expr].index == ast_index<typename ast_t::char_class>)
    struct state<Fwd, Expr, Cont...>
    {
        static constexpr auto cla = ast.exprs[Expr].value.[: info_t::type::nsdms[ast_index<typename ast_t::char_class>] :];

        template<std::bidirectional_iterator I, std::sentinel_for<I> S>
        [[gnu::always_inline]] static constexpr bool operator()(result<I>& res, staging_info<I>& si, const I first, const S last, I& it)
            requires (std::same_as<char_type, char> or std::same_as<char_type, char32_t>)
        {
            if constexpr (Fwd)
            {
                if (it == last)
                    return false;
            }
            else
            {
                if (it == first)
                    return false;

                --it;
            }

            if (not cs_possible<cla.data.cs>(*it))
                return false;

            if constexpr (Fwd)
                ++it;

            [[clang::musttail]] return state<Fwd, Cont...>::operator()(res, si, first, last, it);
        }

        template<std::bidirectional_iterator I, std::sentinel_for<I> S>
        [[gnu::always_inline]] static constexpr bool operator()(result<I>& /* res */, staging_info<I>& /* si */, const I /* first */, const S /* last */, I& /* it */)
            requires (not std::same_as<char_type, char> and not std::same_as<char_type, char32_t>)
        {

            static_assert(false, "Unimplemented");
            return false;
        }
    };

    template<bool Fwd, std::size_t Expr, std::size_t... Cont>
        requires (Expr < ast.exprs.size() and ast.exprs[Expr].index == ast_index<typename ast_t::alt>)
    struct state<Fwd, Expr, Cont...>
    {
        static constexpr auto alt = ast.exprs[Expr].value.[: info_t::type::nsdms[ast_index<typename ast_t::alt>] :];

        template<std::bidirectional_iterator I, std::sentinel_for<I> S>
        [[gnu::always_inline]] static constexpr bool operator() (result<I>& res, staging_info<I>& si, const I first, const S last, I& it)
            requires (not alt.idxs.empty())
        {
            template for (constexpr std::size_t Next : alt.idxs | std::views::take(alt.idxs.size() - 1))
            {
                if (auto saved_it = it; state<Fwd, Next, Cont...>::operator()(res, si, first, last, it))
                    return true;
                else
                    it = saved_it;
            }

            [[clang::musttail]] return state<Fwd, alt.idxs.back(), Cont...>::operator()(res, si, first, last, it);
        }

        template<std::bidirectional_iterator I, std::sentinel_for<I> S>
        [[gnu::always_inline]] static constexpr bool operator() (result<I>& /* res */, const I /* first */, const S /* last */, I& /* it */)
            requires (alt.idxs.empty())
        {
            return false;
        }
    };

    template<bool Fwd, std::size_t Expr, std::size_t... Cont>
        requires (Expr < ast.exprs.size() and ast.exprs[Expr].index == ast_index<typename ast_t::concat>)
    struct state<Fwd, Expr, Cont...>
    {
        static constexpr auto cat = ast.exprs[Expr].value.[: info_t::type::nsdms[ast_index<typename ast_t::concat>] :];

        template<std::bidirectional_iterator I, std::sentinel_for<I> S>
        [[gnu::always_inline]] static constexpr bool operator()(result<I>& res, staging_info<I>& si, const I first, const S last, I& it)
        {
            using next_sequence_t = [: [] consteval {
                std::vector<std::meta::info> vec{};
                vec.reserve(cat.idxs.size());
                if (Fwd)
                {
                    for (const auto idx: cat.idxs)
                        vec.push_back(std::meta::reflect_constant(idx));
                }
                else
                {
                    for (const auto idx: cat.idxs | std::views::reverse)
                        vec.push_back(std::meta::reflect_constant(idx));
                }
                return substitute(^^std::index_sequence, vec);
            }() :];

            constexpr auto [...Next] = next_sequence_t{};
            [[clang::musttail]] return state<Fwd, Next..., Cont...>::operator()(res, si, first, last, it);
        }
    };

    template<bool Fwd, std::size_t Expr, std::size_t... Cont>
        requires (Expr < ast.exprs.size() and ast.exprs[Expr].index == ast_index<typename ast_t::repeat>)
    struct state<Fwd, Expr, Cont...>
    {
        static constexpr auto rep = ast.exprs[Expr].value.[: info_t::type::nsdms[ast_index<typename ast_t::repeat>] :];

        template<std::bidirectional_iterator I, std::sentinel_for<I> S>
        [[gnu::always_inline]] static constexpr bool operator()(result<I>& res, staging_info<I>& si, const I first, const S last, I& it)
            requires (rep.reps.min == rep.reps.max)
        {
            using next_sequence_t = [: [] consteval {
                std::vector vec(rep.reps.min, std::meta::reflect_constant(rep.idx));
                return substitute(^^std::index_sequence, vec);
            }() :];

            constexpr auto [...Next] = next_sequence_t{};
            [[clang::musttail]] return state<Fwd, Next..., Cont...>::operator()(res, si, first, last, it);
        }

        template<std::bidirectional_iterator I, std::sentinel_for<I> S>
        static constexpr bool operator()(result<I>& res, staging_info<I>& si, const I first, const S last, I& it)
            requires (rep.reps.min == 0 and rep.reps.max < 0 and rep.mode == repeater_mode::greedy)
        {
            if (auto saved_it = it; state<Fwd, rep.idx, Expr, Cont...>::operator()(res, si, first, last, it))
                return true;
            else
                it = saved_it;

            [[clang::musttail]] return state<Fwd, Cont...>::operator()(res, si, first, last, it);
        }

        template<std::bidirectional_iterator I, std::sentinel_for<I> S>
        static constexpr bool operator()(result<I>& res, staging_info<I>& si, const I first, const S last, I& it)
            requires (rep.reps.min == 0 and rep.reps.max < 0 and rep.mode == repeater_mode::lazy)
        {
            if (auto saved_it = it; state<Fwd, Cont...>::operator()(res, si, first, last, it))
                return true;
            else
                it = saved_it;

            [[clang::musttail]] return state<Fwd, rep.idx, Expr, Cont...>::operator()(res, si, first, last, it);
        }

        template<std::bidirectional_iterator I, std::sentinel_for<I> S>
        [[gnu::always_inline]] static constexpr bool operator()(result<I>& res, staging_info<I>& si, const I first, const S last, I& it)
            requires (rep.reps.min == 0 and rep.reps.max < 0 and rep.mode == repeater_mode::possessive)
        {
            while (true)
            {
                if (auto saved_it = it; not state<Fwd, rep.idx>::operator()(res, si, first, last, it))
                {
                    it = saved_it;
                    break;
                }
            }

            [[clang::musttail]] return state<Fwd, Cont...>::operator()(res, si, first, last, it);
        }

        template<std::bidirectional_iterator I, std::sentinel_for<I> S>
        static constexpr bool operator()(result<I>& res, staging_info<I>& si, const I first, const S last, I& it)
            requires (rep.reps.min == 0 and rep.reps.max == 1 and rep.mode == repeater_mode::greedy)
        {
            if (auto saved_it = it; state<Fwd, rep.idx, Cont...>::operator()(res, si, first, last, it))
                return true;
            else
                it = saved_it;

            [[clang::musttail]] return state<Fwd, Cont...>::operator()(res, si, first, last, it);
        }

        template<std::bidirectional_iterator I, std::sentinel_for<I> S>
        static constexpr bool operator()(result<I>& res, staging_info<I>& si, const I first, const S last, I& it)
            requires (rep.reps.min == 0 and rep.reps.max == 1 and rep.mode == repeater_mode::lazy)
        {
            if (auto saved_it = it; state<Fwd, Cont...>::operator()(res, si, first, last, it))
                return true;
            else
                it = saved_it;

            [[clang::musttail]] return state<Fwd, rep.idx, Cont...>::operator()(res, si, first, last, it);
        }

        template<std::bidirectional_iterator I, std::sentinel_for<I> S>
        [[gnu::always_inline]] static constexpr bool operator()(result<I>& res, staging_info<I>& si, const I first, const S last, I& it)
            requires (rep.reps.min == 0 and rep.reps.max == 1 and rep.mode == repeater_mode::possessive)
        {
            if (auto saved_it = it; not state<Fwd, rep.idx>::operator()(res, si, first, last, it))
                it = saved_it;

            [[clang::musttail]] return state<Fwd, Cont...>::operator()(res, si, first, last, it);
        }
    };

    template<bool Fwd, std::size_t Expr, std::size_t... Cont>
        requires (Expr < ast.exprs.size() and ast.exprs[Expr].index == ast_index<typename ast_t::assertion>)
    struct state<Fwd, Expr, Cont...>
    {
        static constexpr auto asr = ast.exprs[Expr].value.[: info_t::type::nsdms[ast_index<typename ast_t::assertion>] :];

        template<std::bidirectional_iterator I, std::sentinel_for<I> S>
        [[gnu::always_inline]] static constexpr bool check_impl(const I first, const S  /* last */, const I it)
            requires (asr.type == assert_type::text_start)
        {
            return (it == first);
        }

        template<std::bidirectional_iterator I, std::sentinel_for<I> S>
        [[gnu::always_inline]] static constexpr bool check_impl(const I  /* first */, const S last, const I it)
            requires (asr.type == assert_type::text_end)
        {
            return (it == last);
        }

        template<std::bidirectional_iterator I, std::sentinel_for<I> S>
        [[gnu::always_inline]] static constexpr bool check_impl(const I first, const S  /* last */, const I it)
            requires (asr.type == assert_type::line_start)
        {
            return (it == first or *std::ranges::prev(it) == '\n');
        }

        template<std::bidirectional_iterator I, std::sentinel_for<I> S>
        [[gnu::always_inline]] static constexpr bool check_impl(const I  /* first */, const S last, const I it)
            requires (asr.type == assert_type::line_end)
        {
            return (it == last or *it == '\n');
        }

        template<std::bidirectional_iterator I, std::sentinel_for<I> S>
        [[gnu::always_inline]] static constexpr bool check_impl(const I first, const S last, const I it)
            requires (asr.type == assert_type::ascii_word_boundary)
        {
            static constexpr auto is_ascii_word_character = [](std::iter_value_t<I> c){
                return ('0' <= c and c <= '9') or ('A' <= c and c <= 'Z') or ('a' <= c and c <= 'z') or (c == '_');
            };

            if (it == first or not is_ascii_word_character(*std::ranges::prev(it)))
                return (it != last and is_ascii_word_character(*it));
            else /* if (it != first and is_ascii_word_character(*std::ranges::prev(it))) */
                return (it == last or not is_ascii_word_character(*it));
        }

        template<std::bidirectional_iterator I, std::sentinel_for<I> S>
        [[gnu::always_inline]] static constexpr bool check_impl(const I first, const S last, const I it)
            requires (asr.type == assert_type::not_ascii_word_boundary)
        {
            static constexpr auto is_ascii_word_character = [](std::iter_value_t<I> c){
                return ('0' <= c and c <= '9') or ('A' <= c and c <= 'Z') or ('a' <= c and c <= 'z') or (c == '_');
            };

            if (it == first or not is_ascii_word_character(*std::ranges::prev(it)))
                return (it == last or not is_ascii_word_character(*it));
            else /* if (it != first and is_ascii_word_character(*std::ranges::prev(it))) */
                return (it != last and is_ascii_word_character(*it));
        }

        template<std::bidirectional_iterator I, std::sentinel_for<I> S>
        [[gnu::always_inline]] static constexpr bool operator()(result<I>& res, staging_info<I>& si, const I first, const S last, I& it)
        {
            if (check_impl(first, last, it))
                [[clang::musttail]] return state<Fwd, Cont...>::operator()(res, si, first, last, it);

            return false;
        }
    };

    template<bool Fwd, std::size_t Expr, std::size_t... Cont>
        requires (Expr < ast.exprs.size() and ast.exprs[Expr].index == ast_index<typename ast_t::backref>)
    struct state<Fwd, Expr, Cont...>
    {
        static constexpr auto bref = ast.exprs[Expr].value.[: info_t::type::nsdms[ast_index<typename ast_t::backref>] :];

        template<std::bidirectional_iterator I, std::sentinel_for<I> S>
        [[gnu::always_inline]] static constexpr bool operator()(result<I>& res, staging_info<I>& si, const I first, const S last, I& it)
        {
            static_assert(bref.number < result<I>::submatch_count, "Backreference to non-existent submatch");

            const auto submatch = res.template force_get<bref.number>();

            if (not submatch.matched())
                return false;

            if constexpr (Fwd)
            {
                if constexpr (std::random_access_iterator<I> and std::sized_sentinel_for<S, I>)
                {
                    if (static_cast<std::size_t>(std::ranges::distance(it, last)) < submatch.size())
                        return false;

                    auto submatch_it = submatch.begin();
                    int mismatch{ false };  /* encourage vectorisation */
                    for (std::size_t i{ 0 }, i_end{ submatch.size() }; i < i_end; ++i)
                        mismatch |= (it[i] != submatch_it[i]);

                    if (mismatch)
                        return false;

                    std::ranges::advance(it, static_cast<std::ptrdiff_t>(submatch.size()));
                }
                else
                {
                    for (char_type c : submatch)
                    {
                        if (it == last or *it != c)
                            return false;

                        ++it;
                    }
                }
            }
            else
            {
                if constexpr (std::random_access_iterator<I> and std::sized_sentinel_for<S, I>)
                {
                    if (static_cast<std::size_t>(std::ranges::distance(first, it)) < submatch.size())
                        return false;

                    std::ranges::advance(it, -static_cast<std::ptrdiff_t>(submatch.size()));

                    auto submatch_it = submatch.begin();
                    int mismatch{ false };  /* encourage vectorisation */
                    for (std::size_t i{ 0 }, i_end{ submatch.size() }; i < i_end; ++i)
                        mismatch |= (it[i] != submatch_it[i]);

                    if (mismatch)
                        return false;
                }
                else
                {
                    for (char_type c : submatch | std::views::reverse)
                    {
                        if (it == first)
                            return false;

                        --it;

                        if (*it != c)
                            return false;
                    }
                }
            }

            [[clang::musttail]] return state<Fwd, Cont...>::operator()(res, si, first, last, it);
        }
    };

    template<bool Fwd, std::size_t Expr, std::size_t... Cont>
        requires (Expr < ast.exprs.size() and ast.exprs[Expr].index == ast_index<typename ast_t::tag>)
    struct state<Fwd, Expr, Cont...>
    {
        static constexpr auto tag = ast.exprs[Expr].value.[: info_t::type::nsdms[ast_index<typename ast_t::tag>] :];

        template<std::bidirectional_iterator I, std::sentinel_for<I> S>
        [[gnu::always_inline]] static constexpr bool operator()(result<I>& res, staging_info<I>& si, const I first, const S last, I& it)
            requires (ast.staging.contains(tag.number))
        {
            si.reg[get_staging_index(tag.number)] = it;

            [[clang::musttail]] return state<Fwd, Cont...>::operator()(res, si, first, last, it);
        }

        template<std::bidirectional_iterator I, std::sentinel_for<I> S>
        [[gnu::always_inline]] static constexpr bool operator()(result<I>& res, staging_info<I>& si, const I first, const S last, I& it)
            requires (not ast.staging.contains(tag.number))
        {
            static_assert(tag.number < res.reg_.size());

            static constexpr auto staged = [] consteval {
                std::vector<tag_number_t> vec;
                for (const auto& [k, v] : ast.staging)
                    if (v == tag.number)
                        vec.emplace_back(k);
                std::ranges::sort(vec);
                auto [it, _] = std::ranges::unique(vec);
                vec.erase(it, vec.end());
                return define_static_array(vec);
            }();

            using saved_t = std::conditional_t<std::contiguous_iterator<I>, I, bool>;
            std::array<saved_t, 1 + staged.size()> saved;

            if constexpr (std::contiguous_iterator<I>)
            {
                saved.back() = res.reg_[tag.number];

                if constexpr (not ast.staging.empty())
                    template for (constexpr auto i : std::views::iota(0uz, staged.size()))
                        saved[i] = res.reg_[staged[i]];
            }
            else
            {
                saved.back() = res.enabled_[tag.number];

                if constexpr (not ast.staging.empty())
                    template for (constexpr auto i : std::views::iota(0uz, staged.size()))
                        saved[i] = res.enabled_[staged[i]];
            }

            res.reg_[tag.number] = it;

            if constexpr (not ast.staging.empty())
                template for (constexpr auto tagnum : staged)
                    res.reg_[tagnum] = si.reg[get_staging_index(tagnum)];

            if constexpr (not std::contiguous_iterator<I>)
            {
                res.enabled_[tag.number] = true;

                if constexpr (not ast.staging.empty())
                    template for (constexpr auto tagnum : staged)
                        res.enabled_[tagnum] = true;
            }

            if (auto saved_it = it; state<Fwd, Cont...>::operator()(res, si, first, last, it))
                return true;
            else
                it = saved_it;

            if constexpr (std::contiguous_iterator<I>)
            {
                res.reg_[tag.number] = saved.back();

                if constexpr (not ast.staging.empty())
                    template for (constexpr auto i : std::views::iota(0uz, staged.size()))
                        res.reg_[staged[i]] = saved[i];
            }
            else
            {
                res.enabled_[tag.number] = saved.back();

                if constexpr (not ast.staging.empty())
                    template for (constexpr auto i : std::views::iota(0uz, staged.size()))
                        res.enabled_[staged[i]] = saved[i];
            }

            return false;
        }
    };

public:
    struct full_match;

    template<bool IsSearch, bool IsIterator>
    struct partial_match;

    static consteval auto get_matcher(fsm_flags f)
    {
        if (f.longest_match)
        {
            if (not f.is_search and not f.enable_fallback and not f.is_iterator)
                return ^^full_match;
        }
        else if (f.enable_fallback)
        {
            return substitute(^^partial_match, { std::meta::reflect_constant(f.is_search), std::meta::reflect_constant(f.is_iterator) });
        }

        /* invalid flag combination */
        throw regex_error("naive_matcher_adaptor: invalid fsm flag combination");
    }
};

template<string_literal Pattern>
struct naive_matcher<Pattern>::full_match
{
    template<std::bidirectional_iterator I, std::sentinel_for<I> S>
    static constexpr auto operator()(const I first, const S last)
    {
        result<I> res{ first };
        staging_info<I> si{};
        if (I it{ first }; state<true, ast.root_idx, require_full_match>::operator()(res, si, first, last, it))
        {
            apply_final_staging_info(res, si);
            res.match_end_ = it;

            if constexpr (not std::contiguous_iterator<I>)
                res.match_success_ = true;
        }
        return res;
    }
};

template<string_literal Pattern>
template<bool IsSearch, bool IsIterator>
struct naive_matcher<Pattern>::partial_match
{
private:
    template<std::bidirectional_iterator I, std::sentinel_for<I> S>
    [[gnu::always_inline]] static constexpr auto outer_state(const I first, I continue_from, const S last)
    {
        result<I> res{ continue_from };
        staging_info<I> si{};
        if (state<true, ast.root_idx>::operator()(res, si, first, last, continue_from))
        {
            apply_final_staging_info(res, si);
            res.match_end_ = continue_from;

            if constexpr (not std::contiguous_iterator<I>)
                res.match_success_ = true;
        }
        return res;
    }

    template<std::bidirectional_iterator I, std::sentinel_for<I> S>
    [[gnu::always_inline]] static constexpr auto outer_state(const I first, I continue_from, const S last)
        requires IsSearch
    {
        result<I> res{};
        staging_info<I> si{};
        while (true)
        {
            if (I it{ continue_from }; state<true, ast.root_idx>::operator()(res, si, first, last, it))
            {
                apply_final_staging_info(res, si);
                res.match_end_ = it;
                res.match_start_ = continue_from;

                if constexpr (not std::contiguous_iterator<I>)
                    res.match_success_ = true;
                return res;
            }

            res.clear_match();

            if (continue_from == last)
                return res;

            ++continue_from;
        }
    }

    template<std::bidirectional_iterator I, std::sentinel_for<I> S>
    [[gnu::always_inline]] static constexpr auto non_empty_outer_state(const I first, I continue_from, const S last)
    {
        result<I> res{ continue_from };
        staging_info<I> si{};
        if (state<true, ast.root_idx, require_non_empty_match>::operator()(res, si, first, last, continue_from))
        {
            apply_final_staging_info(res, si);
            res.match_end_ = continue_from;

            if constexpr (not std::contiguous_iterator<I>)
                res.match_success_ = true;
        }
        return res;
    }

    template<std::bidirectional_iterator I, std::sentinel_for<I> S>
    [[gnu::always_inline]] static constexpr auto non_empty_outer_state(const I first, I continue_from, const S last)
        requires IsSearch
    {
        result<I> res{ continue_from };
        staging_info<I> si{};

        if (I it{ continue_from }; state<true, ast.root_idx, require_non_empty_match>::operator()(res, si, first, last, it))
        {
            apply_final_staging_info(res, si);
            res.match_end_ = it;

            if constexpr (not std::contiguous_iterator<I>)
                res.match_success_ = true;
        }
        else if (continue_from != last)
        {
            res = outer_state(first, ++continue_from, last);
        }

        return res;
    }

public:
    template<std::bidirectional_iterator I>
    using result = naive_matcher::result<I>;

    template<std::bidirectional_iterator I, std::sentinel_for<I> S>
    static constexpr auto operator()(const I first, const S last)
        requires (not IsIterator)
    {
        return outer_state(first, first, last);
    }

    template<std::bidirectional_iterator I>
    struct iterated_result;
};

template<string_literal Pattern>
template<bool IsSearch, bool IsIterator>
template<std::bidirectional_iterator I>
struct naive_matcher<Pattern>::partial_match<IsSearch, IsIterator>::iterated_result
{
    static constexpr bool needs_begin{ true };

    struct state_type
    {
        using continue_type = terminal_object;

        [[no_unique_address]] continue_type continue_at{};
    };

    iterated_result() = default;

    constexpr iterated_result(const I first, const std::sentinel_for<I> auto last)
    {
        resume(first, first, last);
    }

    constexpr iterated_result(const I first, const std::sentinel_for<I> auto last, match_non_empty_t)
    {
        resume<true>(first, first, last);
    }

    constexpr iterated_result(const I first, const I it, const std::sentinel_for<I> auto last, state_type stf, bool prev_empty)
        : res{ it }, stf{ stf }
    {
        if constexpr (ast.empty_match_possible)
        {
            if (prev_empty)
            {
                resume<true>(first, it, last);
                return;
            }
        }
        resume(first, it, last);
    }

    constexpr I advance(const I first, const std::sentinel_for<I> auto last)
    {
        const auto& match = res.template force_get<0>();
        const I current = match.end();

        if constexpr (ast.empty_match_possible)
        {
            if (match.empty())
            {
                if (current == last)
                    res.clear_match();
                else
                    resume<true>(first, current, last);
                return current;
            }
        }

        resume(first, current, last);
        return current;
    }

private:
    template<bool NonEmptyMatch = false>
    constexpr void resume(const I first, const I current, const std::sentinel_for<I> auto last)
    {
        if constexpr (NonEmptyMatch and ast.empty_match_possible)
            res = non_empty_outer_state(first, current, last);
        else
            res = outer_state(first, current, last);
    }

public:
    result<I> res{};
    state_type stf{};
};

template<string_literal Pattern, fsm_flags Flags>
struct naive_matcher_adaptor : public [: naive_matcher<Pattern>::get_matcher(Flags) :] {};

template<string_literal Pattern, fsm_flags Flags>
    requires (Flags.return_bool)
struct naive_matcher_adaptor<Pattern, Flags>
{
    using underlying = naive_matcher_adaptor<Pattern, [](fsm_flags f) consteval { f.return_bool = false; return f; }(Flags)>;

    template<typename... Ts>
    static constexpr bool operator()(Ts&&... args)
    {
        return underlying::operator()(std::forward<Ts>(args)...);
    }
};

}
}

namespace srx {
namespace detail {

template<typename CharT>
struct optimised_tr_edge
{
    using value_type = std::make_unsigned_t<CharT>;

    value_type sub;
    value_type rng;
    value_type msk;
};

template<unconstrained_charset_like CharsetType>
consteval auto make_optimised_edges(const CharsetType& sc, bool inverted = false)
{
    using char_type = CharsetType::char_type;
    using ote = optimised_tr_edge<char_type>;
    using uchar_type = typename ote::value_type;
    std::vector<ote> result;

    if (inverted)
    {
        for (const auto& [first, last] : (~sc).get_intervals())
            result.emplace_back(static_cast<uchar_type>(first),
                                static_cast<uchar_type>(static_cast<char_type>(last - first)),
                                static_cast<uchar_type>(~0u));
    }
    else
    {
        for (const auto& [first, last] : sc.get_intervals())
            result.emplace_back(static_cast<uchar_type>(first),
                                static_cast<uchar_type>(static_cast<char_type>(last - first)),
                                static_cast<uchar_type>(~0u));
    }

    std::vector<ote> input;

    while (result.size() != input.size())
    {
        std::swap(result, input);
        result.clear();

        std::vector<bool> visited(input.size(), false);

        for (std::size_t i{ 0 }, i_end{ input.size() }; i < i_end; ++i)
        {
            if (visited.at(i))
                continue;

            visited.at(i) = true;
            const auto& [sub1, rng1, msk1] = input.at(i);

            bool inserted{ false };

            for (std::size_t j{ i + 1 }; j < i_end; ++j)
            {
                if (visited.at(j))
                    continue;

                const auto& [sub2, rng2, msk2] = input.at(j);

                if (rng1 != rng2 or msk1 != msk2)
                    continue;

                const uchar_type masked_bit{ static_cast<uchar_type>(sub1 ^ sub2) };

                if (not std::has_single_bit(masked_bit) or (masked_bit | msk1) != msk1)
                    continue;

                visited.at(j) = true;
                inserted = true;
                result.emplace_back(std::ranges::min(sub1, sub2), rng1, static_cast<uchar_type>(msk1 - masked_bit));
                break;
            }

            if (not inserted)
                result.emplace_back(sub1, rng1, msk1);
        }
    }

    return result;
}

template<typename CharT>
consteval std::size_t score_optimised_edges(const std::vector<optimised_tr_edge<CharT>>& otes, bool inverted = false)
{
    using ote_t = optimised_tr_edge<CharT>;
    using value_type = ote_t::value_type;

    static constexpr std::size_t bit_count{ std::numeric_limits<value_type>::digits };
    static constexpr std::size_t range_penalty{ 2 };
    static constexpr std::size_t scale{ 4 }; /* arbitrarily selected constant */

    if (otes.empty())
        return -1uz;

    /* note: this is just a heuristic; there may be better ones */

    std::size_t count{ 0 };
    std::size_t ops{ 0 };

    for (const auto& [sub, rng, msk] : otes)
    {
        std::size_t mask_bits{ bit_count - std::popcount(msk) };
        std::size_t number{ (1 + rng) * mask_bits };
        count += number;
        ops += 2 + ((rng != 0) * range_penalty) + (mask_bits != 0);
    }

    if (inverted)
        count = (~0uz >> (std::numeric_limits<std::size_t>::digits - bit_count)) - count;

    return (count * scale) + ops;
}

template<typename CharT>
consteval auto get_flattened_states(const tdfa_info<CharT>& dfa, std::size_t state, bool trailing = false)
{
    std::vector<std::size_t> result;

    while (true)
    {
        if (dfa.final_nodes.contains(state))
            break;

        /* assume fallback_nodes is subset of final_nodes */

        if (dfa.nodes.at(state).size() != 1)
        {
            if (trailing)
                result.emplace_back(state);
            break;
        }

        const auto& tr = dfa.nodes.at(state).front();
        result.emplace_back(state);
        state = tr.next;
    }

    return result;
}

template<typename CharT>
consteval auto get_edge_scores(const tdfa_info<CharT>& dfa, const std::span<const std::size_t> qs)
{
    if (qs.empty())
        throw std::range_error("get_edge_scores: qs is empty");

    const auto edges = qs
                       | std::views::transform([&](std::size_t q){
                             if (dfa.nodes[q].size() == 1)
                             {
                                 const auto& cs = dfa.nodes[q].front().cs;
                                 const bool should_invert{ cs.should_invert() };
                                 return std::pair{ make_optimised_edges(cs, should_invert), should_invert };
                             }
                             else
                             {
                                 charset<CharT> cs;
                                 for (const auto tr : dfa.nodes[q])
                                     cs |= tr.cs;
                                 const bool should_invert{ cs.should_invert() };
                                 return std::pair{ make_optimised_edges(cs, should_invert), should_invert };
                             }
                         })
                       | std::ranges::to<std::vector>();

    return edges
           | std::views::transform([](const auto& e){ return score_optimised_edges(e.first, e.second); })
           | std::ranges::to<std::vector>();
}

template<typename CharT>
consteval auto get_outer_state_position_pair(const tdfa_info<CharT>& dfa, const std::span<const std::size_t> qs)
{
    auto scores = get_edge_scores(dfa, qs);

    const auto left_elem = std::ranges::min_element(scores);
    *left_elem = -1uz;
    const std::size_t left{ static_cast<std::size_t>(std::ranges::distance(scores.begin(), left_elem)) };

    const auto right_elem = std::ranges::min_element(scores.rbegin(), scores.rend());
    const std::size_t right{ static_cast<std::size_t>(std::ranges::distance(right_elem, scores.rend()) - 1) };

    if (left > right)
        return std::pair{ right, left };
    else
        return std::pair{ left, right };
}

template<typename CharT>
consteval auto get_outer_state_position_single(const tdfa_info<CharT>& dfa, const std::span<const std::size_t> qs)
{
    const auto scores = get_edge_scores(dfa, qs);
    const auto left_elem = std::ranges::min_element(scores);
    return static_cast<std::size_t>(std::ranges::distance(scores.begin(), left_elem));
}

template<static_charset Sc>
[[gnu::always_inline]] constexpr bool tr_possible(const typename decltype(Sc)::char_type c)
{
#if false
    /* simple code path */
    template for (constexpr auto pair : Sc.get_intervals())
        if (pair.first <= c and c <= pair.second)
            return true;
    return false;
#else
    using char_type = decltype(Sc)::char_type;
    using uchar_type = std::make_unsigned_t<char_type>;

    if constexpr (std::signed_integral<char_type>)
    {
        static constexpr static_charset usc{ Sc.make_unsigned() };
        return tr_possible<usc>(static_cast<uchar_type>(c));
    }
    else
    {
        static constexpr bool inverted{ false /* Sc.should_invert() */ };

        template for (constexpr auto ote : define_static_array(make_optimised_edges(Sc, inverted)))
        {
            uchar_type d{ c };

            if constexpr (ote.msk != ~0u)
                d &= ote.msk;

            if constexpr (ote.rng == 0u)
            {
                if (d == ote.sub)
                    return not inverted;
            }
            else
            {
                d -= ote.sub;
                if (d <= ote.rng)
                    return not inverted;
            }
        }
        return inverted;
    }
#endif
}

template<static_charset Sc>
[[gnu::always_inline]] constexpr bool tr_possible_exclude_null(const typename decltype(Sc)::char_type c)
{
    using char_type = decltype(Sc)::char_type;

    if constexpr (tr_possible<Sc>(char_type{}))
    {
        if (c == char_type{})
            return false;
    }

    return tr_possible<Sc>(c);
}

template<static_charset Sc, std::unsigned_integral UCharT, typename Abi>
[[gnu::always_inline]] constexpr auto vector_tr_find(const std::simd::basic_vec<UCharT, Abi>& c_vec)
{
    using char_type = decltype(Sc)::char_type;

    if constexpr (std::signed_integral<char_type>)
    {
        static_assert(std::same_as<UCharT, std::make_unsigned_t<char_type>>);
        static constexpr static_charset usc{ Sc.make_unsigned() };
        return vector_tr_find<usc>(c_vec);
    }
    else
    {
        static_assert(std::same_as<UCharT, char_type>);
        using vec_type = std::simd::basic_vec<UCharT, Abi>;
        using mask_type = vec_type::mask_type;

        static constexpr bool inverted{ Sc.should_invert() };
        mask_type result{ inverted };

        template for (constexpr auto ote : define_static_array(make_optimised_edges(Sc, inverted)))
        {
            auto d_vec{ c_vec };

            if constexpr (ote.msk != ~0u)
            {
                static constexpr vec_type mask_vec{ ote.msk };
                d_vec &= mask_vec;
            }

            if constexpr (ote.rng == 0u)
            {
                static constexpr vec_type cmp_vec{ ote.sub };

                if constexpr (inverted)
                    result &= (d_vec != cmp_vec);
                else
                    result |= (d_vec == cmp_vec);
            }
            else
            {
                static constexpr vec_type sub_vec{ ote.sub };
                static constexpr vec_type lte_vec{ ote.rng };
                d_vec -= sub_vec;

                if constexpr (inverted)
                    result &= (d_vec > lte_vec);
                else
                    result |= (d_vec <= lte_vec);
            }
        }

        return result;
    }
}

template<std::meta::info Action, typename ResultType>
static constexpr auto exec_lexer_action(const ResultType& res)
{
    static_assert(Action != std::meta::info{});

    if constexpr (is_enumerator(Action))
    {
        return [: Action :];
    }
    if constexpr (is_value(Action) or is_object(Action) or is_variable(Action))
    {

        if constexpr (is_invocable_type(type_of(Action), {}))
            return [: Action :]();
        else if constexpr (is_invocable_type(type_of(Action), { type_of(^^res) }))
            return [: Action :](res);
        else
            return [: Action :];
    }
    else if constexpr (is_type(Action))
    {
        using action_type = [: Action :];

        if constexpr (dealias(Action) == ^^void)
            return; /* skip lexer action */
        else if constexpr (not is_default_constructible_type(Action))
            static_assert(false, "Invalid lexer action: type is not default-constructible");
        else if constexpr (is_invocable_type(Action, {}))
            return action_type{}();
        else if constexpr (is_invocable_type(Action, { type_of(^^res) }))
            return action_type{}(res);
        else
            return action_type{};
    }
    else if constexpr (is_function(Action) or is_function_template(Action))
    {
        static_assert(false, "Unimplemented!");
    }
    else
    {
        static_assert(false, "Invalid lexer action");
    }
}

template<std::meta::info Info>
struct p1306dfa
{
    static_assert(has_template_arguments(type_of(Info)), "Invalid reflection value");
    using char_type = typename [: template_arguments_of(type_of(Info))[0] :];

    static constexpr bool is_regex{ template_of(type_of(Info)) == ^^tdfa_info };
    static constexpr bool is_lexer{ template_of(type_of(Info)) == ^^lexer_info };
    static_assert(is_regex != is_lexer);

private:
    static constexpr tdfa_info<char_type> DFA = [] consteval {
        if constexpr (is_regex)
            return [: Info :];
        else if constexpr (is_lexer)
            return [: Info :].dfa;
        else
            static_assert(false, "Invalid reflection value");
    }();

    static constexpr bool never_empty{ DFA.additional_continue_nodes.empty() };
    static constexpr bool fixed_length{ DFA.min_max_lengths.first != std::numeric_limits<std::size_t>::max()
                                        and DFA.min_max_lengths.first == DFA.min_max_lengths.second };
    static constexpr bool branch_free{ std::ranges::all_of(DFA.nodes, [](const auto& n){ return n.size() <= 1; }) };

public:
    template<std::bidirectional_iterator I>
        requires std::is_nothrow_convertible_v<std::iter_value_t<I>, char_type>
    using result = static_match_results<I, DFA.make_match_result_info()>;

    template<std::bidirectional_iterator I>
        requires std::is_nothrow_convertible_v<std::iter_value_t<I>, char_type>
    using token = typename [: [] consteval {
        if constexpr (not is_lexer)
            return ^^terminal_object;
        else
            return [: Info :].return_type;
    }() :];

    template<std::bidirectional_iterator I>
        requires std::is_nothrow_convertible_v<std::iter_value_t<I>, char_type>
    struct iterated_result;

    struct stateful
    {
        static constexpr bool has_continue{ DFA.has_continue() };
        static constexpr bool is_stateless{ not (has_continue /* and other has_x */) };

        using continue_type = maybe_type_t<has_continue, tdfa::continue_at_t>;

        [[no_unique_address]] continue_type continue_at{ detail::tdfa::no_continue };
    };

private:
    static constexpr std::size_t fallback_disabled{ std::numeric_limits<std::size_t>::max() };
    static constexpr std::size_t avoid_simd_threshold{ (1uz << std::numeric_limits<std::make_unsigned_t<char_type>>::digits) / 2 };

    struct generation
    {
        static constexpr bool enabled{ DFA.flags.adapted_search and DFA.register_count > 0 };

        std::array<std::size_t, DFA.register_count> reg{};
        std::size_t current{ 0 };
    };

    using gen_info = maybe_type_t<generation::enabled, generation>;

    template<typename I, bool B = false>
    struct overspill
    {
        [[no_unique_address]] gen_info gen;

        overspill() = default;
    };

    template<typename I>
    struct overspill<I, true>
    {
        result<I> res;
        [[no_unique_address]] gen_info gen;
        [[no_unique_address]] stateful stf;

        overspill() = default;
        constexpr overspill(I first) noexcept(noexcept(result<I>{ first })) : res{ first } {}
    };

    template<typename I>
    using token_ref = maybe_type_t<is_lexer, std::add_lvalue_reference_t<token<I>>>;

    template<typename I>
    using result_ref = std::add_lvalue_reference_t<result<I>>;

    template<typename I>
    using iterated_result_ref = std::add_lvalue_reference_t<iterated_result<I>>;

    template<typename I, bool B = false>
    using overspill_ref = std::add_lvalue_reference_t<overspill<I, B>>;

    template<typename I, int X = 0>
    struct context
    {
        /* empty context */
    };

    template<typename I>
    struct context<I, 1>
    {
        /* store results */
        result_ref<I> res;
        [[no_unique_address]] overspill_ref<I> osr;

        [[gnu::always_inline]] constexpr auto& get_res() noexcept { return res; }
    };

    template<typename I>
    struct context<I, 2>
    {
        /* store results with iterated search info */
        iterated_result_ref<I> itr;
        [[no_unique_address]] overspill_ref<I> osr;

        [[gnu::always_inline]] constexpr auto& get_res() noexcept { return itr.res; }
        [[gnu::always_inline]] constexpr auto& get_stf() noexcept { return itr.stf; }
    };

    template<typename I>
    struct context<I, 3>
    {
        /* store token, with result in overspill only */
        token_ref<I> tok;
        overspill_ref<I, true> osr;

        [[gnu::always_inline]] constexpr auto& get_res() noexcept { return osr.res; }
        [[gnu::always_inline]] constexpr auto& get_stf() noexcept { return osr.stf; }
    };

    template<typename I>
    context(result<I>&, overspill<I>&) -> context<I, 1>;

    template<typename I>
    context(iterated_result<I>&, overspill<I>&) -> context<I, 2>;

    template<typename I>
    context(token<I>&, overspill<I, true>&) -> context<I, 3>;

    template<typename I>
    struct fallback_info
    {
        [[no_unique_address]] maybe_type_t<(not DFA.flags.return_bool), I> it;
        std::size_t state{ fallback_disabled };

        constexpr explicit(false) fallback_info(I it) : it{ it } {}
    };

    template<typename I>
    using maybe_fallback_t = maybe_type_t<DFA.flags.enable_fallback, fallback_info<I>>;

    /* post-match register cleaning */

    template<std::bidirectional_iterator I>
    static constexpr void clean_generations(result<I>& res, const overspill<I, false>& osp)
        requires generation::enabled
    {
        if constexpr (std::contiguous_iterator<I>)
        {
            for (std::size_t i{ 0 }; i < DFA.register_count; ++i)
                res.reg_[i] = (osp.gen.reg[i] == osp.gen.current) ? res.reg_[i] : I{};
        }
        else
        {
            for (std::size_t i{ 0 }; i < DFA.register_count; ++i)
                res.enabled_[i] = (osp.gen.reg[i] == osp.gen.current);
        }
    }

    template<std::bidirectional_iterator I>
    static constexpr void clean_generations(overspill<I, true>& osp)
        requires generation::enabled
    {
        if constexpr (std::contiguous_iterator<I>)
        {
            for (std::size_t i{ 0 }; i < DFA.register_count; ++i)
                osp.res.reg_[i] = (osp.gen.reg[i] == osp.gen.current) ? osp.res.reg_[i] : I{};
        }
        else
        {
            for (std::size_t i{ 0 }; i < DFA.register_count; ++i)
                osp.res.enabled_[i] = (osp.gen.reg[i] == osp.gen.current);
        }
    }

    /* register operations */

    template<std::size_t Blk, std::bidirectional_iterator I, int X>
    static constexpr void register_operations(context<I, X> ctx, const I it)
    {
        if constexpr (Blk != tdfa::no_transition_regops and X != 0)
        {
            template for (constexpr register_operation op : DFA.regops[Blk])
            {
                if constexpr (op.is_copy)
                    ctx.get_res().reg_[op.dst] = ctx.get_res().reg_[op.cpy_src];
                else if constexpr (op.set_val)
                    ctx.get_res().reg_[op.dst] = it;
                else if constexpr (not generation::enabled and std::contiguous_iterator<I>)
                    ctx.get_res().reg_[op.dst] = I{};

                if constexpr (not generation::enabled and not std::contiguous_iterator<I>)
                {
                    if constexpr (op.is_copy)
                        ctx.get_res().enabled_[op.dst] = ctx.get_res().enabled_[op.cpy_src];
                    else
                        ctx.get_res().enabled_[op.dst] = op.set_val;
                }
            }

            if constexpr (generation::enabled)
            {
                template for (constexpr register_operation op : DFA.regops[Blk])
                {
                    if constexpr (op.is_copy)
                        ctx.osr.gen.reg[op.dst] = ctx.osr.gen.reg[op.cpy_src];
                    else if constexpr (op.set_val)
                        ctx.osr.gen.reg[op.dst] = ctx.osr.gen.current;
                    else
                        ctx.osr.gen.reg[op.dst] = 0;
                }
            }
        }
    }

    /* final and fallback */

    template<std::size_t Blk, std::ptrdiff_t Offset, std::bidirectional_iterator I, int X>
    static constexpr void set_final(context<I, X> ctx, const I it)
    {
        if constexpr (X != 0)
        {
            register_operations<Blk>(ctx, it);
            ctx.get_res().match_end_ = std::ranges::prev(it, Offset);

            if constexpr (not std::contiguous_iterator<I>)
                ctx.get_res().match_success_ = true;
        }
    }

    template<std::size_t Blk, std::ptrdiff_t Offset, tdfa::continue_at_t ContinueAt, std::bidirectional_iterator I, int X>
    static constexpr void set_fallback(context<I, X> ctx, const I it)
    {
        if constexpr (X != 0)
        {
            set_final<Blk, Offset>(ctx, it);

            if constexpr (stateful::has_continue and ContinueAt != tdfa::no_continue)
            {
                if constexpr (X == 1)
                    ctx.res.continue_at_ = ContinueAt;
                else
                    ctx.get_stf().continue_at = ContinueAt;
            }
        }
    }

    /* lexer actions */

    template<std::meta::info Action, std::bidirectional_iterator I, std::sentinel_for<I> S>
    static constexpr bool lexer_action(context<I, 3> ctx, I it, const S end, maybe_fallback_t<I> fallback)
    {
        static constexpr auto func{ substitute(^^exec_lexer_action, { reflect_constant(Action), ^^result<I> }) };

        if constexpr (return_type_of(func) == ^^void)
        {
            /* void should be returned to continue */
            [: func :](ctx.osr.res);
            [[clang::musttail]] return skip_current(ctx, it, end, fallback);
        }
        else
        {
            ctx.tok = token<I>([: func
                :](ctx.osr.res));
            return true;
        }
    }

    template<std::bidirectional_iterator I, std::sentinel_for<I> S>
    static constexpr void token_end_of_input(context<I, 3> ctx, I it, const S /* last */)
    {
        /* note: similar to lexer_action but always returns */
        static constexpr auto action = [: Info :].eof_action;
        static constexpr auto func{ substitute(^^exec_lexer_action, { reflect_constant(action), ^^result<I> }) };

        set_final<tdfa::no_transition_regops, 0>(ctx, it);

        if constexpr (return_type_of(func) == ^^void)
            [: func :](ctx.osr.res);
        else
            ctx.tok = token<I>([: func
                :](ctx.osr.res));
    }

    template<std::bidirectional_iterator I, std::sentinel_for<I> S>
    static constexpr bool skip_current(context<I, 3> ctx, I it, const S last, maybe_fallback_t<I> /* fallback */)
    {
        if (it == last) [[unlikely]]
        {
            token_end_of_input(ctx, it, last);
            return false;
        }

        if constexpr (generation::enabled)
            ++ctx.osr.gen.current;

        if constexpr (not stateful::has_continue)
        {
            if constexpr (DFA.flags.maybe_no_empty and not never_empty
                          and DFA.match_start != DFA.additional_continue_nodes.back())
            {
                const bool empty{ ctx.osr.res.template force_get<0>().empty() };
                ctx.osr.res.reset(it);
                if (empty)
                    [[clang::musttail]] return initial_state<DFA.additional_continue_nodes.back()>(ctx, it, last, it);
            }
            else
            {
                ctx.osr.res.reset(it);
            }

            [[clang::musttail]] return initial_state<DFA.match_start>(ctx, it, last, it);
        }
        else if constexpr (DFA.flags.maybe_no_empty and not never_empty)
        {
            const stateful state_info{ std::exchange(ctx.osr.stf, {}) };
            const bool empty{ ctx.osr.res.template force_get<0>().empty() };
            ctx.osr.res.reset(it);

            template for (constexpr std::size_t i : std::views::iota(0uz, DFA.continue_nodes.size()))
            {
                if (i == state_info.continue_at)
                {
                    if constexpr (DFA.continue_nodes[i] != DFA.additional_continue_nodes[i])
                        if (empty)
                            [[clang::musttail]] return initial_state<DFA.continue_nodes[i]>(ctx, it, last, it);
                    [[clang::musttail]] return initial_state<DFA.additional_continue_nodes[i]>(ctx, it, last, it);
                }
            }
        }
        else
        {
            const stateful state_info{ std::exchange(ctx.osr.stf, {}) };
            ctx.osr.res.reset(it);

            template for (constexpr std::size_t i : std::views::iota(0uz, DFA.continue_nodes.size))
                if (i == state_info.continue_at)
                    [[clang::musttail]] return initial_state<DFA.continue_nodes[i]>(ctx, it, last, it);
        }

        return false;
    }

    /* end of input actions */

    template<std::size_t Alt, std::bidirectional_iterator I, std::sentinel_for<I> S, int X>
    [[gnu::always_inline]] static constexpr bool success(context<I, X> /* ctx */, I /* it */, const S /* last */, maybe_fallback_t<I> /* fallback */)
        requires is_regex
    {
        return true;
    }

    template<std::size_t Alt, std::bidirectional_iterator I, std::sentinel_for<I> S>
    static constexpr bool success(context<I, 3> ctx, I it, const S last, maybe_fallback_t<I> fallback)
        requires is_lexer
    {
        if constexpr (generation::enabled)
            clean_generations(ctx.osr);
        [[clang::musttail]] return lexer_action<[: Info :].actions[Alt]>(ctx, it, last, fallback);
    }

    template<std::bidirectional_iterator I, std::sentinel_for<I> S, int X>
    [[gnu::always_inline]] static constexpr bool failure(context<I, X> /* ctx */, I /* it */, const S /* last */, maybe_fallback_t<I> /* fallback */)
        requires is_regex
    {
        return false;
    }

    template<std::bidirectional_iterator I, std::sentinel_for<I> S>
    static constexpr bool failure(context<I, 3> ctx, I it, const S last, maybe_fallback_t<I> fallback)
        requires is_lexer
    {
        set_final<tdfa::no_transition_regops, 0>(ctx, it);
        [[clang::musttail]] return lexer_action<[: Info :].error_action>(ctx, it, last, fallback);
    }

    /* state fallback */

    template<std::bidirectional_iterator I, std::sentinel_for<I> S, int X>
        requires (DFA.flags.enable_fallback)
    static constexpr bool fallback_state(context<I, X> ctx, I it, const S last, fallback_info<I> fallback)
    {
        if (fallback.state == fallback_disabled)
            [[clang::musttail]] return failure(ctx, it, last, fallback);

        template for (constexpr auto pair : DFA.fallback_nodes)
        {
            if (fallback.state == pair.first)
            {
                static constexpr auto fni = DFA.final_nodes.at(pair.first);
                if constexpr (not DFA.flags.return_bool)
                    set_fallback<pair.second.op_index, fni.offset, pair.second.continue_at>(ctx, fallback.it);
                [[clang::musttail]] return success<fni.alternative>(ctx, it, last, fallback);
            }
        }

        [[clang::musttail]] return failure(ctx, it, last, fallback);
    }

    /* next state functions */

    template<std::size_t DFAState, std::bidirectional_iterator I, std::sentinel_for<I> S, int X>
    static constexpr bool state(context<I, X> ctx, I it, const S last, maybe_fallback_t<I> fallback)
    {
        static constexpr auto* final_node = DFA.final_nodes.at_if(DFAState);
        static constexpr auto* fallback_node = DFA.fallback_nodes.at_if(DFAState);

        if constexpr (DFA.flags.enable_fallback and fallback_node != nullptr)
        {
            fallback.state = DFAState;
            if constexpr (not DFA.flags.return_bool)
                fallback.it = it;
        }

        if (it != last)
        {
            template for (constexpr static_transition<char_type> tr : DFA.nodes[DFAState])
            {
                if (tr_possible<tr.cs>(*it))
                {
                    register_operations<tr.op_index>(ctx, it);
                    [[clang::musttail]] return state<tr.next>(ctx, ++it, last, fallback);
                }
            }

            if constexpr (final_node != nullptr and DFA.flags.enable_fallback and fallback_node != nullptr)
            {
                set_fallback<final_node->op_index, final_node->offset, fallback_node->continue_at>(ctx, it);
                [[clang::musttail]] return success<final_node->alternative>(ctx, it, last, fallback);
            }
        }
        else
        {
            if constexpr (final_node != nullptr)
            {
                if constexpr (DFA.flags.enable_fallback and fallback_node != nullptr)
                    set_fallback<final_node->op_index, final_node->offset, fallback_node->continue_at>(ctx, it);
                else
                    set_final<final_node->op_index, final_node->offset>(ctx, it);
                [[clang::musttail]] return success<final_node->alternative>(ctx, it, last, fallback);
            }
        }

        if constexpr (DFA.flags.enable_fallback and fallback_node == nullptr)
            [[clang::musttail]] return fallback_state(ctx, it, last, fallback);
        [[clang::musttail]] return failure(ctx, it, last, fallback);
    }

    template<std::size_t DFAState, std::bidirectional_iterator I, int X>
    static constexpr bool state(context<I, X> ctx, I it, const cstr_sentinel_t last, maybe_fallback_t<I> fallback)
    {
        static constexpr auto* final_node = DFA.final_nodes.at_if(DFAState);
        static constexpr auto* fallback_node = DFA.fallback_nodes.at_if(DFAState);

        if constexpr (DFA.flags.enable_fallback and fallback_node != nullptr)
        {
            fallback.state = DFAState;
            if constexpr (not DFA.flags.return_bool)
                fallback.it = it;
        }

        template for (constexpr static_transition<char_type> tr : DFA.nodes[DFAState])
        {
            if (tr_possible_exclude_null<tr.cs>(*it))
            {
                register_operations<tr.op_index>(ctx, it);
                [[clang::musttail]] return state<tr.next>(ctx, ++it, last, fallback);
            }
        }

        if constexpr (final_node != nullptr)
        {
            if constexpr (DFA.flags.enable_fallback and fallback_node != nullptr)
            {
                set_fallback<final_node->op_index, final_node->offset, fallback_node->continue_at>(ctx, it);
                [[clang::musttail]] return success<final_node->alternative>(ctx, it, last, fallback);
            }
            else
            {
                if (it == last) [[likely]]
                {
                    set_final<final_node->op_index, final_node->offset>(ctx, it);
                    [[clang::musttail]] return success<final_node->alternative>(ctx, it, last, fallback);
                }
            }
        }

        if constexpr (DFA.flags.enable_fallback and fallback_node == nullptr)
            [[clang::musttail]] return fallback_state(ctx, it, last, fallback);
        [[clang::musttail]] return failure(ctx, it, last, fallback);
    }

    template<std::size_t DFAState, std::size_t Count, std::bidirectional_iterator I, std::sized_sentinel_for<I> S, int X>
    static constexpr bool unchecked_state(context<I, X> ctx, I it, const S last, maybe_fallback_t<I> fallback)
    {
        if constexpr (Count == 0)
        {
            [[clang::musttail]] return state<DFAState>(ctx, it, last, fallback);
        }
        else
        {
            template for (constexpr static_transition<char_type> tr : DFA.nodes[DFAState])
            {
                if (tr_possible<tr.cs>(*it))
                {
                    register_operations<tr.op_index>(ctx, it);
                    [[clang::musttail]] return unchecked_state<tr.next, Count - 1>(ctx, ++it, last, fallback);
                }
            }

            [[clang::musttail]] return failure(ctx, it, last, fallback);
        }
    }

    template<std::size_t DFAState, std::size_t Count, integer_sequence_like Skip, std::bidirectional_iterator I, std::sized_sentinel_for<I> S, int X>
    static constexpr bool unchecked_state_skip(context<I, X> ctx, I it, const S last, maybe_fallback_t<I> fallback)
    {
        if constexpr (sequence_helper<Skip>::empty or Count == 0 or DFA.nodes[DFAState].size() != 1)
        {
            [[clang::musttail]] return unchecked_state<DFAState, Count>(ctx, it, last, fallback);
        }
        else if constexpr (Count == sequence_helper<Skip>::head)
        {
            static constexpr auto tr = DFA.nodes[DFAState].front();
            register_operations<tr.op_index>(ctx, it);
            [[clang::musttail]] return unchecked_state_skip<tr.next, Count - 1, typename sequence_helper<Skip>::tail>(ctx, ++it, last, fallback);
        }
        else
        {
            static constexpr auto tr = DFA.nodes[DFAState].front();
            if (tr_possible<tr.cs>(*it))
            {
                register_operations<tr.op_index>(ctx, it);
                [[clang::musttail]] return unchecked_state_skip<tr.next, Count - 1, Skip>(ctx, ++it, last, fallback);
            }

            [[clang::musttail]] return failure(ctx, it, last, fallback);
        }
    }

    /* next state function entry point */

    template<std::size_t DFAState, std::bidirectional_iterator I, std::sentinel_for<I> S, int X>
    static constexpr bool initial_state(context<I, X> ctx, I it, const S last, maybe_fallback_t<I> fallback)
    {
        static constexpr auto min_length = static_cast<std::ptrdiff_t>(DFA.min_max_lengths.first);

        if constexpr (not std::sized_sentinel_for<S, I>)
        {
            [[clang::musttail]] return state<DFAState>(ctx, it, last, fallback);
        }
        else if constexpr (fixed_length and not DFA.flags.enable_fallback)
        {
            if (std::ranges::distance(it, last) == min_length)
                [[clang::musttail]] return unchecked_state<DFAState, min_length>(ctx, it, last, fallback);
        }
        else
        {
            if (std::ranges::distance(it, last) >= min_length)
                [[clang::musttail]] return unchecked_state<DFAState, min_length>(ctx, it, last, fallback);
        }

        [[clang::musttail]] return failure(ctx, it, last, it);
    }

    /* outer dfa next state functions  */

    template<std::size_t DFAState, std::bidirectional_iterator I, std::sentinel_for<I> S, int X>
    static constexpr bool scalar_outer_state(context<I, X> ctx, I it, const S last)
    {
        if constexpr (generation::enabled)
            ++ctx.osr.gen.current;

        if (it != last)
        {
            if (state<DFAState>(ctx, it, last, it))
            {
                if constexpr (X != 0 and result<I>::has_match_start)
                    ctx.get_res().match_start_ = it;
                return true;
            }
        }
        else
        {
            if constexpr (static constexpr auto* final_node = DFA.final_nodes.at_if(DFAState); final_node != nullptr)
            {
                set_final<final_node->op_index, final_node->offset>(ctx, it);
                if constexpr (X != 0 and result<I>::has_match_start)
                     ctx.get_res().match_start_ = it;
                return true;
            }
            return false;
        }

        template for (constexpr static_transition<char_type> tr : DFA.outer_transitions)
        {
            if (tr_possible<tr.cs>(*it))
                [[clang::musttail]] return scalar_outer_state<tr.next>(ctx, ++it, last);
        }

        return false;
    }

    template<std::size_t DFAState, std::bidirectional_iterator I, std::sized_sentinel_for<I> S, int X>
    static constexpr bool scalar_outer_state(context<I, X> ctx, I it, const S last)
    {
        if constexpr (generation::enabled)
            ++ctx.osr.gen.current;

        static constexpr auto min_length = static_cast<std::ptrdiff_t>(DFA.min_max_lengths.first);

        if (std::ranges::distance(it, last) >= min_length)
        {
            if (unchecked_state<DFAState, min_length>(ctx, it, last, it))
            {
                if constexpr (X != 0 and result<I>::has_match_start)
                    ctx.get_res().match_start_ = it;
                return true;
            }
        }
        else
        {
            if constexpr (static constexpr auto* final_node = DFA.final_nodes.at_if(DFAState); final_node != nullptr)
            {
                set_final<final_node->op_index, final_node->offset>(ctx, it);
                if constexpr (X != 0 and result<I>::has_match_start)
                    ctx.get_res().match_start_ = it;
                return true;
            }
            return false;
        }

        template for (constexpr static_transition<char_type> tr : DFA.outer_transitions)
        {
            if (tr_possible<tr.cs>(*it))
                [[clang::musttail]] return scalar_outer_state<tr.next>(ctx, ++it, last);
        }

        return false;
    }

    template<std::size_t DFAState, std::bidirectional_iterator I, std::sized_sentinel_for<I> S, int X>
        requires (never_empty and DFA.continue_nodes.size() == 1 and DFA.continue_nodes[0] == DFAState
                  and (DFA.flags.enable_fallback or not fixed_length))
    static constexpr bool scalar_outer_state(context<I, X> ctx, I it, const S last)
    {
        static constexpr auto min_length = static_cast<std::ptrdiff_t>(DFA.min_max_lengths.first);

        while (std::ranges::distance(it, last) >= min_length)
        {
            if constexpr (generation::enabled)
                ++ctx.osr.gen.current;

            if (unchecked_state<DFAState, min_length>(ctx, it, last, it))
            {
                if constexpr (X != 0 and result<I>::has_match_start)
                    ctx.get_res().match_start_ = it;
                return true;
            }

            ++it;
        }

        /* since the DFA is never empty, we can skip assigning final node information */
        static_assert(DFA.final_nodes.at_if(DFAState) == nullptr);

        return false;
    }

    template<std::size_t DFAState, std::bidirectional_iterator I, std::sized_sentinel_for<I> S, int X>
        requires (never_empty and DFA.continue_nodes.size() == 1 and DFA.continue_nodes[0] == DFAState
                  and (/* degenerate case */ fixed_length and not DFA.flags.enable_fallback))
    static constexpr bool scalar_outer_state(context<I, X> ctx, I it, const S last)
    {
        static constexpr auto length = static_cast<std::ptrdiff_t>(DFA.min_max_lengths.first);

        if (const auto input_size = std::ranges::distance(it, last); input_size >= length)
        {
            std::ranges::advance(it, input_size - length);

            if (unchecked_state<DFAState, length>(ctx, it, last, it))
            {
                if constexpr (X != 0 and result<I>::has_match_start)
                    ctx.get_res().match_start_ = it;
                return true;
            }
        }

        /* since the DFA is never empty, we can skip assigning final node information */
        static_assert(DFA.final_nodes.at_if(DFAState) == nullptr);

        return false;
    }

    /* simd outer dfa next state functions */

    template<std::size_t DFAState, std::size_t Count, integer_sequence_like Skip,
             std::contiguous_iterator I, std::sized_sentinel_for<I> S, int X>
        requires (never_empty and DFA.continue_nodes.size() == 1 and DFA.continue_nodes[0] == DFAState)
    [[gnu::always_inline]] static constexpr bool vector_candidate_check(context<I, X> ctx, I it, const S last, unsigned long long mask)
    {
        while (mask != 0) [[unlikely]]
        {
            const auto offset = std::countr_zero(mask);
            mask &= (mask - 1);

            if (unchecked_state_skip<DFAState, Count, Skip>(ctx, it + offset, last, it + offset))
            {
                if constexpr (X != 0 and result<I>::has_match_start)
                    ctx.get_res().match_start_ = it + offset;
                return true;
            }
            else
            {
                if constexpr (generation::enabled)
                    ++ctx.osr.gen.current;
            }
        }

        return false;
    }

    template<std::size_t DFAState, std::contiguous_iterator I, std::sized_sentinel_for<I> S, int X>
        requires (never_empty and DFA.continue_nodes.size() == 1 and DFA.continue_nodes[0] == DFAState
                  and fixed_length and branch_free)
    static constexpr bool vector_outer_state(context<I, X> ctx, I it, const S last)
    {
        static constexpr auto length = static_cast<std::ptrdiff_t>(DFA.min_max_lengths.first);
        static constexpr auto states = define_static_array(get_flattened_states(DFA, DFAState));

        using uchar_type = std::make_unsigned_t<char_type>;
        using vec_type = std::simd::vec<uchar_type>;
        using mask_type = vec_type::mask_type;

        static constexpr auto flags = [] consteval {
            if constexpr (not std::same_as<char_type, uchar_type>)
                return std::simd::flag_convert;
            else
                return std::simd::flag_default;
        }();

        static constexpr auto [position1, position2] = get_outer_state_position_pair(DFA, states);
        using skipped = std::index_sequence<length - position1, length - position2>;

        static constexpr bool avoid_simd = [] consteval {
            std::size_t count1{ DFA.nodes[states[position1]].front().cs.count() };
            std::size_t count2{ DFA.nodes[states[position2]].front().cs.count() };
            return std::saturating_mul(count1, count2)  > std::saturating_mul(avoid_simd_threshold, avoid_simd_threshold);
        }();

        if constexpr (avoid_simd)
        {
            /* using simd in this case is likely a pessimisation; resort to non-vectorised implementation instead */
            return scalar_outer_state<DFAState>(ctx, it, last);
        }
        else
        {
            const std::ptrdiff_t max{ std::ranges::distance(it, last) - (length - 1) };

            for (std::ptrdiff_t i{ max / vec_type::size() }; i > 0; --i) [[likely]]
            {
                const auto block1 = std::simd::unchecked_load<vec_type>(it + position1, last, flags);
                const auto block2 = std::simd::unchecked_load<vec_type>(it + position2, last, flags);

                const auto mask1 = vector_tr_find<DFA.nodes[states[position1]].front().cs>(block1);
                const auto mask2 = vector_tr_find<DFA.nodes[states[position2]].front().cs>(block2);

                if (vector_candidate_check<DFAState, length, skipped>(ctx, it, last, (mask1 & mask2).to_ullong()))
                    return true;

                it += vec_type::size();
            }

            if (const auto epi_size = max % vec_type::size(); epi_size > 0) [[likely]]
            {
                const mask_type epi_mask{ (1uz << epi_size) - 1 };

#if false
                /* bug involving non-mask overloads of simd::partial_load */
                const auto block1 = std::simd::partial_load<vec_type>(it + position1, epi_size, flags);
                const auto block2 = std::simd::partial_load<vec_type>(it + position2, epi_size, flags);
#else
                const auto block1 = std::simd::partial_load<vec_type>(it + position1, epi_size, epi_mask, flags);
                const auto block2 = std::simd::partial_load<vec_type>(it + position2, epi_size, epi_mask, flags);
#endif

                const auto mask1 = vector_tr_find<DFA.nodes[states[position1]].front().cs>(block1);
                const auto mask2 = vector_tr_find<DFA.nodes[states[position2]].front().cs>(block2);

                return vector_candidate_check<DFAState, length, skipped>(ctx, it, last, (mask1 & mask2 & epi_mask).to_ullong());
            }

            return false;
        }
    }

    template<std::size_t DFAState, std::contiguous_iterator I, std::sized_sentinel_for<I> S, int X>
        requires (never_empty and DFA.continue_nodes.size() == 1 and DFA.continue_nodes[0] == DFAState
                  and (not fixed_length or not branch_free))
    static constexpr bool vector_outer_state(context<I, X> ctx, I it, const S last)
    {
        static constexpr auto min_length = static_cast<std::ptrdiff_t>(DFA.min_max_lengths.first);
        static constexpr auto states = define_static_array(get_flattened_states(DFA, DFAState, true));

        using uchar_type = std::make_unsigned_t<char_type>;
        using vec_type = std::simd::vec<uchar_type>;
        using mask_type = vec_type::mask_type;

        static constexpr auto flags = []{
            if constexpr (not std::same_as<char_type, uchar_type>)
                return std::simd::flag_convert;
            else
                return std::simd::flag_default;
        }();

        static constexpr auto position = get_outer_state_position_single(DFA, states);
        using skipped = std::index_sequence<min_length - position>;

        static constexpr static_charset combined_cs = []{
            charset<char_type> result;
            template for (constexpr auto tr : DFA.nodes[position])
                result |= tr.cs;
            return static_charset{ result };
        }();

        static constexpr bool avoid_simd{ combined_cs.count() > avoid_simd_threshold };

        if constexpr (avoid_simd)
        {
            /* using simd in this case is likely a pessimisation; resort to non-vectorised implementation instead */
            return scalar_outer_state<DFAState>(ctx, it, last);
        }
        else
        {
            const std::ptrdiff_t max{ std::ranges::distance(it, last) - (min_length - 1) };

            for (std::ptrdiff_t i{ max / vec_type::size() }; i > 0; --i) [[likely]]
            {
                const auto block = std::simd::unchecked_load<vec_type>(it + position, last, flags);
                const auto mask = vector_tr_find<combined_cs>(block);

                if (vector_candidate_check<DFAState, min_length, skipped>(ctx, it, last, mask.to_ullong()))
                    return true;

                it += vec_type::size();
            }

            if (const auto epi_size = max % vec_type::size(); epi_size > 0) [[likely]]
            {
                const mask_type epi_mask{ (1uz << epi_size) - 1 };

#if false
                /* bug involving non-mask overloads of simd::partial_load */
                const auto block = std::simd::partial_load<vec_type>(it + position, epi_size, flags);
#else
                const auto block = std::simd::partial_load<vec_type>(it + position, epi_size, epi_mask, flags);
#endif
                const auto mask = vector_tr_find<combined_cs>(block);

                return vector_candidate_check<DFAState, min_length, skipped>(ctx, it, last, (mask & epi_mask).to_ullong());
            }

            return false;
        }
    }

    /* outer state function dispatch */

    template<std::size_t DFAState, std::bidirectional_iterator I, std::sentinel_for<I> S, int X>
    static constexpr bool outer_state(context<I, X> ctx, I it, const S last)
    {
        if constexpr (std::contiguous_iterator<I> and std::sized_sentinel_for<S, I>
                      and never_empty and DFA.continue_nodes.size() == 1 and DFA.continue_nodes[0] == DFAState)
            return vector_outer_state<DFAState>(ctx, it, last);
        else
            return scalar_outer_state<DFAState>(ctx, it, last);
    }

    static constexpr bool regex_return_bool{ is_regex and DFA.flags.return_bool };
    static constexpr bool regex_normal{ is_regex and not DFA.flags.return_bool };
    static constexpr bool regex_nonempty{ regex_normal and DFA.flags.maybe_no_empty };
    static constexpr bool lexer_normal{ is_lexer };
    static constexpr bool lexer_nonempty{ lexer_normal and DFA.flags.maybe_no_empty };

public:
    template<std::bidirectional_iterator I, std::sentinel_for<I> S>
        requires regex_return_bool and stateful::is_stateless
    [[nodiscard]] static constexpr bool operator()(const I first, const S last)
    {
        if constexpr (DFA.flags.adapted_search)
            return outer_state<DFA.match_start>(context<I, 0>{}, first, last);
        else
            return initial_state<DFA.match_start>(context<I, 0>{}, first, last, first);
    }

    template<std::bidirectional_iterator I, std::sentinel_for<I> S>
        requires regex_normal and stateful::is_stateless
    [[nodiscard]] static constexpr result<I> operator()(const I first, const S last)
    {
        result<I> res{ first };
        overspill<I> osp{};

        if constexpr (DFA.flags.adapted_search)
            outer_state<DFA.match_start>(context{ res, osp }, first, last);
        else
            initial_state<DFA.match_start>(context{ res, osp }, first, last, first);

        if constexpr (generation::enabled)
            clean_generations(res, osp);
        return res;
    }

    template<std::bidirectional_iterator I, std::sentinel_for<I> S>
        requires regex_nonempty and stateful::is_stateless
    [[nodiscard]] static constexpr result<I> operator()(const I first, const S last, match_non_empty_t)
    {
        result<I> res{ first };
        overspill<I> osp{};

        static constexpr auto start_state{ never_empty ? DFA.match_start : DFA.additional_continue_nodes.back() };

        if constexpr (DFA.flags.adapted_search)
            outer_state<start_state>(context{ res, osp }, first, last);
        else
            initial_state<start_state>(context{ res, osp }, first, last, first);

        if constexpr (generation::enabled)
            clean_generations(res, osp);
        return res;
    }

    template<std::bidirectional_iterator I, std::sentinel_for<I> S>
        requires lexer_normal and stateful::is_stateless
    [[nodiscard]] static constexpr token<I> operator()(I& first, const S last)
    {
        token<I> tok;
        overspill<I, true> osp{ first };

        if (first == last) [[unlikely]]
        {
            token_end_of_input(context{ tok, osp }, first, last);
            return tok;
        }

        initial_state<DFA.match_start>(context{ tok, osp }, first, last, first);
        first = osp.res.match_end_;
        return tok;
    }

    template<std::bidirectional_iterator I, std::sentinel_for<I> S>
        requires lexer_normal and result<I>::has_continue
    [[nodiscard]] static constexpr token<I> operator()(I& first, const S last, const tdfa::continue_at_t continue_at)
    {
        token<I> tok;
        overspill<I, true> osp{ first };

        if (first == last) [[unlikely]]
        {
            token_end_of_input(context{ tok, osp }, first, last);
            return tok;
        }

        template for (constexpr std::size_t i : std::views::iota(0uz, DFA.continue_nodes.size()))
        {
            if (i == continue_at)
            {
                initial_state<DFA.continue_nodes[i]>(context{ tok, osp }, first, last, first);
#ifndef __GNUC_MINOR__
                break;
#else
                first = osp.res.match_end_;
                return tok;
#endif
            }
        }

        first = osp.res.match_end_;
        return tok;
    }

    template<std::bidirectional_iterator I, std::sentinel_for<I> S>
        requires lexer_nonempty and stateful::is_stateless
    [[nodiscard]] static constexpr token<I> operator()(I& first, const S last, match_non_empty_t)
    {
        static constexpr auto start_state{ never_empty ? DFA.match_start : DFA.additional_continue_nodes.back() };

        token<I> tok;
        overspill<I, true> osp{ first };

        if (first == last) [[unlikely]]
        {
            token_end_of_input(context{ tok, osp }, first, last);
            return tok;
        }

        initial_state<start_state>(context{ tok, osp }, first, last, first);
        first = osp.res.match_end_;
        return tok;
    }

    template<std::bidirectional_iterator I, std::sentinel_for<I> S>
        requires lexer_nonempty and result<I>::has_continue
    [[nodiscard]] static constexpr token<I> operator()(I& first, const S last, const tdfa::continue_at_t continue_at, match_non_empty_t)
    {
        token<I> tok;
        overspill<I, true> osp{ first };

        if (first == last) [[unlikely]]
        {
            token_end_of_input(context{ tok, osp }, first, last);
            return tok;
        }

        template for (constexpr std::size_t i : std::views::iota(0uz, DFA.continue_nodes.size()))
        {
            if (i == continue_at)
            {
                static constexpr auto start_state{ never_empty ? DFA.continue_nodes[i] : DFA.additional_continue_nodes[i] };
                initial_state<start_state>(context{ tok, osp }, first, last, first);
#ifndef __GNUC_MINOR__
                break;
#else
                first = osp.res.match_end_;
                return tok;
#endif
            }
        }

        first = osp.res.match_end_;
        return tok;
    }
};

template<std::meta::info Info>
template<std::bidirectional_iterator I>
    requires std::is_nothrow_convertible_v<std::iter_value_t<I>, typename p1306dfa<Info>::char_type>
struct p1306dfa<Info>::iterated_result
{
    static constexpr bool needs_begin{ not never_empty };
    using state_type = stateful;

    iterated_result() = default;

    constexpr iterated_result(const I first, const std::sentinel_for<I> auto last)
        : res{ first }
    {
        start(first, last);
    }

    constexpr iterated_result(const I first, const std::sentinel_for<I> auto last, match_non_empty_t)
        : res{ first }
    {
        start<true>(first, last);
    }

    constexpr iterated_result(const I /* first */, const I it, const std::sentinel_for<I> auto last, stateful stf, bool prev_empty)
        : res{ it }, stf{ stf }
    {
        if constexpr (not never_empty)
        {
            if (prev_empty)
            {
                resume<true>(it, last);
                return;
            }
        }
        resume(it, last);
    }

    constexpr I advance(const std::sentinel_for<I> auto last)
        requires never_empty
    {
        const auto& match = res.template force_get<0>();
        const I current = match.end();
        res.reset(current);
        resume(current, last);
        return current;
    }

    constexpr I advance(const I first, const std::sentinel_for<I> auto last)
    {
        const auto& match = res.template force_get<0>();
        const I current = match.end();

        if constexpr (not never_empty)
        {
            if (match.empty())
            {
                if (current == last)
                {
                    res.clear_match();
                    return current;
                }

                res.reset(current);
                if (current == first)
                    start<true>(current, last);
                else
                    resume<true>(current, last);
                return current;
            }
        }

        res.reset(current);
        resume(current, last);
        return current;
    }

private:
    template<bool NonEmptyMatch = false>
    constexpr void start(const I first, const std::sentinel_for<I> auto last)
    {
        static constexpr bool use_alt{ NonEmptyMatch and not never_empty };
        static constexpr auto start_state{ use_alt ? DFA.additional_continue_nodes.back() : DFA.match_start };

        overspill<I> osp{};

        if constexpr (DFA.flags.adapted_search)
            outer_state<start_state>(context{ *this, osp }, first, last);
        else
            initial_state<start_state>(context{ *this, osp }, first, last, first);

        if constexpr (generation::enabled)
            clean_generations(res, osp);
    }

    template<bool NonEmptyMatch = false>
    constexpr void resume(const I first, const std::sentinel_for<I> auto last)
    {
        if constexpr (not stateful::has_continue)
        {
            return start<NonEmptyMatch>(first, last);
        }
        else
        {
            static constexpr bool use_alt{ NonEmptyMatch and not never_empty };
            const stateful state_info{ std::exchange(stf, {}) };
            overspill<I> osp{};

            template for (constexpr std::size_t i : std::views::iota(0uz, DFA.continue_nodes.size()))
            {
                static constexpr auto start_state{ use_alt ? DFA.additional_continue_nodes[i] : DFA.continue_nodes[i] };

                if (state_info.continue_at == i)
                {
                    if constexpr (DFA.flags.adapted_search)
                        outer_state<start_state>(context{ *this, osp }, first, last);
                    else
                        initial_state<start_state>(context{ *this, osp }, first, last, first);
#ifndef __GNUC_MINOR__
                    break;
#else
                    if constexpr (generation::enabled)
                        clean_generations(res, osp);
                    return;
#endif
                }
            }

            if constexpr (generation::enabled)
                clean_generations(res, osp);
            return;
        }
    }

public:
    result<I> res;
    stateful stf;
};

template<string_literal Pattern, fsm_flags Flags>
using p1306_matcher = p1306dfa<(^^re<std::meta::reflect_constant_string(Pattern.view()), pack_flags(Flags)>)>;

template<string_literal Pattern, fsm_flags Flags>
using p1306_searcher = p1306dfa<(^^re<std::meta::reflect_constant_string(Pattern.view()), pack_flags(adapt_searcher_flags_to_matcher(Flags))>)>;

}
}

namespace srx {

enum class mode : unsigned char
{
    standard,
    linear,
    backtrack
};

namespace detail {

consteval auto get_matcher_refl(mode i, bool is_search = false)
{
    if (i == mode::backtrack)
        return ^^detail::naive_matcher_adaptor;
    else if (i == mode::standard and is_search)
        return ^^detail::p1306_searcher;
    else
        return ^^detail::p1306_matcher;
}

}

template<string_literal Pattern, mode Mode = mode::standard>
struct static_regex
{
    using char_type = decltype(Pattern)::value_type;
    static_assert(std::same_as<char, char_type>); /* temporary: remove later */

    template<std::bidirectional_iterator I, std::sentinel_for<I> S>
        requires std::same_as<std::iter_value_t<I>, char_type>
    [[nodiscard]] static constexpr auto match(I first, S last)
    {
        using namespace detail::default_fsm_flags;
        using matcher_t = [: detail::get_matcher_refl(Mode) :]<Pattern, full_match>;
        return matcher_t{}(first, last);
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
    [[nodiscard]] static constexpr auto prefix_match(I first, S last)
    {
        using namespace detail::default_fsm_flags;
        using matcher_t = [: detail::get_matcher_refl(Mode) :]<Pattern, partial_match>;
        return matcher_t{}(first, last);
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
    [[nodiscard]] static constexpr auto search(I first, S last)
    {
        using namespace detail::default_fsm_flags;
        using matcher_t = [: detail::get_matcher_refl(Mode, true) :]<Pattern, search_single>;
        return matcher_t{}(first, last);
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
    [[nodiscard]] static constexpr bool is_match(I first, S last)
    {
        using namespace detail::default_fsm_flags;
        using matcher_t = [: detail::get_matcher_refl(Mode) :]<Pattern, detail::adapt_flags_return_bool(full_match)>;
        return matcher_t{}(first, last);
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
    [[nodiscard]] static constexpr bool starts_with_match(I first, S last)
    {
        using namespace detail::default_fsm_flags;
        using matcher_t = [: detail::get_matcher_refl(Mode) :]<Pattern, detail::adapt_flags_return_bool(partial_match)>;
        return matcher_t{}(first, last);
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
    [[nodiscard]] static constexpr bool contains_match(I first, S last)
    {
        using namespace detail::default_fsm_flags;
        using matcher_t = [: detail::get_matcher_refl(Mode, true) :]<Pattern, detail::adapt_flags_return_bool(search_single)>;
        return matcher_t{}(first, last);
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

    template<std::bidirectional_iterator I, std::sentinel_for<I> S>
        requires std::same_as<std::iter_value_t<I>, char_type>
    [[nodiscard]] static constexpr auto range(I first, S last)
    {
        return range(std::ranges::subrange(first, last));
    }

    template<std::ranges::bidirectional_range R>
        requires std::same_as<std::ranges::range_value_t<R>, char_type>
    [[nodiscard]] static constexpr auto range(R&& r);

    template<typename CharT>
        requires std::same_as<CharT, char_type>
    [[nodiscard]] static constexpr auto range(const CharT* cstr)
    {
        return range(std::ranges::subrange(cstr, detail::cstr_sentinel));
    }

    template<std::bidirectional_iterator I, std::sentinel_for<I> S>
        requires std::same_as<std::iter_value_t<I>, char_type>
    [[nodiscard]] static constexpr auto sequential_range(I first, S last)
    {
        return range(std::ranges::subrange(first, last));
    }

    template<std::ranges::bidirectional_range R>
        requires std::same_as<std::ranges::range_value_t<R>, char_type>
    [[nodiscard]] static constexpr auto sequential_range(R&& r);

    template<typename CharT>
        requires std::same_as<CharT, char_type>
    [[nodiscard]] static constexpr auto sequential_range(const CharT* cstr)
    {
        return range(std::ranges::subrange(cstr, detail::cstr_sentinel));
    }
};

template<string_literal Pattern, mode Mode = mode::standard>
inline constexpr static_regex<Pattern, Mode> srx;

namespace detail {

template<typename R>
concept static_regex_match_view_like = is_template_instantiation_of_impl(^^std::ranges::range_value_t<R>, ^^static_match_results);

template<typename Regex>
concept static_regex_like = is_template_instantiation_of_impl(^^Regex, ^^static_regex);

template<typename Regex>
concept regex_like = static_regex_like<Regex>;

template<typename Regex, typename CharT>
concept typed_static_regex_like = static_regex_like<Regex> and std::same_as<CharT, typename Regex::char_type>;

template<typename Regex, typename CharT>
concept typed_regex_like = typed_static_regex_like<Regex, CharT> ;

template<typename F, bool PermitNonBorrowedRange = false>
struct static_regex_free_function
{
    /* Reflecting overload sets is currently not possible,
       so we pass a lambda as a template parameter instead */

    template<std::bidirectional_iterator I, std::sentinel_for<I> S>
    static constexpr auto operator()(I first, S last)
    {
        return std::invoke(F{}, first, last);
    }

    template<std::ranges::bidirectional_range R>
        requires PermitNonBorrowedRange or std::ranges::borrowed_range<R>
    static constexpr auto operator()(R&& r)
    {
        return std::invoke(F{}, std::forward<R>(r));
    }

    template<typename CharT>
    static constexpr auto operator()(const CharT* cstr)
    {
        return std::invoke(F{}, cstr);
    }
};

}

template<string_literal Pattern, mode Mode = mode::standard>
inline constexpr auto static_regex_match =
    detail::static_regex_free_function<decltype([]<typename... Ts>(Ts&&... args) static {
        return static_regex<Pattern, Mode>::match(std::forward<Ts>(args)...);
    })>{};

template<string_literal Pattern, mode Mode = mode::standard>
inline constexpr auto static_regex_prefix_match =
    detail::static_regex_free_function<decltype([]<typename... Ts>(Ts&&... args) static {
        return static_regex<Pattern, Mode>::prefix_match(std::forward<Ts>(args)...);
    })>{};

template<string_literal Pattern, mode Mode = mode::standard>
inline constexpr auto static_regex_search =
    detail::static_regex_free_function<decltype([]<typename... Ts>(Ts&&... args) static {
        return static_regex<Pattern, Mode>::search(std::forward<Ts>(args)...);
    })>{};

template<string_literal Pattern, mode Mode = mode::standard>
inline constexpr auto static_regex_search_all =
    detail::static_regex_free_function<decltype([]<typename... Ts>(Ts&&... args) static {
        return static_regex<Pattern, Mode>::range(std::forward<Ts>(args)...);
    }), true>{};

template<string_literal Pattern, mode Mode = mode::standard>
inline constexpr auto static_regex_match_consecutive =
    detail::static_regex_free_function<decltype([]<typename... Ts>(Ts&&... args) static {
        return static_regex<Pattern, Mode>::sequential_range(std::forward<Ts>(args)...);
    }), true>{};

namespace literals {

template<string_literal Pattern>
consteval static_regex<Pattern> operator ""_srx()
{
    return {};
}

}
}

namespace srx {
namespace detail {

template<std::bidirectional_iterator I, std::sentinel_for<I> S, typename Regex>
class stashing_regex_iterator;

template<std::bidirectional_iterator I, std::sentinel_for<I> S, string_literal Pattern, mode Mode>
class stashing_regex_iterator<I, S, static_regex<Pattern, Mode>>
{
    using matcher_type  = [: detail::get_matcher_refl(Mode, true) :]<Pattern, detail::default_fsm_flags::search_all>;
    using result_type   = matcher_type::template iterated_result<I>;

public:
    using iterator_concept  = std::input_iterator_tag;
    using iterator_category = std::input_iterator_tag;
    using value_type        = matcher_type::template result<I>;;
    using difference_type   = std::ptrdiff_t;

    stashing_regex_iterator() requires std::default_initializable<I> and std::default_initializable<S> = default;

    constexpr explicit stashing_regex_iterator(I first, S last)
        : result_{ first, last }, first_{ std::move(first) }, last_{ std::move(last) } {}

    constexpr const value_type& operator*() const noexcept
    {
        return result_.res;
    }

    constexpr const value_type* operator->() const noexcept
    {
        return &result_.res;
    }

    constexpr stashing_regex_iterator& operator++()
    {
        if (not result_.res)
            return *this;

        if constexpr (result_type::needs_begin)
            result_.advance(first_, last_);
        else
            result_.advance(last_);

        return *this;
    }

    constexpr void operator++(int)
    {
        ++*this;
    }

    friend constexpr bool operator==(const stashing_regex_iterator& x, std::default_sentinel_t)
    {
        return not x.result_.res.has_value();
    }

    template<std::ranges::input_range W, int...>
        requires std::ranges::view<W>
    friend class submatches_view;

private:
    using maybe_it = maybe_type_t<result_type::needs_begin, I>;

    result_type result_;
    [[no_unique_address]] maybe_it first_;
    [[no_unique_address]] S last_;
    [[no_unique_address]] matcher_type matcher_;
};

template<typename CharT>
consteval auto replace_fmt_pattern()
{
    if constexpr (std::same_as<CharT, char>)
        return string_literal{ R"(\$(?:[&`'+$]|\d+))" };
    else if constexpr (std::same_as<CharT, wchar_t>)
        return string_literal{ LR"(\$(?:[&`'+$]|\d+))" };
    else if constexpr (std::same_as<CharT, char8_t>)
        return string_literal{ u8R"(\$(?:[&`'+$]|\d+))" };
    else if constexpr (std::same_as<CharT, char16_t>)
        return string_literal{ uR"(\$(?:[&`'+$]|\d+))" };
    else if constexpr (std::same_as<CharT, char32_t>)
        return string_literal{ UR"(\$(?:[&`'+$]|\d+))" };
    else
        return;
}

template<std::bidirectional_iterator I, std::sentinel_for<I> S>
constexpr std::size_t parse_unsigned(I first, const S last, const std::size_t submatch_count)
{
    static constexpr std::size_t base{ 10 };
    std::size_t result{ 0 };

    while (first != last)
    {
        result = std::saturating_add(std::saturating_mul(result, base), static_cast<std::size_t>(*first - '0'));
        ++first;
    }

    if (result >= submatch_count)
        throw regex_error("replace_fmt: invalid replacement");

    return result;
}

namespace replace_constants {

enum replace_constant : std::size_t
{
    match       =  0uz,
    prematch    = -1uz,
    postmatch   = -2uz,
    skip        = -3uz,
};

}

template<std::bidirectional_iterator I, std::sentinel_for<I> S>
constexpr std::size_t parse_fmt_replace(I first, const S last, const std::size_t submatch_count)
{
    if (first == last)
        throw regex_error("replace_fmt: invalid replacement");

    if (*first == '&')
        return replace_constants::match;
    else if (*first == '`')
        return replace_constants::prematch;
    else if (*first == '\'')
        return replace_constants::postmatch;
    else if (*first == '+')
        return submatch_count - 1;
    else if (*first == '$')
        return replace_constants::skip;
    else
        return parse_unsigned(first, last, submatch_count);
}

template<std::bidirectional_iterator I>
class replace_fmt
{
public:
    using char_type = std::iter_value_t<I>;
    using subrange_type = std::ranges::subrange<I>;

    constexpr replace_fmt() requires std::default_initializable<I>
        : subranges_{ subrange_type{} }, captures_{} {}

    template<std::sentinel_for<I> S>
    constexpr replace_fmt(I first, S last, const std::size_t submatch_count)
    {
        using matcher_type = [: detail::get_matcher_refl(mode::standard, true) :]<detail::replace_fmt_pattern<char_type>(), detail::default_fsm_flags::search_all>;
        using result_type = matcher_type::template iterated_result<I>;

        result_type delim_matcher_result{ first, last };

        while (delim_matcher_result.res)
        {
            const auto& [mfirst, mlast] = get<0>(delim_matcher_result.res);
            const auto& cap = captures_.emplace_back(detail::parse_fmt_replace(std::ranges::next(mfirst), mlast, submatch_count));

            if (cap == replace_constants::skip) /* treat $$ in format as single $ */
                subranges_.emplace_back(first, std::ranges::next(mfirst));
            else
                subranges_.emplace_back(first, mfirst);

            first = mlast;
            delim_matcher_result.advance(last);
        }

        if constexpr (std::same_as<I, S>)
            subranges_.emplace_back(first, last);
        else
            subranges_.emplace_back(first, std::ranges::next(first, last));
    }

    constexpr auto zipped() const
    {
        return std::views::zip(subranges_, captures_);
    }

    constexpr const auto& trailing() const
    {
        return subranges_.back();
    }

    constexpr const auto& subranges() const noexcept
    {
        return subranges_;
    }

    constexpr const auto& captures() const noexcept
    {
        return captures_;
    }

private:
    std::vector<subrange_type> subranges_;
    std::vector<std::size_t> captures_;
};

template<typename CharT>
class static_replace_fmt
{
public:
    using char_type = CharT;
    using subrange_type = detail::static_span<char_type>;

    explicit consteval static_replace_fmt(std::basic_string_view<CharT> sv, const std::size_t match_count)
        : static_replace_fmt(replace_fmt{ sv.begin(), sv.end(), match_count }) {}

    constexpr auto zipped() const
    {
        return std::views::zip(subranges_, captures_);
    }

    constexpr const auto& trailing() const
    {
        return subranges_.back();
    }

    constexpr const auto& subranges() const noexcept
    {
        return subranges_;
    }

    constexpr const auto& captures() const noexcept
    {
        return captures_;
    }

private:
    explicit consteval static_replace_fmt(const replace_fmt<const char_type*>& fmt)
        : subranges_{ fmt.subranges() | std::views::transform(make_subrange) }
        , captures_{ fmt.captures() } {}

    static consteval subrange_type make_subrange(const std::ranges::subrange<const char_type*>& s)
    {
        return s;
    }

    detail::static_span<subrange_type> subranges_;
    detail::static_span<std::size_t> captures_;
};

}

template<string_literal Fmt>
struct fmt_t {};

template<string_literal Fmt>
inline constexpr fmt_t<Fmt> fmt;

template<class I, class O>
using regex_replace_result = std::ranges::in_out_result<I, O>;

namespace detail {

struct replace_impl
{
private:
    template<typename CharT, std::bidirectional_iterator I, std::sentinel_for<I> S, std::output_iterator<CharT> O,
             string_literal Pattern, mode Mode, std::bidirectional_iterator F, std::sentinel_for<F> FmtS>
    static constexpr regex_replace_result<I, O>
    impl(I first, const S last, O result, static_regex<Pattern, Mode> /* regex */, F fmt_first, FmtS fmt_last)
    {
        using iterator_type = stashing_regex_iterator<I, S, static_regex<Pattern, Mode>>;
        using sentinel_type = std::default_sentinel_t;
        using result_type = iterator_type::value_type;

        const replace_fmt fmt{ fmt_first, fmt_last, result_type::submatch_count };

        iterator_type it{ first, last };
        sentinel_type end{};

        for (; it != end; ++it)
        {
            auto [mfirst, mlast] = get<0>(*it);

            result = std::ranges::copy(first, mfirst, result).out;

            for (const auto& [substr, idx] : fmt.zipped())
            {
                result = std::ranges::copy(substr, result).out;

                if (idx == replace_constants::match)
                    result = std::ranges::copy(mfirst, mlast, result).out;
                else if (idx == replace_constants::prematch)
                    result = std::ranges::copy(first, mfirst, result).out;
                else if (idx == replace_constants::postmatch)
                    result = std::ranges::copy(mlast, last, result).out;
                else if (idx != replace_constants::skip)
                    result = std::ranges::copy(it->at(idx), result).out;
            }

            result = std::ranges::copy(fmt.trailing(), result).out;
            first = mlast;
        }

        return std::ranges::copy(first, last, result);
    }

    template<typename CharT, std::bidirectional_iterator I, std::sentinel_for<I> S, std::output_iterator<CharT> O,
             string_literal Pattern, mode Mode, string_literal Fmt>
    static constexpr regex_replace_result<I, O>
    impl(I first, const S last, O result, static_regex<Pattern, Mode> /* regex */, fmt_t<Fmt>)
    {
        using iterator_type = stashing_regex_iterator<I, S, static_regex<Pattern, Mode>>;
        using sentinel_type = std::default_sentinel_t;
        using result_type = iterator_type::value_type;

        static constexpr static_replace_fmt fmt{ Fmt.view(), result_type::submatch_count };

        iterator_type it{ first, last };
        sentinel_type end{};

        for(; it != end; ++it)
        {
            auto [mfirst, mlast] = get<0>(*it);
            result = std::ranges::copy(first, mfirst, result).out;

            template for (constexpr auto pair : fmt.zipped())
            {
                constexpr std::size_t idx{ get<1>(pair) };
                result = std::ranges::copy(get<0>(pair), result).out;

                if constexpr (idx == replace_constants::match)
                    result = std::ranges::copy(mfirst, mlast, result).out;
                else if constexpr (idx == replace_constants::prematch)
                    result = std::ranges::copy(first, mfirst, result).out;
                else if constexpr (idx == replace_constants::postmatch)
                    result = std::ranges::copy(mlast, last, result).out;
                else if constexpr (idx != replace_constants::skip)
                    result = std::ranges::copy(get<idx>(*it), result).out;
            }

            result = std::ranges::copy(fmt.trailing(), result).out;
            first = mlast;
        }

        return std::ranges::copy(first, last, result);
    }

public:
    template<std::bidirectional_iterator I, std::sentinel_for<I> S, std::output_iterator<std::iter_value_t<I>> O, regex_like Regex,
             std::bidirectional_iterator F, std::sentinel_for<F> FmtS, std::same_as<std::iter_value_t<I>> CharT = Regex::char_type>
        requires std::same_as<std::iter_value_t<F>, CharT>
    static constexpr regex_replace_result<I, O>
    operator()(I first, S last, O result, Regex pattern, F fmt, FmtS fmt_last)
    {
        return impl<CharT>(first, last, result, pattern, fmt, fmt_last);
    }

    template<std::ranges::bidirectional_range R, std::output_iterator<std::ranges::range_value_t<R>> O,
             regex_like Regex, std::ranges::bidirectional_range FmtR, std::same_as<std::ranges::range_value_t<R>> CharT = Regex::char_type>
        requires std::same_as<std::ranges::range_value_t<FmtR>, CharT> and (not std::convertible_to<FmtR, std::basic_string_view<CharT>>)
    static constexpr regex_replace_result<std::ranges::borrowed_iterator_t<R>, O>
    operator()(R&& r, O result, Regex pattern, FmtR&& fmt)
    {
        return operator()(std::ranges::begin(r), std::ranges::end(r), result, pattern, std::ranges::begin(fmt), std::ranges::end(fmt));
    }

    template<std::bidirectional_iterator I, std::sentinel_for<I> S, std::output_iterator<std::iter_value_t<I>> O,
             regex_like Regex, std::same_as<std::iter_value_t<I>> CharT>
    static constexpr regex_replace_result<I, O>
    operator()(I first, S last, O result, Regex pattern, std::basic_string_view<CharT> fmt)
    {
        return operator()(first, last, result, pattern, fmt.begin(), fmt.end());
    }

    template<std::ranges::bidirectional_range R, std::output_iterator<std::ranges::range_value_t<R>> O,
             regex_like Regex, std::same_as<std::ranges::range_value_t<R>> CharT>
    static constexpr regex_replace_result<std::ranges::borrowed_iterator_t<R>, O>
    operator()(R&& r, O result, Regex pattern, std::basic_string_view<CharT> fmt)
    {
        return operator()(std::ranges::begin(r), std::ranges::end(r), result, pattern, fmt.begin(), fmt.end());
    }

    template<std::ranges::contiguous_range CR, regex_like Regex,
             std::same_as<std::ranges::range_value_t<CR>> CharT = Regex::char_type>
    static constexpr std::basic_string<CharT>
    operator()(CR&& cr, Regex pattern, std::basic_string_view<std::type_identity_t<CharT>> fmt)
    {
        std::basic_string<CharT> result;
        operator()(std::ranges::begin(cr), std::ranges::end(cr), std::back_inserter(result), pattern, fmt.begin(), fmt.end());
        return result;
    }

    template<typename CharT, typed_regex_like<CharT> Regex>
    static constexpr std::basic_string<CharT>
    operator()(const CharT* cstr, Regex pattern, std::basic_string_view<std::type_identity_t<CharT>> fmt)
    {
        std::basic_string<CharT> result;
        operator()(cstr, cstr_sentinel, std::back_inserter(result), pattern, fmt.begin(), fmt.end());
        return result;
    }

    template<std::bidirectional_iterator I, std::sentinel_for<I> S, std::output_iterator<std::iter_value_t<I>> O,
             static_regex_like Regex, string_literal Fmt, std::same_as<std::iter_value_t<I>> CharT = Regex::char_type>
        requires std::same_as<typename decltype(Fmt)::value_type, CharT>
    static constexpr regex_replace_result<I, O>
    operator()(I first, S last, O result, Regex pattern, fmt_t<Fmt>)
    {
        return impl<CharT>(first, last, result, pattern, fmt<Fmt>);
    }

    template<std::ranges::bidirectional_range R, std::output_iterator<std::ranges::range_value_t<R>> O,
             static_regex_like Regex, string_literal Fmt, std::same_as<std::ranges::range_value_t<R>> CharT = Regex::char_type>
        requires std::same_as<typename decltype(Fmt)::value_type, CharT>
    static constexpr regex_replace_result<std::ranges::borrowed_iterator_t<R>, O>
    operator()(R&& r, O result, Regex pattern, fmt_t<Fmt>)
    {
        return operator()(std::ranges::begin(r), std::ranges::end(r), result, pattern, fmt<Fmt>);
    }

    template<std::ranges::contiguous_range CR, static_regex_like Regex, string_literal Fmt,
             std::same_as<std::ranges::range_value_t<CR>> CharT = Regex::char_type>
        requires std::same_as<typename decltype(Fmt)::value_type, CharT>
    static constexpr std::basic_string<CharT>
    operator()(CR&& cr, Regex pattern, fmt_t<Fmt>)
    {
        std::basic_string<CharT> result;
        operator()(std::ranges::begin(cr), std::ranges::end(cr), std::back_inserter(result), pattern, fmt<Fmt>);
        return result;
    }

    template<typename CharT, typed_static_regex_like<CharT> Regex, string_literal Fmt>
        requires std::same_as<typename decltype(Fmt)::value_type, CharT>
    static constexpr std::basic_string<CharT>
    operator()(const CharT* cstr, Regex pattern, fmt_t<Fmt>)
    {
        std::basic_string<CharT> result;
        operator()(cstr, cstr_sentinel, std::back_inserter(result), pattern, fmt<Fmt>);
        return result;
    }
};

}

inline constexpr detail::replace_impl regex_replace;

}

namespace srx {
namespace detail {

struct sequential_match_tag_t {};
inline constexpr sequential_match_tag_t sequential_match_tag;

}

template<std::ranges::bidirectional_range V, typename Regex, bool Sequential = false>
    requires std::ranges::view<V>
class regex_match_view
{
    static_assert(false, "regex_match_view: invalid regex");
};

template<std::ranges::bidirectional_range V, string_literal Pattern, mode Mode, bool Sequential>
    requires std::ranges::view<V>
class regex_match_view<V, static_regex<Pattern, Mode>, Sequential> : std::ranges::view_interface<regex_match_view<V, static_regex<Pattern, Mode>, Sequential>>
{
    class iterator;

public:
    regex_match_view() requires std::default_initializable<V> = default;
    constexpr explicit regex_match_view(V base, static_regex<Pattern, Mode> /* regex */) : base_{ std::move(base) } {}

    constexpr explicit regex_match_view(detail::sequential_match_tag_t) noexcept(noexcept(regex_match_view()))
        requires Sequential and std::default_initializable<V> {};

    constexpr explicit regex_match_view(detail::sequential_match_tag_t, V base, static_regex<Pattern, Mode> /* regex */)
        requires Sequential
        : base_{ std::move(base) } {}

    [[nodiscard]] constexpr V base() const& requires std::copy_constructible<V> { return base_; }
    [[nodiscard]] constexpr V base() && { return std::move(base_); }

    [[nodiscard]] constexpr iterator begin()
    {
        /* since regex_match_view is an input range, there
            is no need to cache future calls to begin() */
        auto current = std::ranges::begin(base_);
        cached_result_ = result_type{ current, std::ranges::end(base_) };
        return iterator{ *this, std::move(current) };
    }

    [[nodiscard]] constexpr std::default_sentinel_t end()
    {
        return std::default_sentinel;
    }

private:
    static constexpr auto flags = Sequential ? detail::default_fsm_flags::match_sequential : detail::default_fsm_flags::search_all;
    using matcher_type = [: detail::get_matcher_refl(Mode, not Sequential) :]<Pattern, flags>;
    using result_type = matcher_type::template iterated_result<std::ranges::iterator_t<V>>;

    static constexpr matcher_type matcher{};

    V base_{};
    result_type cached_result_;
};

template<std::ranges::bidirectional_range V, string_literal Pattern, mode Mode, bool Sequential>
    requires std::ranges::view<V>
class regex_match_view<V, static_regex<Pattern, Mode>, Sequential>::iterator
{
public:
    using iterator_concept  = std::input_iterator_tag;
    using iterator_category = std::input_iterator_tag;
    using value_type        = matcher_type::template result<std::ranges::iterator_t<V>>;
    using difference_type   = std::ranges::range_difference_t<V>;

    iterator() requires std::default_initializable<std::ranges::iterator_t<V>> = default;

    constexpr explicit iterator(regex_match_view& parent, std::ranges::iterator_t<V> current)
        : parent_{ std::addressof(parent) }, current_{ std::move(current) } {}

    constexpr std::ranges::iterator_t<V> base() const
    {
        return current_;
    }

    constexpr const value_type& operator*() const noexcept
    {
        return parent_->cached_result_.res;
    }

    constexpr iterator& operator++()
    {
        if (not parent_->cached_result_.res)
            return *this;

        if constexpr (result_type::needs_begin)
            current_ = parent_->cached_result_.advance(begin(), end());
        else
            current_ = parent_->cached_result_.advance(end());

        return *this;
    }

    constexpr void operator++(int)
    {
        ++*this;
    }

    friend constexpr bool operator==(const iterator& x, std::default_sentinel_t)
    {
        return not x.parent_->cached_result_.res.has_value();
    }

    template<std::ranges::input_range W, int...>
        requires std::ranges::view<W>
    friend class submatches_view;

    template<std::ranges::input_range W, typename>
        requires std::ranges::view<W>
    friend class replace_view;

private:
    constexpr std::ranges::iterator_t<V> begin() const
    {
        return std::ranges::begin(parent_->base_);
    }

    constexpr std::ranges::sentinel_t<V> end() const
    {
        return std::ranges::end(parent_->base_);
    }

    regex_match_view* parent_{ nullptr };
    std::ranges::iterator_t<V> current_{};
};

template<typename R, string_literal Pattern, mode Mode>
regex_match_view(R&&, static_regex<Pattern, Mode>) -> regex_match_view<std::views::all_t<R>, static_regex<Pattern, Mode>, false>;

template<typename R, string_literal Pattern, mode Mode>
regex_match_view(detail::sequential_match_tag_t, R&&, static_regex<Pattern, Mode>) -> regex_match_view<std::views::all_t<R>, static_regex<Pattern, Mode>, true>;

template<std::ranges::input_range V, int... Submatches>
    requires std::ranges::view<V>
class submatches_view
{
    static_assert(false, "submatches_view: invalid range");
};

template<std::ranges::input_range V, int... Submatches>
    requires std::ranges::view<V> and detail::static_regex_match_view_like<V>
class submatches_view<V, Submatches...> : std::ranges::view_interface<submatches_view<V, Submatches...>>
{
    using match_result_type = std::ranges::range_value_t<V>;
    static constexpr int submatch_limit{ match_result_type::submatch_count };

    template<int Submatch>
    static constexpr bool submatch_is_valid{ ((-1 == Submatch) or (Submatch < match_result_type::submatch_count)) };
    static_assert((submatch_is_valid<Submatches> and ... and true));

    struct iterator;

public:
    submatches_view() requires std::default_initializable<V> = default;

    constexpr explicit submatches_view(V base, std::integer_sequence<int, Submatches...> /* submatches */)
        requires (sizeof...(Submatches) > 0)
        : base_{ std::move(base) } {}

    template<std::ranges::input_range R>
        requires std::same_as<std::ranges::range_value_t<R>, int>
    constexpr explicit submatches_view(V base, R&& submatches)
        requires (sizeof...(Submatches) == 0)
        : base_{ std::move(base) }, dynamic_submatches_(std::from_range, std::forward<R>(submatches))
    {
        if (dynamic_submatches_.empty())
            throw std::invalid_argument("submatches_view::submatches_view: no submatches specified");

        if (not std::ranges::all_of(dynamic_submatches_, [](int s){ return (-1 == s) or (s < submatch_limit); }))
            throw std::out_of_range("submatches_view::submatches_view: invalid submatch index");
    }

    [[nodiscard]] constexpr V base() const& requires std::copy_constructible<V> { return base_; }
    [[nodiscard]] constexpr V base() && { return std::move(base_); }

    [[nodiscard]] constexpr iterator begin()
    {
        return iterator{ *this, std::ranges::begin(base_), std::ranges::end(base_) };
    }

    [[nodiscard]] constexpr std::default_sentinel_t end()
    {
        return std::default_sentinel;
    }

private:
    using dynamic_submatches_t = detail::maybe_type_t<sizeof...(Submatches) == 0, std::vector<int>>;
    using underlying_type = std::remove_cvref_t<decltype(std::declval<V>().base().base())>;
    using submatch_type = submatch<std::ranges::iterator_t<underlying_type>>;

    static constexpr bool maybe_has_suffix_iterator{ ((sizeof...(Submatches) == 0) or ... or (Submatches == -1)) };
    static constexpr std::size_t suffix_index{ -1uz };

    V base_{};
    [[no_unique_address]] dynamic_submatches_t dynamic_submatches_;
};

template<std::ranges::input_range V, int... Submatches>
    requires std::ranges::view<V> and detail::static_regex_match_view_like<V>
struct submatches_view<V, Submatches...>::iterator
{
public:
    using iterator_concept  = std::input_iterator_tag;
    using iterator_category = std::input_iterator_tag;
    using value_type        = submatch_type;
    using difference_type   = std::ranges::range_difference_t<V>;

    iterator() requires std::default_initializable<std::ranges::iterator_t<V>>
                        and std::default_initializable<std::ranges::sentinel_t<V>>
                        and std::default_initializable<std::ranges::iterator_t<underlying_type>> = default;

    constexpr iterator(submatches_view& parent, std::ranges::iterator_t<V> current, std::ranges::sentinel_t<V> end)
        : parent_{ std::addressof(parent) }, current_{ std::move(current) }, end_{ std::move(end) }
    {
        if constexpr (maybe_has_suffix_iterator)
            set_suffix_iterator_if_needed();
    }

    constexpr const std::ranges::iterator_t<V>& base() const& noexcept
    {
        return current_;
    }

    constexpr std::ranges::iterator_t<V>& base() &&
    {
        return std::move(current_);
    }

    constexpr value_type operator*() const
    {
        using sf = detail::submatch_factory<std::ranges::iterator_t<underlying_type>>;

        if constexpr (maybe_has_suffix_iterator)
            if (index_ == suffix_index)
                return sf::make_submatch(current_.base(), current_.end());

        const auto submatch_index = get_submatch(index_);

        if constexpr (maybe_has_suffix_iterator)
            if (submatch_index == -1)
                return sf::make_submatch(current_.base(), get<0>(*current_).begin());

        return (*current_)[submatch_index];
    }

    constexpr iterator& operator++()
    {
        ++index_;

        if (index_ == submatch_count() and current_ != end_)
        {
            ++current_;
            index_ = 0;

            if constexpr (maybe_has_suffix_iterator)
                set_suffix_iterator_if_needed();
        }

        return *this;
    }

    constexpr void operator++(int)
    {
        ++*this;
    }

    friend constexpr bool operator==(const iterator& x, std::default_sentinel_t)
    {
        if constexpr (maybe_has_suffix_iterator)
            return x.current_ == x.end_ and x.index_ != suffix_index;
        else
            return x.current_ == x.end_;
    }

private:
    using maybe_parent_ptr = detail::maybe_type_t<sizeof...(Submatches) == 0, submatches_view*>;

    constexpr void set_suffix_iterator_if_needed()
    {
        if (current_ == end_ and current_.base() != current_.end())
        {
            if constexpr (sizeof...(Submatches) == 0)
            {
                if (std::ranges::contains(parent_->dynamic_submatches_, -1))
                    index_ = suffix_index;
            }
            else
            {
                index_ = suffix_index;
            }
        }
    }

    [[nodiscard]] static consteval std::size_t submatch_count()
        requires (sizeof...(Submatches) > 0)
    {
        return sizeof...(Submatches);
    }

    [[nodiscard]] constexpr std::size_t submatch_count() const
        requires (sizeof...(Submatches) == 0)
    {
        return parent_->dynamic_submatches_.size();
    }

    [[nodiscard]] static constexpr int get_submatch(std::size_t i)
        requires (sizeof...(Submatches) > 0)
    {
        static constexpr std::array static_submatches{ Submatches... };
        return static_submatches[i];
    }

    [[nodiscard]] constexpr int get_submatch(std::size_t i) const
        requires (sizeof...(Submatches) == 0)
    {
        return parent_->dynamic_submatches_[i];
    }

    [[no_unique_address]] maybe_parent_ptr parent_{ nullptr };
    std::ranges::iterator_t<V> current_{};
    [[no_unique_address]] std::ranges::sentinel_t<V> end_{};
    std::size_t index_{ 0 };
};

template<typename R, int... Submatches>
submatches_view(R&&, std::integer_sequence<int, Submatches...>) -> submatches_view<std::views::all_t<R>, Submatches...>;

template<typename R, typename Submatches>
submatches_view(R&&, Submatches&&) -> submatches_view<std::views::all_t<R>>;

template<std::ranges::input_range V, typename Fmt>
    requires std::ranges::view<V>
class replace_view
{
    static_assert(false, "replace_view: invalid range");
};

template<std::ranges::input_range V, string_literal Fmt>
    requires std::ranges::view<V> and detail::static_regex_match_view_like<V>
class replace_view<V, fmt_t<Fmt>> : std::ranges::view_interface<replace_view<V, fmt_t<Fmt>>>
{
    using match_result_type = std::ranges::range_value_t<V>;
    static constexpr int submatch_limit{ match_result_type::submatch_count };

    struct iterator;

public:
    replace_view() requires std::default_initializable<V> = default;

    constexpr explicit replace_view(V base, fmt_t<Fmt> /* fmt */)
        : base_{ std::move(base) } {}

    [[nodiscard]] constexpr V base() const& requires std::copy_constructible<V> { return base_; }
    [[nodiscard]] constexpr V base() && { return std::move(base_); }

    [[nodiscard]] constexpr iterator begin()
    {
        return iterator{ std::ranges::begin(base_), std::ranges::end(base_) };
    }

    [[nodiscard]] constexpr std::default_sentinel_t end()
    {
        return std::default_sentinel;
    }

private:
    using underlying_type = std::remove_cvref_t<decltype(std::declval<V>().base().base())>;
    static_assert(std::same_as<std::ranges::range_value_t<underlying_type>, typename decltype(Fmt)::value_type>);

    static constexpr detail::static_replace_fmt fmt{ Fmt.view(), submatch_limit };
    static constexpr std::size_t max_index{ fmt.subranges().size() + fmt.captures().size() };
    static constexpr std::size_t suffix_index{ -1uz };

    using char_type = std::ranges::range_value_t<underlying_type>;
    using base_subrange = std::ranges::subrange<std::ranges::iterator_t<underlying_type>>;
    using fmt_subrange = std::ranges::subrange<std::ranges::iterator_t<typename decltype(fmt)::subrange_type>>;
    using next_type = std::variant<base_subrange, fmt_subrange>;

    static constexpr std::size_t base_subrange_index{ 0 };
    static constexpr std::size_t fmt_subrange_index{ 1 };

    V base_{};
};

template<std::ranges::input_range V, string_literal Fmt>
    requires std::ranges::view<V> and detail::static_regex_match_view_like<V>
struct replace_view<V, fmt_t<Fmt>>::iterator
{
public:
    using iterator_concept  = std::input_iterator_tag;
    using iterator_category = std::input_iterator_tag;
    using value_type        = std::ranges::range_value_t<underlying_type>;
    using difference_type   = std::ranges::range_difference_t<V>;

    iterator() requires std::default_initializable<std::ranges::iterator_t<V>>
                        and std::default_initializable<std::ranges::sentinel_t<V>>
                        and std::default_initializable<std::ranges::iterator_t<underlying_type>> = default;

    constexpr iterator(std::ranges::iterator_t<V> current, std::ranges::sentinel_t<V> end)
        : current_{ std::move(current) }, end_{ std::move(end) }
        , subrange_{ std::in_place_index<base_subrange_index>, current_.base(), get<0>(*current_).begin() }
    {
        if (subrange_.visit(detail::overloads{ [](const auto& sub){ return sub.empty(); } }))
            find_next();
    }

    constexpr const std::ranges::iterator_t<V>& base() const& noexcept
    {
        return current_;
    }

    constexpr std::ranges::iterator_t<V>& base() &&
    {
        return std::move(current_);
    }

    constexpr value_type operator*() const
    {
        return subrange_.visit(detail::overloads{ [](const auto& sub){ return *sub.begin(); } });
    }

    constexpr iterator& operator++()
    {
        if (subrange_.visit(detail::overloads{ [](auto& sub){ sub.advance(1); return sub.empty(); } }))
            find_next();

        return *this;
    }

    constexpr void operator++(int)
    {
        ++*this;
    }

    friend constexpr bool operator==(const iterator& x, std::default_sentinel_t)
    {
        return x.subrange_.visit(detail::overloads{ [](const auto& sub){ return sub.empty(); } });
    }

private:
    constexpr void find_next()
    {
        while (current_ != end_)
        {
            ++index_;

            if (index_ == max_index)
            {
                index_ = suffix_index;
                ++current_;

                if (current_ == end_)
                {
                    subrange_.template emplace<base_subrange_index>(current_.base(), current_.end());
                    return;
                }

                const auto& subrange_ref = subrange_.template emplace<base_subrange_index>(current_.base(), get<0>(*current_).begin());

                if (not subrange_ref.empty())
                    return;

                ++index_;
            }

            template for (constexpr std::size_t i : std::views::iota(0uz, max_index))
            {
                if (index_ == i)
                {
                    namespace drc = detail::replace_constants;

                    if constexpr (i % 2 == 0)
                    {
                        static constexpr fmt_subrange format{ fmt.subranges()[i / 2] };

                        if constexpr (not format.empty())
                        {
                            subrange_.template emplace<fmt_subrange_index>(format);
                            return;
                        }
                    }
                    else if constexpr (constexpr auto submatch_index = fmt.captures()[i / 2]; submatch_index == drc::prematch)
                    {
                        const auto mfirst = get<0>(*current_).begin();

                        if (mfirst != current_.base())
                        {
                            subrange_.template emplace<base_subrange_index>(current_.base(), mfirst);
                            return;
                        }
                    }
                    else if constexpr (submatch_index == drc::postmatch)
                    {
                        const auto mlast = get<0>(*current_).end();

                        if (mlast != current_.end())
                        {
                            subrange_.template emplace<base_subrange_index>(mlast, current_.end());
                            return;
                        }
                    }
                    else if constexpr (submatch_index != drc::skip)
                    {
                        submatch capture{ get<submatch_index>(*current_) };

                        if (not capture.empty())
                        {
                            subrange_.template emplace<base_subrange_index>(capture.begin(), capture.end());
                            return;
                        }
                    }
                }
            }
        }
    }

    std::ranges::iterator_t<V> current_{};
    [[no_unique_address]] std::ranges::sentinel_t<V> end_{};
    next_type subrange_{};
    std::size_t index_{ suffix_index };
};

template<typename R, string_literal Fmt>
replace_view(R&&, fmt_t<Fmt>) -> replace_view<std::views::all_t<R>, fmt_t<Fmt>>;

template<std::ranges::input_range V, std::ranges::bidirectional_range Fmt>
    requires std::ranges::view<V> and detail::static_regex_match_view_like<V> and std::ranges::view<Fmt>
class replace_view<V, Fmt> : std::ranges::view_interface<replace_view<V, Fmt>>
{
    using match_result_type = std::ranges::range_value_t<V>;
    static constexpr int submatch_limit{ match_result_type::submatch_count };

    struct iterator;

public:
    replace_view() requires std::default_initializable<V> and std::default_initializable<Fmt> = default;

    constexpr explicit replace_view(V base, Fmt fmt)
        : base_{ std::move(base) }, format_input_{ std::move(fmt) }
        , fmt_{ std::ranges::begin(format_input_), std::ranges::end(format_input_), submatch_limit }
        , max_index_{ fmt_.captures().size() + fmt_.subranges().size() } {}

    [[nodiscard]] constexpr V base() const& requires std::copy_constructible<V> { return base_; }
    [[nodiscard]] constexpr V base() && { return std::move(base_); }

    [[nodiscard]] constexpr iterator begin()
    {
        return iterator{ *this, std::ranges::begin(base_), std::ranges::end(base_) };
    }

    [[nodiscard]] constexpr std::default_sentinel_t end()
    {
        return std::default_sentinel;
    }

private:
    using underlying_type = std::remove_cvref_t<decltype(std::declval<V>().base().base())>;
    static_assert(std::same_as<std::ranges::range_value_t<underlying_type>, std::ranges::range_value_t<Fmt>>);

    using format_type = detail::replace_fmt<std::ranges::iterator_t<Fmt>>;
    static constexpr std::size_t suffix_index{ -1uz };

    using char_type = std::ranges::range_value_t<underlying_type>;
    using base_subrange = std::ranges::subrange<std::ranges::iterator_t<underlying_type>>;
    using fmt_subrange = format_type::subrange_type;
    using next_type = std::variant<base_subrange, fmt_subrange>;

    static constexpr std::size_t base_subrange_index{ 0 };
    static constexpr std::size_t fmt_subrange_index{ 1 };

    V base_{};
    Fmt format_input_{};
    format_type fmt_{};
    std::size_t max_index_{ 0 };
};

template<std::ranges::input_range V, std::ranges::bidirectional_range Fmt>
    requires std::ranges::view<V> and detail::static_regex_match_view_like<V> and std::ranges::view<Fmt>
struct replace_view<V, Fmt>::iterator
{
public:
    using iterator_concept  = std::input_iterator_tag;
    using iterator_category = std::input_iterator_tag;
    using value_type        = std::ranges::range_value_t<underlying_type>;
    using difference_type   = std::ranges::range_difference_t<V>;

    iterator() requires std::default_initializable<std::ranges::iterator_t<V>>
                        and std::default_initializable<std::ranges::sentinel_t<V>>
                        and std::default_initializable<std::ranges::iterator_t<underlying_type>> = default;

    constexpr iterator(replace_view& parent, std::ranges::iterator_t<V> current, std::ranges::sentinel_t<V> end)
        : parent_{ std::addressof(parent) }, current_{ std::move(current) }, end_{ std::move(end) }
        , subrange_{ std::in_place_index<base_subrange_index>, current_.base(), get<0>(*current_).begin() }
    {
        if (subrange_.visit(detail::overloads{ [](const auto& sub){ return sub.empty(); } }))
            find_next();
    }

    constexpr const std::ranges::iterator_t<V>& base() const& noexcept
    {
        return current_;
    }

    constexpr std::ranges::iterator_t<V>& base() &&
    {
        return std::move(current_);
    }

    constexpr value_type operator*() const
    {
        return subrange_.visit(detail::overloads{ [](const auto& sub){ return *sub.begin(); } });
    }

    constexpr iterator& operator++()
    {
        if (subrange_.visit(detail::overloads{ [](auto& sub){ sub.advance(1); return sub.empty(); } }))
            find_next();

        return *this;
    }

    constexpr void operator++(int)
    {
        ++*this;
    }

    friend constexpr bool operator==(const iterator& x, std::default_sentinel_t)
    {
        return x.subrange_.visit(detail::overloads{ [](const auto& sub){ return sub.empty(); } });
    }

private:
    constexpr void find_next()
    {
        while (current_ != end_)
        {
            ++index_;

            if (index_ == parent_->max_index_)
            {
                index_ = suffix_index;
                ++current_;

                if (current_ == end_)
                {
                    subrange_.template emplace<base_subrange_index>(current_.base(), current_.end());
                    return;
                }

                const auto& subrange_ref = subrange_.template emplace<base_subrange_index>(current_.base(), get<0>(*current_).begin());

                if (not subrange_ref.empty())
                    return;

                ++index_;
            }

            namespace drc = detail::replace_constants;

            if (index_ % 2 == 0)
            {
                const auto& format = parent_->fmt_.subranges().at(index_ / 2);

                if (not format.empty())
                {
                    subrange_.template emplace<fmt_subrange_index>(format);
                    return;
                }
            }
            else if (const auto submatch_index = parent_->fmt_.captures().at(index_ / 2); submatch_index == drc::prematch)
            {
                const auto mfirst = get<0>(*current_).begin();

                if (mfirst != current_.base())
                {
                    subrange_.template emplace<base_subrange_index>(current_.base(), mfirst);
                    return;
                }
            }
            else if (submatch_index == drc::postmatch)
            {
                const auto mlast = get<0>(*current_).end();

                if (mlast != current_.end())
                {
                    subrange_.template emplace<base_subrange_index>(mlast, current_.end());
                    return;
                }
            }
            else if (submatch_index != drc::skip)
            {
                submatch capture{ (*current_).at(submatch_index) };

                if (not capture.empty())
                {
                    subrange_.template emplace<base_subrange_index>(capture.begin(), capture.end());
                    return;
                }
            }
        }
    }

    replace_view* parent_{ nullptr };
    std::ranges::iterator_t<V> current_{};
    [[no_unique_address]] std::ranges::sentinel_t<V> end_{};
    next_type subrange_{};
    std::size_t index_{ suffix_index };
};

template<typename R, std::ranges::bidirectional_range Fmt>
replace_view(R&&, Fmt&&) -> replace_view<std::views::all_t<R>, std::views::all_t<Fmt>>;

template<std::ranges::bidirectional_range V, typename Regex>
    requires std::ranges::view<V>
class regex_split_view
{
    static_assert(false, "regex_split_view: invalid regex");
};

template<std::ranges::bidirectional_range V, string_literal Pattern, mode Mode>
    requires std::ranges::view<V>
class regex_split_view<V, static_regex<Pattern, Mode>> : std::ranges::view_interface<regex_split_view<V, static_regex<Pattern, Mode>>>
{
    class iterator;
    class sentinel;

public:
    regex_split_view() requires std::default_initializable<V> = default;
    constexpr explicit regex_split_view(V base, static_regex<Pattern, Mode> /* regex */) : base_{ std::move(base) } {}

    [[nodiscard]] V base() const& requires std::copy_constructible<V> { return base_; }
    [[nodiscard]] V base() && requires std::copy_constructible<V> { return std::move(base_); }

    [[nodiscard]] constexpr iterator begin()
    {
        if (not cache_engaged_)
        {
            const auto beg = std::ranges::begin(base_);
            const auto end = std::ranges::end(base_);
            result_type result{ beg, end, detail::match_non_empty };

            if (result.res)
            {
                auto [mfirst, mlast] = get<0>(result.res);
                cached_begin_next_ = { std::move(mfirst), std::move(mlast) };
                cached_begin_continue_at_ = result.stf.continue_at;
            }
            else
            {
                const auto it = std::ranges::next(beg, end);
                cached_begin_next_ = { it, it };
            }

            cache_engaged_ = true;
        }

        return iterator{ *this, std::ranges::begin(base_), cached_begin_next_, cached_begin_continue_at_ };
    }

    [[nodiscard]] constexpr sentinel end()
    {
        return sentinel{ std::ranges::end(base_) };
    }

    [[nodiscard]] constexpr iterator end()
        requires std::ranges::common_range<V>
    {
        return iterator{ *this, std::ranges::end(base_), {} };
    }

    /* workaround to avoid implementing non-propagating-cache<T> */
    constexpr regex_split_view(const regex_split_view& other) : base_{ other.base_ } {}
    constexpr regex_split_view(regex_split_view&& other) noexcept : base_{ std::move(other).base_ } {}
    constexpr regex_split_view& operator=(const regex_split_view& other) { base_ = other; }
    constexpr regex_split_view& operator=(regex_split_view&& other) noexcept { base_ = std::move(other).base_; cache_engaged_ = false; }
    constexpr ~regex_split_view() = default;

private:
    using matcher_type   = [: detail::get_matcher_refl(Mode, true) :]<Pattern, detail::default_fsm_flags::search_all>;
    using result_type    = matcher_type::template iterated_result<std::ranges::iterator_t<V>>;
    using next_type      = std::ranges::subrange<std::ranges::iterator_t<V>>;
    using state_type     = result_type::state_type;

    static constexpr matcher_type matcher{};

    V base_{};
    next_type cached_begin_next_;
    [[no_unique_address]] state_type::continue_type cached_begin_continue_at_;
    bool cache_engaged_{ false };
};

template<std::ranges::bidirectional_range V, string_literal Pattern, mode Mode>
    requires std::ranges::view<V>
class regex_split_view<V, static_regex<Pattern, Mode>>::iterator
{
public:
    using iterator_concept  = std::forward_iterator_tag;
    using iterator_category = std::input_iterator_tag;
    using value_type        = std::ranges::subrange<std::ranges::iterator_t<V>>;
    using difference_type   = std::ranges::range_difference_t<V>;

    iterator() requires std::default_initializable<std::ranges::iterator_t<V>> = default;

    constexpr explicit iterator(regex_split_view& parent, std::ranges::iterator_t<V> current, next_type next)
        : parent_{ std::addressof(parent) }, current_{ std::move(current) }, next_{ std::move(next) } {}

    constexpr explicit iterator(regex_split_view& parent, std::ranges::iterator_t<V> current, next_type next, state_type::continue_type cont)
        : parent_{ std::addressof(parent) }, current_{ std::move(current) }, next_{ std::move(next) }, continue_at_{ cont } {}

    constexpr std::ranges::iterator_t<V>& base() const
    {
        return current_;
    }

    constexpr value_type operator*() const
    {
        return { current_, next_.begin() };
    }

    constexpr iterator& operator++()
    {
        current_ = next_.begin();

        if (const auto end = std::ranges::end(parent_->base_); current_ != end)
        {
            current_ = next_.end();

            if (current_ == end)
            {
                trailing_empty_ = true;
                next_ = { current_, current_ };
            }
            else
            {
                result_type result{ std::ranges::begin(parent_->base_), current_, end, state_type{ continue_at_ }, next_.empty() };

                if (result.res)
                {
                    auto [mfirst, mlast] = get<0>(result.res);
                    next_ = { std::move(mfirst), std::move(mlast) };
                    continue_at_ = result.stf.continue_at;
                }
                else
                {
                    const auto it = std::ranges::next(current_, end);
                    next_ = { it, it };
                }
            }
        }
        else
        {
            trailing_empty_ = false;
        }

        return *this;
    }

    constexpr iterator operator++(int)
    {
        auto tmp = *this;
        ++*this;
        return tmp;
    }

    friend constexpr bool operator==(const iterator& x, const iterator& y)
        requires std::equality_comparable<std::ranges::iterator_t<V>>
    {
        return x.current_ == y.current_ and x.trailing_empty_ == y.trailing_empty_;
    }

    friend class sentinel;

private:
    regex_split_view* parent_{ nullptr };
    std::ranges::iterator_t<V> current_{};
    next_type next_{};
    [[no_unique_address]] state_type::continue_type continue_at_{};
    bool trailing_empty_{ false };
};

template<std::ranges::bidirectional_range V, string_literal Pattern, mode Mode>
    requires std::ranges::view<V>
class regex_split_view<V, static_regex<Pattern, Mode>>::sentinel
{
public:
    sentinel() = default;

    constexpr explicit sentinel(std::ranges::sentinel_t<V> end)
        : end_{ end } {}

    friend constexpr bool operator==(const iterator& x, const sentinel& y)
    {
        return x.current_ == y.end_ and not x.trailing_empty_;
    }

private:
    [[no_unique_address]] std::ranges::sentinel_t<V> end_{};
};

template<typename R, string_literal Pattern, mode Mode>
regex_split_view(R&&, static_regex<Pattern, Mode>) -> regex_split_view<std::views::all_t<R>, static_regex<Pattern, Mode>>;

namespace views {
namespace detail {

template<typename Range, typename Regex>
concept can_regex_match_view = requires
{
    regex_match_view(std::declval<Range>(), std::declval<Regex>());
};

template<typename Range, typename Regex>
concept can_regex_sequential_match_view = requires
{
    regex_match_view(srx::detail::sequential_match_tag, std::declval<Range>(), std::declval<Regex>());
};

template<typename Range, typename T>
concept can_submatches_view = requires
{
    submatches_view(std::declval<Range>(), std::declval<T>());
};

template<typename Range, typename T>
concept can_replace_view = requires
{
    replace_view(std::declval<Range>(), std::declval<T>());
};

template<typename Range, typename T>
concept can_regex_split_view = requires
{
    regex_split_view(std::declval<Range>(), std::declval<T>());
};

template<typename Regex>
struct static_regex_match_adaptor_closure : std::ranges::range_adaptor_closure<static_regex_match_adaptor_closure<Regex>>
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
        requires srx::detail::static_regex_like<Regex>
    [[nodiscard]] consteval auto operator()(Regex /* x */) const
    {
        return static_regex_match_adaptor_closure<Regex>();
    }
};

template<typename Regex>
struct static_regex_sequential_match_adaptor_closure : std::ranges::range_adaptor_closure<static_regex_sequential_match_adaptor_closure<Regex>>
{
    template<std::ranges::viewable_range Range>
        requires detail::can_regex_match_view<Range, Regex>
    [[nodiscard]] constexpr auto operator()(Range&& r) const
    {
        return regex_match_view(srx::detail::sequential_match_tag, std::forward<Range>(r), Regex{});
    }
};

struct regex_sequential_match_adaptor
{
    template<std::ranges::viewable_range Range, typename Regex>
        requires detail::can_regex_sequential_match_view<Range, Regex>
    [[nodiscard]] constexpr auto operator()(Range&& r, Regex&& x) const
    {
        return regex_match_view(srx::detail::sequential_match_tag, std::forward<Range>(r), std::forward<Regex>(x));
    }

    template<typename Regex>
        requires srx::detail::static_regex_like<Regex>
    [[nodiscard]] consteval auto operator()(Regex /* x */) const
    {
        return static_regex_sequential_match_adaptor_closure<Regex>();
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

    constexpr explicit dynamic_submatches_adaptor_closure(T submatches)
        : sub_{ std::forward<T>(submatches) } {}

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

template<string_literal Fmt>
struct static_replace_adaptor_closure : std::ranges::range_adaptor_closure<static_replace_adaptor_closure<Fmt>>
{
    template<std::ranges::viewable_range Range>
        requires detail::can_replace_view<Range, fmt_t<Fmt>>
    [[nodiscard]] constexpr auto operator()(Range&& r) const
    {
        return replace_view(std::forward<Range>(r), fmt<Fmt>);
    }
};

template<typename T>
struct dynamic_replace_adaptor_closure : std::ranges::range_adaptor_closure<dynamic_replace_adaptor_closure<T>>
{
    template<std::ranges::viewable_range Range>
        requires detail::can_replace_view<Range, std::views::all_t<T>>
    [[nodiscard]] constexpr auto operator()(Range&& r) const
    {
        return replace_view(std::forward<Range>(r), fmt_);
    }

    constexpr explicit dynamic_replace_adaptor_closure(T submatches)
        : fmt_{ std::forward<T>(submatches) } {}

private:
    std::views::all_t<T> fmt_;
};

struct replace_adaptor
{
    template<std::ranges::viewable_range Range, typename Fmt>
        requires detail::can_replace_view<Range, Fmt>
    [[nodiscard]] constexpr auto operator()(Range&& r, Fmt&& fmt) const
    {
        return replace_view(std::forward<Range>(r), std::forward<Fmt>(fmt));
    }

    template<std::ranges::viewable_range Range, typename CharT>
        requires detail::can_replace_view<Range, std::ranges::subrange<const CharT*, srx::detail::cstr_sentinel_t>>
    [[nodiscard]] constexpr auto operator()(Range&& r, const CharT* fmtstr) const
    {
        return replace_view(std::forward<Range>(r), std::ranges::subrange(fmtstr, srx::detail::cstr_sentinel));
    }

    template<typename Fmt>
    [[nodiscard]] constexpr auto operator()(Fmt&& fmt) const
    {
        return dynamic_replace_adaptor_closure{ std::forward<Fmt>(fmt) };
    }

    template<typename CharT>
    [[nodiscard]] constexpr auto operator()(const CharT* fmtstr) const
    {
        return dynamic_replace_adaptor_closure{ std::ranges::subrange(fmtstr, srx::detail::cstr_sentinel) };
    }

    template<srx::string_literal Fmt>
    [[nodiscard]] consteval auto operator()(fmt_t<Fmt>) const
    {
        return static_replace_adaptor_closure<Fmt>();
    }
};

template<typename Regex>
struct static_regex_split_adaptor_closure : std::ranges::range_adaptor_closure<static_regex_split_adaptor_closure<Regex>>
{
    template<std::ranges::viewable_range Range>
        requires detail::can_regex_split_view<Range, Regex>
    [[nodiscard]] constexpr auto operator()(Range&& r) const
    {
        return regex_split_view(std::forward<Range>(r), Regex{});
    }
};

struct regex_split_adaptor
{
    template<std::ranges::viewable_range Range, typename Regex>
        requires detail::can_regex_split_view<Range, Regex>
    [[nodiscard]] constexpr auto operator()(Range&& r, Regex&& x) const
    {
        return regex_split_view(std::forward<Range>(r), std::forward<Regex>(x));
    }

    template<typename Regex>
        requires srx::detail::static_regex_like<Regex>
    [[nodiscard]] consteval auto operator()(Regex /* x */) const
    {
        return static_regex_split_adaptor_closure<Regex>();
    }
};

}

inline constexpr detail::regex_match_adaptor regex_match;
inline constexpr detail::regex_sequential_match_adaptor regex_sequential_match;
inline constexpr detail::submatches_adaptor submatches;
inline constexpr detail::replace_adaptor replace;
inline constexpr detail::regex_split_adaptor regex_split;

template<string_literal Pattern, mode Mode = mode::standard>
inline constexpr detail::static_regex_match_adaptor_closure<static_regex<Pattern, Mode>> static_regex_match;

template<string_literal Pattern, mode Mode = mode::standard>
inline constexpr detail::static_regex_sequential_match_adaptor_closure<static_regex<Pattern, Mode>> static_regex_sequential_match;

template<int... Submatches> requires (sizeof...(Submatches) > 0)
inline constexpr detail::static_submatches_adaptor_closure<Submatches...> static_submatches;

template<string_literal Fmt>
inline constexpr detail::static_replace_adaptor_closure<Fmt> static_replace;

template<string_literal Pattern, mode Mode = mode::standard>
inline constexpr detail::static_regex_split_adaptor_closure<static_regex<Pattern, Mode>> static_regex_split;

}

template<string_literal Pattern, mode Mode>
template<std::ranges::bidirectional_range R>
    requires std::same_as<std::ranges::range_value_t<R>, typename static_regex<Pattern, Mode>::char_type>
constexpr auto static_regex<Pattern, Mode>::range(R&& r)
{
    return views::regex_match(std::forward<R>(r), srx<Pattern, Mode>);
}

template<string_literal Pattern, mode Mode>
template<std::ranges::bidirectional_range R>
    requires std::same_as<std::ranges::range_value_t<R>, typename static_regex<Pattern, Mode>::char_type>
constexpr auto static_regex<Pattern, Mode>::sequential_range(R&& r)
{
    return views::regex_sequential_match(std::forward<R>(r), srx<Pattern, Mode>);
}

}

