#include "tree_sitter/parser.h"

#include <wctype.h>

enum TokenType {
    DESCENDANT_OP,
    PSEUDO_CLASS_SELECTOR_COLON,
    ERROR_RECOVERY,
};

static inline void advance(TSLexer *lexer) { lexer->advance(lexer, false); }

static inline void skip(TSLexer *lexer) { lexer->advance(lexer, true); }

// Whether `c` can open the class name that a pseudo-class selector requires right after
// its colon: the first character of `identifier` (/--|-?[a-zA-Z_\xA0-\xFF]/, the `--`
// branch starts with '-') or of `escape_sequence` (a backslash).
static inline bool is_class_name_start(int32_t c) {
    if (c == '\\') {
        return true;
    }
    if (c < 0x80) {
        return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_' || c == '-';
    }
    return c >= 0xA0 && c <= 0xFF;
}

void *tree_sitter_css_strict_external_scanner_create() { return NULL; }

void tree_sitter_css_strict_external_scanner_destroy(void *payload) { (void)payload; }

unsigned tree_sitter_css_strict_external_scanner_serialize(void *payload, char *buffer) {
    (void)payload;
    (void)buffer;
    return 0;
}

void tree_sitter_css_strict_external_scanner_deserialize(void *payload, const char *buffer, unsigned length) {
    (void)payload;
    (void)buffer;
    (void)length;
}

bool tree_sitter_css_strict_external_scanner_scan(void *payload, TSLexer *lexer, const bool *valid_symbols) {
    (void)payload;
    if (valid_symbols[ERROR_RECOVERY]) {
        return false;
    }

    if (iswspace(lexer->lookahead) && valid_symbols[DESCENDANT_OP]) {
        lexer->result_symbol = DESCENDANT_OP;

        skip(lexer);
        while (iswspace(lexer->lookahead)) {
            skip(lexer);
        }
        lexer->mark_end(lexer);

        if (lexer->lookahead == '#' || lexer->lookahead == '.' || lexer->lookahead == '[' || lexer->lookahead == '-' ||
            lexer->lookahead == '*' || iswalnum(lexer->lookahead)) {
            return true;
        }

        if (lexer->lookahead == ':') {
            advance(lexer);
            if (iswspace(lexer->lookahead)) {
                return false;
            }
            for (;;) {
                if (lexer->lookahead == ';' || lexer->lookahead == '}' || lexer->eof(lexer)) {
                    return false;
                }
                if (lexer->lookahead == '{') {
                    return true;
                }
                advance(lexer);
            }
        }
    }

    if (valid_symbols[PSEUDO_CLASS_SELECTOR_COLON]) {
        while (iswspace(lexer->lookahead)) {
            skip(lexer);
        }
        if (lexer->lookahead == ':') {
            advance(lexer);
            if (lexer->lookahead == ':') {
                return false;
            }
            lexer->mark_end(lexer);
            lexer->result_symbol = PSEUDO_CLASS_SELECTOR_COLON;

            // A pseudo-class selector needs a class name immediately after the colon, so when the
            // first non-whitespace character there cannot start one — a digit, '#', '(', a quote,
            // ... — this colon introduces a declaration (`color: 0 auto;`) and the lookahead
            // below, which reads the whole value looking for a `{`, cannot turn it into a
            // selector. On the stylesheets in examples/ that skips ~55% of these scans and ~26%
            // of the characters this scanner reads, about 4% of their parse time.
            //
            // The answers it gives up are the ones where a `{` or EOF arrives before the value's
            // `;` or `}`. The loop below would then report a pseudo-class colon and the parser
            // would go on to fail on the class name, while this path reports a declaration colon:
            // `color: "{"` parses as the declaration it is instead of an error, and a value left
            // unterminated at EOF (`a: 0`) reaches the same failure through a different tree.
            // A '/' still takes the slow path, because a comment may sit between the colon
            // and the name. The lookahead uses advance() rather than skip() so the token
            // marked above keeps covering just the colon.
            bool after_whitespace = iswspace(lexer->lookahead);
            while (iswspace(lexer->lookahead)) {
                advance(lexer);
            }
            if (after_whitespace && lexer->lookahead == '{') {
                // `a: {`: the selector the loop below would find on its first step.
                return true;
            }
            if (!is_class_name_start(lexer->lookahead) && lexer->lookahead != '/' && !lexer->eof(lexer)) {
                return false;
            }

            // We need a `{` to be a pseudo class selector, a `;` indicates a property.
            // This does not apply if we're in a comment, however.
            bool in_comment = false;
            if (after_whitespace && lexer->lookahead == '/') {
                // Whitespace was already consumed above, so the comment opener sits under
                // the cursor instead of one step ahead, where the loop below expects it.
                advance(lexer);
                if (lexer->lookahead == '*') {
                    in_comment = true;
                }
            }
            while (lexer->lookahead != ';' && lexer->lookahead != '}' && !lexer->eof(lexer)) {
                advance(lexer);
                if (lexer->lookahead == '{' && !in_comment) {
                    return true;
                }
                if (lexer->lookahead == '/' && !in_comment) {
                    advance(lexer);
                    if (lexer->lookahead == '*') {
                        in_comment = true;
                    }
                } else if (lexer->lookahead == '*' && in_comment) {
                    advance(lexer);
                    if (lexer->lookahead == '/') {
                        in_comment = false;
                    }
                }
            }

            // If we're at eof, and we happened to *not* find an opening brace to indicate we have a pseudo class
            // selector, we should *still* return one at EOF. This will improve error recovery, and the malformed code
            // can be parsed as an erroneous pseudo-class selector, rather than an erroneous property.
            return lexer->eof(lexer);
        }
    }

    return false;
}
