#include <stdlib.h>
#include <stdio.h>
#include "expressions.h"

int allocate_unit(Exp *unit_exp) {
    unit_exp->e->unit = (Unit *) malloc(sizeof(Unit));
    if (!unit_exp->e->unit) {
        printf("Error: failed to allocate memory for Unit in unit\n");
        free(unit_exp->env);
        free(unit_exp->e);
        free(unit_exp);
        return 0;
    }
    return 1;
}

Exp *unit(void) {
    Exp *unit_exp = allocate_exp_base("unit");
    if (!(unit_exp && allocate_unit(unit_exp))) return NULL;

    unit_exp->name = UNIT;
    unit_exp->is_irreducible = 1;

    return unit_exp;
}

Exp *copy_unit(void) {
    return unit();
}

char *to_string_unit(void) {
    char *str;
    str = (char *) malloc(3 * sizeof(char));
    if (!str) printf("Error: failed to allocate memory for char * in to_string_unit\n");
    else snprintf(str, 3, "()");
    return str;
}

int free_unit(Exp *exp) {
    if (exp->name == UNIT) {
        free(exp->e->unit);
        free(exp->e);
        free_env(exp->env);
        free(exp);
        return 1;
    } else {
        printf("Warning: attempted to call free_unit on non-unit\n");
        return 0;
    }
}