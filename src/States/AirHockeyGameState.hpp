#pragma once

#include "State.hpp"
#include <SFML/Graphics.hpp>
#include <optional>


struct Puck {
    public:
        double x, y, vx, vy, diam;

        Puck(double x, double y, double vx, double vy, double diam);
        int getIndex(double x, double y, double diam);
        void draw(sf::RenderWindow* window);

    private:
        sf::Texture pucks[28];
};

struct Paddle {
    public:
        int xPos, yPos;
        double x, y, vx, vy, diam;

        Paddle(int xPos, int yPos, double vx, double vy, double diam);
        int getIndex(double x, double y, double diam);
        void draw(sf::RenderWindow* window);

    private:
        sf::Texture paddles[18];
};

class AirHockeyGameState : public State {
    public:
        void init(Context* ctx) override;

        void tick() override;
        void render(sf::RenderWindow& window) override;

        Puck puck = Puck(300,400,0,0,50);
        Paddle p1paddle = Paddle(3,11,0,0,67);
        Paddle p2paddle = Paddle(3,2,0,0,67);

        std::optional<sf::Sprite> field;
        std::optional<sf::Sprite> fieldBack;    

        sf::Clock clock;
        sf::Time time;
        float dt;
        
        bool Up = true, Down = true, Left = true, Right = true, W = true, S = true, A = true, D = true; // virtual keys to prevent one press from registering 1000 times

        AirHockeyGameState();
        ~AirHockeyGameState() override = default;
    private:
        std::optional<sf::Text> p1Name, p2Name, p1Score, p2Score;
};