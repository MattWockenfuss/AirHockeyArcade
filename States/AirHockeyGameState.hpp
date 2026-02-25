#pragma once

#include "State.hpp"
#include <SFML/Graphics.hpp>


class AirHockeyGameState : public State {
    public:
        AirHockeyGameState(Context& ctx);

        void tick() override;
        void render(sf::RenderWindow& window) override;

        ~AirHockeyGameState() override = default;
    private:
        sf::Text text, label;

        sf::RectangleShape square;
        float x, y;
        float w, h;
};