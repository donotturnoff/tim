#ifndef INT_TYPE_H
#define INT_TYPE_H

#include "Type.h"
#include <string>

class IntType : public Type {
public:
    virtual std::string get_name();
};

#endif
