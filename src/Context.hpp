#pragma once

namespace sf {
    class RenderWindow;
}

class AssetManager;
class KeyManager;
class GameStateManager;
class InputManager;



struct Context {
    sf::RenderWindow*   p1window = nullptr;
    sf::RenderWindow*   p2window = nullptr;
    AssetManager*       assets = nullptr;
    KeyManager*         keys = nullptr;
    GameStateManager*   gsm = nullptr;
    InputManager*       input = nullptr;

    //audio manager
    //etc
};

/*
Because #include doesn’t mean “the compiler now knows everything in that header forever.” It means “paste the contents of that file right here, 
then keep compiling.” With circular includes, the compiler can end up seeing an incomplete/empty version of a header at the moment it needs it.

Here’s what likely happened in your project:
    1. The compiler starts compiling some file that includes State.hpp.
    2. State.hpp says #include "../Context.hpp", so the compiler opens Context.hpp.
    3. Context.hpp (your old version) includes GameStateManager.hpp.
    4. GameStateManager.hpp includes State.hpp again.
    5. But State.hpp has #pragma once, so the second time it’s included, it is skipped completely to prevent infinite recursion.

Now you’re in a situation where:
    GameStateManager.hpp is being parsed,
    it expects State (or Context) to be defined,
    but the header that would define it was skipped due to #pragma once,
    and the compiler hasn’t reached the place where the type is fully defined yet.


*/