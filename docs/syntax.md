# Supported Syntax

Matching is currently limited to ascii text inputs.

| $e ::=$                         | matches                                                     |
|:--------------------------------|:------------------------------------------------------------|
| `a`                             | the character `'a'`                                         |
| `.`                             | any character except newline (unless *dotall* flag set)     |
| $e_1$ <code>&#124;</code> $e_2$ | either the pattern $e_1$ or the pattern $e_2$               |
| $e_1 e_2$                       | the pattern $e_1$ followed by $e_2$                         |
| $e_1 \textit{rep}$              | the pattern $e_1$ repeated according to $rep$ *(see below)* |
| $\textit{cc}$                   | the character class $cc$ *(see below)*                      |
| $\textit{gp}$                   | the group $\textit{gp}$ *(see below)*                       |
| $\textit{as}$                   | the assertion $\textit{as}$ *(see below)*                   |

Note that `.` currently only matches a single code unit, rather than a single code point.

## Repeaters

| $\textit{rep} ::=$   | repeats a pattern                              |
|:---------------------|:-----------------------------------------------|
| `*`                  | zero or more times *(greedy)*                  |
| `+`                  | one or more times *(greedy)*                   |
| `?`                  | zero or one times *(greedy)*                   |
| `{` $n$ `}`          | $n$ times *(greedy)*                           |
| `{` $n$ `,}`         | $n$ or more times *(greedy)*                   |
| `{` $n$ `,` $m$ `}`  | between $n$ and $m$ times inclusive *(greedy)* |
| `*?`                 | zero or more times *(lazy)*                    |
| `+?`                 | one or more times *(lazy)*                     |
| `??`                 | zero or one times *(lazy)*                     |
| `{` $n$ `}?`         | $n$ times *(lazy)*                             |
| `{` $n$ `,}?`        | $n$ or more times *(lazy)*                     |
| `{` $n$ `,` $m$ `}?` | between $n$ and $m$ times inclusive *(lazy)*   |

## Character Classes

| $\textit{cc} ::=$         | matches                                                      |
|:--------------------------|:-------------------------------------------------------------|
| `[` $c$ `]`               | a single character class element $c$                         |
| `[^` $c$ `]`              | NOT the single character class element $c$                   |
| `[` $c_1\ldots{}c_n$ `]`  | the set union of character class elements $c_1$ to $c_n$     |
| `[^` $c_1\ldots{}c_n$ `]` | NOT the set union of character class elements $c_1$ to $c_n$ |
| $\textit{pcc}$            | the perl character class $\textit{pcc}$ *(see below)*        |

### Character Class Elements

| $c ::=$ | matches                                         |
|:--------|:------------------------------------------------|
| `a`     | the character `'a'`                             |
| `a-z`   | any character between `'a'` and `'z'` inclusive |
| $pcc$   | the perl character class $pcc$ *(see below)*    |
| $ncc$   | the named character class $ncc$ *(see below)*   |


### Perl Character Classes

| $\textit{pcc} ::=$ | matches                    | equivalent to   |
|:-------------------|:---------------------------|:----------------|
| `\w`               | any digit                  | `[0-9]`         |
| `\W`               | NOT a digit                | `[^0-9]`        |
| `\w`               | any word character         | `[0-9A-Za-z_]`  |
| `\W`               | NOT a word character       | `[^0-9A-Za-z_]` |
| `\s`               | any whitespace character   | `[\t\n\f\r ]`   |
| `\S`               | NOT a whitespace character | `[^\t\n\f\r ]`  |

### Named Character Classes

| $\textit{ncc} ::=$ | name            | equivalent to      |
|:-------------------|:----------------|:-------------------|
| `[:alnum:]`        | alphanumeric    | `[0-9A-Za-z]`      |
| `[:alpha:]`        | alphabetic      | `[A-Za-z]`         |
| `[:ascii:]`        | ascii           | `[\0-\177]`        |
| `[:blank:]`        | blank           | `[\t ]`            |
| `[:cntrl:]`        | control         | `[\0-\37\177]`     |
| `[:digit:]`        | digits          | `[0-9]`            |
| `[:graph:]`        | graphical       | `[!-~]`            |
| `[:lower:]`        | lowercase       | `[a-z]`            |
| `[:print:]`        | printable       | `[ -~]`            |
| `[:punct:]`        | punctuation     | ``[!-/:-@[-`{-~]`` |
| `[:space:]`        | whitespace      | `[\v\t\n\f\r ]`    |
| `[:upper:]`        | uppercase       | `[A-Z]`            |
| `[:word:]`         | word characters | `[0-9A-Za-z_]`     |
| `[:xdigit:]`       | hex digits      | `[0-9A-Fa-f]`      |

## Groups

| $\textit{gp} ::=$                 | is a                                                                    |
|:----------------------------------|:------------------------------------------------------------------------|
| `(` $e$ `)`                       | numbered capturing group                                                |
| `(?:` $e$ `)`                     | non-capturing group                                                     |
| `(?` $\textit{flags}$ `:` $e$ `)` | non-capturing group, setting $\textit{flags}$ within the group          |
| `(?` $\textit{flags}$ `)`         | flag setter, setting $\textit{flags}$ to its containing capturing group |
| `(#` $\ldots{}$ `)`               | comment *(ignored)*                                                     |

### Flags

| $\textit{flag} ::=$ | name        | effect                                                    |
|:--------------------|:------------|:----------------------------------------------------------|
| `i`                 | *caseless*  | case insensitive matching                                 |
| `m`                 | *multiline* | `^` and `$` additionally match the start and end of lines |
| `s`                 | *dotall*    | `.` additionally matches newlines                         |
| `U`                 | *ungreedy*  | swaps greedy and lazy repetition                          |

Flags can be set (`xyz`) and can be unset (`-xy`).
They can also be set and unset at the same time: `xy-z` sets `x` and `y` and unsets `z`.

## Assertions

| $\textit{as} ::=$ | matches                                                       |
|:------------------|:--------------------------------------------------------------|
| `^`               | start of text (and start of line if *multiline* flag set)     |
| `$`               | end of text (and end of line if *multiline* flag set)         |
| `\A`              | start of text                                                 |
| `\b`              | word boundary *(i.e. `\w` on one side and `\W` on the other)* |
| `\B`              | NOT word boundary                                             |
| `\z`              | end of text                                                   |

Assertions match without an consuming input character.


## Escape sequences

The usual escape sequences (`'\t'`, `'\n'`, etc...) are accepted by the tokeniser, except for `'\b'` which is treated as a word-boundary assertion *(see above)*.

Octal, Hexadecimal and Unicode escapes are also accepted, but at the moment non-ascii code points may not be correctly interpreted.
Additionally, octal escape sequences that are not prefixed by `\o` must be longer than 1 digit, except for `\0`; `\1` up to `\7` are treated as backreferences and are rejected by the parser.

Named unicode characters are not supported.

`\Q...\E` matches the literal text between `'\Q'` and `'\E'`.
