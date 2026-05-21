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
| <code>Twain</code>                                                |    608 |  3,829 |   261,920 |
| <code>(?i)Twain</code>                                            |    956 | 14,608 |   298,579 |
| <code>[a-z]shing</code>                                           |    690 | 32,326 |   353,540 |
| <code>Huck[a-zA-Z]+&#124;Saw[a-zA-Z]+</code>                      |    987 |  9,121 |   385,291 |
| <code>\b\w+nn\b</code>                                            | 47,483 | 67,230 |   848,006 |
| <code>[a-q][^u-z]{13}x</code>                                     |    720 | 65,392 |   792,717 |
| <code>Tom&#124;Sawyer&#124;Huckleberry&#124;Finn</code>           |  1,879 | 11,156 |   634,035 |
| <code>(?i)(?:Tom&#124;Sawyer&#124;Huckleberry&#124;Finn)</code>   | 17,873 | 34,550 |   813,869 |
| <code>.{0,2}(?:Tom&#124;Sawyer&#124;Huckleberry&#124;Finn)</code> | 32,636 | 68,931 | 2,053,460 |
| <code>.{2,4}(?:Tom&#124;Sawyer&#124;Huckleberry&#124;Finn)</code> | 34,642 | 68,772 | 2,133,458 |
| <code>Tom.{10,25}river&#124;river.{10,25}Tom</code>               |  4,058 | 13,403 |   389,803 |
| <code>[a-zA-Z]+ing</code>                                         | 59,647 | 66,260 | 1,128,182 |
| <code>\s[a-zA-Z]{0,12}ing\s</code>                                | 39,761 | 58,385 |   633,407 |
| <code>(?:[A-Za-z]awyer&#124;[A-Za-z]inn)\s</code>                 | 12,192 | 40,809 |   570,223 |
| <code>["'][^"']{0,30}[?!\.][\"']</code>                           |  2,935 |  9,883 |   322,545 |

*Compiled with `-std=c++26 -O3 -DNDEBUG -march=x86-64-v3` using GCC 16.1.1 and libstdc++. Average of 15 runs.*

