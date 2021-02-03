#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "expressions.h"

Exp *function(Exp *arg, Exp *body, Env *env) {
    Exp *function_exp = allocate_exp_base();
    function_exp->e->function = (Function *) malloc_or_die(sizeof(Function));

    function_exp->name = FUNCTION;
    function_exp->is_irreducible = 1;
    function_exp->e->function->arg = arg;
    function_exp->e->function->body = body;

    add_env(function_exp->env, env);

    return function_exp;
}

Type *type_function(TypeEnv *env, Exp *exp, Type *expected) {
    Function *function = exp->e->function;
    Exp *arg = function->arg;
    Exp *body = function->body;

    push_type_env_var(env, arg->e->var->name, arg->e->var->t);

    // Type the argument first so that if the type gets refined the effects on the environment are seen when typing the body
    // Then retype the argument after typing the body so any effects on the environment caused by typing the body are seen by the argument
    // TODO: improve this system
    type(env, arg, expected->name == FUNCTION_T ? most_refined_type(arg->e->var->t, expected->t->f->arg) : generic_t());
    Type *body_t = type(env, body, expected->name == FUNCTION_T ? expected->t->f->body : generic_t());
    Type *arg_t = type(env, arg, expected->name == FUNCTION_T ? most_refined_type(arg->e->var->t, expected->t->f->arg) : generic_t());
    pop_type_env_var(env);

    Type *t = (Type *) malloc_or_die(sizeof(Type));
    t->t = (TypeChoice *) malloc_or_die(sizeof(TypeChoice));
    t->t->f = (FunctionType *) malloc_or_die(sizeof(FunctionType));

    t->name = FUNCTION_T;
    t->t->f->arg = arg_t;
    t->t->f->body = body_t;

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
    size_t len = (len1 + len2 + 11);

    char *buf = (char *) malloc_or_die(len * sizeof(char));
    snprintf(buf, len, "(fun %s -> %s)", var_str, body_str);

    free(var_str);
    free(body_str);
    return buf;
}

void free_function(Exp *exp, int descend) {
    if (exp->name == FUNCTION) {
        if (descend) {
            free_exp(exp->e->function->arg, 1);
            free_exp(exp->e->function->body, 1);
        }
        free(exp->e->function);
        free_env(exp->env);
        free(exp->e);
        free(exp);
    } else {
        die(INTERPRETER_ERR, "attempted to call free_function on non-function");
    }
}