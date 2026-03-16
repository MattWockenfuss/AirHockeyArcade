#pragma once

#include <string>
#include <unordered_map>
#include <SFML/Graphics.hpp>


class AssetManager {
    public:
        AssetManager();

        sf::Texture& getAsset(std::string key);
        sf::Font& getFont(std::string key);
    private:
        std::unordered_map<std::string, sf::Texture> assets;
        std::unordered_map<std::string, sf::Font> fonts;
        
        void loadAsset(std::string key, std::string pathname);
        void loadFont(std::string key, std::string pathname);
        //void loadAsset(std::string name);
};