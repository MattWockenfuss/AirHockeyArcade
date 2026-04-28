#!/bin/bash

echo "Building for Linux WITHOUT GPIO access"

mkdir -p build

gcc -c src/sqlite/sqlite3.c -o build/sqlite3.o

g++ \
src/Game.cpp \
src/AssetManager.cpp \
src/AudioManager.cpp \
src/IO/InputManager.cpp \
src/KeyManager.cpp \
src/IO/KeyboardInput.cpp \
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
-lsfml-graphics \
-lsfml-window \
-lsfml-system \
-lsfml-audio \
-lasound \
-o build/game
./build/game