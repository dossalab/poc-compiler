#include "token.h"
#include <stdio.h>

const char *decode_token_type(token_type_t type)
{
    switch (type) {
    case TOKEN_CBRACKET_OPEN:
        return "{";
    case TOKEN_CBRACKET_CLOSE:
        return "}";
    case TOKEN_BRACKET_OPEN:
        return "(";
    case TOKEN_BRACKET_CLOSE:
        return ")";
    case TOKEN_DOT:
        return ".";
    case TOKEN_COMMA:
        return ",";
    case TOKEN_QUESTIONMARK:
        return "?";
    case TOKEN_SEMICOLON:
        return ";";
    case TOKEN_COLON:
        return ":";
    case TOKEN_AMPERSAND:
        return "&";
    case TOKEN_DOUBLE_AMPERSAND:
        return "&&";
    case TOKEN_GREATER:
        return ">";
    case TOKEN_GREATER_EQUAL:
        return ">=";
    case TOKEN_LESS:
        return "<";
    case TOKEN_LESS_EQUAL:
        return "<=";
    case TOKEN_EQUAL:
        return "=";
    case TOKEN_DOUBLE_EQUAL:
        return "==";
    case TOKEN_PIPE:
        return "|";
    case TOKEN_DOUBLE_PIPE:
        return "||";
    case TOKEN_QUOTATION:
        return "\"";
    case TOKEN_COMPLEX:
        return "(complex)";
    case TOKEN_END:
        return "(end)";
    case TOKEN_IF:
        return "IF";
    case TOKEN_ELSE:
        return "ELSE";
    case TOKEN_WHILE:
        return "WHILE";
    case TOKEN_DO:
        return "DO";
    case TOKEN_INT:
        return "INT";
    case TOKEN_VOID:
        return "VOID";
    case TOKEN_RETURN:
        return "RETURN";
    case TOKEN_PLUS:
        return "+";
    case TOKEN_PLUS_EQUAL:
        return "+=";
    case TOKEN_MINUS:
        return "-";
    case TOKEN_MINUS_EQUAL:
        return "-=";
    }

    printf("fixme - %d\n", type);
    return NULL;
}
