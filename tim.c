#include "tim.h"
#include "types.h"
#include "expressions.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

void die(ErrorType e, const char *msg, ...) {
    va_list args;
    va_start(args, msg);

    va_end(args);
    switch (e) {
        case TYPE_ERR: fprintf(stderr, "Type error: "); break;
        case SCOPE_ERR: fprintf(stderr, "Scope error: "); break;
        case RUNTIME_ERR: fprintf(stderr, "Runtime error: "); break;
        case MEMORY_ERR: fprintf(stderr, "Memory error: "); break;
        default: fprintf(stderr, "Interpreter error: "); break;
    }
    vfprintf(stderr, msg, args);
    fprintf(stderr, "\n");
    exit(e);
}

void *malloc_or_die(size_t size) {
    void *ptr = malloc(size);
    if (!ptr) die(MEMORY_ERR, "malloc failed");
    return ptr;
}

void exec_prog(Exp *prog) {
    char *exp_str = to_string_exp(prog);
    printf("%s ", exp_str);
    fflush(stdout);
    free(exp_str);

    TypeEnv *te = (TypeEnv *) malloc_or_die(sizeof(TypeEnv));
    te->ef = NULL;
    Type *t = type(te, prog, generic_t());
    char *t_str = to_string_type(t);
    printf(": %s\n", t_str);

    while (prog && !prog->is_irreducible) {
        Exp *new_prog = step(prog);
        prog = new_prog;
    }
    exp_str = to_string_exp(prog);
    printf("-> %s\n", exp_str);
    free(exp_str);
    //free_exp(prog, 1);
}

int main(void) {
    Exp *id = function(var("x", NULL), get_var("x", NULL), NULL);
    Exp *apply1 = apply(
            get_var("f", NULL),
            get_var("z", NULL),
            NULL
    );
    Exp *apply2 = apply(
            function(
                    var("f", function_t(integer_t(), integer_t())),
                    function(
                            var("z", integer_t()),
                            apply1,
                            NULL
                    ),
                    NULL),
            function(
                    var("w", integer_t()),
                    plus(
                            get_var("w", NULL),
                            integer(2),
                            NULL
                    ),
                    NULL
            ),
            NULL
    );
    Exp *prog1 = apply(function(var("x", NULL), apply2, NULL), integer(7), NULL);
    Exp *prog2 = function(var("x", NULL), apply2, NULL);
    Exp *prog3 = apply(apply(id, function(var("y", NULL), apply2, NULL), NULL), integer(7), NULL); // breaks it
    Exp *prog4 = apply2;
    Exp *prog5 = apply(function(var("x", integer_t()), plus(integer(1), get_var("x", NULL), NULL), NULL), integer(1), NULL);
    Exp *prog6 = function(var("x", NULL), function(var("y", NULL), get_var("y", NULL), NULL), NULL);
    Exp *prog7 = function(var("x", NULL), plus(integer(2), get_var("x", NULL), NULL), NULL);
    Exp *prog8 = apply(id, function(var("y", NULL), apply2, NULL), NULL);
    Exp *prog9 = function(var("y", NULL), apply2, NULL);
    Exp *prog10 = apply(function(var("y", NULL), apply2, NULL), integer(7), NULL);
    Exp *prog11 = apply(id, integer(1), NULL);
    exec_prog(prog1);
    exec_prog(prog2);
    exec_prog(prog3);
    exec_prog(prog4);
    exec_prog(prog5);
    exec_prog(prog6);
    exec_prog(prog7);
    exec_prog(prog8);
    exec_prog(prog9);
    exec_prog(prog10);
    exec_prog(prog11);
    return 0;
}