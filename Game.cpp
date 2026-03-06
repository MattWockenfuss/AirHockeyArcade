#include <chrono>
#include "Game.hpp"





Game::Game(){
    ctx.window = &window;
    ctx.assets = &assetManager;
    ctx.keys = &keyManager;
    ctx.gpio = &GPIOmanager;
    ctx.audio = &audioManager;
    ctx.gsm = &gsm;
}

void Game::initialization(){
    ctx.gsm -> init(&ctx);
    ctx.gpio -> init(&ctx);




    //this function is called at the beginning of the game to create all of the objects that we will need
    std::vector<sf::VideoMode> modes = sf::VideoMode::getFullscreenModes();
	

	//Chooses largest resolution and highest bpp, makes window fullscreen
	window.create(modes[0], "Arcade", sf::State::Fullscreen);

    gsm.changeState(States::Menu);
}


void Game::start(){
    //called when we want to start the game
    running = true;
    run();
}
void Game::stop(){
    //called when we want to stop the game
    running = false;
    window.close(); //SFML will close down window and its resources needed
}

void Game::tick(){
    /*
        This loop processes all pending window events by repeatedly calling window.pollEvent(), which returns an std::optional containing 
        an event if one is available. For each event, it checks whether the event is of type sf::Event::Closed using the templated is<>()
        function. If the user has clicked the window’s close button, it sets running to false, which causes the main game loop to exit cleanly.

        window.pollEvent() returns a std::optional<sf::Event>, which means it may or may not contain an event. The loop only runs when the 
        optional actually contains an sf::Event, so inside the loop we know it is safe to access it. We use the dereference operator *event to 
        extract the actual sf::Event object from the optional so it can be passed to handleEvent.

        Thess comment was written by ChatGPT
    */

    while (const std::optional event = window.pollEvent()) {
        if (event -> is<sf::Event::Closed>()) {
            running = false;
        }

        keyManager.handleEvent(*event);

    }
    ctx.keys -> tick();

    gsm.getCurrentState() -> tick();
    GPIOmanager.tick();

    if(ctx.keys -> ESC){
        running = false;
    }

}
void Game::render(){
    window.clear();

    gsm.getCurrentState() -> render(window);
    GPIOmanager.render(window);

    window.display();
}

void Game::run(){
    /*
        This function runs the main game loop using a fixed timestep. Each iteration measures how much time has passed in nanoseconds, 
        accumulates that time, and calls tick() whenever enough time has built up for one update, ensuring the game logic runs at a 
        steady 60 updates per second. If an update occurs, it then calls render() to draw the current frame before continuing the loop 
        until the game stops.

        This comment was written by ChatGPT
    */

    initialization();

    int fps = 60;
    long long timePerTick = 1'000'000'000 / fps;   //16,666,666 or 16 million nanoseconds per tick
    long long delta = 0;

    std::chrono::steady_clock::time_point now;
    std::chrono::steady_clock::time_point lastTime = std::chrono::steady_clock::now();

    int ticks = 0;
    int frames = 0;
    bool shouldRender = false;

    while(running) {
        now = std::chrono::steady_clock::now();
        long long duration = std::chrono::duration_cast<std::chrono::nanoseconds>(now - lastTime).count();
        delta += duration;
        lastTime = now;

        if(delta >= timePerTick){
            //then tick and render
            ticks++;
            tick();
            shouldRender = true;
            delta -= timePerTick;
        }

        if(shouldRender){
            frames++;
            render();
            shouldRender = false;
        }
    }

    stop();
}






int main(){
    Game game;
    game.start();

    return 0;
}