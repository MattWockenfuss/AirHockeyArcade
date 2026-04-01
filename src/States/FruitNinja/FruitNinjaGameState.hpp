#pragma once

#include "../State.hpp"
#include <SFML/Graphics.hpp>
#include <optional>


struct Fruit{
	public:
	int type;
	int state;
	double x;
	double y;
	int w;
	int h;
	double vx;
	double vy;
	int frame;
	float time;
	sf::Texture tileSet;
	
	Fruit(sf::Texture tileSet, int type, int state, int x, int y, int w, int h, double vx, double vy);
	void move(float dt);
	void draw(sf::RenderWindow* window, double screenRatio);
};

class FruitNinjaGameState : public State {
    public:
        void init(Context* ctx) override;
		
        void tick() override;
        void p1render(sf::RenderWindow& p1render) override;
		void p2render(sf::RenderWindow& p2render) override;
    private:
		// screen
		float width, height;
		double screenRatio;
		
		// time
		sf::Clock clock;
		sf::Time time;
		float dt;
		
		// textures
		std::optional<sf::Texture> backTex;
		std::optional<sf::Texture> redTex;
		std::optional<sf::Texture> greenTex;
		std::optional<sf::Texture> mellonTex;
		std::optional<sf::Texture> appleTex;
		std::optional<sf::Texture> orangeTex;
		std::optional<sf::Texture> lemonTex;
		std::vector<sf::Texture> fruitTexs;
		
		std::optional<sf::Sprite> back;
		std::optional<sf::Sprite> guy;
		
		// animation vars
		int guyX = 0;
		int guyFacing = 0;
		int guyFrame = 0;
		bool guyRed = true;
		bool guyCanChange = false;
		bool guySwing = false;
		bool cutFruit = false;
		int guyMove = 0;
		float guyStep = 0;
		float guyTime = 0.0;
		
		// objects
		std::vector<Fruit*> fruits;
		int fallenFruit;
		float fruitTime = 0;
		int fruitType = 0;
		int fruitX = 0;
		std::vector<int> fruitDist = {0,0,0,0,1,1,1,1,1,1,2,2,2,2,2,2,3,3,3,3,3,4,4,4,4,5,5,5,6,6,7};
};