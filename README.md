# tree-sitter-css-strict

A [tree-sitter](https://tree-sitter.github.io/tree-sitter/) grammar for strict CSS.

This project is a maintained fork of
[`tree-sitter/tree-sitter-css`](https://github.com/tree-sitter/tree-sitter-css), licensed under
MIT. It tracks upstream CSS syntax while deliberately treating `//` as invalid outside strings and
URLs, as browsers do. The upstream grammar accepts JavaScript-style line comments for injected CSS;
this grammar is for standalone CSS files where accepting them can hide broken stylesheets.

It also accepts valid unquoted relative URLs such as `url(../fonts/a.eot)`. Protocol-relative URLs
such as `url(//cdn.example.com/a.png)`, double slashes in strings, and `/* block comments */` remain
valid.

## Using it

```sh
cargo add tree-sitter tree-sitter-css-strict
```

```rust
let mut parser = tree_sitter::Parser::new();
let language = tree_sitter_css_strict::LANGUAGE;
parser
    .set_language(&language.into())
    .expect("Error loading strict CSS parser");
let tree = parser.parse("a { color: red; }", None).unwrap();
assert!(!tree.root_node().has_error());
```

```sh
npm install tree-sitter-css-strict
```

```js
import Parser from 'tree-sitter';
import CSS from 'tree-sitter-css-strict';

const parser = new Parser();
parser.setLanguage(CSS);
```

A GitHub Packages copy is also published as `@meloncholera/tree-sitter-css-strict`. The Rust crate
exports `HIGHLIGHTS_QUERY`; the Node binding exposes it as `CSS.HIGHLIGHTS_QUERY`.

## Building

```sh
npx --yes --package=tree-sitter-cli@0.27.0 -- tree-sitter generate
cargo build
```

The generated parser (`src/parser.c`, `src/grammar.json`, and `src/node-types.json`) is committed.
Regenerate and commit the diff after every `grammar.js` change.

## Testing

```sh
CC=gcc CXX=g++ npx --yes --package=tree-sitter-cli@0.27.0 -- tree-sitter test
cargo test
```

On Windows without MSVC, set `CC` and `CXX` to an installed GCC-compatible toolchain. The corpus
includes strict-mode cases for line comments, URLs, strings, and block comments.

## License

MIT. See [LICENSE](LICENSE) and [NOTICE](NOTICE).
