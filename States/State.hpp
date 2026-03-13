#pragma once

class Context;

namespace sf {
    class RenderWindow;
}

class State {
    public:
        virtual void init(Context* ctx) = 0;
        virtual void tick() = 0;
        virtual void render(sf::RenderWindow& window) = 0;

        virtual ~State()  = default;
    protected:
        Context* ctx;
};