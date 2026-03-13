#pragma once
#include <string>
#include <gpiod.h>

/*
    This class represents 1 digital pin on our Raspberry Pi 5. Note that not every pin is instantiated to be used.



*/


class DigitalPin {
    public:
        DigitalPin(int i, int pin, std::string GPIO_NAME, std::string secondary_name){
            this -> i = i;
            this -> pin = pin;
            this -> GPIO_NAME = GPIO_NAME;
            this -> secondary_name = secondary_name;
        }
        int i;
        int pin;

        std::string GPIO_NAME;
        std::string secondary_name;

        bool lastKnownValue;
        
        void read(gpiod_line_request* inReq) {
            // Returns 1 or 0 for that GPIO line (or <0 on failure)
            lastKnownValue = gpiod_line_request_get_value(inReq, pin) == 1;
        }

        void write(gpiod_line_request* outReq, bool on) {
            lastKnownValue = on;
            gpiod_line_request_set_value(
                outReq,
                pin,
                on ? GPIOD_LINE_VALUE_ACTIVE : GPIOD_LINE_VALUE_INACTIVE //quick inline conditional
                //kinda like in excel. if on ? then first, otherwise second.
            );
        }

};