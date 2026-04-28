#pragma once

#include <string>

namespace sf {
    class RenderWindow;
}

class AssetManager;
class KeyManager;
class GameStateManager;
class InputManager;
class AudioManager;
class LeaderboardInterface;



struct Context {
    std::string p1name = "";
    std::string p2name = "";
    bool renderp2 = false;
    sf::RenderWindow*   p1window = nullptr;
    sf::RenderWindow*   p2window = nullptr;
    AssetManager*       assets = nullptr;
    KeyManager*         keys = nullptr;
    GameStateManager*   gsm = nullptr;
    InputManager*       input = nullptr;
    AudioManager*       audio = nullptr;
    LeaderboardInterface* leaderboard = nullptr;

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


    Okay so leaderboard? Usernames? 

    How do we want to do that? sqlite3 database to store names and highscores? Or just a text file?

    when do we ask the players to enter their names? after their game? ask? Or head of their game? It would be really nice if they could enter their names at the same time?
    we could have a separate game state and do it at the beginning of the game.

    Maybe we could have a separate game state for the leaderboard

    Im thinking we have a main menu screen, next after that we have a name enter screen, then the game selection screen, where they can pick a game, view the leaderboard, or exit.
    If they exit they go back to the main menu, which will have them reenter their name upon contining.

    Those names are used for all of the players, and every score is entered to the leaderboard? maybe ever winner is entered in the database?



    I like this, lets work on the name enter game state, then the leaderboard game state.

*/