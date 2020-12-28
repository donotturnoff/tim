#ifndef UNIT_TYPE_H
#define UNIT_TYPE_H

#include "Type.h"
#include <string>

class UnitType : public Type {
public:
    virtual std::string get_name();
    virtual std::string to_string();
};

#endif
