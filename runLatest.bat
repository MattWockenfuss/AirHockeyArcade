@echo off
echo .

g++ Game.cpp AssetManager.cpp States\GameStateManager.cpp States\State.cpp States\MenuState.cpp States\AirHockeyGameState.cpp IO\KeyboardInput.cpp IO\InputManager.cpp KeyManager.cpp -ISFML\include -LSFML\lib -lsfml-graphics -lsfml-window -lsfml-system -o build\game.exe
build\game.exe

