#pragma once

#include "InputInterface.hpp"
#include "InputManager.hpp"

#include <SFML/Graphics.hpp>
#include <gpiod.h>
#include <memory>
#include <string>
#include "DigitalPin.hpp"

namespace sf{
    class RenderWindow;
}

class Context;

class GPIOManager : public InputInterface{
    public:
        void init(Context* ctx, InputManager* input) override;
        void tick() override;
        void render(sf::RenderWindow& window) override;

        DigitalPin P1A = DigitalPin(3 , 2 , "GPIO-02", "P1A");
        DigitalPin P1B = DigitalPin(5 , 3 , "GPIO-03", "P1B");
        DigitalPin P1X = DigitalPin(7 , 4 , "GPIO-04", "P1X");
        DigitalPin P1Y = DigitalPin(11, 17, "GPIO-17", "P1Y");

        DigitalPin P2A = DigitalPin(13, 27, "GPIO-27", "P2A");
        DigitalPin P2B = DigitalPin(15, 22, "GPIO-22", "P2B");
        DigitalPin P2X = DigitalPin(19, 10, "GPIO-10", "P2X");
        DigitalPin P2Y = DigitalPin(21, 9 , "GPIO-09", "P2Y");

        //these are for writing
        DigitalPin P1A_LED = DigitalPin(8 , 14, "GPIO-14", "P1A_LED");
        DigitalPin P1B_LED = DigitalPin(10, 15, "GPIO-15", "P1B_LED");
        DigitalPin P1X_LED = DigitalPin(12, 18, "GPIO-18", "P1X_LED");
        DigitalPin P1Y_LED = DigitalPin(16, 23, "GPIO-23", "P1Y_LED");

        DigitalPin P2A_LED = DigitalPin(18, 24, "GPIO-24", "P2A_LED");
        DigitalPin P2B_LED = DigitalPin(22, 25, "GPIO-25", "P2B_LED");
        DigitalPin P2X_LED = DigitalPin(24, 8 , "GPIO-08", "P2X_LED");
        DigitalPin P2Y_LED = DigitalPin(26, 7 , "GPIO-07", "P2Y_LED");

        ~GPIOManager();
    private:
        bool triggeredFlag = false;

        //Graphics Stuff
        std::optional<sf::Text> pinout_text, i_text;
        sf::RectangleShape square_background;

        //Non Graphics Stuff
        gpiod_chip* chip = nullptr;
        gpiod_line_request* inReq = nullptr;
        gpiod_line_request* outReq = nullptr;

        std::array<unsigned int, 8> inPins{};
        std::array<unsigned int, 8> outPins{};

        std::array<std::string, 40> GPIO_Labels{}; 


};


