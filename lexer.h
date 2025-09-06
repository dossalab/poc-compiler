#pragma once

#include "reader.h"
#include "token.h"

typedef struct {
    reader_t *reader;
} lexer_t;

token_type_t lexer_pop(lexer_t *lexer, token_meta_t *meta);
void lexer_init(lexer_t *lexer, reader_t *reader);
