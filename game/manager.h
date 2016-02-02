#include <list>
#include <vector>
#include <SDL.h>
#include "ioManager.h"
#include "clock.h"
#include "world.h"
#include "viewport.h"
#include "hud.h"
#include "health.h"
#include "player.h"
#include "scaledSprite.h"
#include <vector>
#include <list>
#include "explodingSprite.h"
#include "sound.h"

class Manager {
public:
  Manager ();
  ~Manager ();
  void play();
  void switchSprite();
  void displayHud(bool showHud ){
	  hudFlag=showHud;
  }



private:
  const bool env;
  const IOManager& io;
  Clock& clock;

  SDL_Surface * const screen;
  SDL_Surface * ufoSurface;
  World worldFarBack;
  World worldBack;  
  World worldBackSecond;
  World worldMiddle;
  World worldFront;
  Viewport& viewport;

  bool makeVideo;
  SDLSound sound;
  int frameCount;
  const std::string username;
  const std::string title;
  const int frameMax;
  int hudFlag;
  hud hudDisplay;
  std::vector<Drawable*> sprites;  
  Health healthBar;
  unsigned int score;
  unsigned int numberOfUfo;
  bool healthRemaining;
  bool playerExist;
  bool godMode;
  void drawHud () const;
  void draw() const;
  void update();
  void makeScaledPlanets();
  void checkForCollisions(Uint32 &);


  Manager(const Manager&);
  Manager& operator=(const Manager&);
  void makeFrame();
};
