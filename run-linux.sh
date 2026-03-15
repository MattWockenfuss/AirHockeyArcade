#!/bin/bash

echo "Building for Linux WITHOUT GPIO access"


g++ Game.cpp AssetManager.cpp States/State.cpp States/GameStateManager.cpp States/MenuState.cpp States/AirHockeyGameState.cpp KeyManager.cpp IO/InputManager.cpp IO/KeyboardInput.cpp -lsfml-graphics -lsfml-window -lsfml-system -o build/game
./build/game