#include <iostream>
#include "sprite.h"
#include "ioManager.h"
#include "player.h"
#include "explodingSprite.h"

class SmartSprite : public Sprite {
public:
  SmartSprite(const std::string&, const Vector2f& pos, const Player& p);
  void draw() const;
  void update(Uint32 ticks);
  void goLeft();
  void goRight();
  void goUp();
  void goDown();
  bool isExploding(){ 
    if(esp) return true;
    else return false;
  }
  bool explosionDone() {return exploded;}
  static void incrSafeDistance() {++safeDistance; }
  static void decrSafeDistance() { --safeDistance; }
  static float getSafeDistance() { return safeDistance; }
  ~SmartSprite(){ delete esp;}
  void explode();
private:	
  enum MODE {NORMAL, EVADE};
  IOManager& io;	
  const Player & enemy;
  static float safeDistance;
  MODE currentMode;
  SmartSprite(const SmartSprite &);
  SmartSprite& operator=(const SmartSprite &rhs);
  ExplodingSprite * esp;
  bool exploded;
};
