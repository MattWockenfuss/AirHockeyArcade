#!/bin/bash

echo "Building for PiOS WITH GPIO access"


g++ src/Game.cpp src/AssetManager.cpp src/States/State.cpp src/States/GameStateManager.cpp src/States/MenuState.cpp src/States/AirHockeyGameState.cpp src/KeyManager.cpp src/IO/InputManager.cpp src/IO/GPIOManager.cpp -lsfml-graphics -lsfml-window -lsfml-system -DHAVE_GPIOD -lgpiod -lgpiodcxx -o build/game
./build/game