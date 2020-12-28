#include "expressions.h"

Not::Not(Exp *e1) {
    this->e1 = e1;
}

Exp *Not::step() {
    if (!e1->is_value()) {
        Exp *next = e1->step();
        delete e1;
        return new Not(next);
    } else {
        Bool *bool1 = dynamic_cast<Bool *>(e1);
        if (!bool1) {
            delete e1;
            throw TypeError(TypeError::DYNAMIC, "Operand of ! is not a bool");
        } else {
            bool val1 = bool1->get_value();
            delete e1;
            return new Bool(!val1);
        }
    }
}

Type *Not::type() {
    BoolType *bool_t = new BoolType();
    Type *t1 = e1->type();
    if (*t1 != *bool_t) {
        delete bool_t;
        delete t1;
        delete e1;
        throw TypeError(TypeError::STATIC, "Operand of ! is not a bool");
    } else {
        delete t1;
        return bool_t;
    }
}

bool Not::is_value() {
    return false;
}

std::string Not::to_string() {
    return "(!" + e1->to_string() + ")";
}

Not::~Not() {}
