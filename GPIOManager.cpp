#include "GPIOManager.hpp"
#include <iostream>
#include "Context.hpp"
#include "AssetManager.hpp"
#include <sstream>

/*
    The GPIO Manager is responsible for reading and writing GPIO pins

    it also has a render to display the information for easy view
*/

GPIOManager::GPIOManager(){}
void GPIOManager::init(Context& ctx){
    pinout_text.setFont(ctx.assets -> getFont("Consolas"));
    pinout_text.setCharacterSize(24);


    std::cout << "[GPIOManager] New GPIOManager Created! FINALLY" << std::endl;

    pinout_text.setFillColor(sf::Color::Yellow);
    square_background.setPosition(sf::Vector2f(350.f, 250.f)); // center-ish

    square_background = sf::RectangleShape(sf::Vector2f(300.f, 800.f));
    square_background.setFillColor(sf::Color::Green);
    square_background.setPosition(sf::Vector2f(350.f, 250.f)); // center-ish

    P1A = false;
    P1B = false;
    P1X = false;
    P1Y = false;

    P2A = false;
    P2B = false;
    P2X = false;
    P2Y = false;

    P1A_LED = false;
    P1B_LED = false;
    P1X_LED = false;
    P1Y_LED = false;

    P2A_LED = false;
    P2B_LED = false;
    P2X_LED = false;
    P2Y_LED = false;

    
    chip = gpiod_chip_open("/dev/gpiochip0");
    if(!chip){
        std::cerr << " Failed to Open Chip!" << std::endl;
        std::abort();
    }

    inPins  = { 2, 3, 4, 17, 27, 22, 10, 9 };     // 8 button GPIOs (BCM)
    outPins = { 14, 15, 18, 23, 24, 25, 28, 7 };   // 8 LED GPIOs (BCM)

    // ---------- INPUT REQUEST (all 8 lines at once) ----------
    auto* inSettings = gpiod_line_settings_new();
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
    gpiod_request_config_set_consumer(outReqCfg, "leds");

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
    P1A = read(inPins[0]);
    P1B = read(inPins[1]);
    P1X = read(inPins[2]);
    P1Y = read(inPins[3]);

    P2A = read(inPins[4]);
    P2B = read(inPins[5]);
    P2X = read(inPins[6]);
    P2Y = read(inPins[7]);

    write(outPins[0], P1A_LED);
    write(outPins[1], P1B_LED);
    write(outPins[2], P1X_LED);
    write(outPins[3], P1Y_LED);

    write(outPins[4], P2A_LED);
    write(outPins[5], P2B_LED);
    write(outPins[6], P2X_LED);
    write(outPins[7], P2Y_LED);
}

void GPIOManager::render(sf::RenderWindow& window){
    if(overlay){

        std::ostringstream ss;
        ss  << "P1A = " << P1A << std::endl
            << "P1B = " << P1B << std::endl
            << "P1X = " << P1X << std::endl
            << "P1Y = " << P1Y << std::endl

            << "P2A = " << P2A << std::endl
            << "P2B = " << P2B << std::endl
            << "P2X = " << P2X << std::endl
            << "P2Y = " << P2Y << std::endl;

        pinout_text.setString(ss.str());
        window.draw(square_background);
        window.draw(pinout_text);
    }





}