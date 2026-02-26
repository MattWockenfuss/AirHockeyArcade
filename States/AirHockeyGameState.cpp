#include "AirHockeyGameState.hpp"
#include <iostream>
#include <cmath>
#include <sstream>

#include "../Context.hpp"
#include "../AssetManager.hpp"
#include "../KeyManager.hpp"



AirHockeyGameState::AirHockeyGameState(Context& ctx)
    : 
        State(ctx), 
        text(ctx.assets -> getFont("Consolas"), "", 24), 
        label(ctx.assets -> getFont("ST-SimpleSquare"), "", 96)
    {
    std::cout << "AirHockeyGameState Created!" << std::endl;

    square = sf::RectangleShape(sf::Vector2f(100.f, 100.f));
    square.setFillColor(sf::Color::Green);
    square.setPosition(sf::Vector2f(350.f, 250.f)); // center-ish

    text.setFillColor(sf::Color::White);
    text.setPosition(sf::Vector2f(20.0f, 20.0f));
    
    label.setFillColor(sf::Color::Yellow);
    label.setPosition(sf::Vector2f(100.0f, 100.0f));
    label.setString("AirHockeyGameState State");

}

void AirHockeyGameState::tick() {
    //we want to square to loop around a circle?

    static float t = 0.0f;  //static means this var keeps its value between function calls, which is interesting
    t += 0.016f;




    float sinT = std::sin(t);
    float cosT = std::cos(t);
    float tanT = std::tan(t);
    float expT = std::exp(sinT);  //e ^ t
    float powT = std::pow(t, cosT);

    x = 400.0f + sinT * 100.0f;
    y = 400.0f + cosT * 100.0f;
    w = 100.0f + expT * 10.0f;
    h = 100.0f + powT * 20.0f;

    float logValue = std::log(std::abs(sinT) + 1.0f);  //log is ln
    float log10Value = std::log10(std::abs(cosT) + 1.0f);

    // if(ctx.keys -> F1){
    //     square.setPosition(sf::Vector2f(500.0f, 500.0f));
    //     square.setSize(sf::Vector2f(50.0f, 50.0f));
    // } else {
    //     square.setPosition(sf::Vector2f(x, y));
    //     square.setSize(sf::Vector2f(w, h));
    // }



    int r = abs(sinT) * 256;
    int g = abs(cosT) * 256;
    int b = abs(tanT) * 256;


    std::ostringstream ss;
    ss << std::fixed << std::setprecision(2);
    ss << "t = " << t << std::endl
        << "sin(t) = " << sinT << std::endl
        << "cos(t) = " << cosT << std::endl
        << "tan(t) = " << tanT << std::endl
        << "exp(t) = " << expT << std::endl
        << "pow(t) = " << powT << std::endl
        << std::endl
        << "x = " << x << std::endl
        << "y = " << y << std::endl
        << "w = " << w << std::endl
        << "h = " << h << std::endl
        << std::endl
        << "ln = " << logValue << std::endl
        << "log10 = " << log10Value << std::endl
        << std::endl
        << "r = " << r << std::endl
        << "g = " << g << std::endl
        << "b = " << b << std::endl;

    text.setString(ss.str());
    square.setFillColor(sf::Color(120, 120, 120));

}

void AirHockeyGameState::render(sf::RenderWindow& window) {
    window.draw(square);
    window.draw(text);
    window.draw(label);
}