#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "expressions.h"

int allocate_get_var(Exp *get_var_exp) {
    get_var_exp->e->get_var = (GetVar *) malloc(sizeof(GetVar));
    if (!get_var_exp->e->get_var) {
        printf("Error: failed to allocate memory for GetVar in get_var\n");
        free(get_var_exp->env);
        free(get_var_exp->e);
        free(get_var_exp);
        return 0;
    }
    return 1;
}

Exp *get_var(char *name, Env *env) {
    Exp *get_var_exp = allocate_exp_base("get_var");
    if (!(get_var_exp && allocate_get_var(get_var_exp))) return NULL;

    get_var_exp->name = GET_VAR;
    get_var_exp->is_irreducible = 0;
    get_var_exp->e->get_var->name = name;

    add_env(get_var_exp->env, env);

    return get_var_exp;
}

Exp *step_get_var(Exp *exp) {
    Env *env = exp->env;
    GetVar *get_var = exp->e->get_var;
    Exp *var = get_env_var(env, get_var->name);
    return var;
}

Type *type_get_var(Exp *exp) {
    Env *env = exp->env;
    char *name = exp->e->get_var->name;
    return get_env_var_type(env, name);
}

Exp *copy_get_var(Exp *exp) {
    GetVar *g = exp->e->get_var;
    return get_var(g->name, copy_env(exp->env));
}

char *to_string_get_var(Exp *exp) {
    Exp *env_exp = get_env_var(exp->env, exp->e->get_var->name);
    char *name = exp->e->get_var->name;
    char *exp_str = to_string_exp(env_exp);
    if (exp_str) {
        size_t buf_size = strlen(name) + strlen(exp_str) + 4;
        char *get_var_str = (char *) malloc(buf_size * sizeof(char));
        if (!get_var_str) {
            printf("Memory error: failed to allocate memory for char * in to_string_get_var\n");
        }
        snprintf(get_var_str, buf_size, "%s{=%s}", name, exp_str);
        return get_var_str;
    } else {
        size_t buf_size = strlen(name) + 1;
        char *get_var_str = (char *) malloc(buf_size * sizeof(char));
        if (!get_var_str) {
            printf("Memory error: failed to allocate memory for char * in to_string_get_var\n");
        }
        strcpy(get_var_str, name);
        return get_var_str;
    }
}

int free_get_var(Exp *exp) {
    if (exp->name == GET_VAR) {
        free(exp->e->get_var);
        free(exp->e);
        free_env(exp->env);
        free(exp);
        return 1;
    } else {
        printf("Warning: attempted to call free_get_var on non-get_var\n");
        return 0;
    }
}