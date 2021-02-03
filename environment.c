#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "environment.h"

void put_env_var(Env *env, char *name, Exp *val) {
    if (!env) die(INTERPRETER_ERR, "environment is null in put_env_var(NULL, %s, %s)", name, to_string_exp(val));

    EnvFrame *frame = (EnvFrame *) malloc_or_die(sizeof(EnvFrame));
    frame->name = name;
    frame->val = val;
    frame->next = env->ef;
    if (frame->next) frame->next->prev = frame;
    frame->prev = NULL;
    env->ef = frame;
    if (!env->back_ef) env->back_ef = frame;
}

Exp *get_env_var(Env *env, char *name) {
    if (!env) return NULL;
    EnvFrame *current = env->ef;
    while (current != NULL) {
        if (strcmp(current->name, name) == 0) {
            return copy_exp(current->val); // FIXME: copy_exp()?
        }
        current = current->next;
    }
    return NULL;
}
void add_env(Env *dst_env, Env *src_env) {
    if (!dst_env) die(INTERPRETER_ERR, "destination environment is null in add_env(NULL, %s)\n", to_string_env(src_env));
    if (!src_env) return;

    EnvFrame *current = src_env->back_ef;
    while (current) {
        char *name = current->name;
        Exp *val = current->val;
        put_env_var(dst_env, name, val);
        current = current->prev;
    }
}

Env *copy_env(Env *env) {
    Env *new_env = (Env *) malloc_or_die(sizeof(Env));
    new_env->ef = NULL;
    new_env->back_ef = NULL;
    add_env(new_env, env);
    return new_env;
}

char *to_string_env_frame(EnvFrame *ef) {
    char *name = ef->name;
    char *exp_str = to_string_exp(ef->val);
    size_t buf_size = strlen(name) + strlen(exp_str) + 4;
    char *str = (char *) malloc_or_die(buf_size * sizeof(char));
    snprintf(str, buf_size, "[%s=%s]", name, exp_str);
    return str;
}

char *to_string_env(Env *env) {
    // TODO: fix this mess, this is just a temporary solution while I do more important things
    char *str = (char *) malloc_or_die(1000*sizeof(char));
    str[0] = '{';
    str[1] = '\0';
    EnvFrame *current = env->ef;
    while (current) {
        strcat(str, to_string_env_frame(current));
        current = current->next;
    }
    size_t len = strlen(str);
    str[len] = '}';
    str[len+1] = '\0';
    if (!(str = realloc(str, len+2))) die(MEMORY_ERR, "realloc failed");
    return str;
}

void free_env(Env *env) {
    EnvFrame *current = env->ef;
    while (current) {
        free_exp(current->val, 1);
        EnvFrame *next = current->next;
        free(current);
        current = next;
    }
    free(env);
}

Type *get_type_env_var(TypeEnv *env, char *name) {
    if (!env) die(INTERPRETER_ERR, "environment is null in get_type_env_var(NULL, %s)", name);
    TypeEnvFrame *current = env->ef;
    while (current != NULL) {
        if (strcmp(current->name, name) == 0) {
            return copy_type(current->t); // FIXME: copy_type()?
        }
        current = current->next;
    }
    return NULL;
}

int set_type_env_var(TypeEnv *env, char *name, Type *t) {
    if (!env) die(INTERPRETER_ERR, "environment is null in set_type_env_var(NULL, %s, %s)", name, to_string_type(t));
    TypeEnvFrame *current = env->ef;
    while (current != NULL) {
        if (strcmp(current->name, name) == 0) {
            current->t = copy_type(t); // FIXME: copy_type()?
            return 1;
        }
        current = current->next;
    }
    return 0;
}

void push_type_env_var(TypeEnv *env, char *name, Type *t) {
    if (!env) die(INTERPRETER_ERR, "environment is null in push_type_env_var(NULL, %s, %s)", name, to_string_type(t));

    TypeEnvFrame *frame = (TypeEnvFrame *) malloc_or_die(sizeof(TypeEnvFrame));
    frame->name = name;
    frame->t = copy_type(t);
    frame->next = env->ef;
    env->ef = frame;
}

void pop_type_env_var(TypeEnv *env) {
    if (!env) die(INTERPRETER_ERR, "environment is null in push_type_env_var(NULL)");
    TypeEnvFrame *frame = env->ef;
    env->ef = env->ef->next;
    free(frame);
}

char *to_string_type_env_frame(TypeEnvFrame *ef) {
    char *name = ef->name;
    char *t_str = to_string_type(ef->t);
    size_t buf_size = strlen(name) + strlen(t_str) + 4;
    char *str = (char *) malloc_or_die(buf_size * sizeof(char));
    snprintf(str, buf_size, "[%s:%s]", name, t_str);
    return str;
}

char *to_string_type_env(TypeEnv *env) {
    // TODO: fix this mess, this is just a temporary solution while I do more important things
    char *str = (char *) malloc_or_die(1000*sizeof(char));
    str[0] = '{';
    str[1] = '\0';
    TypeEnvFrame *current = env->ef;
    while (current) {
        strcat(str, to_string_type_env_frame(current));
        current = current->next;
    }
    size_t len = strlen(str);
    str[len] = '}';
    str[len+1] = '\0';
    if (!(str = realloc(str, len+2))) die(MEMORY_ERR, "realloc failed");
    return str;
}