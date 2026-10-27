# tree-sitter-css-strict

[![Verify](https://github.com/alfpricot/tree-sitter-css-strict/actions/workflows/verify.yml/badge.svg?branch=dev)](https://github.com/alfpricot/tree-sitter-css-strict/actions/workflows/verify.yml)
[![crates.io](https://img.shields.io/crates/v/tree-sitter-css-strict)](https://crates.io/crates/tree-sitter-css-strict)

A [tree-sitter](https://tree-sitter.github.io/tree-sitter/) grammar for strict CSS, for standalone stylesheets. It is a maintained fork of [`tree-sitter/tree-sitter-css`](https://github.com/tree-sitter/tree-sitter-css) that tracks upstream CSS syntax but treats `//` as invalid outside strings and URLs, as browsers do. The upstream grammar accepts JavaScript-style line comments for injected CSS, which can hide broken stylesheets in a standalone file. This grammar also accepts valid unquoted relative URLs such as `url(../fonts/a.eot)`; protocol-relative URLs such as `url(//cdn.example.com/a.png)`, double slashes in strings, and `/* block comments */` remain valid.

## Installation

Rust:

```sh
cargo add tree-sitter tree-sitter-css-strict
```

Node:

```sh
npm install tree-sitter-css-strict
```

A GitHub Packages copy is also published as `@alfpricot/tree-sitter-css-strict`.

### Requirements

- Rust 1.85 or later (`rust-version` in `Cargo.toml`) for the crate.
- A C compiler, because both bindings compile the generated parser (`src/parser.c`) and `src/scanner.c`; the Node binding falls back to compiling when no prebuilt binary matches the platform.
- Node 24 for development (`.nvmrc`); the `tree-sitter` peer dependency is `^0.25.0` and is optional.

## Usage

Rust:

```rust
let mut parser = tree_sitter::Parser::new();
let language = tree_sitter_css_strict::LANGUAGE;
parser
    .set_language(&language.into())
    .expect("Error loading strict CSS parser");
let tree = parser.parse("a { color: red; }", None).unwrap();
assert!(!tree.root_node().has_error());
```

Node:

```js
import Parser from 'tree-sitter';
import CSS from 'tree-sitter-css-strict';

const parser = new Parser();
parser.setLanguage(CSS);
```

The Rust crate exports `HIGHLIGHTS_QUERY`; the Node binding exposes it as `CSS.HIGHLIGHTS_QUERY`. See [ARCHITECTURE.md](ARCHITECTURE.md) for how the grammar, bindings, and queries fit together.

## How to contribute

Branch from `dev`, run `tree-sitter generate` after every `grammar.js` change, and commit the regenerated parser files with it. Run `tree-sitter test`, `cargo test`, `npm run lint`, and `npm run format:check` before opening a pull request. [CONTRIBUTING.md](CONTRIBUTING.md) has the exact commands.

## License

MIT. See [LICENSE](LICENSE) and [NOTICE](NOTICE).
