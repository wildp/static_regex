#pragma once

#include <stdexcept>

#include <rx/tree.hpp>

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

    constexpr auto append_int(const std::integral auto value, std::string& result)
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

        std::basic_string<CharT> result;

        auto visitor{ [](this const auto& rec, const expr_tree_t& pet, std::size_t pos, std::basic_string<CharT>& result) constexpr -> void {
            pet.get_expr(pos).visit(detail::overloads{
                [&](const expr_tree_t::empty&) constexpr {
                },
                [&](const expr_tree_t::any&) constexpr {
                    result += '.';
                },
                [&](const expr_tree_t::assertion& asr) constexpr {
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
                [&](const expr_tree_t::concat& cat) constexpr {
                    for (const auto idx : cat.idxs) rec(pet, idx, result);
                },
                [&](const expr_tree_t::alt& alt) constexpr {
                    for (const auto idx : alt.idxs) { rec(pet, idx, result); result += "|"; }
                    if (alt.idxs.size() > 0) result.pop_back();
                },
                [&](const expr_tree_t::capture& cap) constexpr {
                    result += '(';
                    if constexpr (ExtraParens) result += '(';
                    rec(pet, cap.idx, result);
                    if constexpr (ExtraParens) result += ')';
                    result += ')';
                },
                [&](const expr_tree_t::backref& bref) constexpr {
                    result += '\\';
                    append_int(bref.number, result);
                },
                [&](const expr_tree_t::repeat& rep) constexpr {
                    if constexpr (ExtraParens) result += '(';
                    rec(pet, rep.idx, result);
                    if constexpr (ExtraParens) result += ')';

                    if (rep.min == 0 and rep.max < 0) result += '*';
                    else if (rep.min == 1 and rep.max < 1) result += '+';
                    else if ((rep.min == 0 and rep.max == 1)) result += '?';
                    else {
                        result += '{';
                        append_int(rep.min, result);
                        if (rep.max != rep.min) result += ',';
                        if (rep.max > rep.min) append_int(rep.max, result);
                        result += '}';
                    }

                    if (rep.mode == detail::repeater_mode::lazy) result += '?';
                    else if (rep.mode == detail::repeater_mode::possessive) result += '+';
                },
                [&](const expr_tree_t::char_lit& lit) constexpr {
                    result += lit.c;
                },
                [&](const expr_tree_t::char_class& cla) constexpr {
                    result += '[';
                    result += cla.name; // TODO: replace this once char classes are properly implemented
                    result += ']';
                },
            });
        }};

        visitor(*this, this->root_idx(), result);

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