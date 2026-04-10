#pragma once

#include "State.hpp"
#include <SFML/Graphics.hpp>
#include <chrono>
#include <optional>


class IdleState : public State {
    public:
        void init(Context* ctx) override;

        void tick() override;
        void p1render(sf::RenderWindow& p1window) override;
        void p2render(sf::RenderWindow& p2window) override;
    private:
        long long sec;
        std::optional<sf::Text> title_text, play_text;

        sf::RectangleShape square;
        float x, y;
        float w, h;
		std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
};