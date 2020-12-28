#ifndef PLUS_H
#define PLUS_H

#include "types.h"
#include "TypeError.h"
#include "Exp.h"
#include "Int.h"
#include <string>

class Plus : public Exp {
    Exp *e1, *e2;
    
public:
    Plus(Exp *e1, Exp *e2);
    virtual Exp *step();
    virtual Type *type();
    virtual bool is_value();
    virtual std::string to_string();
    virtual ~Plus();
};

#endif
