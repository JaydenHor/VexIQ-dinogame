#include "dino.h"

#include <vex.h>


#define MP(a, b) (make_pair((a), (b)))

template <typename Iter, typename RandomGenerator>
Iter select_randomly(Iter start, Iter end, RandomGenerator& g) {
  std::uniform_int_distribution<> dis(0, std::distance(start, end) - 1);
  std::advance(start, dis(g));
  return start;
}

template <typename Iter>
Iter select_randomly(Iter start, Iter end) {
  static std::random_device rd;
  static std::mt19937 gen(rd());
  return select_randomly(start, end, gen);
}

dinogame::dinogame(double c, double b, int md) {
  cactiF = c;
  birdF = b;
  if (cactiF + birdF > 1) {
    // throw out_of_range("Probabilities of cacti and bird are too large");
    // assert(false);
  }
  nothingF = 1 - (cactiF + birdF);
  minDist = md;
  currentCount = 0;
  dinoy = 1;
  score = 0;
  prng = mt19937(RNG());
  obsticleRand = discrete_distribution<int>{c, b, nothingF};
  // Fit under this jump area for cacti:
  //   ##
  //  #  #
  //
  // #    #
  // Duplicates for easier distribution
  // w by h: 1x2, 1x2, 1x2, 2x1, 2x1, 2x2, 2x2, 3x1, 3x1, 1x3, 1x3, 1x3, 4x2,
  // 2x4
  cactiPossible = vector<pair<int, int>>{
      MP(1, 2), MP(1, 2), MP(1, 2), MP(2, 1), MP(2, 1), MP(2, 2), MP(2, 2),
      MP(3, 1), MP(3, 1), MP(1, 3), MP(1, 3), MP(1, 3), MP(4, 2), MP(2, 4)};
  birdPossible = vector<int>{1, 2, 3};
}

dinogame::~dinogame() {}

bool dinogame::jump() {
  if (current == DSTATE::AIR || current == DSTATE::DUCK) {
    return false;
  }
  cJump = JUMPSTATE::UPFAST;
  current = DSTATE::AIR;
  return true;
}

bool dinogame::startduck() {
  if (current == DSTATE::DUCK) {
    return false;
  }
  dinoy = 0;
  current = DSTATE::DUCK;
  return true;
}

bool dinogame::endduck() {
  if (current == DSTATE::AIR || current == DSTATE::RUN) {
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
  for (obsticle i : onscreen) {
    for (int y = i.y1; y <= i.y2; ++y) {
      output[y].replace(i.x1, min(i.x2 - i.x1 + 1, SCREEN_COLS - i.x1), "#");
    }
  }
  return output;
}

void dinogame::draw(vex::brain::lcd b) {
  for (string i : this->getScreen()) {
    b.print(i.c_str());
    b.newLine();
  }
}

unsigned long long dinogame::executeTick() {
  // Order: Resolve jump
  //        Check for obsticles
  //        Update obsticles
  //        Update score
  //        Update time since last obsticle
  //        Add obsticles
  if (current == DSTATE::AIR) {
    switch (cJump) {
      case JUMPSTATE::UPFAST:
        dinoy += 2;
        cJump = JUMPSTATE::UPSLOW;
        break;
      case JUMPSTATE::UPSLOW:
        dinoy += 1;
        cJump = JUMPSTATE::STOP;
        break;
      case JUMPSTATE::STOP:
        cJump = JUMPSTATE::DOWNSLOW;
        break;
      case JUMPSTATE::DOWNSLOW:
        dinoy -= 1;
        cJump = JUMPSTATE::DOWNFAST;
        break;
      case JUMPSTATE::DOWNFAST:
        dinoy -= 2;
        current = DSTATE::RUN;
        break;
      default:
        // throw logic_error("Something has gone seriously wrong");
        // assert(false);
        return -100;
    }
  }
  // Obsticle check
  // Later, make this some sort of queue
  // If obsticle is completly offscreen, remove it;
  for (int i = 0; i < onscreen.size(); ++i) {
    obsticle o = onscreen[i];
    if (o.x1 <= 0 && dinoy <= o.y1 && dinoy >= o.y2) {
      // Hit
      return -1;
    }
    // Yay! You didn't die yet!
    // Check if offscreen
    if (o.x2 < 0) {
      onscreen.erase(onscreen.begin() + i);
    }
  }
  // Update score. For each alive tick, +10 points
  score += 10;
  currentCount += 1;
  if (currentCount >= minDist) {
    // Ranomly generate
    int output = obsticleRand(prng);
    if (output == 0) {
      auto cacti =
          *select_randomly(cactiPossible.begin(), cactiPossible.end(), prng);
      obsticle o;
      o.x1 = SCREEN_COLS;
      o.y1 = SCREEN_ROWS - cacti.second;
      o.x2 = SCREEN_COLS + cacti.first;
      o.y2 = SCREEN_ROWS;
      onscreen.push_back(o);
      currentCount = 0;
    } else if (output == 1) {
      // Bird
      int bird = *select_randomly(birdPossible.begin(), birdPossible.end(), prng);
      obsticle o;
      o.x1 = SCREEN_COLS;
      o.y1 = SCREEN_ROWS - bird;
      o.x2 = SCREEN_COLS;
      o.y2 = SCREEN_ROWS - bird;
      onscreen.push_back(o);
      currentCount = 0;
    }
  }
  return score;
}
