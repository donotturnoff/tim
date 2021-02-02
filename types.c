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
    } else if (t1->name == SUM_T) {
        // Pairwise comparison
        return equal_types(t1->t->s->t1, t2->t->s->t2) && equal_types(t1->t->s->t1, t2->t->s->t2);
    } else if (t1->name == PRODUCT_T) {
        // Pairwise comparison
        return equal_types(t1->t->p->t1, t2->t->p->t2) && equal_types(t1->t->p->t1, t2->t->p->t2);
    } else {
        // Atomic types are equal if their type names are the same
        return 1;
    }
}

Type *generic_t() {
    Type *t = (Type *) malloc(sizeof(Type));
    if (!t) {
        printf("Error: failed to allocate memory for Type in generic_t\n");
        return NULL;
    }
    t->name = GENERIC_T;
    t->t = NULL;
    t->custom = NULL;
    return t;
}

Type *unit_t() {
    Type *t = (Type *) malloc(sizeof(Type));
    if (!t) {
        printf("Error: failed to allocate memory for Type in unit_t\n");
        return NULL;
    }
    t->name = UNIT_T;
    t->t = NULL;
    t->custom = NULL;
    return t;
}

Type *boolean_t() {
    Type *t = (Type *) malloc(sizeof(Type));
    if (!t) {
        printf("Error: failed to allocate memory for Type in boolean_t\n");
        return NULL;
    }
    t->name = BOOLEAN_T;
    t->t = NULL;
    t->custom = NULL;
    return t;
}

Type *integer_t() {
    Type *t = (Type *) malloc(sizeof(Type));
    if (!t) {
        printf("Error: failed to allocate memory for Type in integer_t\n");
        return NULL;
    }
    t->name = INTEGER_T;
    t->t = NULL;
    t->custom = NULL;
    return t;
}

Type *function_t(Type *arg_t, Type *body_t) {
    Type *t = (Type *) malloc(sizeof(Type));
    if (!t) {
        printf("Error: failed to allocate memory for Type in function_t\n");
        return NULL;
    }
    t->name = FUNCTION_T;
    t->custom = NULL;
    t->t = (TypeChoice *) malloc(sizeof(TypeChoice));
    if (!t->t) {
        printf("Error: failed to allocate memory for TypeChoice in function_t\n");
        return NULL;
    }
    t->t->f = (FunctionType *) malloc(sizeof(FunctionType));
    if (!t->t->f) {
        printf("Error: failed to allocate memory for FunctionType in function_t\n");
        return NULL;
    }
    t->t->f->arg = arg_t;
    t->t->f->body = body_t;
    return t;
}

Type *sum_t(Type *t1, Type *t2) {
    Type *t = (Type *) malloc(sizeof(Type));
    if (!t) {
        printf("Error: failed to allocate memory for Type in sum_t\n");
        return NULL;
    }
    t->name = SUM_T;
    t->custom = NULL;
    t->t = (TypeChoice *) malloc(sizeof(TypeChoice));
    if (!t->t) {
        printf("Error: failed to allocate memory for TypeChoice in sum_t\n");
        return NULL;
    }
    t->t->s = (SumType *) malloc(sizeof(SumType));
    if (!t->t->s) {
        printf("Error: failed to allocate memory for SumType in sum_t\n");
        return NULL;
    }
    t->t->s->t1 = t1;
    t->t->s->t2 = t2;
    return t;
}

Type *product_t(Type *t1, Type *t2) {
    Type *t = (Type *) malloc(sizeof(Type));
    if (!t) {
        printf("Error: failed to allocate memory for Type in product_t\n");
        return NULL;
    }
    t->name = PRODUCT_T;
    t->custom = NULL;
    t->t = (TypeChoice *) malloc(sizeof(TypeChoice));
    if (!t->t) {
        printf("Error: failed to allocate memory for TypeChoice in product_t\n");
        return NULL;
    }
    t->t->p = (ProductType *) malloc(sizeof(ProductType));
    if (!t->t->p) {
        printf("Error: failed to allocate memory for ProductType in product_t\n");
        return NULL;
    }
    t->t->p->t1 = t1;
    t->t->p->t2 = t2;
    return t;
}

Type *copy_function_t(Type *t) {
    FunctionType *f_t = t->t->f;
    return function_t(f_t->arg, f_t->body);
}

Type *copy_sum_t(Type *t) {
    SumType *s_t = t->t->s;
    return sum_t(s_t->t1, s_t->t2);
}

Type *copy_product_t(Type *t) {
    ProductType *p_t = t->t->p;
    return product_t(p_t->t1, p_t->t2);
}

Type *copy_type(Type *t) {
    if (!t) return NULL;

    switch (t->name) {
        case GENERIC_T:     return generic_t();
        case UNIT_T:        return unit_t();
        case BOOLEAN_T:     return boolean_t();
        case INTEGER_T:     return integer_t();
        case FUNCTION_T:    return copy_function_t(t);
        case SUM_T:         return copy_sum_t(t);
        case PRODUCT_T:     return copy_product_t(t);

        default: return NULL; // TODO: error message
    }
}

char *to_string_generic_type(void) {
    char *str;
    str = (char *) malloc(8 * sizeof(char));
    if (!str) printf("Error: failed to allocate memory for char * in to_string_generic_type\n");
    else snprintf(str, 8, "generic");
    return str;
}

char *to_string_unit_type(void) {
    char *str;
    str = (char *) malloc(5 * sizeof(char));
    if (!str) printf("Error: failed to allocate memory for char * in to_string_unit_type\n");
    else snprintf(str, 5, "unit");
    return str;
}

char *to_string_boolean_type(void) {
    char *str;
    str = (char *) malloc(8 * sizeof(char));
    if (!str) printf("Error: failed to allocate memory for char * in to_string_boolean_type\n");
    else snprintf(str, 8, "boolean");
    return str;
}

char *to_string_integer_type(void) {
    char *str;
    str = (char *) malloc(8 * sizeof(char));
    if (!str) printf("Error: failed to allocate memory for char * in to_string_integer_type\n");
    else snprintf(str, 8, "integer");
    return str;
}

char *to_string_function_type(Type *t) {
    FunctionType *ft = t->t->f;
    char *arg_str = to_string_type(ft->arg);
    char *body_str = to_string_type(ft->body);
    size_t len1 = strlen(arg_str);
    size_t len2 = strlen(body_str);
    size_t len = len1 + len2 + 7;
    char *buf = (char *) malloc(len * sizeof(char));
    if (!buf) {
        printf("Error: failed to allocate memory in to_string_function_type\n");
        return NULL;
    }
    snprintf(buf, len, "(%s -> %s)", arg_str, body_str);

    free(arg_str);
    free(body_str);

    return buf;
}

char *to_string_sum_type(Type *t) {
    SumType *st = t->t->s;
    char *t1_str = to_string_type(st->t1);
    char *t2_str = to_string_type(st->t2);
    size_t len1 = strlen(t1_str);
    size_t len2 = strlen(t2_str);
    size_t len = len1 + len2 + 6;
    char *buf = (char *) malloc(len * sizeof(char));
    if (!buf) {
        printf("Error: failed to allocate memory in to_string_sum_type\n");
        return NULL;
    }
    snprintf(buf, len, "(%s | %s)", t1_str, t2_str);

    free(t1_str);
    free(t2_str);

    return buf;
}

char *to_string_product_type(Type *t) {
    ProductType *pt = t->t->p;
    char *t1_str = to_string_type(pt->t1);
    char *t2_str = to_string_type(pt->t2);
    size_t len1 = strlen(t1_str);
    size_t len2 = strlen(t2_str);
    size_t len = len1 + len2 + 6;
    char *buf = (char *) malloc(len * sizeof(char));
    if (!buf) {
        printf("Error: failed to allocate memory in to_string_product_type\n");
        return NULL;
    }
    snprintf(buf, len, "(%s * %s)", t1_str, t2_str);

    free(t1_str);
    free(t2_str);

    return buf;
}

char *to_string_type(Type *t) {
    if (!t) return NULL;
    switch (t->name) {
        case GENERIC_T:     return to_string_generic_type(); // TODO
        case UNIT_T:        return to_string_unit_type();
        case BOOLEAN_T:     return to_string_boolean_type();
        case INTEGER_T:     return to_string_integer_type();
        case FUNCTION_T:    return to_string_function_type(t);
        case SUM_T:         return to_string_sum_type(t);
        case PRODUCT_T:     return to_string_product_type(t);

        default: return NULL; // TODO: error on invalid type
    }
}

int free_type(Type *t) {
    if (!t) return 1;
    switch (t->name) {
        case GENERIC_T:
        case UNIT_T:
        case BOOLEAN_T:
        case INTEGER_T:     free(t); return 1;
        case FUNCTION_T:    free_type(t->t->f->arg); free_type(t->t->f->body); free(t->t->f); free(t->t); free(t); return 1;
        case SUM_T:         free_type(t->t->s->t1); free_type(t->t->s->t2); free(t->t->s); free(t->t); free(t); return 1;
        case PRODUCT_T:     free_type(t->t->p->t1); free_type(t->t->p->t2); free(t->t->p); free(t->t); free(t); return 1;

        default: return 0; // TODO: error on invalid type
    }
}