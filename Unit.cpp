#include "expressions.h"

Unit::Unit(void) {};

Type *Unit::type() {
    return new UnitType();
}

std::string Unit::to_string() {
    return "()";
}

Unit::~Unit() {}
