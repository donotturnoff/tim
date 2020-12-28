#include "expressions.h"
#include <iostream>

Plus::Plus(Exp *e1, Exp *e2) {
    this->e1 = e1;
    this->e2 = e2;
}

Exp *Plus::step() {
    if (!e1->is_value()) {
        Exp *next = e1->step();
        delete e1;
        return new Plus(next, e2);
    } else if (!e2->is_value()) {
        Exp *next = e2->step();
        delete e2;
        return new Plus(e1, next);
    } else {
        Int *int1 = dynamic_cast<Int *>(e1);
        Int *int2 = dynamic_cast<Int *>(e2);
        if (!int1) {
            delete e1;
            delete e2;
            throw TypeError(TypeError::DYNAMIC, "First operand of + is not an int");
        } else if (!int2) {
            delete e1;
            delete e2;
            throw TypeError(TypeError::DYNAMIC, "Second operand of + is not an int");
        } else {
            int val1 = int1->get_value();
            int val2 = int2->get_value();
            delete e1;
            delete e2;
            return new Int(val1 + val2);
        }
    }
}

Type *Plus::type() {
    IntType *int_t = new IntType();
    Type *t1 = e1->type();
    Type *t2 = e2->type();
    if (*t1 != *int_t) {
        delete int_t;
        delete t1;
        delete t2;
        delete e1;
        delete e2;
        throw TypeError(TypeError::STATIC, "First operand of + is not an int");
    } else if (*t2 != *int_t) {
        delete int_t;
        delete t1;
        delete t2;
        delete e1;
        delete e2;
        throw TypeError(TypeError::STATIC, "Second operand of + is not an int");
    } else {
        delete t1;
        delete t2;
        return int_t;
    }
}

bool Plus::is_value() {
    return false;
}

std::string Plus::to_string() {
    return "(" + e1->to_string() + "+" + e2->to_string() + ")";
}

Plus::~Plus() {}
