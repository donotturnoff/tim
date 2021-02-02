#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "expressions.h"

int allocate_var(Exp *var_exp) {
    var_exp->e->var = (Var *) malloc(sizeof(Var));
    if (!var_exp->e->var) {
        printf("Error: failed to allocate memory for Var in var\n");
        free(var_exp->env);
        free(var_exp->e);
        free(var_exp);
        return 0;
    }
    return 1;
}

Exp *var(char *name, Type *t) {
    Exp *var_exp = allocate_exp_base("var");
    if (!(var_exp && allocate_var(var_exp))) return NULL;

    var_exp->name = VAR;
    var_exp->is_irreducible = 1;
    var_exp->e->var->name = name;
    var_exp->e->var->t = t;

    return var_exp;
}

Type *type_var(Exp *exp) {
    Var *var = exp->e->var;
    Type *var_t = var->t;
    Type *t;
    if (var_t) {
        t = copy_type(var_t);
    } else {
        t = generic_t();
    }
    return t;
}

Exp *copy_var(Exp *exp) {
    Var *v = exp->e->var;
    return var(v->name, NULL);
}

char *to_string_var(Exp *exp) {
    size_t buf_size = strlen(exp->e->var->name) + 1;
    char *str = (char *) malloc(buf_size * sizeof(char));
    if (!str) {
        printf("Error: failed to allocate memory for char * in to_string_var\n");
        return NULL;
    }
    strcpy(str, exp->e->var->name);
    return str;
}

int free_var(Exp *exp) {
    if (exp->name == VAR) {
        Type *t = exp->e->var->t;
        if (t) {
            if (!free_type(t)) return 0;
        }
        free(exp->e->var);
        free(exp->e);
        free_env(exp->env);
        free(exp);
        return 1;
    } else {
        printf("Warning: attempted to call free_var on non-var\n");
        return 0;
    }
}