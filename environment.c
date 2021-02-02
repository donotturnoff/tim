#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "environment.h"

int put_env_var(Env *env, char *name, Type *t, Exp *val) {
    if (!env) {
        printf("Error: environment is null in put_env_var(NULL, %s, %s)\n", name, to_string_exp(val));
        return 0;
    }
    EnvFrame *frame = (EnvFrame *) malloc(sizeof(EnvFrame));
    if (!frame) {
        printf("Error: failed to allocate memory for EnvFrame in put_env_var(NULL, %s, %s)\n", name, to_string_exp(val));
        return 0;
    }
    frame->name = name;

    if (t) frame->t = copy_type(t);
    else frame->t = NULL;

    if (val) frame->val = copy_exp(val);
    else frame->val = NULL;

    frame->next = env->ef;

    if (frame->next) frame->next->prev = frame;

    frame->prev = NULL;
    env->ef = frame;

    if (!env->back_ef) env->back_ef = frame;

    return 1;
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
    printf( "Runtime error: variable %s not in scope\n", name);
    return NULL;
}

Type *get_env_var_type(Env *env, char *name) {
    if (!env) {
        printf("Error: environment is null in get_env_var_type(NULL, %s)\n", name);
        return NULL;
    }
    EnvFrame *current = env->ef;
    while (current != NULL) {
        if (strcmp(current->name, name) == 0) {
            return copy_type(current->t); // FIXME: copy_type()?
        }
        current = current->next;
    }
    printf( "Type error: variable %s not in scope\n", name);
    return NULL;
}

int add_env(Env *dst_env, Env *src_env) {
    if (!dst_env) {
        printf("Error: destination environment is null in add_env(NULL, %s)\n", to_string_env(src_env));
        return 0;
    }

    if (!src_env) {
        return 1;
    }

    EnvFrame *current = src_env->back_ef;
    while (current) {
        char *name = current->name;
        Type *t = current->t;
        Exp *val = current->val;
        if (!put_env_var(dst_env, name, t, val)) {
            return 0;
        }
        current = current->prev;
    }
    return 1;
}

Env *copy_env(Env *env) {
    Env *new_env = (Env *) malloc(sizeof(Env));
    if (!new_env) {
        printf("Error: failed to allocate memory for Env in copy_env\n");
        return NULL;
    }
    new_env->ef = NULL;
    new_env->back_ef = NULL;
    add_env(new_env, env);
    return new_env;
}

const char *to_string_env(Env *env) {
    return "env"; // TODO: implement to_string_env
}

void free_env(Env *env) {
    EnvFrame *current = env->ef;
    while (current) {
        free_type(current->t);
        free_exp(current->val, 1);
        EnvFrame *next = current->next;
        free(current);
        current = next;
    }
    free(env);
}

void print_env_frame(EnvFrame *ef) {
    const char *t_str = ef->t ? to_string_type(ef->t) : "_";
    const char *val_str = ef->val ? to_string_exp(ef->val) : "_";

    printf("[%s:%s=%s]", ef->name, t_str, val_str);
}

void print_env(Env *env) {
    if (!env) {
        printf("Error: environment is null in print_env\n");
        return;
    }

    printf("{");
    EnvFrame *current = env->ef;
    while(current) {
        print_env_frame(current);
        current = current->next;
    }
    printf("}");
}