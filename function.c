#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "expressions.h"

int allocate_function(Exp *function_exp) {
    function_exp->e->function = (Function *) malloc(sizeof(Function));
    if (!function_exp->e->function) {
        printf("Error: failed to allocate memory for Function in function\n");
        free(function_exp->env);
        free(function_exp->e);
        free(function_exp);
        return 0;
    }
    return 1;
}

Exp *function(Exp *arg, Exp *body, Env *env) {
    Exp *function_exp = allocate_exp_base("function");
    if (!(function_exp && allocate_function(function_exp))) return NULL;

    function_exp->name = FUNCTION;
    function_exp->is_irreducible = 1;
    function_exp->e->function->arg = arg;
    function_exp->e->function->body = body;

    add_env(function_exp->env, env);

    return function_exp;
}

Type *type_function(Exp *exp) {
    Env *env = exp->env;
    Function *function = exp->e->function;
    Exp *arg = function->arg;
    Exp *body = function->body;

    add_env(arg->env, env);
    add_env(body->env, env);
    Type *arg_t = type(arg);
    if (!arg_t) return NULL;
    put_env_var(body->env, arg->e->var->name, arg_t, NULL);
    Type *body_t = type(body);
    if (!body_t) {
        free_type(arg_t);
        return NULL;
    }

    Type *t = (Type *) malloc(sizeof(Type));
    if (!t) {
        printf("Error: failed to allocate memory for Type in type_function\n");
    } else {
        t->name = FUNCTION_T;
        t->t = (TypeChoice *) malloc(sizeof(TypeChoice));
        if (!t->t) {
            printf("Error: failed to allocate memory for TypeChoice in type_function\n");
            free(t);
            t = NULL;
        } else {
            t->t->f = (FunctionType *) malloc(sizeof(FunctionType));
            if (!t->t->f) {
                printf("Error: failed to allocate memory for FunctionType in type_function\n");
                free(t->t);
                free(t);
                t = NULL;
            } else {
                t->t->f->arg = arg_t;
                t->t->f->body = body_t;
            }
        }
    }
    return t;
}

Exp *copy_function(Exp *exp) {
    Function *f = exp->e->function;
    return function(copy_exp(f->arg), copy_exp(f->body), copy_env(exp->env));
}

char *to_string_function(Exp *exp) {
    Function *f = exp->e->function;
    char *var_str = to_string_exp(f->arg);
    char *body_str = to_string_exp(f->body);
    size_t len1 = strlen(var_str);
    size_t len2 = strlen(body_str);
    size_t len = (len1 + len2 + 10);
    char *buf = (char *) malloc(len * sizeof(char));
    if (!buf) {
        printf("Error: failed to allocate memory in to_string_function\n");
        return NULL;
    }
    snprintf(buf, len, "(fn %s => %s)", var_str, body_str);

    free(var_str);
    free(body_str);
    return buf;
}

int free_function(Exp *exp, int descend) {
    if (exp->name == FUNCTION) {
        if (descend && !(free_exp(exp->e->function->arg, 1) && free_exp(exp->e->function->body, 1))) return 0;
        free(exp->e->function);
        free_env(exp->env);
        free(exp->e);
        free(exp);
        return 1;
    } else {
        printf("Warning: attempted to call free_function on non-function\n");
        return 0;
    }
}