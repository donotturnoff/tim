#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "expressions.h"

int allocate_minus(Exp *minus_exp) {
    minus_exp->e->minus = (Minus *) malloc(sizeof(Minus));
    if (!minus_exp->e->minus) {
        printf("Error: failed to allocate memory for Minus in minus\n");
        free(minus_exp->env);
        free(minus_exp->e);
        free(minus_exp);
        return 0;
    }
    return 1;
}

Exp *minus(Exp *exp1, Exp *exp2, Env *env) {
    Exp *minus_exp = allocate_exp_base("minus");
    if (!(minus_exp && allocate_minus(minus_exp))) return NULL;

    minus_exp->name = MINUS;
    minus_exp->is_irreducible = 0;
    minus_exp->e->minus->exp1 = exp1;
    minus_exp->e->minus->exp2 = exp2;

    add_env(minus_exp->env, env);

    return minus_exp;
}

Exp *step_minus(Exp *exp) {
    Env *env = exp->env;
    Minus *m = exp->e->minus;
    Exp *exp1 = m->exp1;
    Exp *exp2 = m->exp2;
    if (!exp1->is_irreducible) {
        add_env(exp1->env, env);
        Exp *new_exp = minus(step(exp1), exp2, env);
        return new_exp;
    } else if (!exp2->is_irreducible) {
        add_env(exp2->env, env);
        Exp *new_exp = minus(exp1, step(exp2), env);
        return new_exp;
    } else {
        if (exp1->name != INTEGER) {
            printf("Error: unexpected dynamic type mismatch in minus operation: operand 1 is not an integer\n");
            return NULL;
        } else if (exp2->name != INTEGER) {
            printf("Error: unexpected dynamic type mismatch in minus operation: operand 2 is not an integer\n");
            return NULL;
        } else {
            int val1 = exp1->e->integer->val;
            int val2 = exp2->e->integer->val;
            return integer(val1 - val2);
        }
    }
}

Type *type_minus(Exp *exp) {
    Env *env = exp->env;
    Minus *m = exp->e->minus;
    Exp *exp1 = m->exp1;
    Exp *exp2 = m->exp2;

    add_env(exp1->env, env);
    add_env(exp2->env, env);

    Type *exp1_t = type(exp1);
    if (!exp1_t) return NULL;
    Type *exp2_t = type(exp2);
    if (!exp2_t) {
        free_type(exp1_t);
        return NULL;
    }

    Type *t = NULL;
    if (exp1_t->name != INTEGER_T) {
        printf("Error: type mismatch in minus operation: operand 1 is a %s but should be an integer\n", to_string_type(exp1_t));
    } else if (exp2_t->name != INTEGER_T) {
        printf("Error: type mismatch in minus operation: operand 2 is a %s but should be an integer\n", to_string_type(exp2_t));
    } else {
        t = (Type *) malloc(sizeof(Type));
        if (!t) printf("Error: failed to allocate memory for Type in type_minus\n");
        else t->name = INTEGER_T;
    }

    free_type(exp1_t);
    free_type(exp2_t);
    return t;
}

Exp *copy_minus(Exp *exp) {
    Minus *m = exp->e->minus;
    return minus(copy_exp(m->exp1), copy_exp(m->exp2), copy_env(exp->env));
}

char *to_string_minus(Exp *exp) {
    Minus *minus = exp->e->minus;
    char *exp1_str = to_string_exp(minus->exp1);
    char *exp2_str = to_string_exp(minus->exp2);
    size_t len1 = strlen(exp1_str);
    size_t len2 = strlen(exp2_str);
    size_t len = (len1 + len2 + 6);
    char *buf = (char *) malloc(len * sizeof(char));
    if (!buf) {
        printf("Error: failed to allocate memory in to_string_minus\n");
        return NULL;
    }
    snprintf(buf, len, "(%s - %s)", exp1_str, exp2_str);

    free(exp1_str);
    free(exp2_str);

    return buf;
}

int free_minus(Exp *exp, int descend) {
    if (exp->name == MINUS) {
        if (descend && !(free_exp(exp->e->minus->exp1, 1) && free_exp(exp->e->minus->exp2, 1))) return 0;
        free(exp->e->minus);
        free_env(exp->env);
        free(exp->e);
        free(exp);
        return 1;
    } else {
        printf("Warning: attempted to call free_minus on non-minus\n");
        return 0;
    }
}