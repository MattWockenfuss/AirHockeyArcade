#include "AssetManager.hpp"
#include <iostream>

AssetManager::AssetManager(){
    //assets

    //fonts
    loadFont("Consolas", "Consolas-Regular.ttf");
    loadFont("SquareSansSerif", "square_sans_serif_7.ttf");
    loadFont("ST-SimpleSquare", "ST-SimpleSquare.otf");
    loadFont("Technoid", "TECHNOID.TTF");

    //music

}

sf::Texture& AssetManager::getAsset(std::string key){
    return assets.at(key);  // throws std::out_of_range if missing
}
sf::Font& AssetManager::getFont(std::string key){
    return fonts.at(key);  // throws std::out_of_range if missing
}
sf::SoundBuffer& AssetManager::getSound(std::string key){
    return sounds.at(key);
}


void AssetManager::loadFont(std::string key, std::string pathname){
    sf::Font font;
    if (!font.openFromFile("assets/fonts/" + pathname)){
        std::cout << "Failed to Font:: " << pathname << std::endl;
        return;
    }
    fonts.emplace(key, std::move(font));
}

void AssetManager::loadSound(std::string key, std::string pathname){
    sf::SoundBuffer sound;
    if (!sound.loadFromFile("assets/sounds/" + pathname)){
        std::cout << "Failed to SoundBuffer:: " << pathname << std::endl;
        return;
    }
    sounds.emplace(key, std::move(sound));
}