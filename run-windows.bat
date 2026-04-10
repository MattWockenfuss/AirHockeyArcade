@echo off
echo.
::change directory to wherever this command is run from, even on a different drive, hence the /d
cd /d "%~dp0"

::	the ^ continues the command onto the next line
::	So essentially we are manually including all of the required .cpp files, then
::	static linking the 3 dlls from the compiler into the .exe, and linking SFML, in SFML\include and 
::	SFML\bin

::	This command only works on windows, a.k.a Matt and Briar's dev machines

if not exist build (
    mkdir build
)

g++ ^
src\Game.cpp ^
src\AssetManager.cpp ^
src\States\GameStateManager.cpp ^
src\States\State.cpp ^
src\States\IdleState.cpp ^
src\States\NameEntryState.cpp ^
src\States\GameSelect\GameSelectState.cpp ^
src\States\AirHockey\AirHockeyGameState.cpp ^
src\States\Tron\TronGameState.cpp ^
src\States\Tron\Tron.cpp ^
src\States\FruitNinja\FruitNinjaGameState.cpp ^
src\IO\KeyboardInput.cpp ^
src\IO\InputManager.cpp ^
src\KeyManager.cpp ^
-I"%~dp0SFML\include" ^
-L"%~dp0SFML\lib" ^
-static-libgcc ^
-static-libstdc++ ^
-Wl,-Bstatic ^
-lwinpthread ^
-Wl,-Bdynamic ^
-lsfml-graphics ^
-lsfml-window ^
-lsfml-system ^
-o build\game.exe

copy /Y SFML\bin\sfml-graphics-3.dll build\
copy /Y SFML\bin\sfml-window-3.dll build\
copy /Y SFML\bin\sfml-system-3.dll build\


build\game.exe

