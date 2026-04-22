# wildp/regex

wildp/regex is an experimental compile-time regular expression library for C++26.

*This library is still under development.*

See [syntax.md](./docs/syntax.md) and [synopsis.md](./docs/synopsis.md) for documentation.

This library is also available as a single header file, which can be regenerated using [amalgamate.cpp](./tools/amalgamate.cpp).

## Dependencies

This library requires C++26 to build.

### Compiler Support

| Compiler | Version                                                           | Flags                 |
|:---------|:------------------------------------------------------------------|:----------------------|
| GCC      | trunk                                                             | `-freflection`        |
| Clang    | [P2996 fork](https://github.com/bloomberg/clang-p2996/tree/p2996) | `-freflection-latest` |


For longer patterns it is necessary to increase the constexpr evaluation step limit.
On GCC use `-fconstexpr-ops-limit=4294967296`, and on Clang use `-fconstexpr-steps=4294967295`.


### Optional Libraries

This library can optionally use [Boost.DynamicBitset](https://www.boost.org/library/latest/dynamic_bitset/) for improved compile times.
Define `RX_USE_BOOST_DYNAMIC_BITSET` before including `<rx/regex.hpp>` to use this.
