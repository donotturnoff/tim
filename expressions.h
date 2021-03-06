#ifndef TIM_EXPRESSIONS_H
#define TIM_EXPRESSIONS_H

#include "tim.h"
#include "environment.h"
#include "types.h"

typedef struct env Env;
typedef struct type_env TypeEnv;

typedef union exp_choice ExpChoice;
typedef struct unit Unit;
typedef struct integer Integer;
typedef struct function Function;
typedef struct plus Plus;
typedef struct apply Apply;
typedef struct var Var;
typedef struct get_var GetVar;

typedef enum exp_name {
    UNDEFINED, UNIT, INTEGER, FUNCTION,
    PLUS, APPLY, VAR, GET_VAR
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
    Integer *integer;
    Function *function;

    Plus *plus;
    Apply *apply;
    Var *var;
    GetVar *get_var;
} ExpChoice;

typedef struct unit {} Unit;

typedef struct integer {
    int val;
} Integer;

typedef struct function {
    Exp *arg;
    Exp *body;
} Function;

typedef struct plus {
    Exp *exp1, *exp2;
} Plus;

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
Type *type(TypeEnv *env, Exp *exp, Type *expected);
char *to_string_exp(Exp *exp);
Exp *copy_exp(Exp *exp);
void free_exp(Exp *exp, int descend);
Exp *allocate_exp_base(void);

Exp *apply(Exp *f, Exp *exp, Env *env);
Exp *step_apply(Exp *exp);
Type *type_apply(TypeEnv *env, Exp *exp, Type *expected);
Exp *copy_apply(Exp *exp);
char *to_string_apply(Exp *exp);
void free_apply(Exp *exp, int descend);

Exp *function(Exp *arg, Exp *body, Env *env);
Type *type_function(TypeEnv *env, Exp *exp, Type *expected);
Exp *copy_function(Exp *exp);
char *to_string_function(Exp *exp);
void free_function(Exp *exp, int descend);

Exp *get_var(char *name, Env *env);
Exp *step_get_var(Exp *exp);
Type *type_get_var(TypeEnv *env, Exp *exp, Type *expected);
Exp *copy_get_var(Exp *exp);
char *to_string_get_var(Exp *exp);
void free_get_var(Exp *exp);

Exp *integer(int val);
Exp *copy_integer(Exp *exp);
Type *type_integer(void);
char *to_string_integer(Exp *exp);
void free_integer(Exp *exp);

Exp *plus(Exp *exp1, Exp *exp2, Env *env);
Exp *step_plus(Exp *exp);
Type *type_plus(TypeEnv *env, Exp *exp);
Exp *copy_plus(Exp *exp);
char *to_string_plus(Exp *exp);
void free_plus(Exp *exp, int descend);

Exp *unit(void);
Exp *copy_unit(void);
Type *type_unit(void);
char *to_string_unit(void);
void free_unit(Exp *exp);

Exp *var(char *name, Type *t);
Type *type_var(TypeEnv *env, Exp *exp, Type *expected);
Exp *copy_var(Exp *exp);
char *to_string_var(Exp *exp);
void free_var(Exp *exp);

#endif //TIM_EXPRESSIONS_H
