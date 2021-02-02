#include <stdlib.h>
#include <string.h>
#include "expressions.h"

Exp *var(char *name, Type *t) {
    Exp *var_exp = allocate_exp_base();
    var_exp->e->var = (Var *) malloc_or_die(sizeof(Var));

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
    char *str = (char *) malloc_or_die(buf_size * sizeof(char));
    strcpy(str, exp->e->var->name);
    return str;
}

void free_var(Exp *exp) {
    if (exp->name == VAR) {
        Type *t = exp->e->var->t;
        if (t) free_type(t);
        free(exp->e->var);
        free(exp->e);
        free_env(exp->env);
        free(exp);
    } else {
        die(INTERPRETER_ERR, "attempted to call free_var on non-var");
    }
}