# tree-sitter-css-strict architecture

tree-sitter-css-strict is a [tree-sitter](https://tree-sitter.github.io/tree-sitter/) grammar for CSS that is strict about what counts as a comment. It is a fork of `tree-sitter/tree-sitter-css`. Upstream accepts JavaScript-style `//` line comments so injected CSS parses; this grammar rejects `//` outside strings and URLs, as browsers do, and accepts valid unquoted relative URLs.

The grammar is written once in `grammar.js`. The tree-sitter CLI generates a C parser from it, the generated files are committed, and two thin bindings (Rust and Node) compile that parser for consumers. Everything else in the repository exists to test the grammar or to build and publish those packages.

The grammar's `name` is `css_strict`, so the C symbol is `tree_sitter_css_strict` and the WASM build is `tree-sitter-css_strict.wasm`, while the crate and npm package are `tree-sitter-css-strict`.

## What tree-sitter-css-strict is not

- Not a CSS validator or linter: it produces a syntax tree and reports parse errors, nothing about property names or values.
- Not a parser for CSS with line comments, such as SCSS or injected CSS in JavaScript; use a grammar that accepts `//` for those.
- Not a multi-language binding set: only the Rust and Node bindings ship.

## Major components

### [grammar.js](grammar.js)

The grammar definition and the source of truth for every generated file. `tree-sitter.json` holds the grammar's metadata and enables only the Rust and Node bindings.

### [src/](src)

The generated parser: `parser.c`, `grammar.json`, and `node-types.json`, all committed. `scanner.c` is the hand-written external scanner. Regenerate the generated files after every `grammar.js` change.

### [queries/](queries)

`highlights.scm`, the syntax-highlighting query that both bindings export.

### [bindings/rust](bindings/rust)

The `tree-sitter-css-strict` crate: `lib.rs` exposes `LANGUAGE` and `HIGHLIGHTS_QUERY`, and `build.rs` compiles `src/parser.c` and `src/scanner.c`.

### [bindings/node](bindings/node)

The `tree-sitter-css-strict` npm package: a native addon (`binding.cc`, built through `binding.gyp`) with `index.js` and `index.d.ts`. It loads a prebuilt binary when one matches the platform and compiles otherwise.

### [test/](test)

`corpus/` holds the tree assertions that `tree-sitter test` runs, `highlight/` holds highlighting fixtures, and `node-kinds.txt` is a snapshot of node kinds that `generate-node-kinds.mjs` regenerates.

### [examples/](examples)

Real-world CSS files that Verify parses, failing on any `ERROR` or `MISSING` node.

### [tools/](tools)

Release helpers: `package-check.mjs`, `release-policy.mjs`, and its tests, used by the release workflow to check and prepare the published packages.

## External systems and data flow

There are no runtime services. Consumers load the language object into the `tree-sitter` runtime (the `tree-sitter` crate or npm package) and parse text with it. At build time the tree-sitter CLI turns `grammar.js` into `src/`. Releases publish to crates.io, npm, and GitHub Packages (the latter as `@alfpricot/tree-sitter-css-strict`), and attach a WASM build to the GitHub Release.

## Build, test, release, and deploy

- **Pull requests and pushes to `dev` and `main`:** `.github/workflows/verify.yml` checks the pull request title, scans for secrets, confirms the committed parser matches `tree-sitter generate`, lints and format-checks the JavaScript, runs the corpus and the `examples/` parse check, runs `cargo fmt`, `cargo clippy`, and `cargo test`, runs the Node binding tests, and lints the workflow files.
- **Releases:** `dev` is the default branch and `main` is release-only. `release-plz.yml` runs on `main` and opens the release pull request; when it merges, `release-tag.yml` tags the commit and dispatches `release.yml`, which re-verifies, builds prebuilt Node binaries and the WASM file, and publishes to crates.io, npm, and GitHub Packages before creating the GitHub Release. `sync-dev-from-main.yml` brings `dev` back up to date after a push to `main`.
- **Maintenance:** `cache-cleanup.yml` deletes stale Actions caches daily.

There is no deployed service.

## Where things live

| Path | Holds |
| --- | --- |
| `grammar.js` | The grammar definition |
| `src/` | Generated parser files and the external scanner |
| `queries/` | Highlight queries |
| `bindings/rust/`, `bindings/node/` | The two language bindings |
| `test/` | Corpus, highlight fixtures, and the node-kind snapshot |
| `examples/` | CSS files Verify parses |
| `tools/` | Release helper scripts |
| `.github/workflows/` | Verify, release, and maintenance workflows |
