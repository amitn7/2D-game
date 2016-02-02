#ifndef HUD_H
#define HUD_H
#include<string>
#include<SDL.h>
#include"ioManager.h"
#include"vector2f.h"
 
class hud{

  public:
  hud();
  void drawHUD(SDL_Surface * const &,const IOManager &, int, int, int, int  )const;

  private:
  const int HUD_HEIGHT;
  const int HUD_WIDTH;
  int x;
  int y;
  Vector2f secondsLocation;
  Vector2f fpsLocation;
  Vector2f playerLocation;
  Vector2f togHudLocation;
  Vector2f healthBarLocation;
  Vector2f activeListLocation;
  Vector2f freeListLocation;
};

#endif
