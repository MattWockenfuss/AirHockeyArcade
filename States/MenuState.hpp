#pragma once

#include "State.hpp"
#include <SFML/Graphics.hpp>
#include <chrono>
#include <optional>


class MenuState : public State {
    public:
        void init(Context* ctx) override;

        void tick() override;
        void render(sf::RenderWindow& window) override;
        
        MenuState();
        ~MenuState() override = default;
    private:
        long long sec;
        std::optional<sf::Text> title_text, play_text, seconds_counter;

        sf::RectangleShape square;
        float x, y;
        float w, h;
        const std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
};