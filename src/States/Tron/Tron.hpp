#pragma once

#include <SFML/Graphics.hpp>
#include <optional>

class Context;

enum class Direction {
    North, South, East, West
};

Direction opposite(Direction d);

struct Joint {
    int x, y;
    Joint* next = nullptr;
};

struct Tron {
    public:
        
        int score = 0;
        sf::RectangleShape square;
        float squareWidth = 6.0f;

        Direction facing, lastFacing;
        int headX, headY;
        int tailX, tailY;
        int length;
        sf::Color color, headColor;
        bool growNextTick = true;

        Joint *front = nullptr, *back = nullptr;
        
        void move();
        void render(sf::RenderWindow& window);

        bool checkForCollision(Tron& otherSnake);

        Tron() = default;
        Tron(Context* ctx, Direction facing, int length, int headX, int headY, int tailX, int tailY, sf::Color color, sf::Color headColor, float yOffset = 0.0f);
    private:
        Context* ctx;
        void enqueue(int x, int y);
        void dequeue();
        Joint peek();

        void renderTronSegment(sf::RenderWindow& window, int tx, int ty, int ex, int ey);

        
        void grow(int lengthToGrow);
        float debugYOffset = 0.0f;
        std::optional<sf::Text> text;

};