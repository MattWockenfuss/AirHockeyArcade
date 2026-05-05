#pragma once

#include "State.hpp"
#include <SFML/Graphics.hpp>
#include <chrono>
#include <optional>


class NameEntryState : public State {
    public:
        void init(Context* ctx) override;

        sf::Clock caretClock;
        sf::Clock instructionsClock;
        bool caretVisible = false;
        bool instructionsVisible = false;

        int p1x = 0;
        int p1y = 0;
        int p2x = 0;
        int p2y = 0;
        bool p1prevUp = false, p1prevLeft = false, p1prevDown = false, p1prevRight = false, p1prevEnter = false;
        bool p2prevUp = false, p2prevLeft = false, p2prevDown = false, p2prevRight = false, p2prevEnter = false;

        int p1_ticksPressingUp = 0;
        int p1_ticksPressingLeft = 0;
        int p1_ticksPressingDown = 0;
        int p1_ticksPressingRight = 0;
        
        int p2_ticksPressingUp = 0;
        int p2_ticksPressingLeft = 0;
        int p2_ticksPressingDown = 0;
        int p2_ticksPressingRight = 0;
        


        bool gameStartingFlag = false;
        std::string p1name = "";
        std::string p2name = "";

        bool p1ready = false;
        bool p2ready = false;



        int NAME_MAX_LENGTH = 5;
        char chars[4][10] = {
            {'1', '2', '3', '4', '5', '6', '7', '8', '9', '0'},
            {'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P'},
            {'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', '!'},
            {'Z', 'X', 'C', 'V', 'B', 'N', 'M', '@', '$', '*'}
        };

        void tick() override;
        void p1render(sf::RenderWindow& p1window) override;
        void p2render(sf::RenderWindow& p2window) override;
    private:
        float viewWidth = 0;
        float viewHeight = 0;
        long long sec;
        std::optional<sf::Text> title_text, play_text, seconds_counter;

        std::optional<sf::Text> charLabel;
        sf::RectangleShape charSquare, borderSquare;


        
};