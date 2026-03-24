#pragma once

#include "State.hpp"

class Context;

enum class States {
    Menu, AirHockey, Snake
};

class GameStateManager {
    public:
        void init(Context* ctx);
        
        void requestStateChange(States s);
        void changeState();
        State* getCurrentState();
        bool pendingStateChange = false;
    private:
        State* currentState;
        Context* ctx;

    
        States pendingState;
};