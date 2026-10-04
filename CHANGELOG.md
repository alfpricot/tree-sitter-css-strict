# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

## [0.1.1](https://github.com/meloncholera/tree-sitter-css-strict/compare/v0.1.0...v0.1.1) - 2026-10-04

### Other

- *(deps)* update eslint and node-gyp, track lockfile ([#4](https://github.com/meloncholera/tree-sitter-css-strict/pull/4))
- align tooling with the shared grammar repository standard ([#3](https://github.com/meloncholera/tree-sitter-css-strict/pull/3))

## [0.1.0] - 2026-09-27

### Added

- Forked the upstream CSS grammar with Rust and Node bindings.
- Accepted valid unquoted relative URLs in `url()`.

### Changed

- Treat JavaScript-style `//` line comments as CSS syntax errors.
