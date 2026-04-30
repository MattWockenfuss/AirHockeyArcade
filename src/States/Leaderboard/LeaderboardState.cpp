#include "LeaderboardState.hpp"
#include <iostream>
#include <cmath>
#include <sstream>

#include "LeaderboardInterface.hpp"
#include "../../Context.hpp"
#include "../../AssetManager.hpp"
#include "../../KeyManager.hpp"
#include "../../IO/InputManager.hpp"
#include "../GameStateManager.hpp"

void LeaderboardState::init(Context* ctx){
    State::init(ctx);

    ctx -> leaderboard -> refreshRecordsList(6, true);

    /*
        okay so in order to do this, we want to list all records and be able to scroll if there is more.
        we want to be able to sort by game, player name, score and data, so all of the appropriate columns
        then render them all

        not hard, but a little tricky

        how can we have it scroll?
        well we can have a highlighted one? the first one is highlighted,
        then you can scroll down and highlight any game, if you make it to the bottom and go down, and there is more, then increase offset


    */

    record_text.emplace(ctx -> assets -> getFont("ArcadeNormal"), "yeet", 52);


    std::cout << "LeaderboardState Created!" << std::endl;


	const sf::FloatRect recordRect = record_text -> getLocalBounds();
	record_text -> setOrigin(recordRect.getCenter());
	record_text -> setPosition(sf::Vector2f(ctx -> p1window -> getSize().x / 2.0f, ctx -> p1window -> getSize().y / 3.0f));
}

void LeaderboardState::tick() {
    //okay so we have a GUI with an index value to move around
    
    //so we can do edge detection
    bool nowEnter = ctx -> input -> P1A;
    bool leftNow  = ctx -> input -> P1_Left;
    bool rightNow = ctx -> input -> P1_Right;
    bool upNow    = ctx -> input -> P1_Up;
    bool downNow  = ctx -> input -> P1_Down;

    /*
        okay so we have 2 situations
        we are either 7 or greater
            then we can only move up and down
        we are less than 7, we can move left, right, and down
    */

    if(index >= 7){
        //okay so we are on a leaderboard record. we can only move up and down, if we are at 7 and move up,
        //then go back to the cached index, otherwise just move up and down
        if(index == 7 && upNow && !prevUp){
            index = cacheIndex;
        }else if(upNow && !prevUp){
            index--;
        }else if(downNow && !prevDown){
            if(index - 7 + 1 < ctx -> leaderboard -> recordLength){
                index++;
            }
        }
    }else{
        //less than 7
        //if we move down, cache the current index, and set the index to 7
        if(downNow && !prevDown){
            cacheIndex = index;
            index = 7;
        }else if(rightNow && !prevRight){
            //only if index != 6,
            if(index < 6) index++;
        }else if(leftNow && !prevLeft){
            //only if index != 0,
            if(index > 1) index--;
        }
    }

    /*
        Okay so we just updated which index we are currently hovering over, now we want to adjust the
        render such that that index is in the middle of the screen. To do so, we will try and put 16
        entries before or 18 after.

        we have some offset variable, offset which is the number of rows not rendered.
        we are trying to determine when to update this offset. We can increase it, when we move down, and we are greater than index - 7 > 16


        if less than 34 total do nothing
        if index - 7 < 16, then do nothing, we are at the top
        if index + 18 > recordLength, then we are at the bottom, so stop their
    
    */

    if(downNow && !prevDown){
        if(index >= 16 && index + 18 < ctx -> leaderboard -> recordLength){
            //then we can move the offset up
            renderOffset += 1;
        }
    }else if(upNow && !prevUp){
        if(index >= 15 && index + 18 < ctx -> leaderboard -> recordLength){
            //then we can move the offset up
            if(renderOffset > 0) renderOffset -= 1;
        }
    }

    /*
        okay now if we hit enter on an index less than 7, we want to sort by that column, starting with ascending, but then descending
        no visual feedback for descending besides the list changing?

        okay so we want every column to flip descending if it was ascending and u hit it again, or if it descending and hit it it goes back to ascending

        how can we accomplish this goal. well we have a descending boolean that flips every time we hit enter on a column starting with false, and we keep track
        of the last hit column, it its different, reset it.



    */

    if(nowEnter && !prevEnter){
        if(index < 7){
            //this value is static, so it wont be forgotten when we leave the scope of the tick method,
            //essentially a shortcut to having a member variable for the sort order of each column.

            //okay so we just hit enter on a column, first, flip descending
            isDescending = !isDescending;
            //then if we are hitting a different column, set it to false
            if(index != lastSortedColumnIndex) isDescending = false;

            lastSortedColumnIndex = index;
            ctx -> leaderboard -> refreshRecordsList(index, isDescending);
        }
    }

    prevEnter = nowEnter;
    prevLeft = leftNow;
    prevRight = rightNow;
    prevUp = upNow;
    prevDown = downNow;
	
	// emergency game exit
	if(ctx->input->P1B && ctx->input->P1Y && ctx->input->P2B && ctx->input->P2X){ // player 1 pressed B and Y, and player 2 pressed B and X at the same time to quit
		ctx -> gsm -> requestStateChange(States::GameSelect, 3.0f, 1.5f);
	}
}





void LeaderboardState::p1render(sf::RenderWindow& p1window) {

    //okay to do the rendering, we need to render boxes for the sorting criteria
    //then render each record

    //maybe for rendering, each square is the same length?
    //split into 7ths

    float w = p1window.getSize().x / 8.0f;
    float h = 100.0f;
    float p = 4.0f;
    float bw = 6.0f;

    float row_spacing = 36.0f;

    //they are being rendered partially inside the box, kick them down a little
    float record_field_y_fix = 15.0f;

    //They are hugging the left side to close
    float record_field_x_offset = 10.0f;

    std::string words[] = {"P1", "P1 Score", "P2 Score", "P2", "Game", "Date"};

    //render the index for debugging
    // record_text -> setCharacterSize(28);
    // record_text -> setString(std::to_string(index) + ", " + std::to_string(renderOffset));
    // record_text -> setOrigin(record_text -> getLocalBounds().getCenter());
    // record_text -> setPosition({p1window.getSize().x - 100.0f, p1window.getSize().y - 100.0f});
    // record_text -> setFillColor(sf::Color::Yellow);
    // p1window.draw(*record_text);

    //render the side bars
    //left
    borderSquare.setSize({bw, p1window.getSize().y + 0.0f});
    borderSquare.setPosition({(w + p) * 1, 0.0f});
    borderSquare.setFillColor(sf::Color::Yellow);
    p1window.draw(borderSquare);

    //right
    borderSquare.setSize({bw, p1window.getSize().y + 0.0f});
    borderSquare.setPosition({(w + p) * 7 - p - bw, 0.0f});
    borderSquare.setFillColor(sf::Color::Yellow);
    p1window.draw(borderSquare);


    for(int i = 1; i <= 6; i++){
        //we draw the top square for all of them with the appropriate text
        borderSquare.setSize({w, h});
        borderSquare.setPosition({(w + p) * i, h});
        
        
        //render inside square
        insideSquare.setSize({w - (bw * 2), h - (bw * 2)});
        insideSquare.setPosition({(w + p) * i + bw, h + bw});

        
        //make the text is centered in the box
        record_text -> setCharacterSize(38);
        
        record_text -> setString(words[i - 1]);
        record_text -> setOrigin(record_text -> getLocalBounds().getCenter());
        record_text -> setPosition({(w + p) * i + (w / 2), h + (h / 2)});

        //okay, depending on index change render colors
        if(i == index){
            //then we are hovering over this one
            borderSquare.setFillColor(sf::Color::Yellow);
            insideSquare.setFillColor(sf::Color::Yellow);
            record_text -> setFillColor(sf::Color::Black);
        }else{
            borderSquare.setFillColor(sf::Color::Yellow);
            insideSquare.setFillColor(sf::Color::Black);
            record_text -> setFillColor(sf::Color::Yellow);
        }

        p1window.draw(borderSquare);
        p1window.draw(insideSquare);
        p1window.draw(*record_text);
    }

    //okay now render for each record, we want them to be left aligned?
    leaderboardrecord* curr = ctx -> leaderboard -> head;
    int curr_index = 0;
    while(curr != nullptr){
        /*
            okay we are looping through the entire linked list of records, we want to skip rendered the top amount of renderOffset records
            
            how do we do this? offset curr_index by renderOffset, it is already bounded.
            then skipp rendering if curr_index < renderOffset
        
        */
        if(curr_index < renderOffset){
            curr = curr -> next;
            curr_index++;
            continue;
        }


        if(curr_index == index - 7){
            //then we are hovering over this one
            borderSquare.setSize({(w * 6 + (p * 5)), row_spacing});
            borderSquare.setPosition({(w + p), record_field_y_fix + (h * 2) + ((curr_index - renderOffset) * row_spacing)});
            borderSquare.setFillColor(sf::Color::Yellow);
            record_text -> setFillColor(sf::Color::Black);

            p1window.draw(borderSquare);
        }else{
            //then we arent hovering hover this one
            borderSquare.setSize({0.0f, 0.0f});
            borderSquare.setFillColor(sf::Color::Blue);
            record_text -> setFillColor(sf::Color::Yellow);
        }

        
        //okay so for each record, render all 6 fields
        for(int i = 0; i < 6; i++){
            switch (i){
            case 0:
                record_text -> setString(curr -> p1name);
                break;
            case 1:
                record_text -> setString(std::to_string(curr -> p1score));
                break;
            case 2:
                record_text -> setString(std::to_string(curr -> p2score));
                break;
            case 3:
                record_text -> setString(curr -> p2name);
                break;
            case 4:
                record_text -> setString(GAMES.at(curr -> game_type));
                break;
            case 5:
                record_text -> setString(curr -> timestamp.substr(0, 10));
                break;
            }
            
            record_text -> setCharacterSize(28);

            if(i == 4 || i == 5){
                record_field_x_offset = 0.0f;
                record_text -> setOrigin({record_text -> getLocalBounds().getCenter().x, 0.0f});
                record_text -> setPosition({(w + p) * (i + 1) + (w / 2), record_field_y_fix + (h * 2) + ((curr_index - renderOffset) * row_spacing)});
            }else{
                record_field_x_offset = 10.0f;
                record_text -> setOrigin({0.0f, 0.0f});
                record_text -> setPosition({(w + p) * (i + 1) + bw + record_field_x_offset, record_field_y_fix + (h * 2) + ((curr_index - renderOffset) * row_spacing)});
            }
            //okay so we know what string, next render it
            p1window.draw(*record_text);
        }

        curr = curr -> next;
        curr_index++;
    }






    //render 1 for idlestate
    //p1window.draw(*record_text);
    
    //record_text -> setString(ctx -> leaderboard -> head -> p1name + " vs " + ctx -> leaderboard -> head -> p2name + " | " + std::to_string(ctx -> leaderboard -> head -> p1score) + " - " + std::to_string(ctx -> leaderboard -> head -> p2score) + " | " + ctx -> leaderboard -> head -> timestamp);
    //record_text -> setPosition(sf::Vector2f(ctx -> p1window -> getSize().x / 5.0f, ctx -> p1window -> getSize().y / 5.0f));
    //p1window.draw(*record_text);
}

void LeaderboardState::p2render(sf::RenderWindow& p2window) {
    //render 2 for idlestate
    p2window.draw(*record_text);
}