#include <stdlib.h>
#include <stdio.h>
#include "expressions.h"

Exp *unit(void) {
    Exp *unit_exp = allocate_exp_base();
    unit_exp->e->unit = (Unit *) malloc_or_die(sizeof(Unit));

    unit_exp->name = UNIT;
    unit_exp->is_irreducible = 1;

    return unit_exp;
}

Type *type_unit(void) {
    return unit_t();
}

Exp *copy_unit(void) {
    return unit();
}

char *to_string_unit(void) {
    char *str = (char *) malloc_or_die(3 * sizeof(char));
    snprintf(str, 3, "()");
    return str;
}

void free_unit(Exp *exp) {
    if (exp->name == UNIT) {
        free(exp->e->unit);
        free(exp->e);
        free_env(exp->env);
        free(exp);
    } else {
        die(INTERPRETER_ERR, "attempted to call free_unit on non-unit");
    }
}