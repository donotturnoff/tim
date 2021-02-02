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

int main(void) {
    Exp *id = function(var("x", NULL), get_var("x", NULL), NULL);
    Exp *apply1 = apply(
            get_var("f", NULL),
            get_var("x", NULL),
            NULL
    );
    Exp *apply2 = apply(
            function(
                    var("f", function_t(integer_t(), integer_t())),
                    function(
                            var("x", integer_t()),
                            apply1,
                            NULL
                    ),
                    NULL),
            function(
                    var("x", integer_t()),
                    plus(
                            get_var("x", NULL),
                            integer(2),
                            NULL
                    ),
                    NULL
            ),
            NULL
    );
    Exp *prog = apply(function(var("x", NULL), apply2, NULL), integer(7), NULL); // breaks it
    prog = apply(apply(id, function(var("x", NULL), apply2, NULL), NULL), integer(7), NULL); // breaks it
    prog = apply(function(var("x", integer_t()), plus(integer(1), get_var("x", NULL), NULL), NULL), integer(1), NULL);
    //prog = plus(integer(1), integer(2), NULL);
    char *exp_str = to_string_exp(prog);
    printf("%s", exp_str);
    Type *t = type(prog);
    char *t_str = to_string_type(t);
    printf(" : %s\n", t_str);
    free(exp_str);

    while (prog && !prog->is_irreducible) {
        Exp *new_prog = step(prog);
        exp_str = to_string_exp(new_prog);
        printf("-> %s\n", exp_str);
        free(exp_str);
        prog = new_prog;
    }
    free_exp(prog, 1);
    return 0;
}