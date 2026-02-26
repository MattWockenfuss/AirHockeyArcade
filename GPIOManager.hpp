#pragma once

#include <SFML/Graphics.hpp>

namespace sf{
    class RenderWindow;
}

class GPIOManager {
    public:
        GPIOManager();
        void tick();
        void render(sf::RenderWindow& window);
        bool overlay = false;
    private:
        sf::RectangleShape square_background;
};


