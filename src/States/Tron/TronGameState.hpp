#pragma once

#include "../State.hpp"
#include <SFML/Graphics.hpp>
#include <chrono>
#include <optional>

#include "Tron.hpp"


class TronGameState : public State {
    public:
        void init(Context* ctx) override;

        void tick() override;
        void p1render(sf::RenderWindow& p1window) override;
        void p2render(sf::RenderWindow& p2window) override;

        Tron p1;
        Tron p2;

    private:
        long long sec;
        std::optional<sf::Text> title_text, play_text, seconds_counter, p1score, p2score;

        sf::RectangleShape square;
        float x, y;
        float w, h;
        const std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
        int tickCount = 0;
};