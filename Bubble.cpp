//
// Created by lucas on 11/29/2020.
//

#include "Bubble.h"

using namespace std;
using namespace sf;

Bubble::Bubble() {
    _bubbleCircle.setRadius(10);
    _bubbleCircle.setFillColor(Color::White);
    _bubbleCircle.setPosition(320, 320);
    _xDir = 0.1; //When we create our Bubble object, we need to make sure its direction values are set to non-zero values.
    _yDir = 0.2; // Do this now, we suggest using values of 0.1 & 0.2 for x & y respectively.
    _mass = _bubbleCircle.getRadius();
    _bubbleID = -9999;
}

void Bubble::setBubbleCircle(){
    _bubbleCircle.setPosition(Vector2f((rand() % 301) + 100,(rand() % 301) + 100));
    /*
     * If we want range of 2.5 to 0.0 then use the code commented below. I think it should be 2.5 to -2.5 though.
     * double range = (2.5 - 0.0);
     * double div = RAND_MAX / range;
     * _xDir = (rand() / div);
     * _yDir = (rand() / div);
     */
    double range = (2.5 - -2.5);
    double div = RAND_MAX / range;
    _xDir = -2.5 + (rand() / div);
    _yDir = -2.5 + (rand() / div);
    _bubbleCircle.setRadius((rand() % 41) + 10);
    _bubbleCircle.setOrigin(Vector2f(_bubbleCircle.getRadius(), _bubbleCircle.getRadius()));
    _bubbleCircle.setFillColor(Color((rand() % 256),(rand() % 256),(rand() % 256)));
    _mass = _bubbleCircle.getRadius();
    _bubbleID = rand() % 9000 + 1000;
    _bubbleCircle.setOutlineThickness(0);
}

void Bubble::setXDir(){
    _xDir = _xDir * -1;
}

void Bubble::setYDir(){
    _yDir = _yDir * -1;
}

CircleShape Bubble::getBubbleCircle() const{
    return _bubbleCircle;
}

double Bubble::getXDir() const{
    return _xDir;
}

double Bubble::getYDir() const{
    return _yDir;
}

void Bubble::updatePosition(vector<Bubble> &myBubbles){
    //We can check this by seeing if the position becomes less than zero (to correspond to the left or top of our window)
    for(auto& i : myBubbles){ //So this is a range-based for loop
        //auto& i is a range_declaration
        //myBubbles is the range_expression
        /*
         * The range is a universal reference (because it is declared as auto&&). A universal reference, also known as a
         * forwarding reference, can bind to either an lvalue or an rvalue expression. This suggests the
         * range_expression can be anything including but not limited to - a variable, a const reference, or a function
         * call that returns a temporary.
         */

        //https://gamedevelopment.tutsplus.com/tutorials/when-worlds-collide-simulating-circle-circle-collisions--gamedev-769 source
        if(_bubbleID == i.getID()){ //If it is the same bubble...break
            break;
        }else{ //Not the same bubbles
            if (i.getBubbleCircle().getPosition().x + i.getBubbleCircle().getRadius() + _bubbleCircle.getRadius() > _bubbleCircle.getPosition().x
                  && i.getBubbleCircle().getPosition().x < _bubbleCircle.getPosition().x + i.getBubbleCircle().getRadius() + _bubbleCircle.getRadius()
                  && i.getBubbleCircle().getPosition().y + i.getBubbleCircle().getRadius() + _bubbleCircle.getRadius() > _bubbleCircle.getPosition().y
                  && i.getBubbleCircle().getPosition().y < _bubbleCircle.getPosition().y + i.getBubbleCircle().getRadius() + _bubbleCircle.getRadius())
            { //This is a simple check that can save time
                //AABBs are overlapping (axis-aligned bounding boxes)
                //If they are overlapping, then we should check for real.
                auto distance = sqrt(((i.getBubbleCircle().getPosition().x - _bubbleCircle.getPosition().x) * (i.getBubbleCircle().getPosition().x - _bubbleCircle.getPosition().x))+ ((i.getBubbleCircle().getPosition().y - _bubbleCircle.getPosition().y) * (i.getBubbleCircle().getPosition().y - _bubbleCircle.getPosition().y)));
                if (distance < i.getBubbleCircle().getRadius() + _bubbleCircle.getRadius()){

                    //balls have collided
                    float newVelX1 = (i.getXDir() * (i.getMass() - _mass) + (2 * _mass * _xDir)) / (i.getMass() + _mass);
                    float newVelY1 = (i.getYDir() * (i.getMass() - _mass) + (2 * _mass * _yDir)) / (i.getMass() + _mass);
                    float newVelX2 = (_xDir * (_mass - i.getMass()) + (2 * i.getMass() * i.getXDir())) / (i.getMass() + _mass);
                    float newVelY2 = (_yDir * (_mass - i.getMass()) + (2 * i.getMass() * i.getYDir())) / (i.getMass() + _mass);

                    i.setSpeedCollision(newVelX1, newVelY1);
                    this->setSpeedCollision(newVelX2, newVelY2);

                    //move both with new velocities to (hopefully) prevent a "bubble stuck in another bubble" situation
                    i.checkBounds();
                    i.getBubbleCircle().move(newVelX1, newVelY1);
                    this->checkBounds();
                    _bubbleCircle.move(newVelX2, newVelY2);
                }
            }
        }
    }
}

void Bubble::checkBounds() {
    if(_bubbleCircle.getPosition().x - _bubbleCircle.getRadius() + _xDir < 0){ //left wall
        this->setXDir();
    }
    if(_bubbleCircle.getPosition().y - _bubbleCircle.getRadius() + _yDir < 0){ //top wall
        this->setYDir();
    }
    if(_bubbleCircle.getPosition().x + _bubbleCircle.getRadius() + _xDir > 640){ //right wall
        this->setXDir();
    }
    if(_bubbleCircle.getPosition().y + _bubbleCircle.getRadius() + _yDir > 640){ //bottom wall
        this->setYDir();
    }
}

void Bubble::setPosition(Event userEvent) {
    float xPo, yPo;
    xPo = static_cast<float>(userEvent.mouseButton.x); //convert event coords
    yPo = static_cast<float>(userEvent.mouseButton.y); //convert event coords
    _bubbleCircle.setPosition(xPo, yPo);
}

bool Bubble::checkForCollision(const vector<Bubble> &myBubbles){
    FloatRect boundingBox = this->getBubbleCircle().getGlobalBounds();
    //We now have the current bubble we want to check collisions for, in the back of the vector.
    if(!myBubbles.empty()){
        for(int i = 0; i < myBubbles.size(); i++){
            FloatRect boundingBox2 = myBubbles.at(i).getBubbleCircle().getGlobalBounds();
            if(boundingBox.intersects(boundingBox2)){
                return false; //there was a collision (not valid location)
            }
        }
    }
    return true; //no collisions, valid location
}

void Bubble::setSpeedCollision(float newXVel, float newYVel){
    _xDir = newXVel; //change x velocity
    _yDir = newYVel; //change y velocity
}

double Bubble::getMass() const{
    return _mass; //get Mass
}

int Bubble::getID() const{
    return _bubbleID; //get ID
}

void Bubble::moveBubble(){
    _bubbleCircle.move(_xDir, _yDir); //Move bubble
}