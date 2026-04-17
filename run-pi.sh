#!/bin/bash

echo "Building for PiOS WITH GPIO access updated!"


#compiling for the raspberry pi with GPIO access

mkdir -p build

g++ src/Game.cpp \
src/AssetManager.cpp \
src/States/State.cpp \
src/States/GameStateManager.cpp \
src/States/MenuState.cpp \
src/States/NameEntryState.cpp \
src/States/AirHockey/AirHockeyGameState.cpp \
src/States/Tron/TronGameState.cpp \
src/States/Tron/Tron.cpp \
src/States/Pong/PongGameState.cpp \
src/States/Leaderboard/LeaderboardState.cpp \
src/KeyManager.cpp \
src/AudioManager.cpp \
src/IO/InputManager.cpp \
src/IO/GPIOManager.cpp \
-lsfml-graphics \
-lsfml-window \
-lsfml-system \
-lsfml-audio \
-lasound \
-DHAVE_GPIOD \
-lgpiod \
-lgpiodcxx \
-o build/game


./build/game