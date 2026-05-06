#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <optional>
#include <cmath>
#include "../State.hpp"
#include <chrono>

struct PongPaddle {
    float x, y;
    sf::RectangleShape rect;
};

struct Ball {
    float x, y, rad;
    float velx, vely;
    sf::CircleShape circ;
};

class PongGameState : public State {
    public:
        void init(Context* ctx) override;

        void tick() override;
        void p1render(sf::RenderTexture& window) override;
        void p2render(sf::RenderTexture& window) override;
        
        PongGameState();
        ~PongGameState() override = default;
    private:
        float padding = 40.0f;
        std::optional<sf::Text> title_text, player1, player2, goal_text, seconds_counter, countdown_timer, win1, win2;
        std::optional<sf::Texture> bgTexture;
        std::optional<sf::Sprite> background;
        const std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
        int tickCount = 0;
        PongPaddle *p1 = new PongPaddle(); 
        PongPaddle *p2 = new PongPaddle();
        Ball *b = new Ball();
        sf::Vector2f winSize;
        sf::RenderWindow *window1;
        unsigned short int sub_steps = 128, sec;
        int score1, score2;
        bool rendergoal, initial, matchphase, gameover;
        std::chrono::steady_clock::time_point countdown;
        std::chrono::seconds time_remaining;

        void collision();
        void p1_score();
        void p2_score();
        void p1_win();
        void p2_win();
        void match_start(unsigned short int dir);
};