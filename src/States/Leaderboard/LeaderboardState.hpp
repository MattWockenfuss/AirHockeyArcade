#pragma once

#include "../State.hpp"
#include <SFML/Graphics.hpp>
#include <chrono>
#include <optional>


class LeaderboardState : public State {
    public:
        void init(Context* ctx) override;

        bool prevUp = false, prevLeft = false, prevDown = false, prevRight = false, prevEnter = false;
        int index = 1;
        int cacheIndex = 1; //used to store the index when we move left and right, so we can return to it when we move up and down
        int renderOffset = 0; //the number of rows of the leaderboard table not rendered.

        int lastSortedColumnIndex = 1; //the index of the column we are currently sorted by
        bool isDescending = false; //whether we are currently sorted in descending order or not 

        void tick() override;
        void p1render(sf::RenderWindow& p1window) override;
        void p2render(sf::RenderWindow& p2window) override;
    private:
        std::optional<sf::Text> record_text;
        sf::RectangleShape borderSquare, insideSquare;
};