#include "expressions.h"

Exp *Value::step() {
    throw ReductionError("Cannot reduce value");
}

bool Value::is_value() {
    return true;
}

Value::~Value() {};
