#pragma once

#include "../State.hpp"
#include <SFML/Graphics.hpp>
#include <optional>
#include <cstdlib>
#include <ctime>
#include <cmath>


struct Puck {
    public:
        double x, y, vx, vy, diam;
        void init(Context* ctx);
        Puck(double x, double y, double vx, double vy, double diam);
        int getIndex(double x, double y, double diam);
        void draw1(sf::RenderWindow* window1);
        void draw2(sf::RenderWindow* window2);
		void setKickoff(int type);

    private:
        std::array<std::optional<sf::Sprite>, 28> pucks;
};

struct Paddle {
    public:
        int xPos, yPos;
        double x, y, vx, vy, diam;

        Paddle(int xPos, int yPos, double vx, double vy, double diam);
        void init(Context* ctx);
        int getIndex(double x, double y, double diam);
        void draw1(sf::RenderWindow* window1);
        void draw2(sf::RenderWindow* window2);

    private:
        std::array<std::optional<sf::Sprite>, 18> paddles;
};

struct Player {
	public:
        std::optional<sf::Text> name_text;
        sf::Color color;
		std::string name;
		int score;
		
		Player(std::string name, sf::Color color);
        void init(Context* ctx);
		void draw1(sf::RenderWindow* window1);
		void draw2(sf::RenderWindow* window2);
};

class AirHockeyGameState : public State {
    public:
        void init(Context* ctx) override;
		
        void tick() override;
        void p1render(sf::RenderWindow& p1window) override;
        void p2render(sf::RenderWindow& p2window) override;
    private:
		Puck puck = Puck(300, 400, 0, 0, 50);
        Paddle p1paddle = Paddle(3, 11, 0, 0, 67);
        Paddle p2paddle = Paddle(3, 2, 0, 0, 67);
		
		Player player1 = Player("PLR", sf::Color(111, 99, 255));
		Player player2 = Player("COM", sf::Color(223, 0, 0));
		
        std::optional<sf::Sprite> field;
        std::optional<sf::Sprite> fieldBack;
		
		std::optional<sf::Text> errMsg_1, errMsg_2;
        
		
        sf::Clock clock;
        sf::Time time;
        float dt;
		float timer = 0.0;
        
        bool Up = true, Down = true, Left = true, Right = true, W = true, S = true, A = true, D = true; // virtual keys to prevent one press from registering 1000 times
		int kickoff = 0;
		
		void moveObjects(Puck* puck, Paddle* paddle1, Paddle* paddle2, float dt, int iter);
};