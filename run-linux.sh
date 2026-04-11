#!/bin/bash

echo "Building for Linux WITHOUT GPIO access"

mkdir -p build

g++ src/Game.cpp src/AssetManager.cpp src/AudioManager.cpp src/States/State.cpp src/States/GameStateManager.cpp src/States/NameEntryState.cpp src/States/GameSelect/GameSelectState.cpp src/States/IdleState.cpp src/States/AirHockey/AirHockeyGameState.cpp src/States/Tron/TronGameState.cpp src/States/Tron/Tron.cpp src/States/FruitNinja/FruitNinjaGameState.cpp src/States/Pong/PongGameState.cpp src/KeyManager.cpp src/IO/InputManager.cpp src/IO/KeyboardInput.cpp -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -lasound -o build/game
./build/game