#ifndef DINO_H
#define DINO_H
#include <vex.h>

#include <random>
#include <vector>

using namespace std;
using namespace vex;

enum class DSTATE { RUN, AIR, DUCK };

enum class JUMPSTATE { UPFAST, UPSLOW, STOP, DOWNSLOW, DOWNFAST, NOTJUMP };

struct obsticle {
  // Rectangle
  int x1, x2, y1, y2;
  char c;
};

class dinogame {
 protected:
  random_device RNG;
  double speed = 18;
  double cactiF;
  double birdF;
  double nothingF;
  int minDist;
  bool canigen;
  vector<obsticle> onscreen;

 public:
  dinogame(double speed, double cacti, double bird, int minDist);
  ~dinogame();
  void changeSpeed(double speed);
  bool jump();
  bool startduck();
  bool endduck();
  DSTATE getstate();
  JUMPSTATE getjump();
  void draw();
  void executeTick();
}

#endif