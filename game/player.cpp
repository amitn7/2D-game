#include "player.h"
#include <list>


Player::~Player() { 
   delete strategy; 
   if(explodingPlayer) delete explodingPlayer;
}

Player::Player(const std::string& name) :
  Drawable(name, 
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/startLoc/x"), 
                    Gamedata::getInstance().getXmlInt(name+"/startLoc/y")), 
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/speedX"),
                    Gamedata::getInstance().getXmlInt(name+"/speedY"))
           ),
  clock(Clock::getInstance()),
  frame(FrameFactory::getInstance().getFrame(name)),
  playerWidth( getFrame()->getWidth() ), 
  playerHeight( getFrame()->getHeight() ),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")), 
  speedX(Gamedata::getInstance().getXmlInt(name+"/speedX")),
  speedY(Gamedata::getInstance().getXmlInt(name+"/speedY")),
  keyPressedX(false),
  keyPressedY(false),
  bulletName(Gamedata::getInstance().getXmlStr(name+"/bulletName")),
  bullets(MultiBullets(bulletName)),
  bulletInterval(Gamedata::getInstance().getXmlInt(name+"/bulletInterval")),
  timeSinceLastBullet(0),
  minBulletSpeed(Gamedata::getInstance().getXmlInt(name+"/minBulletSpeed") ),
  flipPlayer(0),
  flipPlayerUp(0),
  explodingPlayer(),
  strategy( new PerPixelCollisionStrategy ),
  expStart(clock.getSeconds()),
  expInterval(Gamedata::getInstance().getXmlInt(name+"/expInterval") ),
  exploded(false)
{ }

Player::Player(const Player & p) :
  Drawable(p),
  clock(Clock::getInstance()),
  frame(p.frame),
  playerWidth(p.playerWidth),
  playerHeight(p.playerHeight),
  worldWidth(p.worldWidth),
  worldHeight(p.worldHeight),
  speedX(p.speedX),
  speedY(p.speedY),
  keyPressedX(p.keyPressedX),
  keyPressedY(p.keyPressedY),
  bulletName(p.bulletName),
  bullets(p.bullets),
  bulletInterval(p.bulletInterval),
  timeSinceLastBullet(p.timeSinceLastBullet),
  minBulletSpeed(p.minBulletSpeed),
  flipPlayer(p.flipPlayer),
  flipPlayerUp(p.flipPlayerUp),
  explodingPlayer(p.explodingPlayer),
  strategy(p.strategy),
  expStart(p.expStart),
  expInterval(p.expInterval),
  exploded(p.exploded)
{ }

void Player::explode() { 
  if ( explodingPlayer ) return;
  if(exploded) return;
  Sprite sprite(getName(), getPosition(), getVelocity(), getFrame());
  explodingPlayer = new ExplodingSprite(sprite);
}


void Player::draw() const { 
  bullets.draw();
  if (explodingPlayer) {
    explodingPlayer->draw();
    return;
  }
  if(exploded) return;
  Uint32 x = static_cast<Uint32>(X());
  Uint32 y = static_cast<Uint32>(Y());
  if(!flipPlayer && !flipPlayerUp)
  frame->draw(x, y);
  else if(flipPlayer && !flipPlayerUp)
  frame->draw(x, y , Gamedata::getInstance().getXmlInt("rotateAngle"));
  else  frame->draw(x, y , Gamedata::getInstance().getXmlInt("rotateAngleUp"));
  
}


void Player::update(Uint32 ticks) { 
  timeSinceLastBullet += ticks;
  bullets.update(ticks);
  if ( explodingPlayer ) {
    explodingPlayer->update(ticks);
    if ( explodingPlayer->chunkCount() == 0 ) {
      delete explodingPlayer;
      explodingPlayer = NULL;
      exploded =true;
    }
    return;
  }
  if(exploded) return ;
  if(keyPressedX == false ){
      velocityX( 0 );
  }
  if(keyPressedY == false ){
      velocityY( 0 );
  }
  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  setPosition(getPosition() + incr);
  if(Y() < 0 ){
    velocityY(speedY);
     if(!flipPlayerUp)flipPlayer=0;
  }
  if(Y() > worldHeight - playerHeight){
    velocityY(-speedY);
    if(!flipPlayerUp) flipPlayer=0;
  }
  if(X() < 0){
    velocityX(speedX);
    if(!flipPlayerUp) flipPlayer=0;
  }
  if(X() > worldWidth - playerWidth){
    velocityX(-speedX);
     if(!flipPlayerUp) flipPlayer=1;
  }
  keyPressedX = false;
  keyPressedY = false;
  
}

void Player::flipRight() { 
  if ( X() < worldWidth-playerWidth) {
    velocityX(velocity[0]);
    setFlip(0);
    setFlipUp(0);
  }
} 
void Player::flipLeft()  { 
  if ( X() > 0) {
    velocityX(-velocity[0]);
    setFlip(1);
    setFlipUp(0);
  }
}

void Player::flipUp() { 
  if ( Y() > 0) {
    setFlipUp(1);
  }
} 
void Player::shoot(){
  if(explodingPlayer) return;
  if(timeSinceLastBullet > bulletInterval){
   Vector2f vel(0.002,0.001);
   //Vector2f vel = getVelocity();
   float x = X() + playerWidth/1.5;
   float y = Y() + playerHeight/4;
   if(flipPlayerUp){
     vel [1]=- minBulletSpeed; 
      x = X() + playerWidth/5;
      y = Y() ;
    }
   else if (!flipPlayer) vel[0] = minBulletSpeed;
   else vel[0] = -minBulletSpeed;
   bullets.shoot(Vector2f(x,y) , vel);
   timeSinceLastBullet = 0;
  } 
}

void Player::up(){
  keyPressedY = true;
  if( Y() > 0) {
    velocityY(-speedY);
    
 }
}

void Player::down(){
  keyPressedY = true;
  if(Y() < worldHeight - playerHeight && !flipPlayerUp) {
    velocityY(speedY);
  }
  else if(Y() < worldHeight - playerWidth && flipPlayerUp){
    velocityY(speedY);
  }
}

void Player::left(){
  keyPressedX = true;
  if( X() > 0 ) {
    velocityX(-speedX);
  }
}

void Player::right(){
  keyPressedX = true;
  if( X() < worldWidth - playerWidth  && !flipPlayerUp) {
    velocityX(speedX);
  }
  else if(X() < worldWidth - playerHeight  && flipPlayerUp){
    velocityX(speedX);
  }
}

void Player::stop(){
  velocityX(0);
  velocityY(0);
}

