#include "expressions.h"

Int::Int(int value) {
    this->value = value;
}

int Int::get_value() {
    return value;
}

Type *Int::type() {
    return new IntType();
}

std::string Int::to_string() {
    return std::to_string(value);
}

Int::~Int() {}
