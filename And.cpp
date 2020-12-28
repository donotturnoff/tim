#include "expressions.h"

And::And(Exp *e1, Exp *e2) {
    this->e1 = e1;
    this->e2 = e2;
}

Exp *And::step() {
    if (!e1->is_value()) {
        Exp *next = e1->step();
        delete e1;
        return new And(next, e2);
    } else if (!e2->is_value()) {
        Exp *next = e2->step();
        delete e2;
        return new And(e1, next);
    } else {
        Bool *bool1 = dynamic_cast<Bool *>(e1);
        Bool *bool2 = dynamic_cast<Bool *>(e2);
        if (!bool1) {
            delete e1;
            delete e2;
            throw TypeError(TypeError::DYNAMIC, "First operand of && is not a bool");
        } else if (!bool2) {
            delete e1;
            delete e2;
            throw TypeError(TypeError::DYNAMIC, "Second operand of && is not a bool");
        } else {
            bool val1 = bool1->get_value();
            bool val2 = bool2->get_value();
            delete e1;
            delete e2;
            return new Bool(val1 && val2);
        }
    }
}

Type *And::type() {
    BoolType *bool_t = new BoolType();
    Type *t1 = e1->type();
    Type *t2 = e2->type();
    if (*t1 != *bool_t) {
        delete bool_t;
        delete t1;
        delete t2;
        delete e1;
        delete e2;
        throw TypeError(TypeError::STATIC, "First operand of && is not a bool");
    } else if (*t2 != *bool_t) {
        delete bool_t;
        delete t1;
        delete t2;
        delete e1;
        delete e2;
        throw TypeError(TypeError::STATIC, "Second operand && not a bool");
    } else {
        delete t1;
        delete t2;
        return bool_t;
    }
}

bool And::is_value() {
    return false;
}

std::string And::to_string() {
    return "(" + e1->to_string() + "&&" + e2->to_string() + ")";
}

And::~And() {}
