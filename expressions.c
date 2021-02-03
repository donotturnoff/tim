#include <stdlib.h>
#include <stdio.h>
#include "expressions.h"

Exp *step(Exp *exp) {
    if (exp->is_irreducible)
        die(RUNTIME_ERR, "attempted to take reduction step on irreducible expression %s", to_string_exp(exp));

    switch (exp->name) {
        case PLUS:      return step_plus(exp);
        case APPLY:     return step_apply(exp);
        case GET_VAR:   return step_get_var(exp);
        default: die(RUNTIME_ERR, "attempted to take reduction step on unknown expression");
    }
}

Type *type(TypeEnv *env, Exp *exp, Type *expected) {
    Type *t = NULL;
    switch (exp->name) {
        case UNIT:      t = type_unit(); break;
        case INTEGER:   t = type_integer(); break;
        case FUNCTION:  t = type_function(env, exp, expected); break;
        case PLUS:      t = type_plus(env, exp); break;
        case APPLY:     t = type_apply(env, exp, expected); break;
        case VAR:       t = type_var(env, exp, expected); break;
        case GET_VAR:   t = type_get_var(env, exp, expected); break;

        default: die(INTERPRETER_ERR, "attempted to type unknown expression");
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

        default: die(INTERPRETER_ERR, "attempted to copy unknown expression");
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

        default: die(INTERPRETER_ERR, "attempted to convert unknown expression to string");
    }
}

void free_exp(Exp *exp, int descend) {
    if (!exp) die(INTERPRETER_ERR, "attempted to free null expression");

    switch (exp->name) {
        case UNIT:      free_unit(exp);
        case INTEGER:   free_integer(exp);
        case FUNCTION:  free_function(exp, descend);
        case PLUS:      free_plus(exp, descend);
        case APPLY:     free_apply(exp, descend);
        case VAR:       free_var(exp);
        case GET_VAR:   free_get_var(exp);

        default: die(INTERPRETER_ERR, "attempted to free unknown expression");
    }
}

Exp *allocate_exp_base() {
    Exp *exp = (Exp *) malloc_or_die(sizeof(Exp));
    exp->e = (ExpChoice *) malloc_or_die(sizeof(ExpChoice));
    exp->env = (Env *) malloc_or_die(sizeof(Env));

    exp->name = UNDEFINED;
    exp->is_irreducible = 1;
    exp->env->ef = NULL;
    exp->env->back_ef = NULL;

    return exp;
}