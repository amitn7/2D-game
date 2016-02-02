#include<iostream>
#include<cmath>
#include "SDL/SDL_rotozoom.h"
#include "scaledSprite.h"
#include "gamedata.h"
#include "frameFactory.h"


ScaledSprite::ScaledSprite(const std::string& name, SDL_Surface* surface):
  Drawable(name,
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/loc/x"),
             Gamedata::getInstance().getXmlInt(name+"/loc/y")),
           Vector2f((rand()%2?1:-1)*Gamedata::getInstance().getRandFloat(
             Gamedata::getInstance().getXmlInt(name+"/cushion"),
             Gamedata::getInstance().getXmlInt(name+"/speed/x")),
               (rand()%2?1:-1)*Gamedata::getInstance().getRandFloat(
             Gamedata::getInstance().getXmlInt(name+"/cushion"),
             Gamedata::getInstance().getXmlInt(name+"/speed/y"))
           )
          ),
  scale(Gamedata::getInstance().getRandFloat(Gamedata::getInstance().getXmlFloat(name+"/scale/min"),
  Gamedata::getInstance().getXmlFloat(name+"/scale/max"))),
  scaledSurface( rotozoomSurface( surface, 0, scale, SMOOTHING_ON) ),
  frame( new Frame(scaledSurface, scaledSurface->w,scaledSurface->h,
    Gamedata::getInstance().getXmlInt(name+"/src/x"),
    Gamedata::getInstance().getXmlInt(name+"/src/y"))),
  frameWidth(frame->getWidth()),
  frameHeight(frame->getHeight()),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
  exploded(false),
  esp()
  
  
{ }

ScaledSprite::ScaledSprite(const ScaledSprite& s) :
  Drawable(s.getName(),s.getPosition(), s.getVelocity()),
  scale(s.scale),
  scaledSurface(s.scaledSurface),
  frame(s.frame),
  frameWidth(s.frameWidth),
  frameHeight(s.frameHeight),
  worldWidth(s.worldWidth),
  worldHeight(s.worldHeight),
  exploded(s.exploded),
  esp(s.esp)
  
{ }

ScaledSprite& ScaledSprite::operator=(const ScaledSprite& rhs) {
  setName(rhs.getName() );
  setPosition(rhs.getPosition());
  setVelocity(rhs.getVelocity());
  scale = rhs.scale;
  scaledSurface= rhs.scaledSurface;
  frame = rhs.frame;
  frameWidth = rhs.frameWidth;
  frameHeight = rhs.frameHeight;
  worldWidth = rhs.worldWidth;
  worldHeight = rhs.worldHeight;
  exploded=rhs.exploded;
  esp = rhs.esp;
  
  return *this;
}

ScaledSprite::~ScaledSprite() {
  SDL_FreeSurface(scaledSurface);
  if (frame)delete frame;
  if(esp) delete esp;
}
void ScaledSprite::draw() const {
  if (esp) {
    esp->draw();
    return;
  }
  Uint32 x = static_cast<Uint32>(X());
  Uint32 y = static_cast<Uint32>(Y());
  frame->draw(x, y);
  
}
void ScaledSprite::explode() { 
  if ( esp ) return;  
  Sprite sprite(getName(), getPosition(), getVelocity(), getFrame());
  esp = new ExplodingSprite(sprite);
}


void ScaledSprite::update(Uint32 ticks) { 
  if ( esp ) {
    esp->update(ticks);
    if ( esp->chunkCount() == 0 ) {
      exploded=true;
      delete esp;
      esp = NULL;
    }
    return;
  }
  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  setPosition(getPosition() + incr);
   if ( Y() < 0) {
    velocityY( abs( velocityY() ) );
  }
  if ( Y() > worldHeight-frameHeight) {
    velocityY( -abs( velocityY() ) );
  }

  if ( X() < 0) {
    velocityX( abs( velocityX() ) );
  }
  if ( X() > worldWidth-frameWidth) {
    velocityX( -abs( velocityX() ) );
  }  
}
