#include "KeyManager.hpp"
#include <iostream>

KeyManager::KeyManager(){
    //this is the constructor, create keylists

    /*
        Okay so we have a key manager, how do we want this to work?
        okay so we have an enum of controls, like for keys?

        This is only for keyboard input
        things like F1, F2, etc...

        we will read joystick and buttons from GPIOManager
    */

    for(std::size_t i = 0; i < m_keys.size(); i++){
        m_keys[i] = false;
    }

    F1 = false;
    F2 = false;
    F3 = false;
    F4 = false;
    ESC = false;

    std::cout << "[KeyManager] A KeyManager was created!" << std::endl;
}

void KeyManager::handleEvent(const sf::Event& event){
    if(event.is<sf::Event::KeyPressed>()){
        sf::Keyboard::Key key = event.getIf<sf::Event::KeyPressed>() -> code;
        int index = static_cast<int>(key);
        std::cout << "KEY PRESSED:   " << index << std::endl;

        if(index >= 0 && index < static_cast<int>(m_keys.size())){
            m_keys[index] = true;
        }

    }
    if(event.is<sf::Event::KeyReleased>()){
        sf::Keyboard::Key key = event.getIf<sf::Event::KeyReleased>() -> code;
        int index = static_cast<int>(key);
        std::cout << "KEY RELEASED:   " << index << std::endl;
        if(index >= 0 && index < static_cast<int>(m_keys.size())){
            m_keys[index] = false;
        }
    }
}

void KeyManager::tick(){
    F1 = m_keys[static_cast<int>(sf::Keyboard::Key::F1)];
    F2 = m_keys[static_cast<int>(sf::Keyboard::Key::F2)];
    F3 = m_keys[static_cast<int>(sf::Keyboard::Key::F3)];
    F4 = m_keys[static_cast<int>(sf::Keyboard::Key::F4)];
    ESC = m_keys[static_cast<int>(sf::Keyboard::Key::Escape)];
}