#pragma once

#include "State.hpp"
#include <SFML/Graphics.hpp>
#include <optional>


class AirHockeyGameState : public State {
    public:
        void init(Context* ctx) override;

        void tick() override;
        void render(sf::RenderWindow& window) override;

        AirHockeyGameState();
        ~AirHockeyGameState() override = default;
    private:
        std::optional<sf::Text> text, label, text2;

        sf::RectangleShape square;
        float x, y;
        float w, h;
};