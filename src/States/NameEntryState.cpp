#include "NameEntryState.hpp"
#include <iostream>
#include <cmath>
#include <sstream>


#include "../Context.hpp"
#include "../AssetManager.hpp"
#include "../KeyManager.hpp"
#include "../IO/InputManager.hpp"
#include "GameStateManager.hpp"


void NameEntryState::init(Context* ctx){
    State::init(ctx);
    std::cout << "NameEntryState Created!" << std::endl;
    /*
        Okay so we are on the name entry state, players can enter their names, upto 5 characters, via an onscreen keyboard.
        Player 1 can go back and both players have to ready up to continue

        how do we do this? How can we scroll around the 'keyboard'
        What if I represented this as a linked list of characters, and you have one that is selected?
        
        but then each key needs to be explicitly described and rendered, totally doable, but ehhh

        what if we did an 2darray of characters in a grid shape? based on movement of joystick u move around the keyboard
        part of me would prefer them not being in a grid? other part of me thinks its fine?

        okay so they can enter their name in?
        when both players have hit done?, can you cancel?

        so p1ready = true;
        if both are true, then continue

        have a flashing text caret, if you arent ready, it renders?


        okay so both players have readied up, now what?
        change game state to the game selection state, change text for instructions to nothing?
        maybe just not have it render?


    
    */

    charLabel.emplace(ctx -> assets -> getFont("ArcadeNormal"), "", 52);
    // const sf::FloatRect charLabelRect = charLabel -> getLocalBounds();
    // charLabel -> setOrigin(charLabelRect.getCenter());
    // charLabel -> setPosition(sf::Vector2f(ctx -> p1window -> getSize().x / 2.0f, ctx -> p1window -> getSize().y / 1.5f));


}

void NameEntryState::tick() {
    //so each player has their own screen being rendered?
    //we render essentially the same thing, except each screen one of the keys is highlighted because its selected,
    //and we render their name as its being typed in.

    //player 1 can go back to menu, and both players ready up.

    /*
        Okay so this is edge detection, or really rising edge detection, because we are
        trying to capture the onPress. We keep track of the previous state and compare them. 
        Technically there is a slightly more memory optimal solution, considering our joystick
        is gated, but this is cleaner and more 'standard'.
    */
    bool nowEnter = ctx -> input -> P1A;
    bool leftNow  = ctx -> input -> P1_Left;
    bool rightNow = ctx -> input -> P1_Right;
    bool upNow    = ctx -> input -> P1_Up;
    bool downNow  = ctx -> input -> P1_Down;

    /*
        This is more messy than it needs to be, but all of the extra logic is here to ensure
        that we stay in bounds with 2 odd sized buttons, namely the enter and back
    */
    if(!p1ready){
        if (upNow && !p1prevUp){
            if(p1x == 10 && (p1y == 2 || p1y == 3)){
                p1y = 0;
            }else{
                p1y--;
            }
        }else if (leftNow && !p1prevLeft){
            if(p1x == 10 && (p1y == 0 || p1y == 1)){
                p1x = 9;
                p1y = 0;
            }else if(p1x == 10 && (p1y == 2 || p1y == 3)){
                p1x = 9;
                p1y = 2;
            }else{
                p1x--;
            }
        }else if (downNow && !p1prevDown){
            if(p1x == 10 && (p1y == 0 || p1y == 1)){
                p1y = 2;
            }else{
                p1y++;
            }
        }else if (rightNow && !p1prevRight){
            p1x++;
        }
    }

    if(p1x < 0) p1x = 0;
    if(p1x > 10) p1x = 10;
    if(p1y < 0) p1y = 0;
    if(p1y > 3) p1y = 3;

    /*
        Handles a single “enter” input press (edge-triggered) to interact with the name entry UI. 
        If the cursor is on the character grid, it appends the selected character to the player name 
        (up to NAME_MAX_LENGTH). If the cursor is on the READY/UNDO button, it toggles the ready state 
        and assigns a default name (“GUE$T”) if the name is empty. Otherwise (on the back/delete area), 
        it removes the last character from the name if one exists.
    */
    if(nowEnter && !p1prevEnter){
        if(p1x < 10){
            char c = chars[p1y][p1x];
            //std::cout << "ENTERED " << std::string(1, c) << "\n";
            if(p1name.length() < NAME_MAX_LENGTH) p1name += std::string(1, c);
        }else{
            if(p1y == 0 || p1y == 1){
                p1ready = !p1ready;
                if(p1name.empty()) p1name = "GUE$T";
            }else{
                if(!p1name.empty()) p1name.pop_back();
            }
        }
    }
    
    //keep track for next tick
    p1prevUp = upNow;
    p1prevLeft = leftNow;
    p1prevDown = downNow;
    p1prevRight = rightNow;
    p1prevEnter = nowEnter;


    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////             PLAYER 2        ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    //this is just copy pasted
    
    nowEnter = ctx -> input -> P2A;
    leftNow  = ctx -> input -> P2_Left;
    rightNow = ctx -> input -> P2_Right;
    upNow    = ctx -> input -> P2_Up;
    downNow  = ctx -> input -> P2_Down;

    /*
        This is more messy than it needs to be, but all of the extra logic is here to ensure
        that we stay in bounds with 2 odd sized buttons, namely the enter and back
    */
    if(!p2ready){
        if (upNow && !p2prevUp){
            if(p2x == 10 && (p2y == 2 || p2y == 3)){
                p2y = 0;
            }else{
                p2y--;
            }
        }else if (leftNow && !p2prevLeft){
            if(p2x == 10 && (p2y == 0 || p2y == 1)){
                p2x = 9;
                p2y = 0;
            }else if(p2x == 10 && (p2y == 2 || p2y == 3)){
                p2x = 9;
                p2y = 2;
            }else{
                p2x--;
            }
        }else if (downNow && !p2prevDown){
            if(p2x == 10 && (p2y == 0 || p2y == 1)){
                p2y = 2;
            }else{
                p2y++;
            }
        }else if (rightNow && !p2prevRight){
            p2x++;
        }
    }

    if(p2x < 0) p2x = 0;
    if(p2x > 10) p2x = 10;
    if(p2y < 0) p2y = 0;
    if(p2y > 3) p2y = 3;

    /*
        Handles a single “enter” input press (edge-triggered) to interact with the name entry UI. 
        If the cursor is on the character grid, it appends the selected character to the player name 
        (up to NAME_MAX_LENGTH). If the cursor is on the READY/UNDO button, it toggles the ready state 
        and assigns a default name (“GUE$T”) if the name is empty. Otherwise (on the back/delete area), 
        it removes the last character from the name if one exists.
    */
    if(nowEnter && !p2prevEnter){
        if(p2x < 10){
            char c = chars[p2y][p2x];
            //std::cout << "ENTERED " << std::string(1, c) << "\n";
            if(p2name.length() < NAME_MAX_LENGTH) p2name += std::string(1, c);
        }else{
            if(p2y == 0 || p2y == 1){
                p2ready = !p2ready;
                if(p2name.empty()) p2name = "GUE$T";
            }else{
                if(!p2name.empty()) p2name.pop_back();
            }
        }
    }
    
    //keep track for next tick
    p2prevUp = upNow;
    p2prevLeft = leftNow;
    p2prevDown = downNow;
    p2prevRight = rightNow;
    p2prevEnter = nowEnter;

    //either both players readied up, or only 1 and they readied up.
    //also check for flag
    if((p1ready && p2ready && !gameStartingFlag) || (p1ready && !ctx -> renderp2 && !gameStartingFlag)){
        //then start the game
        gameStartingFlag = true;
        ctx -> gsm -> requestStateChange(States::AirHockey, 5.0f, 2.0f);
        ctx -> p1name = this -> p1name;
        ctx -> p2name = this -> p2name;
    }

}

void NameEntryState::p1render(sf::RenderWindow& p1window) {
    /*  
        Renders the player name entry screen, including a centered grid-based character selector, current name display, 
        and interactive UI elements. It calculates layout positions dynamically based on window size, draws selectable 
        character tiles with highlighting for the current cursor position (p1x, p1y), and renders special buttons for READY/UNDO 
        and Back with visual feedback. The function also displays the player’s typed name above the grid, includes a blinking caret 
        when the player is not readied, and shows flashing instructional text that changes based on readiness state. Additionally, 
        it handles visual styling (colors, borders, centering) and includes a debug display of the current selection coordinates.

        This was generated by ChatGPT                                                                                                   */

    //declare some helper vars
    float squareWidth = 80.0f;
    float gap = 19.0f;
    float bw = 4.0f;

    unsigned int width = p1window.getSize().x;
    unsigned int height = p1window.getSize().y;
    float startX = width / 2 - (1.0f / 2) * (13 * (squareWidth + gap) - gap);
    float startY = height / 2 - (1.0f / 2) * (4 * (squareWidth + gap) - gap);


    //the underline for the name entering
    borderSquare.setSize({(13 * (squareWidth + gap) - gap) + (bw * 2), 4.0f});
    borderSquare.setPosition({startX - bw, startY - (gap * 2)});
    p1window.draw(borderSquare);

    //the text for the name they are typing in
    charLabel -> setString(p1name);
    charLabel -> setCharacterSize(96);
    charLabel -> setOrigin({0.0f, charLabel -> getLocalBounds().size.y});
    charLabel -> setFillColor(sf::Color::Yellow);
    charLabel -> setPosition({startX, startY - ((gap * 3) + 2.0f)});
    p1window.draw(*charLabel);

    //rendering the 'keyboard'
    charLabel -> setCharacterSize(52);
    for(int i = 0; i < 10; i++){
        for(int j = 0; j < 4; j++){
            borderSquare.setSize({squareWidth + (bw * 2), squareWidth + (bw * 2)});
            borderSquare.setPosition({startX - bw + ((squareWidth + gap) * i), startY - bw + ((squareWidth + gap) * j)});
            
            charSquare.setSize({squareWidth, squareWidth});
            charSquare.setPosition({startX + ((squareWidth + gap) * i), startY + ((squareWidth + gap) * j)});

            charLabel -> setString(std::string(1, chars[j][i]));
            charLabel -> setOrigin(charLabel -> getLocalBounds().getCenter()); //where we set its position is the center of the string/char
            charLabel -> setFillColor(sf::Color::Yellow);
            charLabel -> setPosition({startX - bw + ((squareWidth + gap) * i) + (squareWidth / 2), startY - bw + ((squareWidth + gap) * j) + (squareWidth / 2)});

            if(p1x == i && p1y == j){
                //if the player is currently selecting this one
                borderSquare.setFillColor(sf::Color(255, 255, 0, 255));
                charSquare.setFillColor(sf::Color(255, 255, 0, 255));
                charLabel -> setFillColor(sf::Color::Black);
            }else{
                borderSquare.setFillColor(sf::Color(255, 255, 0, 255));
                charSquare.setFillColor(sf::Color(0, 0, 0, 255));
                charLabel -> setFillColor(sf::Color::Yellow);
            }

            p1window.draw(borderSquare);
            p1window.draw(charSquare);
            p1window.draw(*charLabel);
        }        
    }

    //Rendering the 'Ready Up' Button
    borderSquare.setSize({squareWidth * 3 + (bw * 2) + gap * 2, squareWidth * 2 + (bw * 2) + gap});
    borderSquare.setPosition({startX - bw + ((squareWidth + gap) * 10), startY - bw + ((squareWidth + gap) * 0)});
    
    charSquare.setSize({squareWidth * 3 + gap * 2, squareWidth * 2 + gap});
    charSquare.setPosition({startX + ((squareWidth + gap) * 10), startY + ((squareWidth + gap) * 0)});
    
    if(p1ready){
        charLabel -> setString("UNDO");
    }else{
        charLabel -> setString("READY");
    }
    
    charLabel -> setOrigin(charLabel -> getLocalBounds().getCenter()); //where we set its position is the center of the string/char
    charLabel -> setPosition({startX - bw + ((squareWidth + gap) * 10) + ((squareWidth * 3 + gap * 2) / 2), startY - bw + ((squareWidth + gap) * 0) + ((squareWidth * 2 + gap) / 2)});
    
    if(p1x == 10 && (p1y == 0 || p1y == 1)){
        borderSquare.setFillColor(sf::Color(255, 255, 0, 255));
        charSquare.setFillColor(sf::Color(255, 255, 0, 255));
        charLabel -> setFillColor(sf::Color::Black);
    }else{
        borderSquare.setFillColor(sf::Color(255, 255, 0, 255));
        charSquare.setFillColor(sf::Color(0, 0, 0, 255));
        charLabel -> setFillColor(sf::Color::Yellow);
    }

    p1window.draw(borderSquare);
    p1window.draw(charSquare);
    p1window.draw(*charLabel);



    //Rendering the 'BACK' Button
    borderSquare.setSize({squareWidth * 3 + (bw * 2) + gap * 2, squareWidth * 2 + (bw * 2) + gap});
    borderSquare.setPosition({startX - bw + ((squareWidth + gap) * 10), startY - bw + ((squareWidth + gap) * 2)});
    
    charSquare.setSize({squareWidth * 3 + gap * 2, squareWidth * 2 + gap});
    charSquare.setPosition({startX + ((squareWidth + gap) * 10), startY + ((squareWidth + gap) * 2)});
    
    charLabel -> setString("Back");
    charLabel -> setOrigin(charLabel -> getLocalBounds().getCenter()); //where we set its position is the center of the string/char
    charLabel -> setFillColor(sf::Color::Yellow);
    charLabel -> setPosition({startX - bw + ((squareWidth + gap) * 10) + ((squareWidth * 3 + gap * 2) / 2), startY - bw + ((squareWidth + gap) * 2) + ((squareWidth * 2 + gap) / 2)});
    
    if(p1x == 10 && (p1y == 2 || p1y == 3)){
        borderSquare.setFillColor(sf::Color(255, 255, 0, 255));
        charSquare.setFillColor(sf::Color(255, 255, 0, 255));
        charLabel -> setFillColor(sf::Color::Black);
    }else{
        borderSquare.setFillColor(sf::Color(255, 255, 0, 255));
        charSquare.setFillColor(sf::Color(0, 0, 0, 255));
        charLabel -> setFillColor(sf::Color::Yellow);
    }

    p1window.draw(borderSquare);
    p1window.draw(charSquare);
    p1window.draw(*charLabel);



    //render the flashing caret when the player is typing
    if(!p1ready){
        float t = caretClock.getElapsedTime().asSeconds();
        if(t > 0.75f){
            caretVisible = !caretVisible;
            caretClock.restart();
        }
        if(caretVisible){
            borderSquare.setSize({12.0f, squareWidth + gap + (bw * 2)});
            //borderSquare.setOrigin(charLabel -> getLocalBounds().getCenter()); //where we set its position is the center of the string/char
            borderSquare.setPosition({startX + (96.0f * p1name.length()), startY - ((gap * 3) + squareWidth + (bw * 2) + 4.0f)});

            p1window.draw(borderSquare);
        }
    }

    //renering the instructions below the keyboard, change based on state
    float t = instructionsClock.getElapsedTime().asSeconds();
    if(t > 1.0f){
        instructionsVisible = !instructionsVisible;
        instructionsClock.restart();
    }
    if(instructionsVisible){
        if(ctx -> renderp2){
            //there is 2 players
            if(p1ready){
                if(p2ready) charLabel -> setString("Starting...");
                else charLabel -> setString("Waiting for other players.");
            }else{
                charLabel -> setString("PRESS 'READY' TO BEGIN");
            }
        }else{
            //then single
            if(p1ready){
                charLabel -> setString("Starting...");
            }else{
                charLabel -> setString("PRESS 'READY' TO BEGIN");
            }
        }

        charLabel -> setOrigin(charLabel -> getLocalBounds().getCenter()); //where we set its position is the center of the string/char
        charLabel -> setFillColor(sf::Color::Yellow);
        charLabel -> setPosition({width / 2.0f, height / 2 + ((1.0f / 2) * (6 * (squareWidth + gap)))});
        
        p1window.draw(*charLabel);
    }

    //Rendering Debug Keyboard coords
    // charLabel -> setOrigin({0.0f, 0.0f});
    // charLabel -> setString(std::to_string(p1x) + ", " + std::to_string(p1y));
    // charLabel -> setFillColor(sf::Color::Yellow);
    // charLabel -> setPosition({150.0f, 150.0f});
    // p1window.draw(*charLabel);
}

void NameEntryState::p2render(sf::RenderWindow& p2window) {
    //declare some helper vars
    float squareWidth = 80.0f;
    float gap = 19.0f;
    float bw = 4.0f;

    unsigned int width = p2window.getSize().x;
    unsigned int height = p2window.getSize().y;
    float startX = width / 2 - (1.0f / 2) * (13 * (squareWidth + gap) - gap);
    float startY = height / 2 - (1.0f / 2) * (4 * (squareWidth + gap) - gap);


    //the underline for the name entering
    borderSquare.setSize({(13 * (squareWidth + gap) - gap) + (bw * 2), 4.0f});
    borderSquare.setPosition({startX - bw, startY - (gap * 2)});
    p2window.draw(borderSquare);

    //the text for the name they are typing in
    charLabel -> setString(p2name);
    charLabel -> setCharacterSize(96);
    charLabel -> setOrigin({0.0f, charLabel -> getLocalBounds().size.y});
    charLabel -> setFillColor(sf::Color::Yellow);
    charLabel -> setPosition({startX, startY - ((gap * 3) + 2.0f)});
    p2window.draw(*charLabel);

    //rendering the 'keyboard'
    charLabel -> setCharacterSize(52);
    for(int i = 0; i < 10; i++){
        for(int j = 0; j < 4; j++){
            borderSquare.setSize({squareWidth + (bw * 2), squareWidth + (bw * 2)});
            borderSquare.setPosition({startX - bw + ((squareWidth + gap) * i), startY - bw + ((squareWidth + gap) * j)});
            
            charSquare.setSize({squareWidth, squareWidth});
            charSquare.setPosition({startX + ((squareWidth + gap) * i), startY + ((squareWidth + gap) * j)});

            charLabel -> setString(std::string(1, chars[j][i]));
            charLabel -> setOrigin(charLabel -> getLocalBounds().getCenter()); //where we set its position is the center of the string/char
            charLabel -> setFillColor(sf::Color::Yellow);
            charLabel -> setPosition({startX - bw + ((squareWidth + gap) * i) + (squareWidth / 2), startY - bw + ((squareWidth + gap) * j) + (squareWidth / 2)});

            if(p2x == i && p2y == j){
                //if the player is currently selecting this one
                borderSquare.setFillColor(sf::Color(255, 255, 0, 255));
                charSquare.setFillColor(sf::Color(255, 255, 0, 255));
                charLabel -> setFillColor(sf::Color::Black);
            }else{
                borderSquare.setFillColor(sf::Color(255, 255, 0, 255));
                charSquare.setFillColor(sf::Color(0, 0, 0, 255));
                charLabel -> setFillColor(sf::Color::Yellow);
            }

            p2window.draw(borderSquare);
            p2window.draw(charSquare);
            p2window.draw(*charLabel);
        }        
    }

    //Rendering the 'Ready Up' Button
    borderSquare.setSize({squareWidth * 3 + (bw * 2) + gap * 2, squareWidth * 2 + (bw * 2) + gap});
    borderSquare.setPosition({startX - bw + ((squareWidth + gap) * 10), startY - bw + ((squareWidth + gap) * 0)});
    
    charSquare.setSize({squareWidth * 3 + gap * 2, squareWidth * 2 + gap});
    charSquare.setPosition({startX + ((squareWidth + gap) * 10), startY + ((squareWidth + gap) * 0)});
    
    if(p2ready){
        charLabel -> setString("UNDO");
    }else{
        charLabel -> setString("READY");
    }
    
    charLabel -> setOrigin(charLabel -> getLocalBounds().getCenter()); //where we set its position is the center of the string/char
    charLabel -> setPosition({startX - bw + ((squareWidth + gap) * 10) + ((squareWidth * 3 + gap * 2) / 2), startY - bw + ((squareWidth + gap) * 0) + ((squareWidth * 2 + gap) / 2)});
    
    if(p2x == 10 && (p2y == 0 || p2y == 1)){
        borderSquare.setFillColor(sf::Color(255, 255, 0, 255));
        charSquare.setFillColor(sf::Color(255, 255, 0, 255));
        charLabel -> setFillColor(sf::Color::Black);
    }else{
        borderSquare.setFillColor(sf::Color(255, 255, 0, 255));
        charSquare.setFillColor(sf::Color(0, 0, 0, 255));
        charLabel -> setFillColor(sf::Color::Yellow);
    }

    p2window.draw(borderSquare);
    p2window.draw(charSquare);
    p2window.draw(*charLabel);



    //Rendering the 'BACK' Button
    borderSquare.setSize({squareWidth * 3 + (bw * 2) + gap * 2, squareWidth * 2 + (bw * 2) + gap});
    borderSquare.setPosition({startX - bw + ((squareWidth + gap) * 10), startY - bw + ((squareWidth + gap) * 2)});
    
    charSquare.setSize({squareWidth * 3 + gap * 2, squareWidth * 2 + gap});
    charSquare.setPosition({startX + ((squareWidth + gap) * 10), startY + ((squareWidth + gap) * 2)});
    
    charLabel -> setString("Back");
    charLabel -> setOrigin(charLabel -> getLocalBounds().getCenter()); //where we set its position is the center of the string/char
    charLabel -> setFillColor(sf::Color::Yellow);
    charLabel -> setPosition({startX - bw + ((squareWidth + gap) * 10) + ((squareWidth * 3 + gap * 2) / 2), startY - bw + ((squareWidth + gap) * 2) + ((squareWidth * 2 + gap) / 2)});
    
    if(p2x == 10 && (p2y == 2 || p2y == 3)){
        borderSquare.setFillColor(sf::Color(255, 255, 0, 255));
        charSquare.setFillColor(sf::Color(255, 255, 0, 255));
        charLabel -> setFillColor(sf::Color::Black);
    }else{
        borderSquare.setFillColor(sf::Color(255, 255, 0, 255));
        charSquare.setFillColor(sf::Color(0, 0, 0, 255));
        charLabel -> setFillColor(sf::Color::Yellow);
    }

    p2window.draw(borderSquare);
    p2window.draw(charSquare);
    p2window.draw(*charLabel);



    //render the flashing caret when the player is typing
    if(!p2ready){
        float t = caretClock.getElapsedTime().asSeconds();
        if(t > 0.75f){
            caretVisible = !caretVisible;
            caretClock.restart();
        }
        if(caretVisible){
            borderSquare.setSize({12.0f, squareWidth + gap + (bw * 2)});
            //borderSquare.setOrigin(charLabel -> getLocalBounds().getCenter()); //where we set its position is the center of the string/char
            borderSquare.setPosition({startX + (96.0f * p2name.length()), startY - ((gap * 3) + squareWidth + (bw * 2) + 4.0f)});

            p2window.draw(borderSquare);
        }
    }

    //renering the instructions below the keyboard, change based on state
    float t = instructionsClock.getElapsedTime().asSeconds();
    if(t > 1.0f){
        instructionsVisible = !instructionsVisible;
        instructionsClock.restart();
    }
    if(instructionsVisible){
        if(p2ready){
            charLabel -> setString("Waiting for other players.");
        }else{
            charLabel -> setString("PRESS 'READY' TO BEGIN");
        }
        charLabel -> setOrigin(charLabel -> getLocalBounds().getCenter()); //where we set its position is the center of the string/char
        charLabel -> setFillColor(sf::Color::Yellow);
        charLabel -> setPosition({width / 2.0f, height / 2 + ((1.0f / 2) * (6 * (squareWidth + gap)))});
        
        p2window.draw(*charLabel);
    }

    //Rendering Debug Keyboard coords
    charLabel -> setOrigin({0.0f, 0.0f});
    charLabel -> setString(std::to_string(p2x) + ", " + std::to_string(p2y));
    charLabel -> setFillColor(sf::Color::Yellow);
    charLabel -> setPosition({150.0f, 150.0f});
    p2window.draw(*charLabel);
}