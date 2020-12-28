#ifndef TYPES_H
#define TYPES_H

#include <string>
#include <array>

class Type {
  public:    
    virtual std::string get_name() = 0;
    
    virtual std::string to_string() = 0;
    
    virtual bool operator==(Type &t) {
        return get_name().compare(t.get_name()) == 0;
    }
    
    virtual bool operator!=(Type &t) {
        return !(*this == t);
    }
    
    virtual ~Type() {}
};

class BoolType : public Type {
  public:
    virtual std::string get_name();
    virtual std::string to_string();
};

class IntType : public Type {
  public:
    virtual std::string get_name();
    virtual std::string to_string();
};

class UnitType : public Type {
  public:
    virtual std::string get_name();
    virtual std::string to_string();
};

class VoidType : public Type {
  public:
    virtual std::string get_name();
    virtual std::string to_string();
};

template<int n>
class TupleType : public Type {
    std::array<Type *, n> types;
  public:
    TupleType(std::array<Type *, n> types);
    virtual std::string get_name();
    virtual std::string to_string();
    std::array<Type *, n> get_types();
    virtual bool operator==(Type &t);
};

class FnType : public Type {
    Type *arg;
    Type *ret;
  public:
    FnType(Type *arg, Type *ret);
    virtual std::string get_name();
    virtual std::string to_string();
    Type *get_arg_type();
    Type *get_ret_type();
    virtual bool operator==(Type &t);
}

#include "TupleType.cpp"

#endif
