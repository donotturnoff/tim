#include "types.h"

FnType::FnType(Type *arg, Type *ret) {
    this->arg = arg;
    this->ret = ret;
}

std::string FnType::get_name() {
    return "fn";
}

std::string FnType::to_string() {
    return "(" + arg.to_string() + " -> " + ret.to_string() + ")";
}

Type *get_arg_type() {
    return arg;
}

Type *get_ret_type() {
    return ret;
}

bool FnType::operator==(Type &t) {
    if (get_name() != t.get_name()) {
        return false;
    } else {
        FnType *fn_type = dynamic_cast<FnType *>(&t);
        return fn_type && (arg == fn_type.get_arg_type()) && (ret == fn_type.get_ret_type());
    }
}
