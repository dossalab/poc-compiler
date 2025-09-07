#pragma once

#include <stdbool.h>

#include "reader.h"
#include "token.h"

typedef struct {
    reader_t *reader;

    struct {
        token_type_t type;
        token_meta_t meta;
        bool populated;
    } peek_buffer;
} lexer_t;

token_type_t lexer_pop(lexer_t *lexer, token_meta_t *meta);
token_type_t lexer_peek(lexer_t *lexer, token_meta_t *meta);

void lexer_init(lexer_t *lexer, reader_t *reader);
