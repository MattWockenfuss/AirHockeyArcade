#include <chrono>
#include "Game.hpp"





Game::Game(){
    ctx.p1window = &*p1Tex;
    ctx.p2window = &*p2Tex;
    ctx.assets = &assetManager;
    ctx.keys = &keyManager;
    ctx.input = &input;
    ctx.gsm = &gsm;
    ctx.audio = &audio;
    ctx.leaderboard = &leaderboardInterface;
}

void Game::initialization(){
    //added because we cant see the top of the monitor
    ctx.gsm -> init(&ctx);
    ctx.input -> init(&ctx);

    /*
        SFML 3.0.2 does NOT support multiple windows in fullscreen, we have to set them as borderless and
        position them manually. This is fine in the final build on the pi with a few manual tweaks, but
        for development on all of our different machines it will be a pain in the ass. It is also worth mentioning
        that input is handled for each window separately, not a problem for our project.

        If you wish to enable both windows to display, set the boolean to True
        This will create 2 separate windows, render both of them, with player2 being rendered exactly 1920 to the left
        This works on my pc, but idk about u guys

    */
    renderPlayer2 = true;
    ctx.renderp2 = renderPlayer2;

    //setup windows
    window1.create(sf::VideoMode({1920, 1080}), "p1", sf::Style::None);
    window2.create(sf::VideoMode({1920, 1080}), "p2", sf::Style::None);
    
    window1.setPosition({0, 0});
    window2.setPosition({1920, 0});

    window1.setFramerateLimit(60);
    window1.setVerticalSyncEnabled(false);

    window2.setFramerateLimit(60);
    window2.setVerticalSyncEnabled(false);



    p1Tex.emplace(sf::Vector2u{960, 540});
    p2Tex.emplace(sf::Vector2u{960, 540});

    p1Sprite.emplace(p1Tex -> getTexture());
    p2Sprite.emplace(p2Tex -> getTexture());

    p1Sprite -> setScale({2.0f, 2.0f});
    p2Sprite -> setScale({2.0f, 2.0f});
    p1Sprite -> setPosition({0.0f, 0.0f});
    p2Sprite -> setPosition({500.0f, 0.0f});
    

    std::cout << "Window1 Size: " << window1.getSize().x << ", " << window1.getSize().y << std::endl;
    std::cout << "Window1 Position: " << window1.getPosition().x << ", " << window1.getPosition().y << std::endl;

    auto b1 = p1Sprite -> getGlobalBounds();
    auto b2 = p2Sprite -> getGlobalBounds();

    std::cout << "p1 pos: " << b1.position.x << ", " << b1.position.y << std::endl;
    std::cout << "p1 size: " << b1.size.x << "x" << b1.size.y << std::endl;

    std::cout << "p2 pos: " << b2.position.x << ", " << b2.position.y << std::endl;
    std::cout << "p2 size: " << b2.size.x << "x" << b2.size.y << std::endl;

    //texture sizes
    std::cout << "p1Tex size: " << p1Tex -> getTexture().getSize().x << "x" << p1Tex -> getTexture().getSize().y << std::endl;
    std::cout << "p2Tex size: " << p2Tex -> getTexture().getSize().x << "x" << p2Tex -> getTexture().getSize().y << std::endl;

    tpsCounter.emplace(ctx.assets -> getFont("ArcadeNormal"), "", 10);
    tpsCounter -> setPosition({10.0f, 10.0f});
    tpsCounter -> setFillColor(sf::Color::Magenta);





    //open the leaderboard
    leaderboardInterface.openDB();
    leaderboardInterface.printTest();
    //Uncomment to remove all records on startup, used in testing
    //leaderboardInterface.removeAllRecords();

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
    window1.close();
    window2.close();

    //close the database interface
    leaderboardInterface.closeDB();
}

void Game::tick(){
    tpsCounter -> setString("ticks:  " + std::to_string(ticksLastSecond) + "\nframes: " + std::to_string(framesLastSecond));
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
    while (const auto p1 = window.pollEvent()) {
        if (p1 -> is<sf::Event::Closed>()) {
            running = false;
        }
        keyManager.handleEvent(*p1);
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
    std::cout << "Window Size: " << window.getSize().x << ", " << window.getSize().y << std::endl;
    std::cout << "Window Position: " << window.getPosition().x << ", " << window.getPosition().y << std::endl;

    std::cout << "view Size: " << window.getView().getSize().x << ", " << window.getView().getSize().y << std::endl;

    p1Tex -> clear(sf::Color::Red);
    p2Tex -> clear(sf::Color::Blue);
    

    // //render p1 stuff
    // if(gsm.getCurrentState() != nullptr) gsm.getCurrentState() -> p1render(*p1Tex);
    // gsm.p1render(*p1Tex);
    // input.render(*p1Tex);
    // if(renderFPSCounter) p1Tex -> draw(*tpsCounter);

    // //player 2
    // if(gsm.getCurrentState() != nullptr) gsm.getCurrentState() -> p2render(*p2Tex);
    // gsm.p2render(*p2Tex);
    // input.render(*p2Tex);
    // if(renderFPSCounter) p2Tex -> draw(*tpsCounter);

    p1Tex -> display();
    p2Tex -> display();

    p1Sprite -> setTexture(p1Tex -> getTexture(), true);
    p2Sprite -> setTexture(p2Tex -> getTexture(), true);

    p1Sprite -> setTextureRect(sf::IntRect({0, 0}, {960, 540}));
    p2Sprite -> setTextureRect(sf::IntRect({0, 0}, {960, 540}));

    p1Sprite -> setScale({2.0f, 2.0f});
    p2Sprite -> setScale({2.0f, 2.0f});

    p1Sprite -> setPosition({0.0f, 0.0f});
    p2Sprite -> setPosition({500.0f, 0.0f});

    

    window1.clear(sf::Color::Red);
    window2.clear(sf::Color::Blue);
    
    window1.draw(*p1Sprite);
    window2.draw(*p2Sprite);

    window1.display();
    window2.display();
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

    int nsSinceLastSecond = 0;


    std::chrono::steady_clock::time_point now;
    std::chrono::steady_clock::time_point lastTime = std::chrono::steady_clock::now();

    bool shouldRender = false;

    while(running) {
        now = std::chrono::steady_clock::now();
        long long duration = std::chrono::duration_cast<std::chrono::nanoseconds>(now - lastTime).count();
        delta += duration;
        nsSinceLastSecond += duration; //for keeping track of seconds
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

        if(nsSinceLastSecond >= 1'000'000'000){
            //then a second has occured
            ticksLastSecond = ticks;
            framesLastSecond = frames;

            ticks = 0;
            frames = 0;
            nsSinceLastSecond = 0;
        }

    }

    stop();
}






int main(){
    Game game;
    game.start();

    return 0;
}