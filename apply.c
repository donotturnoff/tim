#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "expressions.h"

int allocate_apply(Exp *apply_exp) {
    apply_exp->e->apply = (Apply *) malloc(sizeof(Apply));
    if (!apply_exp->e->apply) {
        printf("Error: failed to allocate memory for Apply in apply\n");
        free(apply_exp->env);
        free(apply_exp->e);
        free(apply_exp);
        return 0;
    }
    return 1;
}

Exp *apply(Exp *f, Exp *exp, Env *env) {
    Exp *apply_exp = allocate_exp_base("apply");
    if (!(apply_exp && allocate_apply(apply_exp))) return NULL;

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
            // TODO: error message
            return NULL;
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
    if (!f_t) return NULL;
    Type *exp1_t = type(exp1);
    if (!exp1_t) {
        free_type(f_t);
        return NULL;
    }

    Type *t = NULL;

    Type *arg_t = f_t->t->f->arg;
    Type *body_t = f_t->t->f->body;

    if (arg_t->name == GENERIC_T || equal_types(arg_t, exp1_t)) {
        t = copy_type(body_t);
    } else {
        printf("Error: type mismatch in function application: %s cannot be applied to %s\n", to_string_type(f_t), to_string_type(exp1_t));
    }

    //free_type(f_t);
    free_type(exp1_t);
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
    char *buf = (char *) malloc(len * sizeof(char));
    if (!buf) {
        printf("Error: failed to allocate memory in to_string_apply\n");
        return NULL;
    }
    snprintf(buf, len, "(%s %s)", f_str, exp_str);

    free(f_str);
    free(exp_str);

    return buf;
}

int free_apply(Exp *exp, int descend) {
    if (exp->name == APPLY) {
        if (descend && !(free_exp(exp->e->apply->f, 1) && free_exp(exp->e->apply->exp, 1))) return 0;
        free(exp->e->apply);
        free(exp->e);
        free_env(exp->env);
        free(exp);
        return 1;
    } else {
        printf("Warning: attempted to call free_var on non-var\n");
        return 0;
    }
}