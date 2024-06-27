#ifndef DINO_H
#define DINO_H
#include <vex.h>

#include <random>
#include <vector>

using namespace std;
using namespace vex;

#define SCREEN_ROWS 5
#define SCREEN_COLS 10

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
  mt19937 prng;
  discrete_distribution<int> obsticleRand;
  double speed = 18;
  double cactiF;
  double birdF;
  double nothingF;
  int minDist;
  int currentCount;
  vector<obsticle> onscreen;
  DSTATE current;
  JUMPSTATE cJump;
  int dinoy = 0;
  unsigned long long score = 0;
  vector<pair<int, int>> cactiPossible;
  vector<int> birdPossible;

 public:
  dinogame(double speed, double cacti, double bird, int minDist);
  ~dinogame();
  void changeSpeed(double speed);
  bool jump();
  bool startduck();
  bool endduck();
  DSTATE getstate();
  JUMPSTATE getjump();
  vector<string> getScreen();
  void draw(vex::brain::lcd);
  /**
   * @brief Executes a single tick of the dino game
   * @return -1 for die, or a non-negative integer as the score
   */
  unsigned long long executeTick();
};

#endif