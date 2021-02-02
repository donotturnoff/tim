#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "expressions.h"

int allocate_not(Exp *not_exp) {
    not_exp->e->not = (Not *) malloc(sizeof(Not));
    if (!not_exp->e->not) {
        printf("Error: failed to allocate memory for Not in not\n");
        free(not_exp->env);
        free(not_exp->e);
        free(not_exp);
        return 0;
    }
    return 1;
}

Exp *not(Exp *exp1, Env *env) {
    Exp *not_exp = allocate_exp_base("not");
    if (!(not_exp && allocate_not(not_exp))) return NULL;

    not_exp->name = NOT;
    not_exp->is_irreducible = 0;
    not_exp->e->not->exp1 = exp1;

    add_env(not_exp->env, env);

    return not_exp;
}

Exp *step_not(Exp *exp) {
    Env *env = exp->env;
    Not *n = exp->e->not;
    Exp *exp1 = n->exp1;
    if (!exp1->is_irreducible) {
        add_env(exp1->env, env);
        Exp *new_exp = not(step(exp1), env);
        return new_exp;
    } else {
        if (exp1->name != BOOLEAN) {
            printf("Error: unexpected dynamic type mismatch in not operation: operand 1 is not a boolean\n");
            return NULL;
        } else {
            int val1 = exp1->e->boolean->val;
            return boolean(!val1);
        }
    }
}

Type *type_not(Exp *exp) {
    Env *env = exp->env;
    Not *not = exp->e->not;
    Exp *exp1 = not->exp1;

    add_env(exp1->env, env);
    Type *exp1_t = type(exp1);
    if (!exp1_t) return NULL;

    Type *t = NULL;
    if (exp1_t->name != BOOLEAN_T) {
        printf("Error: type mismatch in not operation: operand 1 is a %s but should be a boolean\n", to_string_type(exp1_t));
    } else {
        t = (Type *) malloc(sizeof(Type));
        if (!t) printf("Error: failed to allocate memory for Type in type_not\n");
        else t->name = BOOLEAN_T;
    }

    free(exp1_t);
    return t;
}

Exp *copy_not(Exp *exp) {
    Not *n = exp->e->not;
    return not(copy_exp(n->exp1), copy_env(exp->env));
}

char *to_string_not(Exp *exp) {
    Not *not = exp->e->not;
    char *exp1_str = to_string_exp(not->exp1);
    size_t len1 = strlen(exp1_str);
    size_t len = (len1 + 4);
    char *buf = (char *) malloc(len * sizeof(char));
    if (!buf) {
        printf("Error: failed to allocate memory in to_string_not\n");
        return NULL;
    }
    snprintf(buf, len, "(!%s)", exp1_str);

    free(exp1_str);

    return buf;
}

int free_not(Exp *exp, int descend) {
    if (exp->name == NOT) {
        if (descend && !free_exp(exp->e->not->exp1, 1)) return 0;
        free(exp->e->not);
        free(exp->e);
        free_env(exp->env);
        free(exp);
        return 1;
    } else {
        printf("Warning: attempted to call free_not on non-not\n");
        return 0;
    }
}