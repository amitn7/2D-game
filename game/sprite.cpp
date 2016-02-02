#include <cmath>
#include "sprite.h"
#include "gamedata.h"
#include "frameFactory.h"
#include "collisionStrategy.h"


Sprite::Sprite(const string& name, const Vector2f& pos, 
               const Vector2f& vel):
  Drawable(name, pos, vel), 
  frame( FrameFactory::getInstance().getFrame( name ) ),
  frameWidth(frame->getWidth()),
  frameHeight(frame->getHeight()),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
  rotatingSprite(0)
  
{ }

Sprite::Sprite(const string& n, const Vector2f& pos, const Vector2f& vel,
               const Frame* frm):
  Drawable(n, pos, vel), 
  frame( frm ),
  frameWidth(frame->getWidth()),
  frameHeight(frame->getHeight()),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
  rotatingSprite(0)
{ }

Sprite::Sprite(const Sprite& s) :
  Drawable(s), 
  frame(s.frame),
  frameWidth(s.getFrame()->getWidth()),
  frameHeight(s.getFrame()->getHeight()),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
  rotatingSprite(0)
{ }

Sprite& Sprite::operator=(const Sprite& rhs) {
  Drawable::operator=( rhs );
  frame = rhs.frame;
  frameWidth = rhs.frameWidth;
  frameHeight = rhs.frameHeight;
  worldWidth = rhs.worldWidth;
  worldHeight = rhs.worldHeight;
  rotatingSprite=rhs.rotatingSprite;
  
  return *this;
}

Sprite::Sprite(const std::string& n) :
  Drawable(n,Vector2f(Gamedata::getInstance().getXmlInt(n+"/startLoc/x"), 
                      Gamedata::getInstance().getXmlInt(n+"/startLoc/y")), 
              Vector2f(Gamedata::getInstance().getXmlInt(n+"/speedX"), 
                       Gamedata::getInstance().getXmlInt(n+"/speedY")) 
           ),
  frame( FrameFactory::getInstance().getFrame(n) ),
  frameWidth(frame->getWidth()),
  frameHeight(frame->getHeight()),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
  rotatingSprite(0)
{ }

void Sprite::draw() const { 
  Uint32 x = static_cast<Uint32>(X());
  Uint32 y = static_cast<Uint32>(Y());
  if(!rotatingSprite){
    frame->draw(x, y);
  }
  else
  {
    frame->draw(x, y , Gamedata::getInstance().getXmlInt("rotateAngle"));
  }
}

int Sprite::getDistance(const Sprite *obj) const { 
  return hypot(X()-obj->X(), Y()-obj->Y());
}

void Sprite::update(Uint32 ticks) { 
  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  setPosition(getPosition() + incr);

  if ( Y() < 0) {
    velocityY( abs( velocityY() ) );
    rotatingSprite=0;
  }
  if ( Y() > worldHeight-frameHeight) {
    velocityY( -abs( velocityY() ) );
    rotatingSprite=1;
  }

  if ( X() < 0) {
    velocityX( abs( velocityX() ) );
    rotatingSprite=0;
  }
  if ( X() > worldWidth-frameWidth) {
    velocityX( -abs( velocityX() ) );
    rotatingSprite=1;
  }  

}
