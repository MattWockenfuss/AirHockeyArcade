#pragma once

#include "../State.hpp"
#include <SFML/Graphics.hpp>
#include <chrono>
#include <optional>

#include "Snake.hpp"


class SnakeGameState : public State {
    public:
        void init(Context* ctx) override;

        void tick() override;
        void render(sf::RenderWindow& window) override;
        
        SnakeGameState();
        ~SnakeGameState() override = default;

        Snake p1;
        Snake p2;

    private:
        long long sec;
        std::optional<sf::Text> title_text, play_text, seconds_counter;

        sf::RectangleShape square;
        float x, y;
        float w, h;
        const std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
        int tickCount = 0;
};