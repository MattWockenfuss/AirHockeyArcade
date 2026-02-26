#pragma once

#include "State.hpp"
#include <SFML/Graphics.hpp>
#include <chrono>


class MenuState : public State {
    public:
        MenuState(Context& ctx);

        void tick() override;
        void render(sf::RenderWindow& window) override;

        ~MenuState() override = default;
    private:
        sf::Text title_text, play_text, seconds_counter;

        sf::RectangleShape square;
        float x, y;
        float w, h;
        const std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
};