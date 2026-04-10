# wildp/regex

wildp/regex is an experimental compile-time regular expression library for C++26.

*This library is still under development.*

See [syntax.md](./docs/syntax.md) and [synopsis.md](./docs/synopsis.md) for documentation.

This library is also available as a single header file, which can be regenerated using [amalgamate.cpp](./tools/amalgamate.cpp).

Currently the only compiler supported is the [Clang P2996 fork](https://github.com/bloomberg/clang-p2996/tree/p2996).
For most patterns it is necessary to to increase the constexpr evaluation step limit (e.g. `-fconstexpr-steps=4294967295` on clang-p2996).
As of 2026-03-10, GCC trunk fails to compile the library.




