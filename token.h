#pragma once

typedef enum {
    TOKEN_CBRACKET_OPEN,
    TOKEN_CBRACKET_CLOSE,
    TOKEN_BRACKET_OPEN,
    TOKEN_BRACKET_CLOSE,
    TOKEN_DOT,
    TOKEN_COMMA,
    TOKEN_QUESTIONMARK,
    TOKEN_SEMICOLON,
    TOKEN_COLON,
    TOKEN_AMPERSAND,
    TOKEN_DOUBLE_AMPERSAND,
    TOKEN_GREATER,
    TOKEN_GREATER_EQUAL,
    TOKEN_LESS,
    TOKEN_LESS_EQUAL,
    TOKEN_EQUAL,
    TOKEN_DOUBLE_EQUAL,
    TOKEN_PIPE,
    TOKEN_DOUBLE_PIPE,
    TOKEN_QUOTATION,
    TOKEN_COMPLEX,

    /* keywords */
    TOKEN_IF,
    TOKEN_ELSE,
    TOKEN_WHILE,
    TOKEN_DO,
    TOKEN_INT,
    TOKEN_RETURN,

    TOKEN_END
} token_type_t;

typedef struct {
    union {
        struct {
            const char *start;
            unsigned len;
        } as_complex;
    };
} token_meta_t;

const char *decode_token_type(token_type_t type);
