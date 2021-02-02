#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "expressions.h"

Type *type_sum(Exp *exp) {
    Env *env = exp->env;
    Sum *sum = exp->e->sum;
    Exp *exp1 = sum->exp1;
    Exp *exp2 = sum->exp2;

    add_env(exp1->env, env);
    add_env(exp2->env, env);
    Type *exp1_t = type(exp1);
    if (!exp1_t) return NULL;
    Type *exp2_t = type(exp2);
    if (!exp2_t) {
        free_type(exp1_t);
        return NULL;
    }

    Type *t = (Type *) malloc(sizeof(Type));
    if (!t) {
        printf("Error: failed to allocate memory for Type in type_sum\n");
    } else {
        t->name = SUM_T;
        t->t = (TypeChoice *) malloc(sizeof(TypeChoice));
        if (!t->t) {
            printf("Error: failed to allocate memory for TypeChoice in type_sum\n");
            free(t);
            t = NULL;
        } else {
            t->t->s = (SumType *) malloc(sizeof(SumType));
            if (!t->t->s) {
                printf("Error: failed to allocate memory for SumType in type_sum\n");
                free(t->t);
                free(t);
                t = NULL;
            } else {
                t->t->s->t1 = exp1_t;
                t->t->s->t2 = exp2_t;
            }
        }
    }
    free_type(exp1_t);
    free_type(exp2_t);
    return t;
}

char *to_string_sum(Exp *exp) {
    Sum *s = exp->e->sum;
    char *exp1_str = to_string_exp(s->exp1);
    char *exp2_str = to_string_exp(s->exp2);
    size_t len1 = strlen(exp1_str);
    size_t len2 = strlen(exp2_str);
    size_t len = len1 + len2 + 6;
    char *buf = (char *) malloc(len * sizeof(char));
    if (!buf) {
        printf("Error: failed to allocate memory in to_string_sum\n");
        return NULL;
    }
    snprintf(buf, len, "(%s | %s)", exp1_str, exp2_str); // FIXME: I think this is wrong

    return buf;
}

int free_sum(Exp *exp, int descend) {
    if (exp->name == SUM) {
        if (descend && !(free_exp(exp->e->sum->exp1, 1) && free_exp(exp->e->sum->exp2, 1))) return 0;
        free(exp->e->sum);
        free(exp->e);
        free_env(exp->env);
        free(exp);
        return 1;
    } else {
        printf("Warning: attempted to call free_sum on non-sum\n");
        return 0;
    }
}