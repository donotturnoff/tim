#ifndef TIM_EXPRESSIONS_H
#define TIM_EXPRESSIONS_H

#include "environment.h"
#include "types.h"

typedef struct env Env;
typedef struct type_env TypeEnv;

typedef union exp_choice ExpChoice;
typedef struct unit Unit;
typedef struct boolean Boolean;
typedef struct integer Integer;
typedef struct function Function;
typedef struct sum Sum;
typedef struct product Product;
typedef struct plus Plus;
typedef struct minus Minus;
typedef struct and And;
typedef struct or Or;
typedef struct not Not;
typedef struct apply Apply;
typedef struct var Var;
typedef struct get_var GetVar;

typedef enum exp_name {
    UNDEFINED, UNIT, BOOLEAN, INTEGER, FUNCTION, SUM, PRODUCT,
    PLUS, MINUS, AND, OR, NOT, APPLY, VAR, GET_VAR
} ExpName;

typedef struct exp {
    ExpName name;
    int is_irreducible;
    ExpChoice *e;
    Env *env;
    // TODO: separate type environment
} Exp;

typedef union exp_choice {
    Unit *unit;
    Boolean *boolean;
    Integer *integer;
    Function *function;
    Sum *sum;
    Product *product;

    Plus *plus;
    Minus *minus;
    And *and;
    Or *or;
    Not *not;
    Apply *apply;
    Var *var;
    GetVar *get_var;
} ExpChoice;

typedef struct unit {} Unit;

typedef struct boolean {
    int val;
} Boolean;

typedef struct integer {
    int val;
} Integer;

typedef struct function {
    Exp *arg;
    Exp *body;
} Function;

typedef struct sum {
    Exp *exp1, *exp2;
} Sum;

typedef struct product {
    Exp *exp1, *exp2;
} Product;

typedef struct plus {
    Exp *exp1, *exp2;
} Plus;

typedef struct minus {
    Exp *exp1, *exp2;
} Minus;

typedef struct and {
    Exp *exp1, *exp2;
} And;

typedef struct or {
    Exp *exp1, *exp2;
} Or;

typedef struct not {
    Exp *exp1;
} Not;

typedef struct apply {
    Exp *f;
    Exp *exp;
} Apply;

typedef struct var {
    char *name;
    Type *t;
} Var;

typedef struct get_var {
    char *name;
} GetVar;

Exp *step(Exp *exp);
Type *type(Exp *exp);
char *to_string_exp(Exp *exp);
Exp *copy_exp(Exp *exp);
int free_exp(Exp *exp, int descend);
Exp *allocate_exp_base(char *name);

Exp *and(Exp *exp1, Exp *exp2, Env *env);
Exp *step_and(Exp *exp);
Type *type_and(Exp *exp);
Exp *copy_and(Exp *exp);
char *to_string_and(Exp *exp);
int free_and(Exp *exp, int descend);

Exp *apply(Exp *f, Exp *exp, Env *env);
Exp *step_apply(Exp *exp);
Type *type_apply(Exp *exp);
Exp *copy_apply(Exp *exp);
char *to_string_apply(Exp *exp);
int free_apply(Exp *exp, int descend);

Exp *boolean(int val);
Exp *copy_boolean(Exp *exp);
char *to_string_boolean(Exp *exp);
int free_boolean(Exp *exp);

Exp *function(Exp *arg, Exp *body, Env *env);
Type *type_function(Exp *exp);
Exp *copy_function(Exp *exp);
char *to_string_function(Exp *exp);
int free_function(Exp *exp, int descend);

Exp *get_var(char *name, Env *env);
Exp *step_get_var(Exp *exp);
Type *type_get_var(Exp *exp);
Exp *copy_get_var(Exp *exp);
char *to_string_get_var(Exp *exp);
int free_get_var(Exp *exp);

Exp *integer(int val);
Exp *copy_integer(Exp *exp);
char *to_string_integer(Exp *exp);
int free_integer(Exp *exp);

Exp *minus(Exp *exp1, Exp *exp2, Env *env);
Exp *step_minus(Exp *exp);
Type *type_minus(Exp *exp);
Exp *copy_minus(Exp *exp);
char *to_string_minus(Exp *exp);
int free_minus(Exp *exp, int descend);

Exp *not(Exp *exp1, Env *env);
Exp *step_not(Exp *exp);
Type *type_not(Exp *exp);
Exp *copy_not(Exp *exp);
char *to_string_not(Exp *exp);
int free_not(Exp *exp, int descend);

Exp *or(Exp *exp1, Exp *exp2, Env *env);
Exp *step_or(Exp *exp);
Type *type_or(Exp *exp);
Exp *copy_or(Exp *exp);
char *to_string_or(Exp *exp);
int free_or(Exp *exp, int descend);

Exp *plus(Exp *exp1, Exp *exp2, Env *env);
Exp *step_plus(Exp *exp);
Type *type_plus(Exp *exp);
Exp *copy_plus(Exp *exp);
char *to_string_plus(Exp *exp);
int free_plus(Exp *exp, int descend);

/*Exp *product(Exp *exp1, Exp *exp2, Env *env);
Type *type_product(Exp *e);
char *to_string_product(Exp *e);
int free_product(Exp *e);

Exp *sum(Exp *exp1, Exp *exp2, Env *env);
Type *type_sum(Exp *e);
char *to_string_sum(Exp *e);
int free_sum(Exp *e);*/ //TODO: sums and products

Exp *unit(void);
Exp *copy_unit(void);
char *to_string_unit(void);
int free_unit(Exp *exp);

Exp *var(char *name, Type *t);
Exp *step_var(Exp *exp);
Type *type_var(Exp *exp);
Exp *copy_var(Exp *exp);
char *to_string_var(Exp *exp);
int free_var(Exp *exp);

#endif //TIM_EXPRESSIONS_H
