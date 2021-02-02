#ifndef TIM_TYPES_H
#define TIM_TYPES_H

typedef enum type_name {
    GENERIC_T, UNIT_T, BOOLEAN_T, INTEGER_T, FUNCTION_T, SUM_T, PRODUCT_T
} TypeName;

typedef union type_choice TypeChoice;
typedef struct function_type FunctionType;
typedef struct sum_type SumType;
typedef struct product_type ProductType;

typedef struct type {
    TypeName name;
    char *custom;
    TypeChoice *t;
} Type;

typedef union type_choice {
    FunctionType *f;
    SumType *s;
    ProductType *p;
} TypeChoice;

typedef struct function_type {
    Type *arg, *body;
} FunctionType;

typedef struct sum_type {
    Type *t1, *t2;
} SumType;

typedef struct product_type {
    Type *t1, *t2;
} ProductType;

int equal_types(Type *t1, Type *t2);
Type *generic_t(void);
Type *unit_t(void);
Type *boolean_t(void);
Type *integer_t(void);
Type *function_t(Type *arg_t, Type *body_t);
Type *sum_t(Type *t1, Type *t2);
Type *product_t(Type *t1, Type *t2);
Type *copy_type(Type *t);
char *to_string_type(Type *t);
int free_type(Type *t);

#endif //TIM_TYPES_H
