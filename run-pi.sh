#!/bin/bash

echo "Building for PiOS WITH GPIO access updated!"


#compiling for the raspberry pi with GPIO access

mkdir -p build

gcc -c src/sqlite/sqlite3.c -o build/sqlite3.o

g++ \
src/Game.cpp \
src/AssetManager.cpp \
src/AudioManager.cpp \
src/IO/InputManager.cpp \
src/KeyManager.cpp \
src/IO/GPIOManager.cpp \
src/States/State.cpp \
src/States/GameStateManager.cpp \
src/States/IdleState.cpp \
src/States/NameEntryState.cpp \
src/States/GameSelect/GameSelectState.cpp \
src/States/AirHockey/AirHockeyGameState.cpp \
src/States/FruitNinja/FruitNinjaGameState.cpp \
src/States/Pong/PongGameState.cpp \
src/States/Tron/TronGameState.cpp \
src/States/Tron/Tron.cpp \
src/States/Leaderboard/LeaderboardState.cpp \
src/States/Leaderboard/LeaderboardInterface.cpp \
build/sqlite3.o \
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