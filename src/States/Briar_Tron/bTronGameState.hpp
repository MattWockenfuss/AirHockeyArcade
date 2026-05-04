#pragma once

#include "../State.hpp"
#include <SFML/Graphics.hpp>
#include <optional>
#include <cstdlib>
#include <ctime>
#include <cmath>

struct Bike {
	public:
		std::string name;
		int score;
		int x;
		int y;
		float visOffset; // used to keep bike moving steadily while position stays discrete
		float virOffset; // used to calculate collisions and give players some leeway
		int dir;
		float speed;
		std::vector<int> queue;
		
		Bike(std::string name, int x, int y, int dir);
		void draw1(sf::RenderWindow* window1, sf::Font font);
		void draw2(sf::RenderWindow* window2, sf::Font font);
};

class bTronGameState : public State {
    public:
        void init(Context* ctx) override;
		
        void tick() override;
        void p1render(sf::RenderWindow& p1window) override;
        void p2render(sf::RenderWindow& p2window) override;
    private:
		Bike player1 = Bike("PLR",32.0f,37.0f,1);
		Bike player2 = Bike("COM",112.0f,37.0f,3);
		
		// std::optional<sf::Text> on-screen message
		
        sf::Clock clock;
        sf::Time time;
        float dt;
		float timer = 0.0;
		double screenRatio;
		
		std::vector<std::vector<int>> gameGrid;
		int gridSX = 144;
		int gridSY = 74;
        
        bool Up = true, Down = true, Left = true, Right = true, W = true, S = true, A = true, D = true; // virtual keys to prevent one press from registering 1000 times
		int kickoff = 0;
		
		void moveObjects(Bike* player1, Bike* player2, std::vector<std::vector<int>> grid, int gridSX, int gridSY, float dt);
};