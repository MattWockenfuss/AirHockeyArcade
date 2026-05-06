#pragma once

#include "State.hpp"
#include <SFML/Graphics.hpp>
#include <optional>
#include <string>
#include <unordered_map>



class Context;




enum class States {
    Idle, NameEntry, GameSelect, AirHockey, Tron, FruitNinja, Pong, Leaderboard
};

/*
    A Hash map of our games that anyone can access
    int p1 = GAMES["AirHockey"]; returns 0

*/
inline const std::unordered_map<int, std::string> GAMES = {
    {0, "Air Hockey"},
    {1, "Pong"},
    {2, "Fruit Ninja"},
    {3, "Tron"}
};


inline const char* stateToString(States s){
    switch (s){
        case States::Idle: return "Idle";
        case States::NameEntry: return "NameEntry";
		case States::GameSelect: return "GameSelect";
        case States::AirHockey: return "AirHockey";
        case States::Tron: return "Tron";
        case States::Leaderboard: return "Leaderboard";
        case States::FruitNinja: return "FruitNinja";
        case States::Pong: return "Pong";
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
        void p1render(sf::RenderTexture& p1window);
        void p2render(sf::RenderTexture& p2window);

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