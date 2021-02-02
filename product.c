#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "expressions.h"

Exp *product(Exp *exp1, Exp *exp2, Env *env) {
    Exp *product = (Exp *) malloc(sizeof(Exp));
    if (!product) {
        printf("Error: failed to allocate memory for Exp in product\n");
        return NULL;
    }

    product->name = PRODUCT;
    product->is_irreducible = 0;

    product->e = (ExpChoice *) malloc(sizeof(ExpChoice));
    if (!product->e) {
        printf("Error: failed to allocate memory for ExpChoice in product\n");
        free(product);
        return NULL;
    }

    product->env = (Env *) malloc(sizeof(Env));
    if (!product->env) {
        printf("Error: failed to allocate memory for Env in product\n");
        free(product->e);
        free(product);
        return NULL;
    }
    add_env(product->env, env);

    product->e->product->exp1 = exp1;
    product->e->product->exp2 = exp2;

    return product;
}

Type *type_product(Exp *exp) {
    Env *env = exp->env;
    Product *product = exp->e->product;
    Exp *exp1 = product->exp1;
    Exp *exp2 = product->exp2;

    add_env(exp1->env, env);
    add_env(exp2->env, env);
    Type *exp1_t = type(exp1);
    if (!exp1_t) return NULL;
    Type *exp2_t = type(exp2);
    if (!exp2_t) {
        free_type(exp1_t);
        return NULL;
    }

    Type *t = (Type *) malloc(sizeof(Type));
    if (!t) {
        printf("Error: failed to allocate memory for Type in type_product\n");
    } else {
        t->name = PRODUCT_T;
        t->t = (TypeChoice *) malloc(sizeof(TypeChoice));
        if (!t->t) {
            printf("Error: failed to allocate memory for TypeChoice in type_product\n");
            free(t);
            t = NULL;
        } else {
            t->t->p = (ProductType *) malloc(sizeof(ProductType));
            if (!t->t->p) {
                printf("Error: failed to allocate memory for ProductType in type_product\n");
                free(t->t);
                free(t);
                t = NULL;
            } else {
                t->t->p->t1 = exp1_t;
                t->t->p->t2 = exp2_t;
            }
        }
    }
    return t;
}

char *to_string_product(Exp *exp) {
    Product *p = exp->e->product;
    char *exp1_str = to_string_exp(p->exp1);
    char *exp2_str = to_string_exp(p->exp2);
    size_t len1 = strlen(exp1_str);
    size_t len2 = strlen(exp2_str);
    size_t len = len1 + len2 + 5;
    char *buf = (char *) malloc(len * sizeof(char));
    if (!buf) {
        printf("Error: failed to allocate memory in to_string_product\n");
        return NULL;
    }
    snprintf(buf, len, "(%s, %s)", exp1_str, exp2_str);

    free(exp1_str);
    free(exp2_str);

    return buf;
}

int free_product(Exp *exp, int descend) {
    if (exp->name == PRODUCT) {
        if (descend && !(free_exp(exp->e->product->exp1, 1) && free_exp(exp->e->product->exp2, 1))) return 0;
        free(exp->e->product);
        free(exp->e);
        free_env(exp->env);
        free(exp);
        return 1;
    } else {
        printf("Warning: attempted to call free_product on non-product\n");
        return 0;
    }
}