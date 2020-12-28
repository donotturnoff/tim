#ifndef TUPLE_TYPE_H
#define TUPLE_TYPE_H

#include "Type.h"
#include <string>
#include <array>

template<int n>
class TupleType : public Type {
    std::array<Type *, n> types;

public:
    TupleType(std::array<Type *, n> types);
    virtual std::string get_name();
    std::array<Type *, n> get_types();
    virtual bool operator==(Type &t);
};

template<int n>
TupleType<n>::TupleType(std::array<Type *, n> types) {
    this->types = types;
}

template<int n>
std::string TupleType<n>::get_name() {
    return std::to_string(n) + "-tuple";
}

template<int n>
std::array<Type *, n> TupleType<n>::get_types() {
    return this->types;
}

template<int n>
bool TupleType<n>::operator==(Type &t) {
    if (get_name().compare(t.get_name()) != 0) {
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
