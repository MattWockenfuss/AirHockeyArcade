#pragma once

class Context;
class InputManager;

namespace sf {
    class RenderTexture;
}

class InputInterface {
    public:
        virtual ~InputInterface() = default;

        virtual void init(Context* ctx, InputManager* input) = 0;
        virtual void tick() = 0;
        virtual void render(sf::RenderTexture& window) = 0;

    protected:
        Context* ctx = nullptr;
        InputManager* input = nullptr;
};