#ifndef TUPLE_TYPE_CPP
#define TUPLE_TYPE_CPP

#include "types.h"

template<int n>
TupleType<n>::TupleType(std::array<Type *, n> types) {
    this->types = types;
}

template<int n>
std::string TupleType<n>::get_name() {
    return std::to_string(n) + "-tuple";
}

template<int n>
std::string TupleType<n>::to_string() {
    return std::to_string(n) + "-tuple"; // TODO: use proper (a,b,c) notation
}

template<int n>
std::array<Type *, n> TupleType<n>::get_types() {
    return this->types;
}

template<int n>
bool TupleType<n>::operator==(Type &t) {
    if (get_name() != t.get_name()) {
        return false;
    } else {
        TupleType<n> *tuple_type = dynamic_cast<TupleType<n> *>(&t);
        if (tuple_type) {
            std::array<Type *, n> t_types = tuple_type->get_types();
            for (int i = 0; i < n; i++) {
                if (*types[i] != *t_types[i]) {
                    return false;
                }
            }
            return true;
        } else {
            return false;
        }
    }
}

#endif
