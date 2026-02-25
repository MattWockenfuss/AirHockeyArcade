#pragma once

#include "State.hpp"
#include <SFML/Graphics.hpp>


class MenuState : public State {
    public:
        MenuState(Context& ctx);

        void tick() override;
        void render(sf::RenderWindow& window) override;

        ~MenuState() override = default;
    private:
        sf::Text text, label;

        sf::RectangleShape square;
        float x, y;
        float w, h;
};