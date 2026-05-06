#include "PongGameState.hpp"

#include "../../Context.hpp"
#include "../../AssetManager.hpp"
#include "../../AudioManager.hpp"
#include "../../KeyManager.hpp"
#include "../GameStateManager.hpp"
#include "../../IO/InputManager.hpp"
#include "../Leaderboard/LeaderboardInterface.hpp"


/*
    Shape funcs and defs
*/

//Initializes player paddle
void paddle_init(PongPaddle *player) {
    player->x = 20.0f;
    player->y = 150.0f;

    sf::RectangleShape rrect = sf::RectangleShape(sf::Vector2f(player->x, player->y));
    player->rect = rrect;
    sf::FloatRect f_rect = player->rect.getLocalBounds();
    player->rect.setOrigin(f_rect.getCenter());
}

//Initializes ball
void ball_init(Ball *ball) {
    ball->x = 0;
    ball->y = 0;
    ball->rad = 20.0f;
    ball->velx = 0;
    ball->vely = 0;

    sf::CircleShape circle = sf::CircleShape(ball->rad);

    ball->circ = circle;

    ball->circ.setOrigin(ball->circ.getLocalBounds().getCenter());
}

//Init Pong game
void PongGameState::init(Context *ctx) {
    State::init(ctx);

    //Load background for both screens
    bgTexture.emplace(ctx -> assets -> getAsset("pongbg"));
    background.emplace(sf::Sprite(bgTexture.value()));

    std::string play1 = ctx -> p1name;
    std::string play2 = ctx -> p2name;

    if (play1 != "") {
        player1.emplace(ctx -> assets -> getFont("ST-SimpleSquare"), play1 + ": ", 40);
        win1.emplace(ctx -> assets -> getFont("ST-SimpleSquare"), "GAME OVER\n" + play1 + " wins!", 100);
    }
    else {
        player1.emplace(ctx -> assets -> getFont("ST-SimpleSquare"), "Player 1: ", 40);

        win1.emplace(ctx -> assets -> getFont("ST-SimpleSquare"), "GAME OVER\nPlayer 1 wins!", 100);
    }
    if (play2 != "") {
        player2.emplace(ctx -> assets -> getFont("ST-SimpleSquare"), play2 + ": ", 40);

        win2.emplace(ctx -> assets -> getFont("ST-SimpleSquare"), "GAME OVER\n" + play2 + " wins!", 100);
    }
    else {
        player2.emplace(ctx -> assets -> getFont("ST-SimpleSquare"), "COM: ", 40);

        win2.emplace(ctx -> assets -> getFont("ST-SimpleSquare"), "GAME OVER\nCOM wins!", 100);
    }

    title_text.emplace(ctx -> assets -> getFont("ST-SimpleSquare"), "Pong", 55);
    seconds_counter.emplace(ctx -> assets -> getFont("ST-SimpleSquare"), "", 40);
    countdown_timer.emplace(ctx -> assets -> getFont("ST-SimpleSquare"), "5", 100); 
    goal_text.emplace(ctx -> assets -> getFont("ST-SimpleSquare"), "GOAL!", 75);

    std::cout << "PongGameState Created!" << std::endl;

    score1 = 0;
    score2 = 0;

    player1 -> setString(player1 -> getString() + std::to_string(score1));
    player2 -> setString(player2 -> getString() + std::to_string(score2));

	const sf::FloatRect titleRect = title_text -> getLocalBounds();
	title_text -> setOrigin(titleRect.getCenter());
	title_text -> setPosition(sf::Vector2f(ctx -> p1window -> getView().getSize().x / 2.0f, padding));
    
    const sf::FloatRect secondsRect = seconds_counter -> getLocalBounds();
    seconds_counter -> setOrigin(secondsRect.getCenter());
    seconds_counter -> setPosition(sf::Vector2f(ctx -> p1window -> getView().getSize().x - 150.0f, 50.0f));

    const sf::FloatRect countdownRect = countdown_timer -> getLocalBounds();
    countdown_timer -> setOrigin(countdownRect.getCenter());
    countdown_timer -> setPosition(sf::Vector2f(ctx -> p1window -> getView().getSize().x / 2.0f, ctx -> p1window -> getView().getSize().y / 2.0f));
    countdown_timer -> setFillColor(sf::Color::Cyan);

    const sf::FloatRect p1Rect = player1 -> getLocalBounds();
	player1 -> setOrigin(p1Rect.getCenter());
	player1 -> setPosition(sf::Vector2f(ctx -> p1window -> getView().getSize().x / 4.5f, padding));
    player1 -> setFillColor(sf::Color::Magenta);
    
    const sf::FloatRect p2Rect = player2 -> getLocalBounds();
	player2 -> setOrigin(p2Rect.getCenter());
	player2 -> setPosition(sf::Vector2f(ctx -> p1window -> getView().getSize().x / 1.25f, padding));
    player2 -> setFillColor(sf::Color::Magenta);

    const sf::FloatRect wint1 = win1 -> getLocalBounds();
    win1 -> setOrigin(wint1.getCenter());
    win1 -> setPosition(sf::Vector2f(ctx -> p1window -> getView().getSize().x / 2.0f, ctx -> p1window -> getView().getSize().y / 2.0f));
    win1 -> setFillColor(sf::Color::Green);

    const sf::FloatRect wint2 = win2 -> getLocalBounds();
    win2 -> setOrigin(wint2.getCenter());
    win2 -> setPosition(sf::Vector2f(ctx -> p1window -> getView().getSize().x / 2.0f, ctx -> p1window -> getView().getSize().y / 2.0f));
    win2 -> setFillColor(sf::Color::Green);

    const sf::FloatRect goalRect = goal_text -> getLocalBounds();
	goal_text -> setOrigin(goalRect.getCenter());
	goal_text -> setPosition(sf::Vector2f(ctx -> p1window -> getView().getSize().x / 2.0f, ctx -> p1window -> getView().getSize().y / 4.0f));
    goal_text -> setFillColor(sf::Color::Blue);
    rendergoal = false;

    winSize = ctx -> p1window -> getView().getSize();

    //To help reduce screen tearing caused by game/monitor refresh rate mismatch, set vsync to true
    //ctx -> p1window -> setVerticalSyncEnabled(true);

    //Pass game object pointers to their init helper functions
    paddle_init(p1);
    paddle_init(p2);
    ball_init(b);

    //Left and right paddle player
    p1->rect.setPosition(sf::Vector2f(padding, winSize.y/2.0f));
    p2->rect.setPosition(sf::Vector2f((winSize.x - padding), winSize.y/2.0f));

    //Ball
    b->circ.setPosition(sf::Vector2f(winSize.x/2.0f, winSize.y/2.0f));
    b->velx = 10.0f;
    b->vely = -10.0f;

    std::cout << "PongGameState Initialized!" << std::endl;

    //False indicates that the match is in (countdown) phase
    matchphase = false;
    initial = true;
    gameover = false;
    match_start(0);
}

void PongGameState::tick() {

    /*
        In the tick method, we have a lot of physics and input calculations here.
        Especially if the ball is going pretty quick, we can get some errors if during one tick step
        the ball overlaps with the paddles or wall.

        For this method I am going to use sub-stepping along with ticks to help increase physics accuracy (only about 4 sub-steps are necessary)
        Luckily this game is pretty simple, but keep in mind this does increase computation dramatically the more sub steps there are.
    */

    //Sub steps is defined as a priv variable with unsigned short int as its data type.
    //By default, it is set to 128 sub steps, but we can change this in the future if need be.

    unsigned short int i = 0;
    tickCount++;
    if(tickCount >= 2){

        tickCount = 0;

        if (matchphase) {

            //Increase ball velocity over time
            if ((b->velx <= 50.0f && b->vely <= 50.0f) && (b->velx >= -50.0f && b->vely >= -50.0f)) {
                int val = 0;
                if (b->velx < 0) val = b->velx * -1;
                else val = b->velx;

                float ratechange = ((1.0f/64.0f) * powf(val, 0.25f));

                // std::cout << "Rate change: " << ratechange << std::endl;
                // std::cout << "val: " << val << std::endl;

                if (b->velx < 0) {
                    //Make sure value remains negative 
                    b->velx -= ratechange;
                }
                else {
                    b->velx += ratechange;
                }

                if (b->vely < 0) {
                    //Make sure value remains negative
                    b->vely -= ratechange;
                }
                else {
                    b->vely += ratechange;
                }
            }

            // std::cout << "Velx: " << b->velx << std::endl;
            // std::cout << "Vely: " << b->vely << std::endl;

            for(i = 0; i < sub_steps; i++) {
                //Move player based on input
                if (ctx -> input -> P1_Up) {
                    p1->rect.setPosition(sf::Vector2f(p1->rect.getPosition().x, p1->rect.getPosition().y - 40.0f/sub_steps));
                }
                else if (ctx -> input -> P1_Down) {
                    p1->rect.setPosition(sf::Vector2f(p1->rect.getPosition().x, p1->rect.getPosition().y + 40.0f/sub_steps));
                }
                if (ctx -> input -> P2_Up) {
                    p2->rect.setPosition(sf::Vector2f(p2->rect.getPosition().x, p2->rect.getPosition().y - 40.0f/sub_steps));
                }
                else if (ctx -> input -> P2_Down) {
                    p2->rect.setPosition(sf::Vector2f(p2->rect.getPosition().x, p2->rect.getPosition().y + 40.0f/sub_steps));
                }
                // emergency game exit
                if(ctx->input->P1B && ctx->input->P1Y && ctx->input->P2B && ctx->input->P2X){ // player 1 pressed B and Y, and player 2 pressed B and X at the same time to quit
                    ctx -> gsm -> requestStateChange(States::GameSelect, 3.0f, 1.5f);
                }
                
                //Move ball
                b->circ.setPosition(sf::Vector2f(b->circ.getPosition().x + b->velx/sub_steps, b->circ.getPosition().y + b->vely/sub_steps));

                //Check for and solve collisions
                collision();
            }
        }
        else {
            for(i = 0; i < sub_steps; i++) {
                //Move player based on input
                if (ctx -> input -> P1_Up) {
                    p1->rect.setPosition(sf::Vector2f(p1->rect.getPosition().x, p1->rect.getPosition().y - 40.0f/sub_steps));
                }
                else if (ctx -> input -> P1_Down) {
                    p1->rect.setPosition(sf::Vector2f(p1->rect.getPosition().x, p1->rect.getPosition().y + 40.0f/sub_steps));
                }
                if (ctx -> input -> P2_Up) {
                    p2->rect.setPosition(sf::Vector2f(p2->rect.getPosition().x, p2->rect.getPosition().y - 40.0f/sub_steps));
                }
                else if (ctx -> input -> P2_Down) {
                    p2->rect.setPosition(sf::Vector2f(p2->rect.getPosition().x, p2->rect.getPosition().y + 40.0f/sub_steps));
                }
                // emergency game exit
                if(ctx->input->P1B && ctx->input->P1Y && ctx->input->P2B && ctx->input->P2X){ // player 1 pressed B and Y, and player 2 pressed B and X at the same time to quit
                    ctx -> gsm -> requestStateChange(States::GameSelect, 3.0f, 1.5f);
                }

                collision();
            }

            time_remaining = std::chrono::duration_cast<std::chrono::seconds>(countdown-std::chrono::steady_clock::now());

            if (std::to_string(time_remaining.count()) != "0") {
                countdown_timer -> setString(std::to_string(time_remaining.count()));
            }
            else if (!gameover) {
                rendergoal = false;
                initial = false;
                matchphase = true;
                countdown_timer -> setString("Go!");
            }
            else {
                //then the game is over, add scores to the leaderboard
                std::string player1name = ctx -> p1name;
                std::string player2name = ctx -> p2name;

                if(player1name == "") player1name = "COM";
                if(player2name == "") player2name = "COM";

                ctx -> leaderboard -> addScore(player1name, player2name, score1, score2, 1);
                ctx -> gsm -> requestStateChange(States::GameSelect, 0.0f, 1.5f);
            }

            const sf::FloatRect countdownRect = countdown_timer -> getLocalBounds();
            countdown_timer -> setOrigin(countdownRect.getCenter());
            countdown_timer -> setPosition(sf::Vector2f(ctx -> p1window -> getView().getSize().x / 2.0f, ctx -> p1window -> getView().getSize().y / 2.0f));
        }
    }
}

void PongGameState::p1render(sf::RenderTexture &window) {
    window.clear();
    window.draw(background.value());
    window.draw(title_text.value());
    window.draw(player1.value());
    window.draw(player2.value());
    window.draw(p1->rect);
    window.draw(p2->rect);
    window.draw(b->circ);
    if (rendergoal) {
       window.draw(goal_text.value());
       window.draw(countdown_timer.value());
    }
    else if (initial) {
       window.draw(countdown_timer.value());
    }
    if (gameover) {
        long long tr = std::chrono::duration_cast<std::chrono::seconds>(time_remaining).count();
        //Draw win text
        if (static_cast<unsigned int>(tr) % 2 == 0) {
            if (score1 == 10) {
                window.draw(win1.value());
            }
            else if (score2 == 10) {
                window.draw(win2.value());
            }
        }
    }
}

void PongGameState::p2render(sf::RenderTexture &window) {
    window.clear();
    window.draw(background.value());
    window.draw(title_text.value());
    window.draw(player1.value());
    window.draw(player2.value());
    window.draw(p1->rect);
    window.draw(p2->rect);
    window.draw(b->circ);
    if (rendergoal) {
        window.draw(goal_text.value());
        window.draw(countdown_timer.value());
    }
    else if (initial) {
       window.draw(countdown_timer.value());
    }
    if (gameover) {
        long long tr = std::chrono::duration_cast<std::chrono::seconds>(time_remaining).count();
        //Draw win text
        if (static_cast<unsigned int>(tr) % 2 == 0) {
            if (score1 == 10) {
                window.draw(win1.value());
            }
            else if (score2 == 10) {
                window.draw(win2.value());
            }
        }
    }
}

//Handles all collision calculations for paddles and ball
void PongGameState::collision() {
    /*
        A lot is going on here, this will just be a general breakdown of the steps.

        First and foremost, we check to see the paddle's collisions. Naturally, we wanna see if the
        paddle is hitting the top or bottom of the screen. The goal is to not have jittery or weird movement.

        Next is the ball, which takes up the majority of the collision calculations here.

        The least costly is the top and bottom wall, which is checked first. The velocity simply gets reflected
        by -1.

        Afterwards, we check to see if the ball is colliding with the paddle's at all.

        The hardest part is checking along the radius of the ball against the paddle objects. I solved this
        by using a nested if conditional. If the ball's radius is in between the paddle's x-width, then
        we know that there's a potential collision happening. If the ball's radius y is in between the paddle's
        height, then we KNOW that, in fact, the ball is colliding with the paddle.

        The advantage of this is that the ball's outline radius will trigger collision all along the outside,
        including with the very edges of the paddle. The one thing here is that this is a noted edge case.
        Primarily, this is due to the fact that steps are not perfect, and the ball COULD slightly overlap
        with the paddle on its, causing undefined behavior.

        Lastly, the last possible collision case is if the ball touches the left or right wall.
        In that event, the opposing side's player scores in that case, then we trigger the goal score method.
    */

    //Bottom of screen
    if (p1->rect.getPosition().y + p1->rect.getSize().y/2 >= winSize.y) {
        p1->rect.setPosition(sf::Vector2f(p1->rect.getPosition().x, winSize.y - p1->rect.getSize().y/2));
    }
    if (p2->rect.getPosition().y + p2->rect.getSize().y/2 >= winSize.y) {
        p2->rect.setPosition(sf::Vector2f(p2->rect.getPosition().x, winSize.y - p2->rect.getSize().y/2));
    }

    //Top of screen
    if (p1->rect.getPosition().y - p1->rect.getSize().y/2 <= 0.0f) {
        p1->rect.setPosition(sf::Vector2f(p1->rect.getPosition().x, 0.0f + p1->rect.getSize().y/2));
    }
    if (p2->rect.getPosition().y - p2->rect.getSize().y/2 <= 0.0f) {
        p2->rect.setPosition(sf::Vector2f(p2->rect.getPosition().x, 0.0f + p2->rect.getSize().y/2));
    }

    //Ball top and bottom wall
    if (b->circ.getPosition().y - b->rad <= 0.0f) {
        b -> circ.setPosition(sf::Vector2f(b->circ.getPosition().x, 0.0f + b->rad));
        ctx -> audio -> playSound(ctx->assets->getSound("Bounce"));
        b->vely *= -1;
    }
    else if (b->circ.getPosition().y + b->rad >= winSize.y) {
        b -> circ.setPosition(sf::Vector2f(b->circ.getPosition().x, winSize.y - b->rad));
        ctx -> audio -> playSound(ctx->assets->getSound("Bounce"));
        b->vely *= -1;
    }

    //Check intersect between ball and paddles
    if (p1->rect.getPosition().x + p1->rect.getSize().x/2 >= b->circ.getPosition().x - b->rad
     && p1->rect.getPosition().x - p1->rect.getSize().x/2 <= b->circ.getPosition().x + b->rad) {
        //If true, then ball is within x-range of paddle 1, potential collision then?

        // std::cout << "P1x: " << p1->rect.getPosition().x << std::endl;
        // std::cout << "P1Uy: " << p1->rect.getPosition().y - p1->rect.getSize().y/2 << std::endl;
        // std::cout << "P1Ly: " << p1->rect.getPosition().y + p1->rect.getSize().y/2 << std::endl;
        // std::cout << "Bx: " << b->circ.getPosition().x << std::endl;
        // std::cout << "By: " << b->circ.getPosition().y << std::endl;

        if (p1->rect.getPosition().y - p1->rect.getSize().y/2 < b->circ.getPosition().y + b->rad &&
            p1->rect.getPosition().y + p1->rect.getSize().y/2 > b->circ.getPosition().y - b->rad) {
            //Ball is within y-range of paddle 1, we have a collision!
            // std::cout << "Paddle 1 collision!" << std::endl;

            //Check for edge cases! That is...if ball hits directly on the top or bottom of the paddle
            if (p1->rect.getPosition().y - p1->rect.getSize().y/2 == b->circ.getPosition().y + b->rad ||
                p1->rect.getPosition().y + p1->rect.getSize().y/2 == b->circ.getPosition().y - b->rad) {
                //If so...reflect y velocity too, or else it'll get stuck inside the paddle

                //Also, it's just gg at that point

                ctx -> audio -> playSound(ctx->assets->getSound("Hit"));
                b->vely *= -1;
                return;
            }

            //We have one more collision case that's possible when it comes to ball and paddle, that is
            //if they collide on their corners. To prevent them from getting stuck inside each other this
            //way, we must reflect both vectors

            //So, if the TOP of the rectangle is GREATER than the CENTER of the ball (meaning farther down)
            //then we know that they must be intersecting at the corner! And vice versa
            if (p1->rect.getPosition().y - p1->rect.getSize().y/2 >= b->circ.getPosition().y ||
                p1->rect.getPosition().y + p1->rect.getSize().y/2 <= b->circ.getPosition().y) {
                    b->velx *= -1;
                    b->vely *= -1;
                    ctx -> audio -> playSound(ctx->assets->getSound("Hit"));

                    return;
            }

            //Reflect x velocity, and then return, as no other collisions are possible right now.
            ctx -> audio -> playSound(ctx->assets->getSound("Hit"));

            //I am TIRED of the ball getting stuck in the paddles, so I'm making the ball snap to the edge
            b->circ.setPosition(sf::Vector2f(((b->circ.getPosition().x + b->rad) + ((p1->rect.getPosition().x + p1->rect.getSize().x/2)-(b->circ.getPosition().x-b->rad))), b->circ.getPosition().y));
            b->velx *= -1;
            return;
        }
    }
    else if (p2->rect.getPosition().x - p2->rect.getSize().x/2 <= b->circ.getPosition().x + b->rad 
        && p2->rect.getPosition().x + p2->rect.getSize().x/2 >= b->circ.getPosition().x - b->rad) {
        //If true, then ball is within x-range of paddle 2, potential collision then?

        // std::cout << "P2x: " << p2->rect.getPosition().x << std::endl;
        // std::cout << "P2Uy: " << p2->rect.getPosition().y - p2->rect.getSize().y/2 << std::endl;
        // std::cout << "P2Ly: " << p2->rect.getPosition().y + p2->rect.getSize().y/2 << std::endl;
        // std::cout << "Bx: " << b->circ.getPosition().x << std::endl;
        // std::cout << "By: " << b->circ.getPosition().y << std::endl;

        if (p2->rect.getPosition().y - p2->rect.getSize().y/2 < b->circ.getPosition().y + b->rad &&
            p2->rect.getPosition().y + p2->rect.getSize().y/2 > b->circ.getPosition().y - b->rad) {
            //Ball is within y-range of paddle 2, we have a collision!
            // std::cout << "Paddle 2 collision!" << std::endl;

            //Check for edge case! That is...if ball hits directly on the top or bottom of the paddle
            if (p2->rect.getPosition().y - p2->rect.getSize().y/2 == b->circ.getPosition().y + b->rad ||
                p2->rect.getPosition().y + p2->rect.getSize().y/2 == b->circ.getPosition().y - b->rad) {
                //If so...reflect y velocity too, or else it'll get stuck inside the paddle

                //Also it's just gg at that point
                ctx -> audio -> playSound(ctx->assets->getSound("Hit"));
                b->vely *= -1;
                return;
            }

            //We have one more collision case that's possible when it comes to ball and paddle, that is
            //if they collide on their corners. To prevent them from getting stuck inside each other this
            //way, we must reflect both vectors

            //So, if the TOP of the rectangle is GREATER than the CENTER of the ball (meaning farther down)
            //then we know that they must be intersecting at the corner! And vice versa
            if (p2->rect.getPosition().y - p2->rect.getSize().y/2 >= b->circ.getPosition().y ||
                p2->rect.getPosition().y + p2->rect.getSize().y/2 <= b->circ.getPosition().y) {
                    ctx -> audio -> playSound(ctx->assets->getSound("Hit"));
                    b->velx *= -1;
                    b->vely *= -1;
                    return;
            }
            
            //Reflect x velocity, and then return, as no other collisions are possible right now.
            ctx -> audio -> playSound(ctx->assets->getSound("Hit"));

            //I am TIRED of the ball getting stuck in the paddles, so I'm making the ball snap to the edge
            b->circ.setPosition(sf::Vector2f(((b->circ.getPosition().x - b->rad) - ((b->circ.getPosition().x + b->rad)-(p2->rect.getPosition().x - p2->rect.getSize().x/2))), b->circ.getPosition().y));
            b->velx *= -1;
            return;
        }
    }

    //Ball left and right wall collisions, this will trigger the scoring mechanism.
    if (b->circ.getPosition().x - b->rad <= 0.0f) {
        p2_score();
    }
    else if (b->circ.getPosition().x + b->rad >= winSize.x) {
        p1_score();
    }
}

//Triggers when player1 (left side) scores
void PongGameState::p1_score() {
    //Reset ball velocity
    b->velx = 10.0f;
    b->vely = 10.0f;

    rendergoal = true;
    score1 += 1;
    player1 -> setString(player1 -> getString().substring(0, player1 -> getString().getSize()-1) + std::to_string(score1));
    if (score1 == 10) p1_win();
    else match_start(2);
}

//Triggers when player2 (right side) scores
void PongGameState::p2_score() {
    //Reset ball velocity
    b->velx = 10.0f;
    b->vely = 10.0f;

    rendergoal = true;
    score2 += 1;
    player2 -> setString(player2 -> getString().substring(0, player2 -> getString().getSize()-1) + std::to_string(score2));
    if (score2 == 10) p2_win();
    else match_start(1);
}

void PongGameState::p1_win() {
    gameover = true;

    p1->rect.setPosition(sf::Vector2f(padding, winSize.y/2.0f));
    p2->rect.setPosition(sf::Vector2f((winSize.x - padding), winSize.y/2.0f));
    b->circ.setPosition(sf::Vector2f(winSize.x/2.0f, winSize.y/2.0f));

    //Begin end countdown!
    sec = 10;
    countdown = std::chrono::steady_clock::now() + std::chrono::seconds(sec);
}

void PongGameState::p2_win() {
    gameover = true;

    p1->rect.setPosition(sf::Vector2f(padding, winSize.y/2.0f));
    p2->rect.setPosition(sf::Vector2f((winSize.x - padding), winSize.y/2.0f));
    b->circ.setPosition(sf::Vector2f(winSize.x/2.0f, winSize.y/2.0f));

    //Begin match countdown!
    sec = 10;
    countdown = std::chrono::steady_clock::now() + std::chrono::seconds(sec);
}

//Defines the match beginning, including ball direction and such.
//dir argument constitutes direction (0 is random, 1 is player 1, 2 is player 2)
void PongGameState::match_start(unsigned short int dir) {
    short int direction;

    //Making a random seed because we'll be using this at least one time, if not more
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::chrono::duration elapsed = end - start;
    unsigned int time = std::chrono::duration_cast<std::chrono::seconds>(elapsed).count();
    std::srand(time);

    //Produces a randomized direction for the ball to begin
    direction = -1 + 2*(std::rand()%2);

    //Check to see if we should randomize x-direction or not
    if(dir ==  0) {
        b->velx *= direction;
        b->vely *= direction;
    }
    else if (dir == 1) {
        //If 1 was passed, this means player 1 scored, and therefore the ball gets thrown in p2's direction (which is positive).
        if (b->velx < 0) {
            //Check to see if the velocity is negative, if it is, then flip
            b->velx *= -1;
        }
        //Randomize y
        b->vely *= direction;
    }
    else if (dir == 2) {
        //If 2 was passed, this means player 2 scored, and therefore the ball gets thrown in p1's direction (which is negative).
        if (b->velx > 0) {
            //Check to see if the velocity is positive, if it is, then flip
            b->velx *= -1;
        }
        //Randomize y
        b->vely *= direction;
    }
    else {
        std::perror("Invalid argument passed, choosing random direction as if 0 was passed");
        //Produces a randomized direction for the ball to begin
        direction = -1 + 2*(std::rand()%2);
        b->velx *= direction;
        b->vely *= direction;
    }

    //Now let's randomize the y-direction again to keep the player's on their toes
    direction = -1 + 2*(std::rand()%2);
    b->vely *= direction;

    //Reset the paddle and ball positions to ready the playing field
    p1->rect.setPosition(sf::Vector2f(padding, winSize.y/2.0f));
    p2->rect.setPosition(sf::Vector2f((winSize.x - padding), winSize.y/2.0f));
    b->circ.setPosition(sf::Vector2f(winSize.x/2.0f, winSize.y/2.0f));
    countdown_timer->setPosition(sf::Vector2f(winSize.x/2.0f, winSize.y/2.0f));

    matchphase = false;

    //Begin match countdown!
    sec = 6;
    countdown = std::chrono::steady_clock::now() + std::chrono::seconds(sec);
}

PongGameState::PongGameState() {}