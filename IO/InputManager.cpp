#include "InputManager.hpp"
#include "KeyboardInput.hpp"
#include "InputInterface.hpp"

#include <iostream>

InputManager::InputManager() = default;



void InputManager::init(Context* ctx){
    this -> ctx = ctx;

    backendInput = new KeyboardInput();
    backendInput -> init(ctx, this);

    std::cout << "[InputManager] Using KEYBOARD INPUT" << std::endl;
}

void InputManager::tick(){
    backendInput -> tick();
}
void InputManager::render(sf::RenderWindow& window){
    backendInput -> render(window);
}





InputManager::~InputManager(){
    delete backendInput;
}