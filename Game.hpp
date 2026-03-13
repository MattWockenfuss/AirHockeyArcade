#pragma once

#include <SFML/Graphics.hpp>
#include "Context.hpp"
#include "States/GameStateManager.hpp"
#include "AssetManager.hpp"
#include "KeyManager.hpp"
#include "GPIOManager.hpp"

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
        
        Context ctx;

        sf::RenderWindow window;
        AssetManager assetManager;
        KeyManager keyManager;
        GPIOManager GPIOmanager;
        GameStateManager gsm;
};