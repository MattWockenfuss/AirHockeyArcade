#pragma once

#include "State.hpp"
#include "../Context.hpp"

enum class States {
    Menu, AirHockey
};

class GameStateManager {
    public:
        GameStateManager(Context& ctx);
        
        void changeState(States s);
        State* getCurrentState();
    private:
        State* currentState;
        Context& ctx;
};