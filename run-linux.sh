#!/bin/bash

echo "Building for Linux WITHOUT GPIO access"

mkdir -p build

g++ src/Game.cpp src/AssetManager.cpp src/States/State.cpp src/States/GameStateManager.cpp src/States/MenuState.cpp src/States/NameEntryState.cpp src/States/AirHockey/AirHockeyGameState.cpp src/States/Tron/TronGameState.cpp src/States/Tron/Tron.cpp src/KeyManager.cpp src/IO/InputManager.cpp src/IO/KeyboardInput.cpp -lsfml-graphics -lsfml-window -lsfml-system -o build/game
./build/game