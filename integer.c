#include <stdlib.h>
#include <stdio.h>
#include "expressions.h"

Exp *integer(int val) {
    Exp *integer_exp = allocate_exp_base();
    integer_exp->e->integer = (Integer *) malloc_or_die(sizeof(Integer));
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
    char *str = (char *) malloc_or_die((len+1) * sizeof(char));
    sprintf(str, "%d", val);
    return str;
}

void free_integer(Exp *exp) {
    if (exp->name == INTEGER) {
        free(exp->e->integer);
        free(exp->e);
        free_env(exp->env);
        free(exp);
    } else {
        die(INTERPRETER_ERR, "attempted to call free_integer on non-integer");
    }
}