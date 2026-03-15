#include "State.hpp"
#include <iostream>

void State::init(Context* ctx){
    this -> ctx = ctx;
    std::cout << "New State Created!" << std::endl;
}