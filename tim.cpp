#include "types.h"
#include "expressions.h"
#include <array>
#include <iostream>

int main(void) {
    std::cout << "Starting" << std::endl;
    Exp *exp = new Not(new Bool(false));
    
    Type *type;
    try {
        std::cout << exp->to_string() << " : ";
        type = exp->type();
        std::cout << type->to_string() << std::endl;  
        delete type;   
        while (!exp->is_value()) {
            Exp *new_exp = exp->step();
            delete exp;
            exp = new_exp;
            std::cout << exp->to_string() << std::endl;
        }
    } catch (TypeError &e) {
        std::cout << "?" << std::endl << e.what() << std::endl;
    } catch (ReductionError &e) {
        std::cout << "?" << std::endl << e.what() << std::endl;
    }
    delete exp;
    
    return 0;
}
