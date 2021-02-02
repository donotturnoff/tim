cmake_minimum_required(VERSION 3.17)
project(tim)

set(CMAKE_C_STANDARD 99)
set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -fsanitize=address")

include_directories(..)

add_executable(tim
        and.c
        apply.c
        boolean.c
        environment.c
        environment.h
        expressions.c
        expressions.h
        function.c
        get_var.c
        integer.c
        minus.c
        not.c
        or.c
        plus.c
        product.c
        sum.c
        tim.c
        types.c
        types.h
        unit.c
        var.c
        )
