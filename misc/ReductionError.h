#ifndef REDUCTION_ERROR_H
#define REDUCTION_ERROR_H

#include <exception>
#include <string>

class ReductionError : public std::exception {
    std::string msg;

public:
    ReductionError(std::string msg);
    virtual const char *what();
};

#endif
