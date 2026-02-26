#include "GPIOManager.hpp"
#include <iostream>

/*
    The GPIO Manager is responsible for reading and writing GPIO pins

    it also has a render to display the information for easy view
*/


GPIOManager::GPIOManager(){
    //probably pass through ctx at some point? hmmm
    std::cout << "[GPIOManager] New GPIOManager Created!" << std::endl;

    square_background = sf::RectangleShape(sf::Vector2f(300.f, 800.f));
    square_background.setFillColor(sf::Color::Green);
    square_background.setPosition(sf::Vector2f(350.f, 250.f)); // center-ish
}

void GPIOManager::tick(){
    
}

void GPIOManager::render(sf::RenderWindow& window){
    if(overlay){

    }

    window.draw(square_background);




}