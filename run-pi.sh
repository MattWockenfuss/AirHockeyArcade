#!/bin/bash

echo "Building for PiOS WITH GPIO access"


g++ Game.cpp AssetManager.cpp States/State.cpp States/GameStateManager.cpp States/MenuState.cpp States/AirHockeyGameState.cpp KeyManager.cpp IO/InputManager.cpp IO/GPIOManager.cpp -lsfml-graphics -lsfml-window -lsfml-system -DHAVE_GPIOD -lgpiod -lgpiodcxx -o build/game
./build/game