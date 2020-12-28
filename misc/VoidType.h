#ifndef VOID_TYPE_H
#define VOID_TYPE_H

#include "Type.h"
#include <string>

class VoidType : public Type {
public:
    virtual std::string get_name();
    virtual std::string to_string();
};

#endif
