#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "expressions.h"

int allocate_or(Exp *or_exp) {
    or_exp->e->or = (Or *) malloc(sizeof(Or));
    if (!or_exp->e->or) {
        printf("Error: failed to allocate memory for Or in or\n");
        free(or_exp->env);
        free(or_exp->e);
        free(or_exp);
        return 0;
    }
    return 1;
}

Exp *or(Exp *exp1, Exp *exp2, Env *env) {
    Exp *or_exp = allocate_exp_base("or");
    if (!(or_exp && allocate_or(or_exp))) return NULL;

    or_exp->name = OR;
    or_exp->is_irreducible = 0;
    or_exp->e->or->exp1 = exp1;
    or_exp->e->or->exp2 = exp2;

    add_env(or_exp->env, env);

    return or_exp;
}

Exp *step_or(Exp *exp) {
    Env *env = exp->env;
    Or *o = exp->e->or;
    Exp *exp1 = o->exp1;
    Exp *exp2 = o->exp2;
    if (!exp1->is_irreducible) {
        add_env(exp1->env, env);
        Exp *new_exp = or(step(exp1), exp2, env);
        return new_exp;
    } else if (!exp2->is_irreducible) {
        add_env(exp2->env, env);
        Exp *new_exp = or(exp1, step(exp2), env);
        return new_exp;
    } else {
        if (exp1->name != BOOLEAN) {
            printf("Error: unexpected dynamic type mismatch in or operation: operand 1 is not a boolean\n");
            return NULL;
        } else if (exp2->name != BOOLEAN) {
            printf("Error: unexpected dynamic type mismatch in or operation: operand 2 is not a boolean\n");
            return NULL;
        } else {
            int val1 = exp1->e->boolean->val;
            int val2 = exp2->e->boolean->val;
            return boolean(val1 || val2);
        }
    }
}

Exp *copy_or(Exp *exp) {
    Or *o = exp->e->or;
    return or(copy_exp(o->exp1), copy_exp(o->exp2), copy_env(exp->env));
}

Type *type_or(Exp *exp) {
    Env *env = exp->env;
    Or *or = exp->e->or;
    Exp *exp1 = or->exp1;
    Exp *exp2 = or->exp2;

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
        printf("Error: type mismatch in or operation: operand 1 is a %s but should be a boolean\n", to_string_type(exp1_t));
    } else if (exp2_t->name != BOOLEAN_T) {
        printf("Error: type mismatch in or operation: operand 2 is a %s but should be a boolean\n", to_string_type(exp2_t));
    } else {
        t = (Type *) malloc(sizeof(Type));
        if (!t) printf("Error: failed to allocate memory for Type in type_or\n");
        else t->name = BOOLEAN_T;
    }

    free_type(exp1_t);
    free_type(exp2_t);
    return t;
}

char *to_string_or(Exp *exp) {
    Or *or = exp->e->or;
    char *exp1_str = to_string_exp(or->exp1);
    char *exp2_str = to_string_exp(or->exp2);
    size_t len1 = strlen(exp1_str);
    size_t len2 = strlen(exp2_str);
    size_t len = (len1 + len2 + 7);
    char *buf = (char *) malloc(len * sizeof(char));
    if (!buf) {
        printf("Error: failed to allocate memory in to_string_or\n");
        return NULL;
    }
    snprintf(buf, len, "(%s || %s)", exp1_str, exp2_str);

    free(exp1_str);
    free(exp2_str);

    return buf;
}

int free_or(Exp *exp, int descend) {
    if (exp->name == OR) {
        if (descend && !(free_exp(exp->e->or->exp1, 1) && free_exp(exp->e->or->exp2, 1))) return 0;
        free(exp->e->or);
        free(exp->e);
        free_env(exp->env);
        free(exp);
        return 1;
    } else {
        printf("Warning: attempted to call free_or on non-or\n");
        return 0;
    }
}