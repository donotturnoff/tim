#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "expressions.h"

int allocate_and(Exp *and_exp) {
    and_exp->e->and = (And *) malloc(sizeof(And));
    if (!and_exp->e->and) {
        printf("Error: failed to allocate memory for And in and\n");
        free(and_exp->env);
        free(and_exp->e);
        free(and_exp);
        return 0;
    }
    return 1;
}

Exp *and(Exp *exp1, Exp *exp2, Env *env) {
    Exp *and_exp = allocate_exp_base("and");
    if (!(and_exp && allocate_and(and_exp))) return NULL;

    and_exp->name = AND;
    and_exp->is_irreducible = 0;
    and_exp->e->and->exp1 = exp1;
    and_exp->e->and->exp2 = exp2;

    add_env(and_exp->env, env);

    return and_exp;
}

Exp *step_and(Exp *exp) {
    Env *env = exp->env;
    And *a = exp->e->and;
    Exp *exp1 = a->exp1;
    Exp *exp2 = a->exp2;
    if (!exp1->is_irreducible) {
        add_env(exp1->env, env);
        Exp *new_exp = and(step(exp1), exp2, env);
        return new_exp;
    } else if (!exp2->is_irreducible) {
        add_env(exp2->env, env);
        Exp *new_exp = and(exp1, step(exp2), env);
        return new_exp;
    } else {
        if (exp1->name != BOOLEAN) {
            printf("Error: unexpected dynamic type mismatch in and operation: operand 1 is not a boolean\n");
            return NULL;
        } else if (exp2->name != BOOLEAN) {
            printf("Error: unexpected dynamic type mismatch in and operation: operand 2 is not a boolean\n");
            return NULL;
        } else {
            int val1 = exp1->e->boolean->val;
            int val2 = exp2->e->boolean->val;
            return boolean(val1 && val2);
        }
    }
}

Type *type_and(Exp *exp) {
    Env *env = exp->env;
    And *a = exp->e->and;
    Exp *exp1 = a->exp1;
    Exp *exp2 = a->exp2;

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
    if (exp1_t->name != BOOLEAN_T) {
        printf("Error: type mismatch in and operation: operand 1 is a %s but should be a boolean\n", to_string_type(exp1_t));
    } else if (exp2_t->name != BOOLEAN_T) {
        printf("Error: type mismatch in and operation: operand 2 is a %s but should be a boolean\n", to_string_type(exp2_t));
    } else {
        t = (Type *) malloc(sizeof(Type));
        if (!t) printf("Error: failed to allocate memory for Type in type_and\n");
        else t->name = BOOLEAN_T;
    }

    free_type(exp1_t);
    free_type(exp2_t);
    return t;
}

Exp *copy_and(Exp *exp) {
    And *a = exp->e->and;
    return and(copy_exp(a->exp1), copy_exp(a->exp2), copy_env(exp->env));
}

char *to_string_and(Exp *exp) {
    And *and = exp->e->and;
    char *exp1_str = to_string_exp(and->exp1);
    char *exp2_str = to_string_exp(and->exp2);
    size_t len1 = strlen(exp1_str);
    size_t len2 = strlen(exp2_str);
    size_t len = (len1 + len2 + 7);
    char *buf = (char *) malloc(len * sizeof(char));
    if (!buf) {
        printf("Error: failed to allocate memory in to_string_and\n");
        return NULL;
    }
    snprintf(buf, len, "(%s && %s)", exp1_str, exp2_str);

    free(exp1_str);
    free(exp2_str);

    return buf;
}

int free_and(Exp *exp, int descend) {
    if (exp->name == AND) {
        if (descend && !(free_exp(exp->e->and->exp1, 1) && free_exp(exp->e->and->exp2, 1))) return 0;
        free(exp->e->and);
        free_env(exp->env);
        free(exp->e);
        free(exp);
        return 1;
    } else {
        printf("Warning: attempted to call free_and on non-and\n");
        return 0;
    }
}