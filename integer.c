#include <stdlib.h>
#include <stdio.h>
#include "expressions.h"

int allocate_integer(Exp *integer_exp) {
    integer_exp->e->integer = (Integer *) malloc(sizeof(Integer));
    if (!integer_exp->e->integer) {
        printf("Error: failed to allocate memory for Integer in integer\n");
        free(integer_exp->env);
        free(integer_exp->e);
        free(integer_exp);
        return 0;
    }
    return 1;
}

Exp *integer(int val) {
    Exp *integer_exp = allocate_exp_base("integer");
    if (!(integer_exp && allocate_integer(integer_exp))) return NULL;

    integer_exp->name = INTEGER;
    integer_exp->is_irreducible = 1;
    integer_exp->e->integer->val = val;

    return integer_exp;
}

Exp *copy_integer(Exp *exp) {
    return integer(exp->e->integer->val);
}

char *to_string_integer(Exp *exp) {
    int val = exp->e->integer->val;
    int len = snprintf(NULL, 0, "%d", val);
    char *str = (char *) malloc((len+1) * sizeof(char));
    if (!str) {
        printf("Error: failed to allocate memory in to_string_int\n");
        return NULL;
    }
    sprintf(str, "%d", val);
    return str;
}

int free_integer(Exp *exp) {
    if (exp->name == INTEGER) {
        free(exp->e->integer);
        free(exp->e);
        free_env(exp->env);
        free(exp);
        return 1;
    } else {
        printf("Warning: attempted to call free_integer on non-integer\n");
        return 0;
    }
}