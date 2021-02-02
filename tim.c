#include "types.h"
#include "expressions.h"
#include "environment.h"
#include <stdio.h>
#include <stdlib.h>

int main(void) {
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
    Exp *prog = apply(
            apply2,
            integer(1),
            NULL
    );
    //Exp *prog = apply(function(var("x", integer_t()), plus(integer(1), get_var("x", NULL), NULL), NULL), integer(1), NULL);
    prog = apply1;
    Type *t = type(prog);
    char *exp_str = to_string_exp(prog);
    if (t) {
        char *t_str = to_string_type(t);
        printf("%s : %s\n", exp_str, t_str);
        free(exp_str);
        free_type(t);
    } else {
        printf("%s : ?\n", exp_str);
        free(exp_str);
        free_type(t);
        free_exp(prog, 1);
        return 1;
    }
    while (prog && !prog->is_irreducible) {
        Exp *new_prog = step(prog);
        exp_str = to_string_exp(new_prog);
        if (new_prog) printf("-> %s\n", exp_str);
        free(exp_str);
        prog = new_prog;
    }
    free_exp(prog, 1);
    return 0;
}