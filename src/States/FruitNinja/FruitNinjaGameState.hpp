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
	
	Fruit(int type, int state, int x, int y, int w, int h, double vx, double vy);
	void init(int type, int state, int x, int y, int w, int h, double vx, double vy);
	void move(float dt);
	void draw(sf::RenderTexture* window, double screenRatio, sf::Text& text, sf::Color& color, sf::Sprite& sprite);
};
struct ScorePoint{
	public:
	int score;
	int x;
	int y;
	int opacity;
	
	ScorePoint(int score, int x, int y);
	void init(int score, int x, int y);
	void draw(sf::RenderTexture* window, double screenRatio, sf::Text& text, sf::Color& color);
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
		
		// fruit
		std::vector<Fruit*> fruits;
		std::vector<std::vector<float>> fruitSongTimes;
		std::vector<float> fruitSongDelays;
		int fruitSong = 0;
		int fruitNote = 1;
		float fruitDelay = 0;
		float songDelay = 60;
		
		// scoring
		std::string name;
		std::vector<ScorePoint*> scorePoints;
		int totalPoints = 0;
		
		FruitNinjaInstance(std::string name);
		// score drawing functions
		void drawNames(sf::RenderTexture* window, double screenRatio, sf::Text& text, sf::Color& color, std::string oppName);
		void drawTotalScores(sf::RenderTexture* window, double screenRatio, sf::Text& text, sf::Color& color, int oppScore);
};

class FruitNinjaGameState : public State {
    public:
        void init(Context* ctx) override;
		
        void tick() override;
        void p1render(sf::RenderTexture& p1render) override;
		void p2render(sf::RenderTexture& p2render) override;
    private:
		bool triggerGameEnd = true; // this is used to make sure the game end is only triggered once, since the tick function will continue to run until the state is changed
	
		// screen
		float width, height;
		double screenRatio;
		
		// time
		sf::Clock clock;
		sf::Time time;
		float dt;
		
		// sprites
		std::optional<sf::Sprite> background;
		std::optional<sf::Sprite> guy;
		std::array<std::optional<sf::Sprite>, 4> fruitSprites;
		
		// object pools
		std::vector<std::vector<Fruit*>> fruitPool;
		std::vector<ScorePoint*> pointPool;
		
		// text
		std::optional<sf::Text> text;
		std::optional<sf::Color> color;
		
		// game instances
		std::vector<FruitNinjaInstance> instances;
		
		// random vars
		int x, y, w, h;
		int type, score, n;
		int erasePoint, fallenFruit;
		double fruitHeight;
};