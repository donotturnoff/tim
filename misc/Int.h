#ifndef INT_H
#define INT_H

#include "types.h"
#include "Exp.h"
#include "Value.h"
#include <string>

class Int : public Value<int> {
public:
    Int(int value);
    virtual Type *type();
    virtual std::string to_string();
    virtual ~Int();
};

#endif
