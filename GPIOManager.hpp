#pragma once

#include <SFML/Graphics.hpp>
#include <gpiod.h>

namespace sf{
    class RenderWindow;
}

class Context;

class GPIOManager {
    public:
        GPIOManager(Context& ctx);
        ~GPIOManager();
        void tick();
        void render(sf::RenderWindow& window);
        bool overlay = false;

        bool P1A, P1B, P1X, P1Y;
        bool P2A, P2B, P2X, P2Y;

        bool P1A_LED, P1B_LED, P1X_LED, P1Y_LED;
        bool P2A_LED, P2B_LED, P2X_LED, P2Y_LED;


    private:
        //Graphics Stuff
        sf::Text pinout_text;
        sf::RectangleShape square_background;

        //Non Graphics Stuff
        gpiod_chip* chip = nullptr;
        gpiod_line_request* inReq = nullptr;
        gpiod_line_request* outReq = nullptr;

        std::array<unsigned int, 8> inPins{};
        std::array<unsigned int, 8> outPins{};

        bool read(unsigned int pin) {
            // Returns 1 or 0 for that GPIO line (or <0 on failure)
            return gpiod_line_request_get_value(inReq, pin) == 1;
        }

        void write(unsigned int pin, bool on) {
            gpiod_line_request_set_value(
                outReq,
                pin,
                on ? GPIOD_LINE_VALUE_ACTIVE : GPIOD_LINE_VALUE_INACTIVE
            );
        }
};


