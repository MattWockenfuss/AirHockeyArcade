#!/bin/bash

g++ Game.cpp AssetManager.cpp States/GameStateManager.cpp States/State.cpp States/MenuState.cpp States/AirHockeyGameState.cpp KeyManager.cpp GPIOManager.cpp -lsfml-graphics -lsfml-window -lsfml-system -lgpiod -lgpiodcxx -o build/game
./build/game