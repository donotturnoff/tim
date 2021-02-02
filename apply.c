#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "expressions.h"

Exp *apply(Exp *f, Exp *exp, Env *env) {
    Exp *apply_exp = allocate_exp_base();
    apply_exp->e->apply = (Apply *) malloc_or_die(sizeof(Apply));

    apply_exp->name = APPLY;
    apply_exp->is_irreducible = 0;
    apply_exp->e->apply->f = f;
    apply_exp->e->apply->exp = exp;

    add_env(apply_exp->env, env);

    return apply_exp;
}

Exp *step_apply(Exp *exp) {
    Env *env = exp->env;
    Apply *a = exp->e->apply;
    Exp *exp1 = a->f;
    Exp *exp2 = a->exp;
    if (!exp1->is_irreducible) {
        add_env(exp1->env, env);
        Exp *new_exp = apply(step(exp1), exp2, env);
        //free_exp(exp1, 1);
        //free_exp(exp, 0);
        return new_exp;
    } else if (!exp2->is_irreducible) {
        add_env(exp2->env, env);
        Exp *new_exp = apply(exp1, step(exp2), env);
        //free_exp(exp2, 1);
        //free_exp(exp, 0);
        return new_exp;
    } else {
        if (exp1->name != FUNCTION) {
            die(RUNTIME_ERR, "argument 1 of apply is not a function");
        } else if (!exp2->is_irreducible) {
            die(RUNTIME_ERR, "argument 2 of apply is not irreducible");
        } else {
            Function *f = exp1->e->function;
            char *arg_name = f->arg->e->var->name;
            Exp *body = f->body;
            add_env(body->env, env);
            add_env(body->env, exp1->env);
            put_env_var(body->env, arg_name, NULL, exp2);
            /*free_exp(f->arg, 1);
            free_exp(exp1, 0);
            free_exp(exp, 0);*/
            return body;
        }
    }
}

Type *type_apply(Exp *exp) {
    Env *env = exp->env;
    Apply *apply = exp->e->apply;
    Exp *function = apply->f;
    Exp *exp1 = apply->exp;

    add_env(function->env, env);
    add_env(exp1->env, env);
    Type *f_t = type(function);
    Type *exp1_t = type(exp1);

    Type *t = NULL;

    Type *arg_t = f_t->t->f->arg;
    Type *body_t = f_t->t->f->body;

    if (arg_t->name == GENERIC_T || equal_types(arg_t, exp1_t)) {
        t = copy_type(body_t);
    } else {
        die(TYPE_ERR, "type mismatch in function application: %s cannot be applied to %s", to_string_type(f_t),
            to_string_type(exp1_t));
    }
    return t;
}

Exp *copy_apply(Exp *exp) {
    Apply *a = exp->e->apply;
    return apply(copy_exp(a->f), copy_exp(a->exp), copy_env(exp->env));
}

char *to_string_apply(Exp *exp) {
    Apply *apply = exp->e->apply;
    char *f_str = to_string_exp(apply->f);
    char *exp_str = to_string_exp(apply->exp);
    size_t len1 = strlen(f_str);
    size_t len2 = strlen(exp_str);
    size_t len = len1 + len2 + 4;
    char *buf = (char *) malloc_or_die(len * sizeof(char));
    snprintf(buf, len, "(%s %s)", f_str, exp_str);

    free(f_str);
    free(exp_str);

    return buf;
}

void free_apply(Exp *exp, int descend) {
    if (exp->name == APPLY) {
        if (descend) {
            free_exp(exp->e->apply->f, 1);
            free_exp(exp->e->apply->exp, 1);
        }
        free(exp->e->apply);
        free(exp->e);
        free_env(exp->env);
        free(exp);
    } else {
        die(INTERPRETER_ERR, "attempted to call free_var on non-var");
    }
}