#ifndef VALUE_H
#define VALUE_H

#include "Exp.h"
#include "ReductionError.h"
#include <string>

template<typename T>
class Value : public Exp {
protected:
    T value;
    
public:
    Value(T value);
    T get_value();
    virtual Exp *step();
    virtual bool is_value();
    virtual ~Value();
};

template<typename T>
Value<T>::Value(T value) {
    this->value = value;
}

template<typename T>
T Value<T>::get_value() {
    return value;
}

template<typename T>
Exp *Value<T>::step() {
    throw ReductionError("Cannot reduce value");
}

template<typename T>
bool Value<T>::is_value() {
    return true;
}

template<typename T>
Value<T>::~Value() {};

#endif
