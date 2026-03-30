#pragma once

#include "State.hpp"

class Context;

enum class States {
    Menu, NameEntry, AirHockey, Tron, FruitNinja
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