# Contributing to tree-sitter-css-strict

A tree-sitter grammar for strict CSS, with Rust and Node bindings.

## Before you start

1. Branch from **`dev`**; `main` is the release branch. A promotion merge into `main` triggers `gala-release.yml`, which computes the version from Conventional Commits, updates `CHANGELOG.md` and the version in `Cargo.toml`, `package.json`, and `tree-sitter.json`, regenerates the parser, tags the release commit, and fast-forwards `dev`; the tag then publishes through `release.yml`.
2. Read [ARCHITECTURE.md](ARCHITECTURE.md) for how the grammar, generated parser, and bindings relate.

## Local setup

```sh
npm install --ignore-scripts
```

You need Node 24 (`.nvmrc`), Rust 1.85 or later, and a C compiler. On Windows without MSVC, set `CC=gcc CXX=g++` to an installed GCC-compatible toolchain before running the tree-sitter CLI.

## Build

```sh
npx --yes --package=tree-sitter-cli@0.27.1 -- tree-sitter generate
cargo build
```

The generated parser (`src/parser.c`, `src/grammar.json`, and `src/node-types.json`) is committed. Regenerate it and commit the diff after every `grammar.js` change.

## Validation before a pull request

```sh
CC=gcc CXX=g++ npx --yes --package=tree-sitter-cli@0.27.1 -- tree-sitter test
cargo fmt --check
cargo clippy --all-targets -- -D warnings
cargo test
npm run lint
npm run format:check
npm test
```

- `tree-sitter test` runs the corpus in `test/corpus/`, which holds tree assertions including the strict-CSS cases for line comments, URL forms, strings, and block comments.
- `cargo test` runs the Rust binding tests; `npm test` runs the Node binding tests in `bindings/node/`.
- `npm run lint` (oxlint) and `npm run format:check` (oxfmt) cover `grammar.js`, `test/*.mjs`, `tools/*.mjs`, and `bindings/node/*.js`; `npm run format` applies formatting. The generated parser under `src/` is excluded.
- After a node-kind or field-name change, regenerate the snapshot with `node test/generate-node-kinds.mjs`; the Verify workflow fails when `test/node-kinds.txt` differs from it.
- Verify also checks that the committed parser matches `tree-sitter generate`, parses the CSS files in `examples/` with no `ERROR` or `MISSING` nodes, scans for secrets, and lints the workflow files.

## Pull requests

- Open pull requests against **`dev`**.
- Use a Conventional Commit title (`feat:`, `fix:`, `docs:`, `chore:`, `test:`, and so on).
- Keep each pull request to one change, and link the issue it resolves in the body.
- Add a corpus test for every grammar change, and commit the regenerated parser files and `test/node-kinds.txt` with it.
