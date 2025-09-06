#include <stdbool.h>
#include <stdio.h>

#include "token.h"
#include "reader.h"
#include "lexer.h"

// const char *program = ".;: while\ta () int main() { if (a == 0) { printf(\"hello world\"); } else { }; return 0; }";

const char *program = "a = 10;";

bool is_operator(token_type_t type)
{
    switch (type) {
    case TOKEN_EQUAL:
    case TOKEN_DOUBLE_EQUAL:
    case TOKEN_LESS:
    case TOKEN_LESS_EQUAL:
    case TOKEN_GREATER:
    case TOKEN_GREATER_EQUAL:
        return true;

    default:
        return false;
    }
}

void compile(lexer_t *lexer)
{
    token_type_t type;
    token_meta_t meta;

    while ((type = lexer_next_token(lexer, &meta)) != TOKEN_END) {
        // debug
        if (type == TOKEN_COMPLEX) {
            printf("'%.*s'\n", meta.as_complex.len, meta.as_complex.start);
        } else {
            printf("%s\n", decode_token_type(type));
        }
    }
}

int main(void)
{
    reader_t reader;
    lexer_t lexer;

    reader_init_from_string(&reader, program);
    lexer_init(&lexer, &reader);

    compile(&lexer);

    return 0;
}
