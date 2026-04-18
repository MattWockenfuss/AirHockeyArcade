#pragma once

#include <string>
#include <unordered_map>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>


class AssetManager {
    public:
        AssetManager();

        sf::Texture& getAsset(std::string key);
        sf::Font& getFont(std::string key);
        sf::Music& getSound(std::string key);
    private:
        std::unordered_map<std::string, sf::Texture> assets;
        std::unordered_map<std::string, sf::Font> fonts;
        std::unordered_map<std::string, sf::Music> sounds;
        
        void loadAsset(std::string key, std::string pathname);
        void loadFont(std::string key, std::string pathname);
        void loadSound(std::string key, std::string pathname);
        //void loadAsset(std::string name);
};