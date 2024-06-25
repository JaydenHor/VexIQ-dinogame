#include "dino.h"
#include <stdexcept>
#include <vex.h>

dinogame::dinogame(double spd, double c, double b, int md) {
    speed = spd;
    cactiF = c;
    birdF = b;
    if(cactiF + birdF > 1){
        throw out_of_range("Probabilities of cacti and bird are too large");
    }
    nothingF = 1 - (cactiF + birdF);
    minDist = md;
    dinoy = 1;
}

dinogame::~dinogame(){}

void dinogame::changeSpeed(double spd) {
    speed = spd;
}

bool dinogame::jump() {
    if(current == DSTATE::AIR || current == DSTATE::DUCK){
        return false;
    }
    cJump = JUMPSTATE::UPFAST;
    current = DSTATE::AIR;
    return true;
}

bool dinogame::startduck() {
    if(current == DSTATE::DUCK){
        return false;
    }
    dinoy = 0;
    current = DSTATE::DUCK;
    return true;
}

bool dinogame::endduck() {
    if(current == DSTATE::AIR || current == DSTATE::RUN){
        return false;
    }
    dinoy = 0;
    current = DSTATE::RUN;
    return true;
}

DSTATE dinogame::getstate() { return current; }

JUMPSTATE dinogame::getjump() { return cJump; }

vector<string> dinogame::getScreen() {
    vector<string> output(SCREEN_ROWS, string(' ', SCREEN_COLS));
    // Dinosour
    output[0][dinoy] = '@';
    // Obsticles
    for(obsticle i : onscreen){
        
    }
}
