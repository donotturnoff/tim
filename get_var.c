#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "expressions.h"

Exp *get_var(char *name, Env *env) {
    Exp *get_var_exp = allocate_exp_base();

    get_var_exp->e->get_var = (GetVar *) malloc_or_die(sizeof(GetVar));

    get_var_exp->name = GET_VAR;
    get_var_exp->is_irreducible = 0;
    get_var_exp->e->get_var->name = name;

    add_env(get_var_exp->env, env);

    return get_var_exp;
}

Exp *step_get_var(Exp *exp) {
    Env *env = exp->env;
    char *name = exp->e->get_var->name;
    Exp *var = get_env_var(env, name);
    if (!var) die(SCOPE_ERR, "variable %s not in scope at runtime", name);
    return var;
}

Type *type_get_var(TypeEnv *env, Exp *exp, Type *expected) {
    char *name = exp->e->var->name;
    Type *env_type = get_type_env_var(env, name);
    if (!env_type) die(SCOPE_ERR, "variable %s not in scope during typecheck", name);
    Type *t = most_refined_type(env_type, expected);
    if (!t) die(TYPE_ERR, "type mismatch in variable %s: expected %s, got %s", name, to_string_type(expected), to_string_type(env_type));
    if (!set_type_env_var(env, name, t)) die(SCOPE_ERR, "variable %s not in scope during typecheck", name);
    return t;
}

Exp *copy_get_var(Exp *exp) {
    GetVar *g = exp->e->get_var;
    return get_var(g->name, copy_env(exp->env));
}

char *to_string_get_var(Exp *exp) {
    char *name = exp->e->get_var->name;
    Exp *env_exp = get_env_var(exp->env, exp->e->get_var->name);
    if (env_exp) {
        char *exp_str = to_string_exp(env_exp);
        size_t buf_size = strlen(name) + strlen(exp_str) + 4;
        char *get_var_str = (char *) malloc_or_die(buf_size * sizeof(char));
        snprintf(get_var_str, buf_size, "%s{=%s}", name, exp_str);
        return get_var_str;
    } else {
        size_t buf_size = strlen(name) + 1;
        char *get_var_str = (char *) malloc_or_die(buf_size * sizeof(char));
        strcpy(get_var_str, name);
        return get_var_str;
    }
}

void free_get_var(Exp *exp) {
    if (exp->name == GET_VAR) {
        free(exp->e->get_var);
        free(exp->e);
        free_env(exp->env);
        free(exp);
    } else {
        die(INTERPRETER_ERR, "attempted to call free_get_var on non-get_var");
    }
}