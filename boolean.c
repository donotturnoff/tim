#include <stdlib.h>
#include <stdio.h>
#include "expressions.h"

int allocate_boolean(Exp *boolean_exp) {
    boolean_exp->e->boolean = (Boolean *) malloc(sizeof(Boolean));
    if (!boolean_exp->e->boolean) {
        printf("Error: failed to allocate memory for Boolean in boolean\n");
        free(boolean_exp->env);
        free(boolean_exp->e);
        free(boolean_exp);
        return 0;
    }
    return 1;
}

Exp *boolean(int val) {
    Exp *boolean_exp = allocate_exp_base("boolean");
    if (!(boolean_exp && allocate_boolean(boolean_exp))) return NULL;

    boolean_exp->name = BOOLEAN;
    boolean_exp->is_irreducible = 1;
    boolean_exp->e->boolean->val = val;

    return boolean_exp;
}

Exp *copy_boolean(Exp *exp) {
    return boolean(exp->e->boolean->val);
}

char *to_string_boolean(Exp *exp) {
    char *str;
    if (exp->e->boolean->val) {
        str = (char *) malloc(5 * sizeof(char));
        if (!str) printf("Error: failed to allocate memory for char * in to_string_boolean\n");
        else snprintf(str, 5, "true");
    } else {
        str = (char *) malloc(6 * sizeof(char));
        if (!str) printf("Error: failed to allocate memory for char * in to_string_boolean\n");
        else snprintf(str, 6, "false");
    }
    return str;
}

int free_boolean(Exp *exp) {
    if (exp->name == BOOLEAN) {
        free(exp->e->boolean);
        free(exp->e);
        free_env(exp->env);
        free(exp);
        return 1;
    } else {
        printf("Warning: attempted to call free_boolean on non-boolean\n");
        return 0;
    }
}