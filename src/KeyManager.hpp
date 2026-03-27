#pragma once

#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <array>

class KeyManager {
    public:
        KeyManager();
        void tick();
        void handleEvent(const sf::Event& event);

        bool F1, F2, F3, F4, F5, ESC;

        bool W, S, A, D;
        bool R, T, F, G;

        bool Up, Down, Left, Right;
        bool J, K, N, M;

    private:
        std::array<bool, static_cast<std::size_t>(sf::Keyboard::KeyCount)> m_keys;
};


