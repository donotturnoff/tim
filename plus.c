#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "expressions.h"

int allocate_plus(Exp *plus_exp) {
    plus_exp->e->plus = (Plus *) malloc(sizeof(Plus));
    if (!plus_exp->e->plus) {
        printf("Error: failed to allocate memory for Plus in plus\n");
        free(plus_exp->env);
        free(plus_exp->e);
        free(plus_exp);
        return 0;
    }
    return 1;
}

Exp *plus(Exp *exp1, Exp *exp2, Env *env) {
    Exp *plus_exp = allocate_exp_base("plus");
    if (!(plus_exp && allocate_plus(plus_exp))) return NULL;

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
            printf("Error: unexpected dynamic type mismatch in plus operation: operand 1 is not an integer\n");
            return NULL;
        } else if (exp2->name != INTEGER) {
            printf("Error: unexpected dynamic type mismatch in plus operation: operand 2 is not an integer\n");
            return NULL;
        } else {
            int val1 = exp1->e->integer->val;
            int val2 = exp2->e->integer->val;
            //free_exp(exp, 1);
            return integer(val1 + val2);
        }
    }
}

Type *type_plus(Exp *exp) {
    Env *env = exp->env;
    Plus *plus = exp->e->plus;
    Exp *exp1 = plus->exp1;
    Exp *exp2 = plus->exp2;

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
        printf("Error: type mismatch in plus operation: operand 1 is a %s but should be an integer\n", to_string_type(exp1_t));
    } else if (exp2_t->name != INTEGER_T) {
        printf("Error: type mismatch in plus operation: operand 2 is a %s but should be an integer\n", to_string_type(exp2_t));
    } else {
        t = (Type *) malloc(sizeof(Type));
        if (!t) printf("Error: failed to allocate memory for Type in type_plus\n");
        else t->name = INTEGER_T;
    }

    free_type(exp1_t);
    free_type(exp2_t);
    return t;
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
    char *buf = (char *) malloc(len * sizeof(char));
    if (!buf) {
        printf("Error: failed to allocate memory in to_string_plus\n");
        return NULL;
    }
    snprintf(buf, len, "(%s + %s)", exp1_str, exp2_str);

    free(exp1_str);
    free(exp2_str);

    return buf;
}

int free_plus(Exp *exp, int descend) {
    if (exp->name == PLUS) {
        if (descend && !(free_exp(exp->e->plus->exp1, 1) && free_exp(exp->e->plus->exp2, 1))) return 0;
        free(exp->e->plus);
        free_env(exp->env);
        free(exp->e);
        free(exp);
        return 1;
    } else {
        printf("Warning: attempted to call free_plus on non-plus\n");
        return 0;
    }
}