#ifndef CLOCK_H
#define CLOCK_H
#include <SDL.h>
#include <string>
#include <deque>


class Manager;


class Clock {
public:
  static Clock& getInstance();
  unsigned int getTicks() const;
  unsigned int getTotalTicks() const { return sumOfAllTicks; }

private:
  friend class Manager;
  friend class Player;

  bool started;
  bool paused;
  bool sloMo;
  const bool framesAreCapped;
  const int frameCap;

  unsigned int frames;
  std::deque<int> recentFrames;
  const unsigned int maxFramesToAvg;

  unsigned int tickSum;
  unsigned int sumOfAllTicks;
  unsigned int timeAtStart;
  unsigned int timeAtPause;
  unsigned int currTicks;
  unsigned int prevTicks;
  unsigned int ticks;

  unsigned int getTicksSinceLastFrame();
  Clock& update();
  void toggleSloMo();
  void reset();
  
  bool isStarted() const { return started; }
  bool isPaused() const  { return paused;  }
  unsigned int getFrames() const  { return frames;  }
  unsigned int getSeconds() const { return getTicks()/1000;  }
  unsigned int capFrameRate() const;
  int getFps() const;
  int getAvgFps() const;
  void draw() const;
  void start();
  void pause();
  void unpause();

  Clock();
  Clock(const Clock&);
  Clock&operator=(const Clock&);
};
#endif
