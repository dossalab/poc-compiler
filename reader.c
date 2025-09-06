#include "reader.h"
#include <string.h>

const char *reader_current_ptr(reader_t *reader) {
    return reader->data + reader->consumed;
}

char reader_peek(reader_t *reader) {
    return *reader_current_ptr(reader);;
}

char reader_pop(reader_t *reader)
{
    unsigned space = reader->size - reader->consumed;
    if (!space) {
        return '\0';
    }

    char ch = *reader_current_ptr(reader);
    reader->consumed++;

    return ch;
}

void reader_init_from_string(reader_t *reader, const char *str)
{
    reader->data = str;
    reader->size = strlen(str);
    reader->consumed = 0;
}
