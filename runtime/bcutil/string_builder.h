/**
 * @author Mushfekur Rahman (rahman.mushfek@gmail.com)
 * @brief a simple string builder
 * @date 2021-02-13
 */

#ifndef STRING_BUILDER_H
#define STRING_BUILDER_H

#include <stdlib.h>
#include <string.h>

#include "omrcomp.h"

typedef struct str_builder {
    U_8* result;
    size_t mem_size;
    size_t len;
} str_builder_t;

str_builder_t* str_builder_init();
void str_builder_add_str(str_builder_t* rb, const char* str);
void str_builder_add_char(str_builder_t* rb, const char c);
void str_builder_free(str_builder_t* rb);

#endif /* STRING_BUILDER_H */
