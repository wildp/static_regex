# wildp/static_regex

wildp/static_regex is an experimental compile-time regular expression library for C++26.

*This library is still under development.*

See [syntax.md](./docs/syntax.md) and [synopsis.md](./docs/synopsis.md) for documentation.

It is also available as a [single header file](./single-include/srx/regex.hpp), which can be regenerated using [amalgamate.cpp](./tools/amalgamate.cpp).


## Dependencies

This library requires C++26 to build.

### Compiler Support

| Compiler | Version                                                           | Flags                 |
|:---------|:------------------------------------------------------------------|:----------------------|
| GCC      | trunk                                                             | `-freflection`        |
| GCC      | 16                                                                | `-freflection`        |
| Clang    | [P2996 fork](https://github.com/bloomberg/clang-p2996/tree/p2996) | `-freflection-latest` |


For longer regex patterns it is necessary to increase the constexpr evaluation step limit.
On GCC use `-fconstexpr-ops-limit=4294967296`, and on Clang use `-fconstexpr-steps=4294967295`.


### Optional Libraries

This library can optionally use [Boost.DynamicBitset](https://www.boost.org/library/latest/dynamic_bitset/) for improved compile times.
Define `SRX_USE_BOOST_DYNAMIC_BITSET` before including `<srx/regex.hpp>` to use this.


## Benchmarks

These patterns are taken from rust-leipzig's [regex comparison tool](https://rust-leipzig.github.io/regex/2017/03/28/comparison-of-regex-engines/).
All times are in microseconds:

| Pattern                                                           | srx    | ctre   | libstdc++ |
|-------------------------------------------------------------------|-------:|-------:|----------:|
| <code>Twain</code>                                                |    668 |  3,862 |   257,815 |
| <code>(?i)Twain</code>                                            |  1,031 | 14,601 |   294,108 |
| <code>[a-z]shing</code>                                           |  2,186 | 36,916 |   347,472 |
| <code>Huck[a-zA-Z]+&#124;Saw[a-zA-Z]+</code>                      |  1,016 |  9,394 |   381,479 |
| <code>\b\w+nn\b</code>                                            | 47,193 | 71,393 |   839,571 |
| <code>[a-q][^u-z]{13}x</code>                                     |    813 | 66,563 |   778,884 |
| <code>Tom&#124;Sawyer&#124;Huckleberry&#124;Finn</code>           |  1,854 | 11,168 |   624,535 |
| <code>(?i)(?:Tom&#124;Sawyer&#124;Huckleberry&#124;Finn)</code>   | 17,968 | 34,542 |   810,980 |
| <code>.{0,2}(?:Tom&#124;Sawyer&#124;Huckleberry&#124;Finn)</code> | 32,434 | 68,314 | 2,018,514 |
| <code>.{2,4}(?:Tom&#124;Sawyer&#124;Huckleberry&#124;Finn)</code> | 34,538 | 69,907 | 2,115,755 |
| <code>Tom.{10,25}river&#124;river.{10,25}Tom</code>               |  4,244 | 13,576 |   386,795 |
| <code>[a-zA-Z]+ing</code>                                         | 59,658 | 65,229 | 1,119,118 |
| <code>\s[a-zA-Z]{0,12}ing\s</code>                                | 41,071 | 59,147 |   627,019 |
| <code>(?:[A-Za-z]awyer&#124;[A-Za-z]inn)\s</code>                 | 21,805 | 40,364 |   561,717 |
| <code>["'][^"']{0,30}[?!\.][\"']</code>                           |  3,046 |  9,727 |   310,596 |

*Compiled with `-std=c++26 -O3 -DNDEBUG -march=x86-64-v3` using GCC 16.1.1 and libstdc++. Average of 15 runs.*

