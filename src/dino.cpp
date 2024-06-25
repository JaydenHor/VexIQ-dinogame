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
}

dinogame::~dinogame(){}

void dinogame::changeSpeed(double spd) {
    speed = spd;
}
