#pragma once

#include "InputInterface.hpp"
#include <SFML/Graphics/RenderTexture.hpp>
#include <optional>

class KeyboardInput : public  InputInterface {
    public:
        void init(Context* ctx, InputManager* input) override;
        void tick() override;
        void render(sf::RenderTexture& window) override;
};