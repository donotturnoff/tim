#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "types.h"

int equal_types(Type *t1, Type *t2) {
    if (t1->name != t2->name) {
        return 0;
    } else if (t1->name == FUNCTION_T) {
        // Pairwise comparison
        return equal_types(t1->t->f->arg, t2->t->f->arg) && equal_types(t1->t->f->body, t2->t->f->body);
    } else {
        // Atomic types are equal if their type names are the same
        return 1;
    }
}

Type *refine_type(Type *base, Type *refined) {
    if (base->name == GENERIC_T) return refined;
    else if (base->name == refined->name) return base;
    else return NULL;
}

Type *most_refined_type(Type *t1, Type *t2) {
    if (t1->name == GENERIC_T) return t2;
    else if (t2->name == GENERIC_T) return t1;
    else if (t1->name == t2->name) return t1;
    else return NULL;
}


Type *allocate_type(TypeName name) {
    Type *t = (Type *) malloc_or_die(sizeof(Type));
    t->name = name;
    t->t = NULL;
    t->custom = NULL;
}

Type *generic_t() {
    return allocate_type(GENERIC_T);
}

Type *unit_t() {
    return allocate_type(UNIT_T);
}

Type *integer_t() {
    return allocate_type(INTEGER_T);
}

Type *function_t(Type *arg_t, Type *body_t) {
    Type *t = (Type *) malloc_or_die(sizeof(Type));
    t->name = FUNCTION_T;
    t->custom = NULL;
    t->t = (TypeChoice *) malloc_or_die(sizeof(TypeChoice));
    t->t->f = (FunctionType *) malloc_or_die(sizeof(FunctionType));
    t->t->f->arg = arg_t;
    t->t->f->body = body_t;
    return t;
}

Type *copy_function_t(Type *t) {
    FunctionType *f_t = t->t->f;
    return function_t(f_t->arg, f_t->body);
}

Type *copy_type(Type *t) {
    if (!t) return NULL;

    switch (t->name) {
        case GENERIC_T:     return generic_t();
        case UNIT_T:        return unit_t();
        case INTEGER_T:     return integer_t();
        case FUNCTION_T:    return copy_function_t(t);

        default: die(INTERPRETER_ERR, "attempted to copy unknown type");
    }
}

char *to_string_generic_type(void) {
    char *str = (char *) malloc_or_die(8 * sizeof(char));
    snprintf(str, 8, "generic");
    return str;
}

char *to_string_unit_type(void) {
    char *str = (char *) malloc_or_die(5 * sizeof(char));
    snprintf(str, 5, "unit");
    return str;
}

char *to_string_integer_type(void) {
    char *str = (char *) malloc_or_die(8 * sizeof(char));
    snprintf(str, 8, "integer");
    return str;
}

char *to_string_function_type(Type *t) {
    FunctionType *ft = t->t->f;
    char *arg_str = to_string_type(ft->arg);
    char *body_str = to_string_type(ft->body);
    size_t len1 = strlen(arg_str);
    size_t len2 = strlen(body_str);
    size_t len = len1 + len2 + 7;
    char *buf = (char *) malloc_or_die(len * sizeof(char));
    snprintf(buf, len, "(%s -> %s)", arg_str, body_str);

    free(arg_str);
    free(body_str);

    return buf;
}

char *to_string_type(Type *t) {
    if (!t) return NULL;
    switch (t->name) {
        case GENERIC_T:     return to_string_generic_type(); // TODO
        case UNIT_T:        return to_string_unit_type();
        case INTEGER_T:     return to_string_integer_type();
        case FUNCTION_T:    return to_string_function_type(t);

        default: die(INTERPRETER_ERR, "attempted to convert unknown type to string");
    }
}

void free_type(Type *t) {
    if (!t) return;
    switch (t->name) {
        case GENERIC_T:
        case UNIT_T:
        case INTEGER_T:     free(t);
        case FUNCTION_T:    free_type(t->t->f->arg); free_type(t->t->f->body); free(t->t->f); free(t->t); free(t);

        default: die(INTERPRETER_ERR, "attempted to free unknown type");
    }
}