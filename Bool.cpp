#include "expressions.h"

Bool::Bool(bool value) {
    this->value = value;
}

bool Bool::get_value() {
    return value;
}

Type *Bool::type() {
    return new BoolType();
}

std::string Bool::to_string() {
    return value ? "true" : "false";
}

Bool::~Bool() {}
