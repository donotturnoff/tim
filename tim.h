#ifndef TIM_TIM_H
#define TIM_TIM_H

#include <stdlib.h>

typedef enum error_type {
    INTERPRETER_ERR=1, TYPE_ERR, SCOPE_ERR, RUNTIME_ERR, MEMORY_ERR
} ErrorType;

void die(ErrorType e, const char *msg, ...);
void *malloc_or_die(size_t size);

#endif //TIM_TIM_H
