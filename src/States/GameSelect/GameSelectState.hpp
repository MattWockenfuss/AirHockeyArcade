#pragma once

#include "../State.hpp"
#include <SFML/Graphics.hpp>
#include <optional>

struct GameOption{
	sf::String name;
	sf::String desc;
	sf::Texture image;
	sf::Color foreColor;
	sf::Color backColor;
	
	GameOption(sf::String name, sf::String desc, sf::Texture img, sf::Color foreColor, sf::Color backColor);
};

class GameSelectState : public State {
    public:
        void init(Context* ctx) override;
		
        void tick() override;
        void p1render(sf::RenderWindow& p1render) override;
		void p2render(sf::RenderWindow& p2render) override; // only one player should be able to select a game, so p2render is just going to call p1render
    private:
		// screen
		float width, height;
		double screenRatio;
		
		// time
		sf::Clock clock;
		sf::Time time;
		float dt;
		float lap_Arrow;
		
		// game options
		std::vector<GameOption> gameOptions;
		int size;
		
		// textures
		std::vector<sf::Texture> arrows; // arrows will be loaded in init()
		std::optional<sf::Sprite> arrowSprite, iconSprite;
		
		// text
		std::optional<sf::Text> title, description;
		sf::FloatRect textRect;
		
		// animation vars
		int animate = 0;
		int selection = 0;
		std::vector<int> indexes;// = getIndexes(selection,3); // this will be loaded in init()
		std::vector<float> frames = {0.5,1.5,2.5};
		std::vector<double> xyz;
};