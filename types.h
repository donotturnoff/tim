#ifndef TIM_TYPES_H
#define TIM_TYPES_H

#include "tim.h"

typedef enum type_name {
    GENERIC_T, UNIT_T, INTEGER_T, FUNCTION_T
} TypeName;

typedef union type_choice TypeChoice;
typedef struct function_type FunctionType;

typedef struct type {
    TypeName name;
    char *custom;
    TypeChoice *t;
} Type;

typedef union type_choice {
    FunctionType *f;
} TypeChoice;

typedef struct function_type {
    Type *arg, *body;
} FunctionType;

int equal_types(Type *t1, Type *t2);
Type *refine_type(Type *base, Type *refined);
Type *most_refined_type(Type *t1, Type *t2);
Type *generic_t(void);
Type *unit_t(void);
Type *integer_t(void);
Type *function_t(Type *arg_t, Type *body_t);
Type *copy_type(Type *t);
char *to_string_type(Type *t);
void free_type(Type *t);

#endif //TIM_TYPES_H
