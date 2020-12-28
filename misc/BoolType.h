#ifndef BOOL_TYPE_H
#define BOOL_TYPE_H

#include "Type.h"
#include <string>

class BoolType : public Type {
public:
    virtual std::string get_name();
};

#endif
