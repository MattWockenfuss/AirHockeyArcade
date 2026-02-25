#pragma once

#include <SFML/Graphics.hpp>
#include "Context.hpp"
#include "States\GameStateManager.hpp"
#include "AssetManager.hpp"

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
        bool running = false;
        bool shouldRender = false;
        
        sf::RenderWindow window;
        AssetManager assetManager;

        Context ctx;
        GameStateManager gsm;   
};