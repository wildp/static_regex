# News

### Version 0.1.0

Improved compilation times.

There are now several implementations that `static_regex` can use: these can selected using an optional second template parameter.
Searching now defaults to a new implementation which attempts to match the regex pattern beginning at each character in the input.
To use the previous search implementation (which prepended `".*?"` to the pattern), use `mode::fast`.
In certain cases this can be faster than the new implementation, but generally has worse compilation times.

A naive matching implementation has also been added
This uses backtracking and therefore has an exponential worst-case time complexity, but supports backreferences.

Other minor changes:
- Added literals for the various `static_regex` implementations.
- Added `range()` to `static_regex` as shorthand for constructing a `regex_match_view`.
- Changed the semantics of `submatches_view` containing index `-1` to match `std::regex_token_iterator`.

### Version 0.0.2

Improved regex matching performance through better code generation.

### Version 0.0.1

Initial release.

