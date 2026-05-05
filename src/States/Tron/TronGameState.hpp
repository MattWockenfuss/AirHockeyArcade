#pragma once

#include "../State.hpp"
#include <SFML/Graphics.hpp>
#include <chrono>
#include <optional>

#include "Tron.hpp"


class TronGameState : public State {
    public:
        bool gameEnd = false;
        float squareWidth = 12.0f;
        sf::Color darkgray = sf::Color(85, 85, 85);
        sf::Color gray = sf::Color(119, 119, 119);

        sf::Color p1body = sf::Color(99, 212, 113);
        sf::Color p1head = sf::Color(99, 164, 108);
        sf::Color p2body = sf::Color(158, 0, 49);
        sf::Color p2head = sf::Color(244, 96, 54);


        void init(Context* ctx) override;
        void tick() override;
        void p1render(sf::RenderWindow& p1window) override;
        void p2render(sf::RenderWindow& p2window) override;

        Tron p1;
        Tron p2;

    private:
        float viewWidth = 0;
        float viewHeight = 0;
        void renderArena(sf::RenderWindow& window);
        void renderGameEndWindow(sf::RenderWindow& window);
        long long sec;
        std::optional<sf::Text> title_text, play_text, seconds_counter, p1score, p2score;

        sf::RectangleShape square;
        float x, y;
        float w, h;
        const std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
        unsigned int totalMSThreshold = 10000;
        unsigned int windowMSThreshold = 3000;
        float windowWidth = 600.0f; 
        float windowHeight = 400.0f;
        float windowYCoord = -windowHeight;
        std::chrono::steady_clock::time_point gameEndStart, curr;
        int tickCount = 0;
        bool p1won = false; //This is false if p2won
        int raisedSquare = -1;
};