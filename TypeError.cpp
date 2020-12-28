#include "expressions.h"

TypeError::TypeError(int src, std::string msg) {
    this->msg = std::string(src == TypeError::STATIC ? "Static type error: " : "Unexpected dynamic type error: ") + msg;
}

const char *TypeError::what() {
    return msg.c_str();
}
