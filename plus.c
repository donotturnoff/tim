#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "expressions.h"

Exp *plus(Exp *exp1, Exp *exp2, Env *env) {
    Exp *plus_exp = allocate_exp_base();
    plus_exp->e->plus = (Plus *) malloc_or_die(sizeof(Plus));

    plus_exp->name = PLUS;
    plus_exp->is_irreducible = 0;
    plus_exp->e->plus->exp1 = exp1;
    plus_exp->e->plus->exp2 = exp2;

    add_env(plus_exp->env, env);

    return plus_exp;
}

Exp *step_plus(Exp *exp) {
    Env *env = exp->env;
    Plus *p = exp->e->plus;
    Exp *exp1 = p->exp1;
    Exp *exp2 = p->exp2;
    if (!exp1->is_irreducible) {
        add_env(exp1->env, env);
        Exp *new_exp = plus(step(exp1), exp2, env);
        //free_exp(exp1, 1);
        //free_plus(exp, 0);
        return new_exp;
    } else if (!exp2->is_irreducible) {
        add_env(exp2->env, env);
        Exp *new_exp = plus(exp1, step(exp2), env);
        //free_exp(exp2, 1);
        //free_plus(exp, 0);
        return new_exp;
    } else {
        if (exp1->name != INTEGER) {
            die(RUNTIME_ERR, "argument 1 of plus is not an integer");
        } else if (exp2->name != INTEGER) {
            die(RUNTIME_ERR, "argument 2 of plus is not an integer");
        } else {
            int val1 = exp1->e->integer->val;
            int val2 = exp2->e->integer->val;
            //free_exp(exp, 1);
            return integer(val1 + val2);
        }
    }
}

Type *type_plus(TypeEnv *env, Exp *exp) {
    Plus *plus = exp->e->plus;
    Exp *exp1 = plus->exp1;
    Exp *exp2 = plus->exp2;

    Type *exp1_t = type(env, exp1, integer_t());
    Type *exp2_t = type(env, exp2, integer_t());

    if (exp1_t->name != INTEGER_T) {
        die(TYPE_ERR, "type mismatch in plus operation: operand 1 is a %s but should be an integer",
            to_string_type(exp1_t));
    } else if (exp2_t->name != INTEGER_T) {
        die(TYPE_ERR, "type mismatch in plus operation: operand 2 is a %s but should be an integer",
            to_string_type(exp2_t));
    } else {
        Type *t = (Type *) malloc_or_die(sizeof(Type));
        t->name = INTEGER_T;
        return t;
    }
}

Exp *copy_plus(Exp *exp) {
    Plus *p = exp->e->plus;
    return plus(copy_exp(p->exp1), copy_exp(p->exp2), copy_env(exp->env));
}

char *to_string_plus(Exp *exp) {
    Plus *plus = exp->e->plus;
    char *exp1_str = to_string_exp(plus->exp1);
    char *exp2_str = to_string_exp(plus->exp2);
    size_t len1 = strlen(exp1_str);
    size_t len2 = strlen(exp2_str);
    size_t len = (len1 + len2 + 6);
    char *buf = (char *) malloc_or_die(len * sizeof(char));
    snprintf(buf, len, "(%s + %s)", exp1_str, exp2_str);

    free(exp1_str);
    free(exp2_str);

    return buf;
}

void free_plus(Exp *exp, int descend) {
    if (exp->name == PLUS) {
        if (descend) {
            free_exp(exp->e->plus->exp1, 1);
            free_exp(exp->e->plus->exp2, 1);
        }
        free(exp->e->plus);
        free_env(exp->env);
        free(exp->e);
        free(exp);
    } else {
        die(INTERPRETER_ERR, "attempted to call free_plus on non-plus");
    }
}