#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "token.h"
#include "reader.h"
#include "lexer.h"

// const char *program = ".;: while\ta () int main() { if (a == 0) { printf(\"hello world\"); } else { }; return 0; }";

const char *program = \
    "do_stuff = 10;\n"
    "b = 10;\n"
    "\n"
    "if a == 10\n"
    "    stuff = 10;\n"
    "\n"
    "call();\n"
    "\n"
    "int a = 10;\n"
    "int b;\n"
    "\n";

typedef struct {
    lexer_t *lexer;

    struct {
        reader_t reader_state;
        lexer_t lexer_state;
        bool populated;
    } state_buffer;
} parser_t;

static void parser_save_state(parser_t *p)
{
    p->state_buffer.reader_state = *p->lexer->reader;
    p->state_buffer.lexer_state = *p->lexer;
    p->state_buffer.populated = true;
}

static void parser_restore_state(parser_t *p)
{
    if (!p->state_buffer.populated) {
        abort();
    }

    *p->lexer->reader = p->state_buffer.reader_state;
    *p->lexer = p->state_buffer.lexer_state;
    p->state_buffer.populated = false;
}

static bool is_token_of_type(parser_t *p, token_type_t expected)
{
    token_type_t type = lexer_peek(p->lexer, NULL);

    if (type == expected) {
        lexer_pop(p->lexer, NULL);
        return true;
    }

    return false;
}

static bool is_identifier(parser_t *p)
{
    return is_token_of_type(p, TOKEN_COMPLEX);
}

static bool is_operand(parser_t *p)
{
    return is_token_of_type(p, TOKEN_COMPLEX);
}

static bool is_assignment_operator(parser_t *p)
{
    switch (lexer_peek(p->lexer, NULL)) {
    case TOKEN_EQUAL:
        lexer_pop(p->lexer, NULL);
        return true;

    default:
        return false;
    }
}

static bool is_relation_operator(parser_t *p)
{
    switch (lexer_peek(p->lexer, NULL)) {
    case TOKEN_DOUBLE_EQUAL:
    case TOKEN_LESS:
    case TOKEN_LESS_EQUAL:
    case TOKEN_GREATER:
    case TOKEN_GREATER_EQUAL:
        lexer_pop(p->lexer, NULL);
        return true;

    default:
        return false;
    }
}

static bool is_assignment(parser_t *p)
{
    parser_save_state(p);

    bool ok = is_operand(p) && is_assignment_operator(p) && is_operand(p);

    if (!ok) {
        parser_restore_state(p);
    }

    return ok;
}

static bool is_comparison(parser_t *p)
{
    return is_operand(p) && is_relation_operator(p) && is_operand(p);
}

static bool is_condition(parser_t *p)
{
    return is_token_of_type(p, TOKEN_IF) && is_comparison(p);
}

static bool is_eof(parser_t *p)
{
    return is_token_of_type(p, TOKEN_END);
}

static bool is_code_block(parser_t *p)
{
    return is_token_of_type(p, TOKEN_CBRACKET_OPEN)
           && is_code_block(p)
           && is_token_of_type(p, TOKEN_CBRACKET_CLOSE);
}

static bool is_function_call(parser_t *p)
{
    parser_save_state(p);

    bool ok = is_identifier(p) && is_token_of_type(p, TOKEN_BRACKET_OPEN) && is_token_of_type(p, TOKEN_BRACKET_CLOSE);

    if (!ok) {
        parser_restore_state(p);
    }

    return ok;
}

static bool is_type_spec(parser_t *p)
{
    return is_token_of_type(p, TOKEN_INT);
}

static bool is_declaration(parser_t *p)
{
    parser_save_state(p);

    bool ok = is_type_spec(p) && is_identifier(p) && is_token_of_type(p, TOKEN_EQUAL) && is_identifier(p);

    if (!ok) {
        parser_restore_state(p);
        parser_save_state(p);

        /* Could be with no assignment */
        ok = is_type_spec(p) && is_identifier(p);
        if (!ok) {
            parser_restore_state(p);
        }
    }

    return ok;
}

static bool is_statement(parser_t *p)
{
    return ((is_declaration(p) || is_assignment(p) || is_function_call(p)) && is_token_of_type(p, TOKEN_SEMICOLON))
            || is_condition(p);
}

static bool expect_source(parser_t *p)
{
    while (true) {
        if (is_eof(p)) {
            break;
        }

        if (!is_statement(p)) {
            printf("expected statement, got %s\n",
                   decode_token_type(lexer_peek(p->lexer, NULL)));
            return false;
        }
    }

    return true;
}

static void compile(reader_t *reader)
{
    lexer_t lexer;
    parser_t parser;

    lexer_init(&lexer, reader);
    parser.lexer = &lexer;
    parser.state_buffer.populated = false;

    bool ok = expect_source(&parser);
    printf("parsing %s\n", ok? "success" : "failure");

    // token_type_t type, peeked;
    // token_meta_t meta, meta_peeked;

    // while ((type = lexer_pop(lexer, &meta)) != TOKEN_END) {
    //     // debug
    //     if (type == TOKEN_COMPLEX) {
    //         printf("'%.*s'\n", meta.as_complex.len, meta.as_complex.start);
    //     } else {
    //         printf("%s\n", decode_token_type(type));
    //     }
    // }
}

int main(void)
{
    reader_t reader;

    reader_init_from_string(&reader, program);

    printf("==== source =====\n%s\n", program);
    compile(&reader);

    return 0;
}
