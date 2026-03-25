#!/bin/bash

echo "Building for Linux WITHOUT GPIO access"


g++ src/Game.cpp src/AssetManager.cpp src/States/State.cpp src/States/GameStateManager.cpp src/States/MenuState.cpp src/States/AirHockeyGameState.cpp src/States/Snake/SnakeGameState.cpp src/States/Snake/Snake.cpp src/KeyManager.cpp src/IO/InputManager.cpp src/IO/KeyboardInput.cpp -lsfml-graphics -lsfml-window -lsfml-system -o build/game
./build/game