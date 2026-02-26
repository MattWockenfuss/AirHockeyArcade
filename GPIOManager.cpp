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


    inPins  = { 4, 5, 6, 12, 13, 16, 17, 27 };     // 8 button GPIOs (BCM)
    outPins = { 18, 19, 20, 21, 22, 23, 24, 25 };   // 8 LED GPIOs (BCM)

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

    gpiod_request_config_free(outReqCfg);
    gpiod_line_config_free(outConfig);
    gpiod_line_settings_free(outSettings);
}


~GPIOManager::GPIOManager() {
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

    }

    window.draw(square_background);




}