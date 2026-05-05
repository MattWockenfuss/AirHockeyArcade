#include "Tron.hpp"

#include <iostream>

#include "../../Context.hpp"
#include "../../AssetManager.hpp"
#include "../../IO/InputManager.hpp"

Direction opposite(Direction d){
    switch (d){
        case Direction::North: return Direction::South;
        case Direction::South: return Direction::North;
        case Direction::East:  return Direction::West;
        case Direction::West:  return Direction::East;
    }
    return Direction::North; //should never reach here
}

std::string directionToString(Direction d){
    switch (d){
        case Direction::North: return "North";
        case Direction::South: return "South";
        case Direction::East:  return "East";
        case Direction::West:  return "West";
    }
    return "North"; //should never reach here
}

Tron::Tron(Context* ctx, Direction facing, int length, int headX, int headY, int tailX, int tailY,sf::Color color, sf::Color headColor, float yOffset){
    this -> ctx = ctx;
    this -> facing = facing;
    this -> headX = headX;
    this -> headY = headY;
    this -> tailX = tailX;
    this -> tailY = tailY;
    this -> length = length;
    this -> color = color;
    this -> headColor = headColor;
    this -> debugYOffset = yOffset;
    this -> lastFacing = facing;
    text.emplace(ctx -> assets -> getFont("Consolas"), "", 22);
}



/*  Every Tron has a queue of joints, ordered by distance from the head. The newest joints are at
    the end of the linked list (tail of queue) and the oldest are in the back, or the head.                 */
void Tron::enqueue(int x, int y){
    Joint* newJoint = new Joint{x, y};
    //okay so when we add a new joint, what are we doing?
    //we are adding it to the end, the tail
    //first check if the tail points to null, cause then this is the first node

    if(front == nullptr){
        //then the list is empty, this is the first
        back = newJoint;
        front = newJoint;
    }else{
        //then there is something in the list, append
        back -> next = newJoint;
        back = newJoint;
    }
}
void Tron::dequeue(){
    if(front == nullptr) return;
    
    //move it up and delete
    Joint* joint = front;
    front = front -> next;
    delete joint;

    //back was pointing to a now deleted node, set to nullptr to prevent dangling pointers
    if(front == nullptr) back = nullptr;
    return;
}

Joint Tron::peek(){
    if(front == nullptr) return Joint{0, 0};

    Joint value = *front;
    //std::cout << "Peeking joint: (" << value.x << ", " << value.y << ") from " << directionToString(value.fromDirection) << std::endl;
    return value;
}

//okay so how can the Tron move?
//every tick they move in the direction we are facing, we need to update the head and tail, and the joints list, possible dequeue

void Tron::render(sf::RenderWindow& window){
    //okay how do we render?
    //render all of the squares from the tail to the head, changing directions at joints
    
    square = sf::RectangleShape({squareWidth, squareWidth});
    square.setFillColor(color);

    int tx = tailX;
    int ty = tailY;

    Joint* current = front;
    while(current != nullptr){
        //so we are starting at the front of the queue, or the tail of the Tron
        //for every joint, we want to loop between our current x and y to the location of the next joint

        //loop to the current joints x and y depending on direction

        renderTronSegment(window, tx, ty, current -> x, current -> y);
        
        tx = current -> x;
        ty = current -> y;
        current = current -> next;
    }

    //std::cout << "Done with joints, going to head!, (tx, ty)(" << tx << ", " << ty << "), (headX, headY)(" << headX << ", " << headY << ")" << std::endl;
    //if there was no joints then we are here with tx and ty at the tail, and we want to loop to the head
    //otherwise tx and ty are at the location of the last joint, so we want to loop to the head from there
    //okay so since we going to the head, we check facing direction

    renderTronSegment(window, tx, ty, headX, headY);

    //next render the head of the Tron!
    square.setFillColor(headColor);
    square.setPosition({headX * squareWidth, headY * squareWidth});
    window.draw(square);



    //  Next we are going to render the debug text for listing all of the information about the Tron
    //  such as head and tail position, length, and joints list

    std::string debug = "Head: (" + std::to_string(headX) + ", " + std::to_string(headY) + ")\n" +
                        "Tail: (" + std::to_string(tailX) + ", " + std::to_string(tailY) + ")\n" +
                        "Length: " + std::to_string(length) + "\n" +
                        "facing: " + directionToString(facing) + "\n" +
                        "lastFacing: " + directionToString(lastFacing) + "\n";

    Joint* currentJoint = front;
    while(currentJoint != nullptr){
        debug += "Joint: (" + std::to_string(currentJoint -> x) + ", " + std::to_string(currentJoint -> y) + ")\n";
        currentJoint = currentJoint -> next;
    }

    text -> setFillColor(sf::Color::White);
    text -> setStyle(sf::Text::Style::Bold);
    text -> setCharacterSize(24);
    text -> setString(debug);
    text -> setPosition({20.0f, debugYOffset + 20.0f});
    text -> setRotation(sf::degrees(0.0f));
    //window.draw(*text);
}

void Tron::renderTronSegment(sf::RenderWindow& window, int tx, int ty, int ex, int ey){
    /*
        This function renders a segment of the Tron from (tx, ty) to (ex, ey), it is used for rendering the joints and head of the Tron
        it does NOT render the square at ex, ey, as that is tx, ty of the next segment, this also means that we must
        render the head of the Tron separately after rendering all of the segments

        Notice the while loop stop when both Xs or Ys are equal
        'Keep looping while at least one coordinate is different, move in the direction of the difference'
    */

    while(tx != ex || ty != ey){
        //std::cout << "\t\tRendering square at: (" << tx << ", " << ty << ")" << std::endl;
        
        square.setPosition({tx * squareWidth, ty * squareWidth});
        window.draw(square);
        
        // text -> setFillColor(sf::Color::White);
        // text -> setRotation(sf::degrees(45.0f));
        // text -> setString("(" + std::to_string(tx) + ", " + std::to_string(ty) + ")");
        // text -> setPosition({tx * squareWidth + 16.0f, ty * squareWidth});
        // window.draw(*text);

        if(ex > tx) tx++;
        else if(ex < tx) tx--;
        else if(ey > ty) ty++;
        else if(ey < ty) ty--;
    }
}


void Tron::move(){

    /*
        Okay how does the Tron move? well depending on the direction they are facing, we need to update
        the coordinates of the head and tail as well as all of the joints

        (1) If we are changing direction, enqueue a new joint at the head 
        (2) Move head in direction we are facing
        (3) If we are NOT growing, move tail in direction of last joint if it exists, otherwise in direction we are facing
        (4) If the tail is at a joint, dequeue the joint
    */



    //  (1) If we are changing direction, enqueue a new joint at the head 
    if(lastFacing != facing){
        //okay so we are changing direction, enqueue a new joint at the head
        enqueue(headX, headY);
        lastFacing = facing;
    }


    //  (2) Move head in direction we are facing
    if(facing == Direction::North) headY--;
    if(facing == Direction::South) headY++;
    if(facing == Direction::East) headX++;
    if(facing == Direction::West) headX--;


    //  (3) If we are NOT growing, move tail in direction of last joint if it exists, otherwise in direction we are facing
    if(!growNextTick){
        std::cout << "Not growing, moving tail!" << std::endl;
        if(front == nullptr){
            //then there are no joints, we are just moving in a straight line, move the tail in the same direction as the head
            if(facing == Direction::North) tailY--;
            if(facing == Direction::South) tailY++;
            if(facing == Direction::East) tailX++;
            if(facing == Direction::West) tailX--;
        }else{
            //then we have atleast 1 joint, so we are moving in the direction of the first joint, or oldest in the queue
            int jx = front -> x;
            int jy = front -> y;

            if(jx > tailX) tailX++;
            else if(jx < tailX) tailX--;
            else if(jy > tailY) tailY++;
            else if(jy < tailY) tailY--;
        }
    }else{
        //std::cout << "Growing Tron by 1!" << std::endl;
        //then we are growing, so we skip moving the tail, and just set growNextTick to false for the next tick
        length++;
        //we dont do this because this is tron, not snake
        //growNextTick = false;
    }


    // (4) If the tail is at a joint, dequeue the joint
    if(front != nullptr && tailX == front -> x && tailY == front -> y){
        //okay so the tail is at a joint, dequeue the joint
        dequeue();
    }



}


bool squarePartOfLine(int tx, int ty, int x1, int y1, int x2, int y2){
    /*
        This function checks whether the point (tx, ty) lies on the line segment
        defined by (x1, y1) and (x2, y2).

        Steps:
        (1) Identify whether the segment is vertical (x1 == x2) or horizontal (y1 == y2)
        (2) Check that the point shares the same fixed coordinate
        (3) Verify that the other coordinate lies within the inclusive range of the segment endpoints
    */

    if(x1 == x2){
        //then x1 == x2
        if(tx == x1){
            if(std::min(y1, y2) <= ty && ty <= std::max(y1, y2)){
                return true;
            }
        }
    }else{
        //then y1 == y2
        if(ty == y1){
            if(std::min(x1, x2) <= tx && tx <= std::max(x1, x2)){
                return true;
            }
        }
    }

    return false;
}

bool Tron::checkForCollision(Tron& otherSnake){
    /*
        Okay we are determining whether or not this snake has collided with something. Here We 
        define collide as the head of the snake being inside the walls or inside the other snake.
        
        The only edge case, is that if the two snakes collide head on, then the left snake collides
        and the right snake wins

        To Determine if there was a collision
        (1) Check if the head is inside any of the walls
        (2) Check if the head is between any two consectutive joints, including the head and tail
            (2.A) NOTE: the 'front' of the list, is closest to the tail, as we enqueue there
        (3) Finally we check if the snake is colliding with itself anywhere, we can skip between the head as that is impossible
    */



    //(1) Check if the head is inside any of the walls
    //(0, 23) -> (0, 89) -> (159, 23) -> (159, 89)
    if(headX == 0 || headX == 159){
        if(23 <= headY && headY <= 89){
            return true;
        }
    }else if(headY == 23 || headY == 89){
        if(0 <= headX && headX <= 159){
            return true;
        }
    }




    //(2) Check if the head is between any two consectutive joints, including the head and tail

    /*
        Okay, starting at the head, we want to loop through all joints and then tail, check if we
        are between any two. of the other snake!
    */

    int x1 = otherSnake.tailX;
    int y1 = otherSnake.tailY;
    int x2 = 0;
    int y2 = 0;

    if(otherSnake.front == nullptr){
        std::cout << "THIS SNAKE HAS NO JOINTS!" << std::endl;
        //then the queue is empty, this snake has no joints!
        x2 = otherSnake.headX;
        y2 = otherSnake.headY;

        if(squarePartOfLine(headX, headY, x1, y1, x2, y2)){
            return true;
        }
    }else{
        /*
            We will start at the head, and loop through x amount of joints, we know there is at least
            one, then the last joint and the tail. We want to be sure that the calling snake's head is not
            colliding with any of the joints
        */
        Joint* curr = otherSnake.front;
        while(curr != nullptr){
            x2 = curr -> x;
            y2 = curr -> y;
            
            if(squarePartOfLine(headX, headY, x1, y1, x2, y2)){
                return true;
            }

            //okay so they dont collide, set x1 and y1 to current joint, and we will check next
            x1 = x2;
            y1 = y2;
            curr = curr -> next;
        }
        //now check the last joint, and tail
        x2 = otherSnake.headX;
        y2 = otherSnake.headY;

        if(squarePartOfLine(headX, headY, x1, y1, x2, y2)){
            return true;
        }

    }

    //(3) Finally we check if the snake is colliding with itself anywhere, we can skip between the head as that is impossible
    x1 = tailX;
    y1 = tailY;
    x2 = 0;
    y2 = 0;
    if(front != nullptr){
        Joint* curr = front;
        while(curr != nullptr){
            x2 = curr -> x;
            y2 = curr -> y;
            
            if(squarePartOfLine(headX, headY, x1, y1, x2, y2)){
                return true;
            }

            //okay so they dont collide, set x1 and y1 to current joint, and we will check next
            x1 = x2;
            y1 = y2;
            curr = curr -> next;
        }
    }




    //then we are sure they dont collide
    return false;
}