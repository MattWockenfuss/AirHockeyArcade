#pragma once
#include <SFML/Graphics.hpp>
#include "AssetManager.hpp"

struct Context {
    sf::RenderWindow* window = nullptr;
    AssetManager* assets = nullptr;
    //audio manager
    //pin manager
    //etc
};