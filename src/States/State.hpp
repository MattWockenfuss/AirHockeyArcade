#pragma once

class Context;

namespace sf {
    class RenderTexture;
}

class State {
    public:
        virtual void init(Context* ctx) = 0;
        virtual void tick() = 0;
        virtual void p1render(sf::RenderTexture& p1window) = 0;
        virtual void p2render(sf::RenderTexture& p2window) = 0;

        virtual ~State()  = default;
    protected:
        Context* ctx;
};