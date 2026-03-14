#pragma once

#include "InputInterface.hpp"
#include <SFML/Graphics.hpp>
#include <optional>

class KeyboardInput : public  InputInterface {
    public:
        void init(Context* ctx, InputManager* input) override;
        void tick() override;
        void render(sf::RenderWindow& window) override;
    private:
        bool triggeredFlag = false;
        std::optional<sf::Text> textbox;
        sf::RectangleShape rect;
};