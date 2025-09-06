#pragma once

typedef struct {
    const char *data;
    unsigned consumed;
    unsigned size;
} reader_t;

const char *reader_current_ptr(reader_t *reader);
char reader_peek(reader_t *reader);
char reader_pop(reader_t *reader);

void reader_init_from_string(reader_t *reader, const char *str);
