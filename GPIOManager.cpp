#include "GPIOManager.hpp"
#include <iostream>
#include "Context.hpp"
#include "AssetManager.hpp"
#include "KeyManager.hpp"
#include <sstream>

/*
    The GPIO Manager is responsible for reading and writing GPIO pins

    it also has a render to display the information for easy view
*/

void GPIOManager::init(Context* ctx){
    this -> ctx = ctx;
    // auto& font = ctx.assets -> getFont("Consolas");

    pinout_text.emplace(ctx -> assets -> getFont("Consolas"), "", 28);
    pinout_text -> setPosition({10.f, 10.f});
    pinout_text -> setFillColor(sf::Color::Yellow);

    
    i_text.emplace(ctx -> assets -> getFont("Consolas"), "", 30);
    i_text -> setPosition({500.0f, 10.0f});
    i_text -> setFillColor(sf::Color::Magenta);

    std::cout << "[GPIOManager] New GPIOManager Created!" << std::endl;



    //I should make a class representing a Digital Pin, that can be either read or write, represents a single variable, knows where to render, etc...



    GPIO_Labels = {
        "   3.3V", "5V     ",
        "GPIO-02", "5V     ",
        "GPIO-03", "GND    ",
        "GPIO-04", "GPIO-14",
        "    GND", "GPIO-15",
        "GPIO-17", "GPIO-18",
        "GPIO-27", "GND    ",
        "GPIO-22", "GPIO-23",
        "   3.3V", "GPIO-24",
        "GPIO-10", "GND    ",
        "GPIO-09", "GPIO-25",
        "GPIO-11", "GPIO-08",
        "    GND", "GPIO-07",
        "GPIO-00", "GPIO-01",
        "GPIO-05", "GND    ",
        "GPIO-06", "GPIO-12",
        "GPIO-13", "GND    ",
        "GPIO-19", "GPIO-16",
        "GPIO-26", "GPIO-20",
        "    GND", "GPIO-21"
    };

    
    chip = gpiod_chip_open("/dev/gpiochip0");
    if(!chip){
        std::cerr << " Failed to Open Chip!" << std::endl;
        std::abort();
    }

    inPins  = { 2, 3, 4, 17, 27, 22, 10, 9 };     // 8 button GPIOs (BCM)
    outPins = { 14, 15, 18, 23, 24, 25, 8, 7 };   // 8 LED GPIOs (BCM)

    // ---------- INPUT REQUEST (all 8 lines at once) ----------
    gpiod_line_settings* inSettings = gpiod_line_settings_new();
    gpiod_line_settings_set_direction(inSettings, GPIOD_LINE_DIRECTION_INPUT);

    //gpiod_line_settings_set_bias(inSettings, GPIOD_LINE_BIAS_PULL_UP);

    // If your buttons have no external resistors, you typically want a pull-up.
    // NOTE: with pull-ups, buttons are commonly "active-low" (pressed reads 0).
    // gpiod_line_settings_set_bias(inSettings, GPIOD_LINE_BIAS_PULL_UP);

    auto* inConfig = gpiod_line_config_new();
    gpiod_line_config_add_line_settings(inConfig, inPins.data(), inPins.size(), inSettings);

    auto* inReqCfg = gpiod_request_config_new();
    gpiod_request_config_set_consumer(inReqCfg, "buttons");

    inReq = gpiod_chip_request_lines(chip, inReqCfg, inConfig);
    if(!inReq){
        std::cerr << "Failed to request the input lines!" << std::endl;
        std::abort();
    }

    gpiod_request_config_free(inReqCfg);
    gpiod_line_config_free(inConfig);
    gpiod_line_settings_free(inSettings);

    // ---------- OUTPUT REQUEST (all 8 lines at once) ----------
    auto* outSettings = gpiod_line_settings_new();
    gpiod_line_settings_set_direction(outSettings, GPIOD_LINE_DIRECTION_OUTPUT);
    gpiod_line_settings_set_output_value(outSettings, GPIOD_LINE_VALUE_INACTIVE);

    auto* outConfig = gpiod_line_config_new();
    gpiod_line_config_add_line_settings(outConfig, outPins.data(), outPins.size(), outSettings);

    auto* outReqCfg = gpiod_request_config_new();
    gpiod_request_config_set_consumer(outReqCfg, "led");

    outReq = gpiod_chip_request_lines(chip, outReqCfg, outConfig);
    if(!outReq){
        std::cerr << "Failed to request the output lines!" << std::endl;
        std::abort();
    }


    gpiod_request_config_free(outReqCfg);
    gpiod_line_config_free(outConfig);
    gpiod_line_settings_free(outSettings);
}




GPIOManager::~GPIOManager() {
    if (outReq) gpiod_line_request_release(outReq);
    if (inReq)  gpiod_line_request_release(inReq);
    if (chip)   gpiod_chip_close(chip);
}

void GPIOManager::tick(){
    if(overlay){
        //P1A_LED = true;
    }else {
        //P1A_LED = false;
    }

    P1A.read(inReq);
    P1B.read(inReq);
    P1X.read(inReq);
    P1Y.read(inReq);
    
    P2A.read(inReq);
    P2B.read(inReq);
    P2X.read(inReq);
    P2Y.read(inReq);

    P1A_LED.write(outReq, P1A_LED.lastKnownValue);
    P1B_LED.write(outReq, P1B_LED.lastKnownValue);
    P1X_LED.write(outReq, P1X_LED.lastKnownValue);
    P1Y_LED.write(outReq, P1Y_LED.lastKnownValue);

    P2A_LED.write(outReq, P2A_LED.lastKnownValue);
    P2B_LED.write(outReq, P2B_LED.lastKnownValue);
    P2X_LED.write(outReq, P2X_LED.lastKnownValue);
    P2Y_LED.write(outReq, P2Y_LED.lastKnownValue);


    //ok so we have this overlay variable we want to flip it when we press F1
    if(ctx -> keys -> F1){
        if(triggeredFlag == false){
            //ok so we are holding F1 and we havent trigger it yet, 
            ctx -> gpio -> overlay = !ctx -> gpio -> overlay;
            triggeredFlag = true;
        }
    }else{// we stopped holding F1
        triggeredFlag = false;
    } 
}

void GPIOManager::render(sf::RenderWindow& window){
    if(!overlay) return;  //only render if the overlay is up
    
    float startX = 300.0f;
    float startY = 100.0f;
    
    float baseWidth = 370.0f;
    float baseHeight = 710.0f;
    
    int row = 0;
    float padding = 35.0f;


    //base background border
    square_background = sf::RectangleShape(sf::Vector2f(baseWidth + 12.0f, baseHeight + 12.0f));
    square_background.setPosition({startX - 15.0f - 6.0f, startY - 6.0f});
    square_background.setFillColor(sf::Color(139, 107, 0));
    window.draw(square_background);

    square_background = sf::RectangleShape(sf::Vector2f(baseWidth, baseHeight));
    square_background.setPosition({startX - 15.0f, startY});
    square_background.setFillColor(sf::Color(255, 240, 102));
    window.draw(square_background);


    // std::ostringstream ss;
    // ss  << "P1A = " << P1A.lastKnownValue << std::endl
    //     << "P1B = " << P1B.lastKnownValue << std::endl
    //     << "P1X = " << P1X.lastKnownValue << std::endl
    //     << "P1Y = " << P1Y.lastKnownValue << std::endl

    //     << "P2A = " << P2A.lastKnownValue << std::endl
    //     << "P2B = " << P2B.lastKnownValue << std::endl
    //     << "P2X = " << P2X.lastKnownValue << std::endl
    //     << "P2Y = " << P2Y.lastKnownValue << std::endl;

    // pinout_text -> setString(ss.str());





    //window.draw(*pinout_text);
    
    //lets draw the pinout



    for(int i = 1; i <= 40; i++){
        std::ostringstream ss;
        ss << "(" << i << ")" << std::endl;

        sf::RectangleShape pinSqr = sf::RectangleShape(sf::Vector2f(30.f, 30.f));
        pinSqr.setFillColor(sf::Color::Black);
        

        sf::RectangleShape pinSqrInside = sf::RectangleShape(sf::Vector2f(26.0f, 26.0f));
        pinSqrInside.setFillColor(sf::Color::Red);
        
        //this is a horrible way to do it.

        if(i == 2 || i == 4){
            //then 5V
            i_text -> setFillColor(sf::Color::Magenta);
            pinSqrInside.setFillColor(sf::Color::Magenta);
        }else if (i == 1 || i == 17){
            //then 3.3V
            i_text -> setFillColor(sf::Color::Blue);
            pinSqrInside.setFillColor(sf::Color::Blue);
        }else if(i == 9 || i == 25 || i == 39 || i == 6 || i == 14 || i == 20 || i == 30 || i == 34){
            //then ground
            i_text -> setFillColor(sf::Color::Black);
            pinSqrInside.setFillColor(sf::Color::Black);
        }else{
            i_text -> setFillColor(sf::Color::Red);
            pinSqrInside.setFillColor(sf::Color::Red);
            //it used to be just stupid, this is just plain dumb
            if(P1A.i == i){
                if(P1A.lastKnownValue) pinSqrInside.setFillColor(sf::Color::Green);
            }else if(P1B.i == i){
                if(P1B.lastKnownValue) pinSqrInside.setFillColor(sf::Color::Green);
            }else if(P1X.i == i){
                if(P1X.lastKnownValue) pinSqrInside.setFillColor(sf::Color::Green);
            }else if(P1Y.i == i){
                if(P1Y.lastKnownValue) pinSqrInside.setFillColor(sf::Color::Green);
            }else if(P2A.i == i){
                if(P2A.lastKnownValue) pinSqrInside.setFillColor(sf::Color::Green);
            }else if(P2B.i == i){
                if(P2B.lastKnownValue) pinSqrInside.setFillColor(sf::Color::Green);
            }else if(P2X.i == i){
                if(P2X.lastKnownValue) pinSqrInside.setFillColor(sf::Color::Green);
            }else if(P2Y.i == i){
                if(P2Y.lastKnownValue) pinSqrInside.setFillColor(sf::Color::Green);
            }

            if(P1A_LED.i == i){
                if(P1A_LED.lastKnownValue) pinSqrInside.setFillColor(sf::Color::Green);
            }else if(P1B_LED.i == i){
                if(P1B_LED.lastKnownValue) pinSqrInside.setFillColor(sf::Color::Green);
            }else if(P1X_LED.i == i){
                if(P1X_LED.lastKnownValue) pinSqrInside.setFillColor(sf::Color::Green);
            }else if(P1Y_LED.i == i){
                if(P1Y_LED.lastKnownValue) pinSqrInside.setFillColor(sf::Color::Green);
            }else if(P2A_LED.i == i){
                if(P2A_LED.lastKnownValue) pinSqrInside.setFillColor(sf::Color::Green);
            }else if(P2B_LED.i == i){
                if(P2B_LED.lastKnownValue) pinSqrInside.setFillColor(sf::Color::Green);
            }else if(P2X_LED.i == i){
                if(P2X_LED.lastKnownValue) pinSqrInside.setFillColor(sf::Color::Green);
            }else if(P2Y_LED.i == i){
                if(P2Y_LED.lastKnownValue) pinSqrInside.setFillColor(sf::Color::Green);
            }



        }
        
        //next we want to set the fill color of the inside rect depending on the value in the GPIO Manager
        //


        if(i % 2 == 0){
            //then its even, rightside,
                i_text -> setPosition({startX + 210.0f, startY + (row * padding)});
                pinSqr.setPosition({startX + 170.0f, startY + (row * padding) + 7.0f});
                pinSqrInside.setPosition({startX + 170.0f + 2.0f, startY + (row * padding) + 7.0f + 2.0f}); //The 7 is to match the font, the 2 is because they are inside the other square, giving a border
                window.draw(pinSqr);
                window.draw(pinSqrInside);
            row++;
        }else{
            //then its odd, left side
            i_text -> setPosition({startX + 1.0f, startY + (row * padding)});
            pinSqr.setPosition({startX + 130.0f, startY + (row * padding) + 7.0f});
            pinSqrInside.setPosition({startX + 130.0f + 2.0f, startY + (row * padding) + 7.0f + 2.0f});
            window.draw(pinSqr);
            window.draw(pinSqrInside);
        }
        i_text -> setString(GPIO_Labels[i - 1]);
        window.draw(*i_text);
    }
    




}