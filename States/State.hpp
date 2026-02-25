#pragma once

#include "../Context.hpp"

namespace sf {
    class RenderWindow;
}

class State {
    public:
        State(Context& ctx);

        virtual void tick() = 0;
        virtual void render(sf::RenderWindow& window) = 0;

        virtual ~State()  = default;
    protected:
        Context& ctx;
};