#include "AssetManager.hpp"
#include <iostream>

AssetManager::AssetManager(){
    //assets

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