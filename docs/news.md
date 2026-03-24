# News

### Version 0.1.2

Fixed the implementation of `regex_match_view` so that patterns which sometimes match an empty substring (e.g. `"()|abc"`) produce the correct sequence of matches.
Additionally, `regex_match_view` and `submatches_view` no longer have stashing iterators: `regex_match_view` now stores the corresponding `static_regex_match_result` in the view object, and `submatches_view::iterator` is now a proxy iterator.

### Version 0.1.1

Added `regex_replace`.

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

