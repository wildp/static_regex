#pragma once

#include <limits>
#include <stdexcept>

#include <rx/tree.hpp>
#include <string>

namespace rx::testing
{
    template<typename CharT>
    class printable_expr_tree : public detail::expr_tree<CharT>
    {
    public:
        using detail::expr_tree<CharT>::expr_tree;

        template<bool ExtraParens = false>
        [[nodiscard]] constexpr std::basic_string<CharT> to_pattern() const;

        [[nodiscard]] constexpr auto to_pattern_parens() const { return to_pattern<true>(); }
    };

    /* printer for patterns */

    template<typename CharT>
    constexpr void append_int(std::basic_string<CharT>& result, const std::integral auto value)
    {
        static constexpr decltype(value) base{ 10 };
        std::vector<char> buf;
        if (value == 0)
            buf.emplace_back('0');
        else for (auto v{ value }; v > 0; v /= base)
            buf.emplace_back('0' + (v % base));
        if (value < 0)
            buf.emplace_back('-');
        std::ranges::move(buf.rbegin(), buf.rend(), std::back_insert_iterator(result));
    }

    template<typename CharT>
    template<bool ExtraParens>
    [[nodiscard]] constexpr std::basic_string<CharT> printable_expr_tree<CharT>::to_pattern() const
    {
        using expr_tree_t = printable_expr_tree<CharT>;

        static constexpr auto escape = [](std::basic_string<CharT>& result, CharT c) {
            if (c == '\177' or (c >= '\0' and c < '\40')) {
                result += '\\';
                switch (c) {
                case '\0': result += '0'; break;
                case '\a': result += 'a'; break;
                case '\b': result += 'b'; break;
                case '\f': result += 'f'; break;
                case '\t': result += 't'; break;
                case '\n': result += 'n'; break;
                case '\r': result += 'r'; break;
                case '\v': result += 'v'; break;
                default:
                    {
                        static constexpr CharT octal_base{ 010 };
                        std::basic_string<CharT> tmp;
                        for (; c != 0; c /= octal_base)
                            tmp.push_back('0' + (c % octal_base));
                        result.append(tmp.rbegin(), tmp.rend());
                    }
                    break;
                }
            }
            else {
                if (c == '-' or c == '[' or c == ']' or c == '^') result += '\\';
                result += c;
            }
        };

        std::basic_string<CharT> result;

        auto visitor = [](this const auto& rec, std::basic_string<CharT>& result, const expr_tree_t& pet, std::size_t pos) -> void {
            pet.get_expr(pos).visit(detail::overloads{
                [&](const expr_tree_t::assertion& asr) {
                    switch (asr.type)
                    {
                    case detail::assert_type::text_start:
                    case detail::assert_type::line_start:
                        result += '^'; break;
                    case detail::assert_type::text_end:
                    case detail::assert_type::line_end:
                        result += '$'; break;
                    default:
                        throw std::runtime_error("unrecognised assert_type");
                    }
                },
                [&](const expr_tree_t::concat& cat) {
                    for (const auto idx : cat.idxs) rec(result, pet, idx);
                },
                [&](const expr_tree_t::alt& alt) {
                    for (const auto idx : alt.idxs) { rec(result, pet, idx); result += '|'; }
                    if (alt.idxs.size() > 0) result.pop_back();
                },
                [&](const expr_tree_t::capture& cap) {
                    result += '(';
                    if constexpr (ExtraParens) result += '(';
                    rec(result, pet, cap.idx);
                    if constexpr (ExtraParens) result += ')';
                    result += ')';
                },
                [&](const expr_tree_t::backref& bref) {
                    result += '\\';
                    append_int(result, bref.number);
                },
                [&](const expr_tree_t::repeat& rep) {
                    if constexpr (ExtraParens) result += '(';
                    rec(result, pet, rep.idx);
                    if constexpr (ExtraParens) result += ')';

                    if (rep.min == 0 and rep.max < 0) result += '*';
                    else if (rep.min == 1 and rep.max < 1) result += '+';
                    else if ((rep.min == 0 and rep.max == 1)) result += '?';
                    else {
                        result += '{';
                        append_int(result, rep.min);
                        if (rep.max != rep.min) result += ',';
                        if (rep.max > rep.min) append_int(result, rep.max);
                        result += '}';
                    }

                    if (rep.mode == detail::repeater_mode::lazy) result += '?';
                    else if (rep.mode == detail::repeater_mode::possessive) result += '+';
                },
                [&](const expr_tree_t::char_str& lit) {
                    result += lit.data;
                },
                [&](const expr_tree_t::char_class& cla) {
                    if (cla.data.get().size() == 1) {
                        using uct = typename expr_tree_t::char_class::underlying_char_type;
                        const auto& [lower, upper]{ cla.data.get().front() };
                        if ((not cla.data.is_negated() and lower == std::numeric_limits<uct>::min() and upper == std::numeric_limits<uct>::max())
                            or (cla.data.is_negated() and lower == '\n' and upper == '\n')) {
                            result += '.';
                            return;
                        }
                    }

                    result += '[';
                    if (cla.data.is_negated())
                        result += '^';
                    for (const auto& [lower, upper] : cla.data.get()) {
                        /* TODO: maybe escape certain characters */
                        escape(result, lower);
                        if (upper == lower) continue;
                        result += '-';
                        escape(result, upper);
                    }
                    result += ']';
                },
            });
        };

        visitor(result, *this, this->root_idx());

        return result;
    }

    /* tree simplification (unused; moved here for safekeeping) */

    // template<typename CharT>
    // constexpr void expr_tree<CharT>::replace_repeat_pass()
    // {
    //     const std::size_t end{ expressions_.size() };   
    //     for (std::size_t i{ 0 }; i < end; ++i)
    //     {
    //         if (not std::holds_alternative<repeat>(get(i)))
    //             continue;
    //
    //         const repeat repeat{ std::get<repeat>(get(i)) };
    //
    //         if (repeat.min == 0 and repeat.max == 0)
    //         {
    //             /* a{0} -> '' */
    //             get(i) = empty{};
    //         }
    //         else if (repeat.min == 0 and repeat.max < 0)
    //         {
    //             /* a{0,} -> a* */
    //             get(i) = zero_or_more{ .idx = repeat.idx, .lazy = repeat.lazy };
    //         }
    //         else if (repeat.min == 0 and repeat.max == 1)
    //         {
    //             /* a{0,} -> a? */
    //             get(i) = zero_or_one{ .idx = repeat.idx, .lazy = repeat.lazy };
    //         }
    //         else if (repeat.min == 1 and repeat.max == 1)
    //         {
    //             /* a{1} -> a */
    //             get(i) = get(repeat.idx);
    //             /* we duplicate the node to avoid changing references others */
    //         }
    //         else if (repeat.min == 1 and repeat.max < 1)
    //         {
    //             /* a{1,} -> a+ */
    //             get(i) = one_or_more{ .idx = repeat.idx, .lazy = repeat.lazy };
    //         }
    //         else 
    //         {
    //             /* general case */
    //
    //             concat replacement{ .idxs = std::vector<std::size_t>(repeat.min, repeat.idx) };
    //
    //             if (repeat.min > repeat.max)
    //             {
    //                 /* a{3,} -> aaa+ */
    //                 const std::size_t inserted_idx{ expressions_.size() };
    //                 expressions_.emplace_back(std::in_place_type<one_or_more>, repeat.idx, repeat.lazy);
    //                 replacement.idxs.back() = inserted_idx;
    //             }
    //             else if (repeat.min < repeat.max)
    //             {
    //                 /* a{2,5} -> aa(a(a(a)?)?)? 
    //                 * a{2,3} -> aaa? 
    //                 * (parentheses are for grouping only) */
    //
    //                 std::size_t last_insert{ expressions_.size() };
    //                 expressions_.emplace_back(std::in_place_type<zero_or_one>, repeat.idx, repeat.lazy);
    //
    //                 for (auto count{ repeat.min + 1 }; count < repeat.max; ++count)
    //                 {
    //                     const std::size_t concat_idx{ expressions_.size() };
    //                     expressions_.emplace_back(std::in_place_type<concat>, std::vector{ repeat.idx, last_insert });
    //                     last_insert = expressions_.size();
    //                     expressions_.emplace_back(std::in_place_type<zero_or_one>, concat_idx, repeat.lazy);
    //                 }
    //
    //                 replacement.idxs.push_back(last_insert);
    //             }
    //
    //             get(i) = std::move(replacement);
    //         }
    //     }
    //
    // }
}