#include <stdlib.h>
#include <string.h>
#include "expressions.h"

Exp *var(char *name, Type *t) {
    Exp *var_exp = allocate_exp_base();
    var_exp->e->var = (Var *) malloc_or_die(sizeof(Var));

    var_exp->name = VAR;
    var_exp->is_irreducible = 1;
    var_exp->e->var->name = name;
    var_exp->e->var->t = t ? t : generic_t();

    return var_exp;
}

Type *type_var(TypeEnv *env, Exp *exp, Type *expected) {
    char *name = exp->e->var->name;
    Type *env_type = get_type_env_var(env, name);
    if (!env_type) die(SCOPE_ERR, "variable %s not in scope during typecheck", name);
    Type *t = most_refined_type(expected, env_type);
    if (!t) die(TYPE_ERR, "type mismatch in variable definition: %s expected to have type %s but actually has %s", name, to_string_type(exp->e->var->t), to_string_type(env_type));
    if (!set_type_env_var(env, name, t)) die(SCOPE_ERR, "variable %s not in scope during typecheck", name);
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