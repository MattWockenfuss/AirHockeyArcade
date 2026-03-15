#pragma once

#include "State.hpp"

class Context;

enum class States {
    Menu, AirHockey
};

class GameStateManager {
    public:
        void init(Context* ctx);
        
        void changeState(States s);
        State* getCurrentState();
    private:
        State* currentState;
        Context* ctx;
};