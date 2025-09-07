#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "lexer.h"
#include "token.h"

static inline bool is_whitespace(char ch) {
    switch (ch) {
        case ' ':
        case '\t':
        case '\n':
            return true;
    }

    return false;
}

static inline bool is_alpha(char ch) {
    switch (ch) {
    case 'a' ... 'z':
    case 'A' ... 'Z':
    case '_':
        return true;
    }

    return false;
}

static inline bool is_digit(char ch) {
    switch (ch) {
    case '0' ... '9':
        return true;
    }

    return false;
}

static inline token_type_t check_for_keyword_token(const char *token, unsigned len) {
    switch (len) {
    case 2:
        if (strncmp(token, "if", 2) == 0) {
            return TOKEN_IF;
        } else if (strncmp(token, "do", 2) == 0) {
            return TOKEN_DO;
        }
        break;

    case 3:
        if (strncmp(token, "int", 3) == 0) {
            return TOKEN_INT;
        }

        break;

    case 4:
        if (strncmp(token, "else", 4) == 0) {
            return TOKEN_ELSE;
        }

        break;

    case 5:
        if (strncmp(token, "while", 5) == 0) {
            return TOKEN_WHILE;
        }

        break;

    case 6:
        if (strncmp(token, "return", 6) == 0) {
            return TOKEN_RETURN;
        }

        break;
    }

    return TOKEN_COMPLEX;
}

token_type_t lexer_pop(lexer_t *lexer, token_meta_t *meta)
{
    char ch;
    token_type_t type;
    bool parsing_complex = false;
    reader_t *reader = lexer->reader;
    token_meta_t meta_dummy;

    if (!meta) {
        meta = &meta_dummy;
    }

    if (lexer->peek_buffer.populated) {
        lexer->peek_buffer.populated = false;

        *meta = lexer->peek_buffer.meta;
        return lexer->peek_buffer.type;
    }

    while ((ch = reader_pop(reader)) != '\0') {
        if (is_whitespace(ch)) {
            continue;
        }

        /* simple tokens */
        switch (ch) {
        case '{':
            return TOKEN_CBRACKET_OPEN;
        case '}':
            return TOKEN_CBRACKET_CLOSE;
        case '(':
            return TOKEN_BRACKET_OPEN;
        case ')':
            return TOKEN_BRACKET_CLOSE;
        case '.':
            return TOKEN_DOT;
        case ',':
            return TOKEN_COMMA;
        case '?':
            return TOKEN_QUESTIONMARK;
        case ';':
            return TOKEN_SEMICOLON;
        case ':':
            return TOKEN_COLON;
        case '\"':
            return TOKEN_QUOTATION;
        }

        /* slightly harder cases */
        switch (ch) {
            case '&':
                if (reader_peek(reader) == '&') {
                    reader_pop(reader);
                    return TOKEN_DOUBLE_AMPERSAND;
                }

                return TOKEN_AMPERSAND;

            case '>':
                if (reader_peek(reader) == '=') {
                    reader_pop(reader);
                    return TOKEN_GREATER_EQUAL;
                }

                return TOKEN_GREATER;

            case '<':
                if (reader_peek(reader) == '=') {
                    reader_pop(reader);
                    return TOKEN_LESS_EQUAL;
                }

                return TOKEN_LESS;

            case '=':
                if (reader_peek(reader) == '=') {
                    reader_pop(reader);
                    return TOKEN_DOUBLE_EQUAL;
                }

                return TOKEN_EQUAL;

            case '|':
                if (reader_peek(reader) == '|') {
                    reader_pop(reader);
                    return TOKEN_DOUBLE_PIPE;
                }

                return TOKEN_PIPE;
        }

        /* complex */
        if (is_alpha(ch) || is_digit(ch)) {
            if (!parsing_complex) {
                meta->as_complex.start = reader_current_ptr(reader) - 1;
                meta->as_complex.len = 0;
                parsing_complex = true;
            }

            meta->as_complex.len++;

            char next = reader_peek(reader);
            if (!is_alpha(next) && !is_digit(next)) {
                parsing_complex = false;
                return check_for_keyword_token(meta->as_complex.start, meta->as_complex.len);
            }

            continue;
        }

        printf("unparsed -> %c (%d)\n", ch, ch);
    }

    return TOKEN_END;
}

token_type_t lexer_peek(lexer_t *lexer, token_meta_t *meta)
{
    if (!lexer->peek_buffer.populated) {
        lexer->peek_buffer.type = lexer_pop(lexer, &lexer->peek_buffer.meta);
        lexer->peek_buffer.populated = true;
    }

    if (meta) {
        *meta = lexer->peek_buffer.meta;
    }

    return lexer->peek_buffer.type;
}

void lexer_init(lexer_t *lexer, reader_t *reader)
{
    lexer->reader = reader;
    lexer->peek_buffer.populated = false;
}
