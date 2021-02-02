#include <stdlib.h>
#include <stdio.h>
#include "expressions.h"

Exp *step(Exp *exp) {
    if (exp->is_irreducible) {
        printf("Error: attempted to take reduction step on irreducible e: %s\n", to_string_exp(exp));
        return NULL;
    }

    switch (exp->name) {
        case PLUS:      return step_plus(exp);
        case APPLY:     return step_apply(exp);
        case GET_VAR:   return step_get_var(exp);

        default: printf("Error: attempted to take reduction step on unknown e: %s\n", to_string_exp(exp)); return NULL;
    }
}

Type *type(Exp *exp) {
    Type *t;
    switch (exp->name) {
        case UNIT:      t = (Type *) malloc(sizeof(Type)); t->name = UNIT_T; break; //TODO: handle malloc errors
        case INTEGER:   t = (Type *) malloc(sizeof(Type)); t->name = INTEGER_T; break;
        case FUNCTION:  t = type_function(exp); break;
        //case SUM:       t = type_sum(e); break;
        //case PRODUCT:   t = type_product(e); break;

        case PLUS:      t = type_plus(exp); break;
        case APPLY:     t = type_apply(exp); break;
        case VAR:       t = type_var(exp); break;
        case GET_VAR:   t = type_get_var(exp); break;

        default:        t = NULL;
    }
    return t;
}

Exp *copy_exp(Exp *exp) {
    if (!exp) return NULL;
    switch (exp->name) {
        case UNIT:      return copy_unit();
        case INTEGER:   return copy_integer(exp);
        case FUNCTION:  return copy_function(exp);
        case PLUS:      return copy_plus(exp);
        case APPLY:     return copy_apply(exp);
        case VAR:       return copy_var(exp);
        case GET_VAR:   return copy_get_var(exp);

        default: return NULL; // TODO: error
    }
}

char *to_string_exp(Exp *exp) {
    if (!exp) return NULL;
    switch (exp->name) {
        case UNIT:      return to_string_unit();
        case INTEGER:   return to_string_integer(exp);
        case FUNCTION:  return to_string_function(exp);
        case PLUS:      return to_string_plus(exp);
        case APPLY:     return to_string_apply(exp);
        case VAR:       return to_string_var(exp);
        case GET_VAR:   return to_string_get_var(exp);

        default: return NULL;
    }
}

int free_exp(Exp *exp, int descend) {
    if (!exp) return 1;

    switch (exp->name) {
        case UNIT:      return free_unit(exp);
        case INTEGER:   return free_integer(exp);
        case FUNCTION:  return free_function(exp, descend);
            //case SUM:       return to_string_sum(e, descend);
            //case PRODUCT:   return to_string_product(e, descend);

        case PLUS:      return free_plus(exp, descend);
        case APPLY:     return free_apply(exp, descend);
        case VAR:       return free_var(exp);
        case GET_VAR:   return free_get_var(exp);

        default: return 0;
    }
}

Exp *allocate_exp_base(char *name) {
    Exp *exp = (Exp *) malloc(sizeof(Exp));
    if (!exp) {
        printf("Error: failed to allocate memory for Exp in %s\n", name);
        return NULL;
    }
    exp->name = UNDEFINED;
    exp->is_irreducible = 1;

    exp->e = (ExpChoice *) malloc(sizeof(ExpChoice));
    if (!exp->e) {
        printf("Error: failed to allocate memory for ExpChoice in %s\n", name);
        free(exp);
        return NULL;
    }

    exp->env = (Env *) malloc(sizeof(Env));
    if (!exp->env) {
        printf("Error: failed to allocate memory for Env in %s\n", name);
        free(exp->e);
        free(exp);
        return NULL;
    }
    exp->env->ef = NULL;
    exp->env->back_ef = NULL;

    return exp;
}