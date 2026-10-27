# Security policy

## Reporting a vulnerability

Do not open a public issue for a vulnerability. Contact the maintainer, [@alfpricot](https://github.com/alfpricot), directly through GitHub, and include what you found, how to reproduce it, and the impact you expect.

A parser crash or hang on some input that cannot affect downstream consumers is an ordinary bug: open a normal [GitHub issue](https://github.com/alfpricot/tree-sitter-css-strict/issues) with the input, or a minimized version of it, and the binding you used (Rust or Node).

## Scope

This policy covers the grammar, the generated parser, the Rust crate, and the Node package built from this repository. Vulnerabilities in the `tree-sitter` runtime or CLI belong to the [tree-sitter project](https://github.com/tree-sitter/tree-sitter).

## Supported versions

Only the latest published release receives fixes.

## Practices

- No credentials in the repository; the Verify workflow scans the working tree and history for secrets.
- Dependencies are pinned in `package.json` and `package-lock.json` and kept current through Dependabot.
- The generated parser is committed and checked in CI against `tree-sitter generate`, so a reviewed grammar change is the only way parser code changes.
