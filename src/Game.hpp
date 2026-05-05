#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Context.hpp"
#include "States/GameStateManager.hpp"
#include "AssetManager.hpp"
#include "KeyManager.hpp"
#include "IO/InputManager.hpp"
#include "AudioManager.hpp"
#include "States/Leaderboard/LeaderboardInterface.hpp"

class Game {
    public:
        Game();
        void initialization();
        void start();
        void stop();
        void run();
        void tick();
        void render();
    private:
        bool renderFPSCounter = true;
        std::optional<sf::Text> tpsCounter;
        std::optional<sf::View> p1View, p2View;
        
        int ticks = 0;
        int frames = 0;

        int ticksLastSecond = 0;
        int framesLastSecond = 0;

        bool running = false;
        bool shouldRender = false;
        bool renderPlayer2 = false;
        
        Context ctx;

        
        sf::RenderWindow window;

        AssetManager assetManager;
        KeyManager keyManager;
        InputManager input;
        GameStateManager gsm;
        AudioManager audio;
        LeaderboardInterface leaderboardInterface;
};