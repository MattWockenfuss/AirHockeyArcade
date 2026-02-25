#include "State.hpp"
#include <iostream>

State::State(Context& ctx)
: ctx(ctx)
{
    std::cout << "New State Created!" << std::endl;
}