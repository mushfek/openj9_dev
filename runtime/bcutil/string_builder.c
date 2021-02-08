/**
 * @author Mushfekur Rahman (rahman.mushfek@gmail.com)
 * @date 2021-02-13
 */

#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "string_builder.h"

static const size_t init_size = 32;

void str_builder_extend(str_builder_t* rb, const size_t add_len);

str_builder_t*
str_builder_init()
{
    str_builder_t* rb;

    rb = calloc(1, sizeof(*rb));
    rb->result = malloc(init_size);

    *rb->result = '\0';
    rb->mem_size = init_size;
    rb->len = 0;

    return rb;
}

void
str_builder_add_str(str_builder_t* rb, const char* str)
{
    if (rb == NULL || str == NULL || *str == '\0') return;

    int len = strlen(str);
    str_builder_extend(rb, len);

    memmove(rb->result+rb->len, str, len);
    rb->len += len;
    rb->result[rb->len] = '\0';
}

void
str_builder_add_char(str_builder_t* rb, const char c)
{
    if (rb == NULL) return;

    str_builder_extend(rb, 1);

    rb->result[rb->len] = c;
    rb->len++;
    rb->result[rb->len] = '\0';
}

void
str_builder_free(str_builder_t* rb)
{
    if (rb == NULL) return;

    free(rb->result);
    free(rb);
}

void
str_builder_extend(str_builder_t* rb, const size_t add_len)
{
    if (rb == NULL || add_len == 0) return;

    if (rb->mem_size >= rb->len+add_len+1) return;

    while (rb->mem_size < rb->len+add_len+1) {
        rb->mem_size <<= 1;
        if (rb->mem_size == 0) {
            rb->mem_size--;
        }
    }
    
    rb->result = realloc(rb->result, rb->mem_size);
}
