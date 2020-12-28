#ifndef EXPRESSIONS_H
#define EXPRESSIONS_H

#include "types.h"
#include <string>
#include <exception>
#include <iostream>

class Exp {
  public:
    virtual Exp *step(void) = 0;
    virtual Type *type(void) = 0;  
    virtual bool is_value(void) = 0;
    virtual std::string to_string(void) = 0;
    virtual ~Exp(void);
};

class Value : public Exp {
  public:
    virtual Exp *step(void);
    virtual bool is_value(void);
    virtual ~Value(void);
};

class Unit : public Value {
  public:
    Unit(void);
    virtual Type *type(void);  
    virtual std::string to_string(void);
    virtual ~Unit(void);
};

class Bool : public Value {
    bool value;
  public:
    Bool(bool value);
    bool get_value(void);
    virtual Type *type(void);  
    virtual std::string to_string(void);
    virtual ~Bool(void);
};

class Int : public Value {
    int value;
  public:
    Int(int value);
    int get_value(void);
    virtual Type *type(void);  
    virtual std::string to_string(void);
    virtual ~Int(void);
};

class Plus : public Exp {
    Exp *e1, *e2;
  public:
    Plus(Exp *e1, Exp *e2);
    virtual Exp *step(void);
    virtual Type *type(void);  
    virtual bool is_value(void);
    virtual std::string to_string(void);
    virtual ~Plus(void);
};

class Minus : public Exp {
    Exp *e1, *e2;
  public:
    Minus(Exp *e1, Exp *e2);
    virtual Exp *step(void);
    virtual Type *type(void);  
    virtual bool is_value(void);
    virtual std::string to_string(void);
    virtual ~Minus(void);
};

class And : public Exp {
    Exp *e1, *e2;
  public:
    And(Exp *e1, Exp *e2);
    virtual Exp *step(void);
    virtual Type *type(void);  
    virtual bool is_value(void);
    virtual std::string to_string(void);
    virtual ~And(void);
};

class Or : public Exp {
    Exp *e1, *e2;
  public:
    Or(Exp *e1, Exp *e2);
    virtual Exp *step(void);
    virtual Type *type(void);  
    virtual bool is_value(void);
    virtual std::string to_string(void);
    virtual ~Or(void);
};

class Not : public Exp {
    Exp *e1;
  public:
    Not(Exp *e1);
    virtual Exp *step(void);
    virtual Type *type(void);  
    virtual bool is_value(void);
    virtual std::string to_string(void);
    virtual ~Not(void);
};

class TypeError : public std::exception {
    int src;
    std::string msg;
  public:
    static const int STATIC = 0, DYNAMIC = 1;
    TypeError(int src, std::string msg);
    virtual const char *what(void);
};

class ReductionError : public std::exception {
    std::string msg;
  public:
    ReductionError(std::string msg);
    virtual const char *what(void);
};

#endif
