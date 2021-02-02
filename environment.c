#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "environment.h"

int put_env_var(Env *env, char *name, Type *t, Exp *val) {
    if (!env) die(INTERPRETER_ERR, "environment is null in put_env_var(NULL, %s, %s)", name, to_string_exp(val));

    EnvFrame *frame = (EnvFrame *) malloc_or_die(sizeof(EnvFrame));

    frame->name = name;
    frame->t = t ? copy_type(t) : NULL;
    frame->val = val ? copy_exp(val) : NULL;
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
    return NULL;
}

Type *get_env_var_type(Env *env, char *name) {
    if (!env) die(INTERPRETER_ERR, "environment is null in get_env_var_type(NULL, %s)", name);
    EnvFrame *current = env->ef;
    while (current) {
        if (strcmp(current->name, name) == 0) {
            return copy_type(current->t); // FIXME: copy_type()?
        }
        current = current->next;
    }
    return NULL;
}

int add_env(Env *dst_env, Env *src_env) {
    if (!dst_env) die(INTERPRETER_ERR, "destination environment is null in add_env(NULL, %s)\n", to_string_env(src_env));

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
    Env *new_env = (Env *) malloc_or_die(sizeof(Env));
    new_env->ef = NULL;
    new_env->back_ef = NULL;
    add_env(new_env, env);
    return new_env;
}

char *to_string_env_frame(EnvFrame *ef) {
    char *name = ef->name;
    char *exp_str = to_string_exp(ef->val);
    size_t buf_size = strlen(name) + (exp_str ? strlen(exp_str) : 1) + 4;
    char *str = (char *) malloc_or_die(buf_size * sizeof(char));
    snprintf(str, buf_size, (exp_str ? "[%s=%s]" : "[%s=_]"), name, exp_str);
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
    if (!(str = realloc(str, len+1))) die(MEMORY_ERR, "realloc failed");
    return str;
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
    if (!env) die(RUNTIME_ERR, "environment is null in print_env\n");

    printf("{");
    EnvFrame *current = env->ef;
    while(current) {
        print_env_frame(current);
        current = current->next;
    }
    printf("}");
}