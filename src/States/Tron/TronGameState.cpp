#include "TronGameState.hpp"
#include <iostream>
#include <cmath>
#include <sstream>

#include "../../Context.hpp"
#include "../../AssetManager.hpp"
#include "../../KeyManager.hpp"
#include "../../IO/InputManager.hpp"
#include "../GameStateManager.hpp"
#include "../Leaderboard/LeaderboardInterface.hpp"


void TronGameState::init(Context* ctx){
    State::init(ctx);

    play_text.emplace(ctx -> assets -> getFont("SquareSansSerif"), "Insert Coin to Play!", 35);
    seconds_counter.emplace(ctx -> assets -> getFont("ST-SimpleSquare"), "", 40);

    p1score.emplace(ctx -> assets -> getFont("ST-SimpleSquare"), "Player 1: 0", 26);
    p2score.emplace(ctx -> assets -> getFont("ST-SimpleSquare"), "Player 2: 0", 26);

    std::cout << "TronGameState Created!" << std::endl;

	const sf::FloatRect textRect = play_text -> getLocalBounds();
	play_text -> setOrigin(textRect.getCenter());

    
    seconds_counter -> setOrigin(seconds_counter -> getLocalBounds().getCenter());

    p1score -> setOrigin(p1score -> getLocalBounds().getCenter());
    p2score -> setOrigin(p2score -> getLocalBounds().getCenter());

    p1 = Tron(ctx, Direction::East, 1, 5, 57, 5, 57, p1body, p1head);
    p2 = Tron(ctx, Direction::West, 1, 154, 57, 154, 57, p2body, p2head, 400.0f);
}

void TronGameState::tick() {
    if(ctx -> window -> getView().getSize().x != viewWidth){
        //if the new x is not viewWidth, update the width and height and everything downstream
        viewWidth = ctx -> window -> getView().getSize().x;
        viewHeight = ctx -> window -> getView().getSize().y;

        play_text -> setPosition({viewWidth / 2.0f, viewHeight / 1.2f});
        seconds_counter -> setPosition({viewWidth / 2, 50.0f});
        p1score -> setPosition({110.0f, 30.0f});
        p2score -> setPosition({viewWidth - 110.0f, 30.0f});
    }
	//assume about 60 ticks per second, it can be slightly less, but good enough
	
	// emergency game exit
	if(ctx -> input -> P1B && 
        ctx -> input -> P1Y && 
        ctx -> input -> P2B && 
        ctx -> input -> P2X){ // player 1 pressed B and Y, and player 2 pressed B and X at the same time to quit
		ctx -> gsm -> requestStateChange(States::GameSelect, 3.0f, 1.5f);
	}

    /*
        if the game ends, as in there is a collision, we want to start a timer for 8 seconds
        over the first 4 seconds a window is moving down, displaying the winner, and final scores

        When game ends:
        (1) Stop moving the snakes, updating scores, and timer
        (2) Start another timer counting down from 8 seconds
        (3) Play a collision sound
        (4) Over the next 4 of total 8 seconds, shift final window into view
        (5) Add game records to leaderboard
        (6) After the total of 8 seconds, transfer to the next gamestate 

        so what do we need?
        a total seconds totalSeconds, that counts up from 0 to totalSeconds
        a window seconds, the first x amount of seconds that the window moves into frame
        timer: we start at 0 and count up
    
    */

    if(gameEnd){
        //okay so the gameEnd has triggered
        curr = std::chrono::steady_clock::now();
        std::chrono::duration elapsed = curr - gameEndStart;
        unsigned int ms = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count();
        

        // Linearly maps elapsed time (ms) from [0, windowMSThreshold]
        // to a vertical screen position from [-windowHeight, screenHeight/2]

        if(ms <= windowMSThreshold){
            windowYCoord = (ms / (float) windowMSThreshold) * (viewHeight / 2 - (-windowHeight)) - windowHeight;
        }
        //std::cout << "MS: " << ms << ", windowYCoord: " << windowYCoord << std::endl;

        //okay now depending on the ms, we want to pick a block to have slightly raised, giving a
        //quick and dirty animation
        /*
            0–19  -> 0
            20–39 -> 1
            40–59 -> 2
            60–79 -> 3
            80–99 -> 4
        */
        if(ms <= 8000){
            raisedSquare = (ms % 500) / 100;
        }else{
            raisedSquare = -1;
        }

        //the transfer back to game state and add scores to leaderboard at over totalSeconds
        if(ms >= totalMSThreshold){
            std::string player1name = ctx -> p1name;
            std::string player2name = ctx -> p2name;

            if(player1name == "") player1name = "COM";
            if(player2name == "") player2name = "COM";
            if(p1won)   ctx -> leaderboard -> addScore(player1name, player2name, p1.score, 0, 3);
            else        ctx -> leaderboard -> addScore(player1name, player2name, 0, p2.score, 3);
            ctx -> gsm -> requestStateChange(States::GameSelect, 0.0f, 1.5f);
        }
        return;
    }



    //get input for the trons
    if(ctx -> input -> P1_Up) p1.facing = Direction::North;
    if(ctx -> input -> P1_Down) p1.facing = Direction::South;
    if(ctx -> input -> P1_Left) p1.facing = Direction::West;
    if(ctx -> input -> P1_Right) p1.facing = Direction::East;

    if(ctx -> input -> P2_Up) p2.facing = Direction::North;
    if(ctx -> input -> P2_Down) p2.facing = Direction::South;
    if(ctx -> input -> P2_Left) p2.facing = Direction::West;
    if(ctx -> input -> P2_Right) p2.facing = Direction::East;




    tickCount++;
    if(tickCount >= 10){
        tickCount = 0;
        p1.move();
        
        //only check for collision after we move, performance improvement
        if(p1.checkForCollision(p2)){
            gameEnd = true;
            gameEndStart = std::chrono::steady_clock::now();
            p1won = false;
        }

        p2.move();
        if(p2.checkForCollision(p1)){
            gameEnd = true;
            gameEndStart = std::chrono::steady_clock::now();
            p1won = true;
        }

    }

    //okay so we process the moves
    //then check for collision
    //if we collide, then end the game, add scores to leaderboard, play a sound, and wait, and exit




    //set the score of the players
    //how should we calculate score? sole based on length? well
    /*
        How should we calculate score?
        it is a functon of length for the winner, and 0 for the loser?
        i like that
    */
    p1.score = p1.length;
    p2.score = p2.length;

    p1score -> setString(ctx -> p1name + ": " + std::to_string(p1.score));
    p2score -> setString(ctx -> p2name + ": " + std::to_string(p2.score));

    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::chrono::duration elapsed = end - start;
    unsigned int seconds = std::chrono::duration_cast<std::chrono::seconds>(elapsed).count();
    seconds_counter -> setString(std::to_string(seconds));
}

void TronGameState::renderArena(sf::RenderWindow& window){
    /*
        This function is used to render the common arena between the two
        different players screens. This is all of the gray blocks scattered about

        the gamewidth and height are 160 x 90 square units. Where each square is
        12.0f x 12.0f. The Top 23 are reserved for the score and name of the game,
        then we have 1 square wide borders. so the game is actually
        135 x 88 inside the playable area

        We render the borders between the 4 corners, 
        (0, 23), (89, 23), (89, 159), (0, 159)
    */

    //top wall
    square.setFillColor(darkgray);
    square.setSize({squareWidth * 160, squareWidth});
    square.setOrigin({0.0f, 0.0f});
    square.setPosition({0 * squareWidth, (23 * squareWidth)});
    window.draw(square);
    
    //left wall
    square.setFillColor(darkgray);
    square.setSize({squareWidth, squareWidth * 87});
    square.setOrigin({0.0f, 0.0f});
    square.setPosition({0 * squareWidth, (23 * squareWidth)});
    window.draw(square);

    //right wall
    square.setFillColor(darkgray);
    square.setSize({squareWidth, squareWidth * 87});
    square.setOrigin({0.0f, 0.0f});
    square.setPosition({159 * squareWidth, (23 * squareWidth)});
    window.draw(square);

    //bottom wall
    square.setFillColor(darkgray);
    square.setSize({squareWidth * 160, squareWidth});
    square.setOrigin({0.0f, 0.0f});
    square.setPosition({0 * squareWidth, (89 * squareWidth)});
    window.draw(square);
}
void TronGameState::renderGameEndWindow(sf::RenderWindow& window){
    /*
        This function draws the game end box on the screen at the specified
        y coordinate.
    */

    float centerX = viewWidth / 2;
    float centerY = windowYCoord;
    int bw = 3;
    square.setSize({windowWidth, windowHeight});
    square.setOrigin(square.getLocalBounds().getCenter());
    square.setPosition({centerX, centerY});
    square.setFillColor(darkgray);
    window.draw(square);

    square.setSize({windowWidth - (2 * bw), windowHeight - (2 * bw)});
    square.setOrigin(square.getLocalBounds().getCenter());
    square.setPosition({centerX, centerY});
    square.setFillColor(sf::Color::Black);
    window.draw(square);

    //alright so what do we want to render on the game end window?
    //who won? with their snake, and score
    //and the game length in seconds?

    //draw the winning snake and their name
    if(!p1won){
        /*
                This code renders a simple “player 2 wins” UI using SFML. It first draws a centered text message showing the winner’s name
            (“<p2name> Won!”), then renders a stylized horizontal snake made of squares (a head and body segments) positioned relative
            to the screen center, with one segment slightly raised for a subtle animation effect. Finally, it displays the player’s score
            in a larger font below the snake. All elements are aligned using origins centered on their bounds and positioned relative to the
            window’s midpoint for consistent layout.
        */
        play_text -> setString(ctx -> p2name + " Won!");
        play_text -> setCharacterSize(32);
        play_text -> setOrigin(play_text -> getLocalBounds().getCenter());
	    play_text -> setPosition({centerX, centerY - (windowHeight / 2) + 40.0f});
        play_text -> setFillColor(p2body);
        window.draw(*play_text);
        
        float sw = 25.0f;
        //then draw red snake
        square.setSize({sw, sw});
        square.setOrigin(square.getLocalBounds().getCenter());
        float yOffset = 90.0f;
        if(0 == raisedSquare) yOffset -= 5.0f;
        square.setPosition({centerX - (2.0f * sw), centerY - (windowHeight / 2) + yOffset});
        square.setFillColor(p2head);
        window.draw(square);

        square.setFillColor(p2body);
        for(int i = 1; i < 5; i++){
            float yOffset = 90.0f;
            if(i == raisedSquare) yOffset -= 5.0f;
            square.setPosition({centerX + ((i + 1) * sw) - (3.0f * sw), centerY - (windowHeight / 2) + yOffset});
            window.draw(square);
        }

        play_text -> setString(std::to_string(p2.score));
        play_text -> setCharacterSize(48);
        play_text -> setOrigin(play_text -> getLocalBounds().getCenter());
	    play_text -> setPosition({centerX, centerY - (windowHeight / 2) + 150.0f});
        play_text -> setFillColor(p2body);
        window.draw(*play_text);
        








    }else{
        //then draw green snake, p1won

        play_text -> setString(ctx -> p1name + " Won!");
        play_text -> setCharacterSize(32);
        play_text -> setOrigin(play_text -> getLocalBounds().getCenter());
	    play_text -> setPosition({centerX, centerY - (windowHeight / 2) + 40.0f});
        play_text -> setFillColor(p1body);
        window.draw(*play_text);
        
        float sw = 25.0f;
        //then draw red snake
        square.setSize({sw, sw});
        square.setOrigin(square.getLocalBounds().getCenter());
        float yOffset = 90.0f;
        if(0 == raisedSquare) yOffset -= 5.0f;
        square.setPosition({centerX - (2.0f * sw), centerY - (windowHeight / 2) + yOffset});
        square.setFillColor(p1head);
        window.draw(square);

        square.setFillColor(p1body);
        for(int i = 1; i < 5; i++){
            float yOffset = 90.0f;
            if(i == raisedSquare) yOffset -= 5.0f;
            square.setPosition({centerX + ((i + 1) * sw) - (3.0f * sw), centerY - (windowHeight / 2) + yOffset});
            window.draw(square);
        }

        play_text -> setString(std::to_string(p1.score));
        play_text -> setCharacterSize(48);
        play_text -> setOrigin(play_text -> getLocalBounds().getCenter());
	    play_text -> setPosition({centerX, centerY - (windowHeight / 2) + 150.0f});
        play_text -> setFillColor(p1body);
        window.draw(*play_text);
    }





}



void TronGameState::p1render(sf::RenderWindow& p1window) {
    //render for snake game state
    renderArena(p1window);

    p1.render(p1window);
    p2.render(p1window);

    p1window.draw(*seconds_counter);

    p1window.draw(*p1score);
    p1window.draw(*p2score);

    if(gameEnd) renderGameEndWindow(p1window);
}

void TronGameState::p2render(sf::RenderWindow& p2window) {
    //render for snake game state
    renderArena(p2window);

    p1.render(p2window);
    p2.render(p2window);

    p2window.draw(*seconds_counter);

    p2window.draw(*p1score);
    p2window.draw(*p2score);

    if(gameEnd) renderGameEndWindow(p2window);
}