#ifndef TIM_ENVIRONMENT_H
#define TIM_ENVIRONMENT_H

#include "expressions.h"
#include "types.h"
#include "environment.h"

typedef struct exp Exp;
typedef struct env_frame EnvFrame;

typedef struct env {
    EnvFrame *ef, *back_ef;
} Env;

typedef struct env_frame {
    char *name;
    Exp *val;
    Type *t;
    EnvFrame *next, *prev;
} EnvFrame;

int put_env_var(Env *env, char *name, Type *t, Exp *val);
Type *get_env_var_type(Env *env, char *name);
Exp *get_env_var(Env *env, char *name);
int add_env(Env *dst_env, Env *src_env);
Env *copy_env(Env *env);
void free_env(Env *env);
const char *to_string_env(Env *env);
void print_env(Env *env);

#endif //TIM_ENVIRONMENT_H
