#ifndef PlAYER__H
#define PlAYER__H
#include <string>
#include "sprite.h"
#include "gamedata.h"
#include "frameFactory.h"
#include "multiBullets.h"
#include "collisionStrategy.h"
#include "explodingSprite.h"
#include "drawable.h"
#include <vector>
#include <list>
#include "clock.h"


class Player : public Drawable {
public:
  Player(const std::string& name);
  Player(const Player&);
  MultiBullets* getBullets() {
    return &bullets;
  }
  bool playerCollidWith(const Drawable* d) {
    if (explodingPlayer) return false ;
    if(clock.getSeconds()-expStart<expInterval) return false;
    if(strategy->execute(*this, *d)){
      expStart= clock.getSeconds();
      return true;
    }
    return false;
  }
  virtual ~Player();
  virtual void draw() const;
  virtual void update(Uint32 ticks);
  
  virtual const Frame* getFrame() const { 
    return frame; 
  }
  virtual void shoot();
  bool collidWith( Drawable* d) {
    return bullets.collidWith(d);
  }
  int getActiveBullet(){return bullets.activeBullet();}
  int getFreeBullet(){return bullets.freeBullet();}
  void stop();
  void right();
  void left();
  void up();
  void down();
  void flipLeft();
  void flipRight();
  void flipUp();
  unsigned int getBulletListSize() const { return bullets.bulletCount(); }
  unsigned int getFreeListSize() const { return bullets.freeCount(); }
  void explode();
  void setFlip(bool f) { flipPlayer = f; }
  void setFlipUp(bool f ){ flipPlayerUp=f;}
  bool explosionDone() {return exploded;}
private:
  Player& operator=(const Player&);  
protected:
  Clock &clock;
  const Frame * frame;
  int playerWidth;
  int playerHeight;
  int worldWidth;
  int worldHeight;
  const int speedX;
  const int speedY;
  bool keyPressedX;
  bool keyPressedY;
  std::string bulletName;
  MultiBullets bullets;
  float bulletInterval;
  float timeSinceLastBullet;
  float minBulletSpeed;
  bool flipPlayer;
  bool flipPlayerUp;
  ExplodingSprite * explodingPlayer;
  CollisionStrategy * strategy;
  unsigned int expStart;
  unsigned int expInterval;
  bool exploded;
};
#endif


