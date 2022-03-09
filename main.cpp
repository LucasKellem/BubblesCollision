/* CSCI 261 Lab A8: Bubble Bobble
 *
 * Author: Lucas Kellem
 *
 * Inside our draw loop, we now need to draw all the Bubbles in our vector. After our event handling, we'll then need
 * to update the positions of all the Bubbles in our vector. You should now see five Bubbles bouncing around the window.
 * Excellent. Let's have the user interact. When the user clicks the left mouse button, we want to create a new Bubble
 * at the location where the user clicked. This new Bubble should have the same starting properties that our original
 * five Bubbles did. After the user clicks the first time, we should see six Bubbles moving around the window. A second
 * click, seven Bubbles. And so forth as the user continues to click. (Hmm, how can we keep track of these new Bubbles?)
 * We may get to the point where there are too many Bubbles on the screen. If the user presses the 'd' or 'D' key, then
 * we want to delete the last Bubble that was added to the window. Obviously if there are no Bubbles in the window,
 * then pressing 'd' should do nothing.
 */

#include <iostream> // for standard input/output
using namespace std; // using the standard namespace

#include <SFML/Graphics.hpp> // include the SFML Graphics Library
using namespace sf; // using the sf namespace

#include <vector> // for vector<>
#include "Bubble.h" // our bubble class
#include <ctime> // for time()

int main() {
    srand(time(0)); //seed your rand loser
    rand() % 100; //throwaway

    vector<Bubble> bubbles; //vector of bubbles

    for(int i = 0; i < 5; i++){ //generate initial 5 bubbles
        Bubble tempBubble;
        tempBubble.setBubbleCircle();
        while(!tempBubble.checkForCollision(bubbles)){ //make sure we don't start off with a colision
            tempBubble.setBubbleCircle();
        }
        bubbles.push_back(tempBubble); //push the bubble back
    }

    // create a RenderWindow object
    // specify the size to be 640x640
    // set the title to be "SFML Example Window"
    RenderWindow window( VideoMode(640, 640), "SFML Example Window" );
    window.setFramerateLimit(200); //without this it gets mighty hectic
    //********************************************
    //  PLACE ANY FILE INPUT PROCESSING BELOW HERE
    //********************************************

    //********************************************
    //  PLACE ANY FILE INPUT PROCESSING ABOVE HERE
    //********************************************

    // while our window is open, keep it open
    // this is our draw loop
    while( window.isOpen() ) {
        window.clear(Color::Black); // clear the contents of the old frame
        // by setting the window to black

        //****************************************
        //  ADD ALL OF OUR DRAWING BELOW HERE
        //****************************************

        for (int i = 0; i < bubbles.size(); i++) { //draw our bubbles
            window.draw(bubbles.at(i).getBubbleCircle());
        }

        //****************************************
        //  ADD ALL OF OUR DRAWING ABOVE HERE
        //****************************************

        window.display(); // display the window

        //****************************************
        // HANDLE EVENTS BELOW HERE
        //****************************************
        Event event;
        while (window.pollEvent(event)) { // ask the window if any events occurred
            if (event.type == Event::Closed) { // if event type is a closed event
                // i.e. they clicked the X on the window
                window.close();                 // then close our window
            } else if (event.type == Event::MouseButtonReleased) {
                if (event.mouseButton.button == Mouse::Left) {
                    Bubble tempBubble;
                    tempBubble.setBubbleCircle();
                    tempBubble.setPosition(event);
                    bubbles.push_back(tempBubble);
                }
            } else if (event.type == Event::KeyPressed) {
                if (event.key.code == Keyboard::D) {
                    if(!bubbles.empty()){
                        bubbles.pop_back();
                    }
                }
            }
        }
        /*
         * Note as well that, in order to calculate collisions properly, you will likely want to move all your objects
         * first, and then perform collision detection on them. Otherwise you may have a situation where Ball1 updates,
         * checks for collisions, collides, then Ball2 updates, is no longer in the same area as Ball1, and reports no
         * collision.
         */
        
        for (int i = 0; i < bubbles.size(); i++) {
            bubbles.at(i).checkBounds();
            bubbles.at(i).moveBubble();
        }
        for (int i = 0; i < bubbles.size(); i++) {
            bubbles.at(i).updatePosition(bubbles);
        }
    }

    return EXIT_SUCCESS; // report our program exited successfully
}

