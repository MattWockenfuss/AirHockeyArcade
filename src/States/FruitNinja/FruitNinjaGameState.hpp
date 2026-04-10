#pragma once

#include "../State.hpp"
#include <SFML/Graphics.hpp>
#include <optional>


struct Fruit{
	public:
	int type;
	int state;
	std::string key;
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
	void draw(sf::RenderWindow* window, double screenRatio, sf::Font font);
};
struct ScorePoint{
	public:
	int score;
	int x;
	int y;
	int opacity;
	
	ScorePoint(int score, int x, int y);
	void draw(sf::RenderWindow* window, double screenRatio, sf::Font font);
};
struct FruitNinjaInstance{
	public:
		// animation vars
		float timer = 0.0;
		int x = 3;
		int facing = 0;
		int frame = 0;
		int swing = -1;
		int cut = 0;
		
		// objects
		float fruitTimer = 0;
		double fruitHeight;
		std::vector<Fruit*> fruits;
		
		// scoring
		std::string name;
		std::vector<ScorePoint*> scorePoints;
		int totalPoints = 0;
		
		void drawNames(sf::RenderWindow* window, double screenRatio, sf::Font font, std::string oppName, bool opaque = false);
		void drawTotalScores(sf::RenderWindow* window, double screenRatio, sf::Font font, int oppScore, bool opaque = false);
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
		std::optional<sf::Texture> mellonTex;
		std::optional<sf::Texture> appleTex;
		std::optional<sf::Texture> orangeTex;
		std::optional<sf::Texture> lemonTex;
		std::vector<sf::Texture> fruitTexs;
		
		std::optional<sf::Sprite> back;
		std::optional<sf::Sprite> guy;
		
		// text
		sf::Font font;
		
		// game instances
		std::vector<FruitNinjaInstance> instances;
		//FruitNinjaInstance p1instance;
		
		// random vars
		int x, y, w, h;
		int type, score, n;
		int erasePoint, fallenFruit;
};