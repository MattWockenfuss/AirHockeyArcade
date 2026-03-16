#include "AssetManager.hpp"
#include <iostream>

AssetManager::AssetManager(){
    //assets
    loadAsset("FieldBack", "field_back.png");
    loadAsset("Field", "field.png");

    loadAsset("Paddle1a", "paddle_1a.png");
    loadAsset("Paddle1b", "paddle_1b.png");
    loadAsset("Paddle1c", "paddle_1c.png");

    loadAsset("Paddle2a", "paddle_2a.png");
    loadAsset("Paddle2b", "paddle_2b.png");
    loadAsset("Paddle2c", "paddle_2c.png");

    loadAsset("Paddle3a", "paddle_3a.png");
    loadAsset("Paddle3b", "paddle_3b.png");
    loadAsset("Paddle3c", "paddle_3c.png");

    loadAsset("Paddle4a", "paddle_4a.png");
    loadAsset("Paddle4b", "paddle_4b.png");
    loadAsset("Paddle4c", "paddle_4c.png");

    loadAsset("Paddle5a", "paddle_5a.png");
    loadAsset("Paddle5b", "paddle_5b.png");
    loadAsset("Paddle5c", "paddle_5c.png");

    loadAsset("Paddle6a", "paddle_6a.png");
    loadAsset("Paddle6b", "paddle_6b.png");
    loadAsset("Paddle6c", "paddle_6c.png");

    loadAsset("Puck0", "puck_0.png");

    loadAsset("Puck1a", "puck_1a.png");
    loadAsset("Puck1b", "puck_1b.png");
    loadAsset("Puck1c", "puck_1c.png");

    loadAsset("Puck2a", "puck_2a.png");
    loadAsset("Puck2b", "puck_2b.png");
    loadAsset("Puck2c", "puck_2c.png");

    loadAsset("Puck3a", "puck_3a.png");
    loadAsset("Puck3b", "puck_3b.png");
    loadAsset("Puck3c", "puck_3c.png");

    loadAsset("Puck4a", "puck_4a.png");
    loadAsset("Puck4b", "puck_4b.png");
    loadAsset("Puck4c", "puck_4c.png");

    loadAsset("Puck5a", "puck_5a.png");
    loadAsset("Puck5b", "puck_5b.png");
    loadAsset("Puck5c", "puck_5c.png");

    loadAsset("Puck6a", "puck_6a.png");
    loadAsset("Puck6b", "puck_6b.png");
    loadAsset("Puck6c", "puck_6c.png");

    loadAsset("Puck7a", "puck_7a.png");
    loadAsset("Puck7b", "puck_7b.png");
    loadAsset("Puck7c", "puck_7c.png");
    
    loadAsset("Puck8a", "puck_8a.png");
    loadAsset("Puck8b", "puck_8b.png");
    loadAsset("Puck8c", "puck_8c.png");

    loadAsset("Puck9a", "puck_9a.png");
    loadAsset("Puck9b", "puck_9b.png");
    loadAsset("Puck9c", "puck_9c.png");

    //fonts
    loadFont("Consolas", "Consolas-Regular.ttf");   
    loadFont("SquareSansSerif", "square_sans_serif_7.ttf");
    loadFont("ST-SimpleSquare", "ST-SimpleSquare.otf");
    loadFont("Technoid", "TECHNOID.TTF");
}

sf::Texture& AssetManager::getAsset(std::string key){
    return assets.at(key);  // throws std::out_of_range if missing
}
sf::Font& AssetManager::getFont(std::string key){
    return fonts.at(key);  // throws std::out_of_range if missing
}


void AssetManager::loadFont(std::string key, std::string pathname){
    sf::Font font;
    if (!font.openFromFile("assets/fonts/" + pathname)){
        std::cout << "Failed to Font:: " << pathname << std::endl;
        return;
    }
    fonts.emplace(key, std::move(font));
}

void AssetManager::loadAsset(std::string key, std::string pathname){
    sf::Texture texture;
    if (!texture.loadFromFile("assets/images/" + pathname)){
        std::cout << "Failed to load Image:: " << pathname << std::endl;
        return;
    }
    assets.emplace(key, std::move(texture));
}