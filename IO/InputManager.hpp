#pragma once

//#include "InputInterface.hpp"

#include <SFML/Graphics.hpp>
#include <optional>
#include <memory>
#include <array>
#include <string>

class InputInterface;
class Context;
namespace sf {
    class RenderWindow;
}


class InputManager {
    public:
        InputManager();
        ~InputManager();

        void init(Context* ctx);
        void tick();
        void render(sf::RenderWindow& window);

        bool overlay = false;

        //Player 1 Inputs
        bool P1_Up    = false;
        bool P1_Down  = false;
        bool P1_Left  = false;
        bool P1_Right = false;

        bool P1A = false;
        bool P1B = false;
        bool P1X = false;
        bool P1Y = false;

        //Player 2 Inputs
        bool P2_Up    = false;
        bool P2_Down  = false;
        bool P2_Left  = false;
        bool P2_Right = false;

        bool P2A = false;
        bool P2B = false;
        bool P2X = false;
        bool P2Y = false;

        //Output LEDs
        bool P1A_LED = false;
        bool P1B_LED = false;
        bool P1X_LED = false;
        bool P1Y_LED = false;

        bool P2A_LED = false;
        bool P2B_LED = false;
        bool P2X_LED = false;
        bool P2Y_LED = false;

    private:
        Context* ctx = nullptr;
        InputInterface* backendInput = nullptr;

        bool triggeredFlag = false;
        //A reference to the actual input
        void renderDebugControllers(sf::RenderWindow& window);
        void drawSquare(float x, float y, bool isGreen, sf::RenderWindow& window);
        std::optional<sf::Text> textbox;
        sf::RectangleShape rect;


};