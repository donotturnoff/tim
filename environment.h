#ifndef TIM_ENVIRONMENT_H
#define TIM_ENVIRONMENT_H

#include "tim.h"
#include "expressions.h"
#include "types.h"
#include "environment.h"

typedef struct exp Exp;
typedef struct env_frame EnvFrame;
typedef struct type_exp TypeExp;
typedef struct type_env_frame TypeEnvFrame;

typedef struct env {
    EnvFrame *ef, *back_ef;
} Env;

typedef struct env_frame {
    char *name;
    Exp *val;
    EnvFrame *next, *prev;
} EnvFrame;

typedef struct type_env {
    TypeEnvFrame *ef;
} TypeEnv;

typedef struct type_env_frame {
    char *name;
    Type *t;
    TypeEnvFrame *next;
} TypeEnvFrame;

void put_env_var(Env *env, char *name, Exp *val);
Exp *get_env_var(Env *env, char *name);
void add_env(Env *dst_env, Env *src_env);
Env *copy_env(Env *env);
void free_env(Env *env);
char *to_string_env(Env *env);

Type *get_type_env_var(TypeEnv *env, char *name);
int set_type_env_var(TypeEnv *env, char *name, Type *t);
void push_type_env_var(TypeEnv *env, char *name, Type *t);
void pop_type_env_var(TypeEnv *env);
char *to_string_type_env(TypeEnv *env);

#endif //TIM_ENVIRONMENT_H
