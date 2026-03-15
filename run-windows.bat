@echo off
echo.

g++ src\Game.cpp src\AssetManager.cpp src\States\GameStateManager.cpp src\States\State.cpp src\States\MenuState.cpp src\States\AirHockeyGameState.cpp src\IO\KeyboardInput.cpp src\IO\InputManager.cpp src\KeyManager.cpp -ISFML\include -LSFML\lib -lsfml-graphics -lsfml-window -lsfml-system -o build\game.exe
build\game.exe

