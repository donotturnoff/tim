#include "expressions.h"

ReductionError::ReductionError(std::string msg) {
    this->msg = msg;
}

const char *ReductionError::what() {
    return this->msg.c_str();
}
