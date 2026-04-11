#include <chrono>
#include "Game.hpp"





Game::Game(){
    ctx.p1window = &p1window;
    ctx.p2window = &p2window;
    ctx.assets = &assetManager;
    ctx.keys = &keyManager;
    ctx.input = &input;
    ctx.gsm = &gsm;
    ctx.audio = &audio;
}

void Game::initialization(){
    ctx.gsm -> init(&ctx);
    ctx.input -> init(&ctx);

    /*
        SFML 3.0.2 does NOT support multiple windows in fullscreen, we have to set them as borderless and
        position them manually. This is fine in the final build on the pi with a few manual tweaks, but
        for development on all of our different machines it will be a pain in the ass. It is also worth mentioning
        that input is handling for each window separately, not a problem for our project.

        If you wish to enable both windows to display, set the boolean to True
        This will create 2 separate windows, render both of them, with player2 being rendered exactly 1920 to the left
        This works on my pc, but idk about u guys

    */
    renderPlayer2 = false;
    ctx.renderp2 = renderPlayer2;


    if(!renderPlayer2){
        std::vector<sf::VideoMode> modes = sf::VideoMode::getFullscreenModes();
        p1window.create(modes[0], "Arcade", sf::State::Fullscreen);
    }else{
        p1window.create(sf::VideoMode({1920, 1080}), "Player1", sf::Style::None);
        p2window.create(sf::VideoMode({1920, 1080}), "Player2", sf::Style::None);
        
        p1window.setPosition({0, 0});
        p2window.setPosition({-1920, 0});
    }




    gsm.requestStateChange(States::Idle, 0.0f, 1.5f);
}


void Game::start(){
    //called when we want to start the game
    running = true;
    run();
}
void Game::stop(){
    //called when we want to stop the game
    running = false;

    //SFML will close down window and its resources needed
    p1window.close();
    if(renderPlayer2) p2window.close();
}

void Game::tick(){
    /*
        This loop processes all pending window events by repeatedly calling window.pollEvent(), which returns an std::optional containing 
        an event if one is available. For each event, it checks whether the event is of type sf::Event::Closed using the templated is<>()
        function. If the user has clicked the window's close button, it sets running to false, which causes the main game loop to exit cleanly.

        window.pollEvent() returns a std::optional<sf::Event>, which means it may or may not contain an event. The loop only runs when the 
        optional actually contains an sf::Event, so inside the loop we know it is safe to access it. We use the dereference operator *event to 
        extract the actual sf::Event object from the optional so it can be passed to handleEvent.

        This comment was written by ChatGPT
    */

    ctx.keys -> tick();
    ctx.gsm -> tick();

    //process all of the events for both windows
    while (const auto p1 = p1window.pollEvent()) {
        if (p1 -> is<sf::Event::Closed>()) {
            running = false;
        }
        keyManager.handleEvent(*p1);
    }
    if(renderPlayer2){
        while (const auto p2 = p2window.pollEvent()) {
            if (p2 -> is<sf::Event::Closed>()) {
                running = false;
            }
            keyManager.handleEvent(*p2);
        }
    }


    input.tick();

    //handle state changes
	if(ctx.keys -> F2){
		ctx.gsm -> requestStateChange(States::NameEntry, 1.5f, 1.5f);
	}
    if(ctx.keys -> F3){
        ctx.gsm -> requestStateChange(States::Tron, 1.5f, 1.5f);
    }
    if(ctx.keys -> F4){
        ctx.gsm -> requestStateChange(States::AirHockey, 1.5f, 1.5f);
    }
	if(ctx.keys -> F5){
        ctx.gsm -> requestStateChange(States::FruitNinja, 1.5f, 1.5f);
    }
    if(ctx.keys -> F8){
        ctx.gsm -> requestStateChange(States::Pong, 1.5f, 1.5f);
    }
    
    if(gsm.getCurrentState() != nullptr) gsm.getCurrentState() -> tick();

    // if(gsm.pendingStateChange){
    //     gsm.changeState();
    //     gsm.pendingStateChange = false;
    // }

    if(ctx.keys -> ESC){
        running = false;
    }

}
void Game::render(){
    p1window.clear();
    if(renderPlayer2) p2window.clear();

    //render the current game state
    if(gsm.getCurrentState() != nullptr){
        gsm.getCurrentState() -> p1render(p1window);
        if(renderPlayer2) gsm.getCurrentState() -> p2render(p2window);
    }


    //render any transisitions if there are any
    gsm.p1render(p1window);
    if(renderPlayer2) gsm.p2render(p2window);

    //render the input manager
    input.render(p1window);
    if(renderPlayer2) input.render(p2window);

    p1window.display();
    if(renderPlayer2) p2window.display();
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