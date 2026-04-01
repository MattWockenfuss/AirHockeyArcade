#pragma once

#include "State.hpp"
#include <SFML/Graphics.hpp>
#include <optional>

class Context;


enum class States {
    Idle, NameEntry, AirHockey, Tron, FruitNinja
};

inline const char* stateToString(States s){
    switch (s){
        case States::Idle: return "Idle";
        case States::NameEntry: return "NameEntry";
        case States::AirHockey: return "AirHockey";
        case States::Tron: return "Tron";
        case States::FruitNinja: return "FruitNinja";
        default: return "Unknown";
    }
}

class GameStateManager {
    public:
        void init(Context* ctx);
        
        void requestStateChange(States s, double fadeOut, double fadeIn);
        void changeState();
        State* getCurrentState();
        bool pendingStateChange = false;

        void tick();
        void p1render(sf::RenderWindow& p1window);
        void p2render(sf::RenderWindow& p2window);

    private:
        State* currentState;
        States currentStateType;
        Context* ctx;

        const double fadeSpeed = 1.0 / 60.0;
        States pendingState;
        double fadeOut, fadeIn;
        double fadeOutMax, fadeInMax;

        std::optional<sf::Text> debugLabel;
        int alphaLeft = 0;
        sf::RectangleShape square;
    
        
};