#include <iostream>
#include <string>
#include <iomanip>
#include <algorithm>
#include "vector2f.h"
#include "multisprite.h"
#include "sprite.h"
#include "gamedata.h"
#include "manager.h"
#include "smartSprite.h"
#include "explodingSprite.h"
#include "fstream"


class ScaledSpriteComparator{ 
public:
  bool operator()( Drawable* lhs,  Drawable* rhs){
    return (static_cast<ScaledSprite*>(lhs)->getScale() < static_cast<ScaledSprite*>(rhs)->getScale());
  }
};

Manager::~Manager() { 
   for (unsigned i = 0; i < sprites.size(); ++i) {
     delete sprites[i];
  }
  sprites.clear();
  SDL_FreeSurface(ufoSurface);
}

Manager::Manager() :
  env( SDL_putenv(const_cast<char*>("SDL_VIDEO_CENTERED=center")) ),
  io( IOManager::getInstance() ),
  clock( Clock::getInstance() ),
  screen( io.getScreen() ),
  ufoSurface(io.loadAndSet(Gamedata::getInstance().getXmlStr("ufo/file") , Gamedata::getInstance().getXmlBool("ufo/transparency") )),
  worldFarBack("farBack", Gamedata::getInstance().getXmlInt("farBack/factorX") ,Gamedata::getInstance().getXmlInt("farBack/factorY")),
  worldBack("back", Gamedata::getInstance().getXmlInt("back/factorX") ,Gamedata::getInstance().getXmlInt("back/factorY")),
  worldBackSecond("backSecond", Gamedata::getInstance().getXmlInt("backSecond/factorX"),Gamedata::getInstance().getXmlInt("backSecond/factorY") ),
  worldMiddle("middle", Gamedata::getInstance().getXmlInt("middle/factorX") ,Gamedata::getInstance().getXmlInt("middle/factorY")),
  worldFront("front", Gamedata::getInstance().getXmlInt("front/factorX"),Gamedata::getInstance().getXmlInt("front/factorY") ),
  viewport( Viewport::getInstance() ),
  makeVideo( false ),
  sound(),
  frameCount( 0 ),
  username(  Gamedata::getInstance().getXmlStr("username") ),
  title( Gamedata::getInstance().getXmlStr("screenTitle") ),
  frameMax( Gamedata::getInstance().getXmlInt("frameMax") ),
  hudFlag(2),
  hudDisplay(),
  sprites(),
  healthBar(),
  score(0),
  numberOfUfo(Gamedata::getInstance().getXmlInt("ufo/numOfUfo")),
  healthRemaining(true),
  playerExist(true),
  godMode(false)
{
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    throw string("Unable to initialize SDL: ");
  }
  SDL_WM_SetCaption(title.c_str(), NULL);
  atexit(SDL_Quit);
  sprites.push_back(new Player("spaceship"));
  makeScaledPlanets();
  viewport.setObjectToTrack(sprites[0]);
}

void Manager::makeScaledPlanets() {
  for (unsigned i = 0; i < numberOfUfo; ++i){
    sprites.push_back(new ScaledSprite("ufo",ufoSurface));
    }
  sort(sprites.begin()+1 , sprites.end() , ScaledSpriteComparator());
}

void Manager::draw() const {
  worldFarBack.draw();
  worldBack.draw();
  worldBackSecond.draw();
  worldMiddle.draw();
  worldFront.draw();
  for (unsigned i = 1; i < sprites.size(); ++i){
    sprites[i]->draw();
  }
  
  if(!(static_cast<Player *>(sprites[0])->explosionDone()))
   sprites[0]->draw();
  else clock.pause();
  if(godMode) io.printMessageAt("In GodMode ", 375, 80);
  if(!healthRemaining) {
    if(score>6000){
      io.printMessageAt("You Won ", 360, 150);
    }
    else { io.printMessageAt("You lost ", 360, 150);}
    
    io.printMessageValueAt("Your final score is ", score, 360, 200);
    io.printMessageAt("Press R to restart ", 360, 230);
  }
   else io.printMessageValueAt("Your score is ", score, 360, 420);
  viewport.draw();
  drawHud();
  SDL_Flip(screen);
}



void Manager::drawHud() const  {
  Player *player = static_cast<Player * >(sprites[0]);
  if(clock.getSeconds() <3 && hudFlag==2){
    hudDisplay.drawHUD(screen, io, clock.getSeconds(),clock.getFps(),player->getActiveBullet(),player->getFreeBullet());
   }
  else if( hudFlag==1) {
    hudDisplay.drawHUD(screen, io,clock.getSeconds(),clock.getFps(),player->getActiveBullet(),player->getFreeBullet());
  }
  healthBar.draw();
}

// Move this to IOManager
void Manager::makeFrame() {
  std::stringstream strm;
  strm << "frames/" << username<< '.' 
       << std::setfill('0') << std::setw(4) 
       << frameCount++ << ".bmp";
  std::string filename( strm.str() );
  std::cout << "Making frame: " << filename << std::endl;
  SDL_SaveBMP(screen, filename.c_str());
}

//checks for collision
void Manager::checkForCollisions(Uint32 &ticks) {
 Player *player = static_cast<Player *>(sprites[0]);
 std::vector<Drawable*>::iterator pos = sprites.begin() + 1;
  while(pos != sprites.end()){
    Drawable* sprite = *pos;
    if(player->collidWith(*pos)){
      sprite->explode();
      score=score+100;
      if(sprite->explosionDone()){
        *pos=new SmartSprite(sprite->getName(), sprite->getPosition() + Vector2f(100,100), *player); 
        delete sprite;
      }
      pos++; 
      sound[1];
    }
    
    else if(static_cast<Player*>(sprites[0])->playerCollidWith(*pos)){
      sprite->explode();
      score=score+100;
      if(sprite->explosionDone()){
        *pos=new SmartSprite(sprite->getName(), sprite->getPosition() + Vector2f(100,100), *player); 
         delete sprite; 
         
      }
      sound[1];
      pos++;
      if(!godMode) healthBar.update(ticks);
      if(healthBar.getCurrentHealth() == 0) static_cast<Player *>(sprites[0])->explode();
    }
    
    else ++pos;
  }
}

void Manager::update() {
  clock.update();
  worldFarBack.update();
  worldBack.update();
  worldBackSecond.update();
  worldMiddle.update();
  worldFront.update();
  Uint32 ticks = clock.getTicksSinceLastFrame();
  for (unsigned int i = 1; i < sprites.size(); ++i) {
    sprites[i]->update(ticks);
  }
  checkForCollisions(ticks);
  if ( makeVideo && frameCount < frameMax ) {
    makeFrame();
  }
 if(healthBar.getCurrentHealth() == 0) 
   healthRemaining = false;  
  sprites[0]->update(ticks);
  
  viewport.update();  
}

void Manager::play() {
  SDL_Event event;
  bool done = false;
  bool keyCatch = false;
  clock.start();

  while ( not done ) {
      SDL_PollEvent(&event);
      Uint8 *keystate = SDL_GetKeyState(NULL);
      if (event.type ==  SDL_QUIT) { done = true; break; }
      if(event.type == SDL_KEYUP) { 
        keyCatch = false; 
      }
      if(event.type == SDL_KEYDOWN || event.type == SDL_KEYUP) {
        if (keystate[SDLK_ESCAPE] || keystate[SDLK_q]) {
          done = true;
          break;
        }
        if ( keystate[SDLK_p] && !keyCatch) {
          keyCatch = true;
          if ( clock.isPaused() ) clock.unpause();
          else clock.pause();
        }
        
        if (keystate[SDLK_F1]  && !keyCatch) {
          keyCatch =true;
          if(hudFlag==2 or hudFlag==1) hudFlag=0;
          
          else hudFlag =1;
        }
        if ( keystate[SDLK_k] && !keyCatch) {
          keyCatch = true;
          healthBar.reset();
        }
        if (keystate[SDLK_SPACE]) {
        
          if(healthRemaining) sound[0];
           static_cast<Player*>(sprites[0])->shoot();
           
        }
        if (keystate[SDLK_a]) {
          static_cast<Player*>(sprites[0])->left();
         
        }
        if (keystate[SDLK_z])  {
          static_cast<Player*>(sprites[0])->flipLeft();
        }
        if (keystate[SDLK_c] ) {
          static_cast<Player*>(sprites[0])->flipRight();
        }
        
        if (keystate[SDLK_d]) {
          static_cast<Player*>(sprites[0])->right();
        }
        if (keystate[SDLK_x]  ) {
          static_cast<Player*>(sprites[0])->flipUp();
        }
        
        if (keystate[SDLK_w] ) {
          static_cast<Player*>(sprites[0])->up();
        }
        if (keystate[SDLK_s] ) {
          static_cast<Player*>(sprites[0])->down();
        }
        
        if(keystate[SDLK_g] &&  !keyCatch) {
          keyCatch =true;
          godMode =!godMode;
        }
        
        if (keystate[SDLK_F4] && !makeVideo  && !keyCatch) {
          keyCatch = true;
          std::cout << "Making video frames" << std::endl;
          makeVideo = true;
        }
        if ((keystate[SDLK_a] && keystate[SDLK_d]) || (keystate[SDLK_w] && keystate[SDLK_s])) {
         static_cast<Player*>(sprites[0])->stop();
         }
        
       if (keystate[SDLK_r] ) {
        godMode=false;
      	score =0;
        clock.reset();
        healthRemaining = true;
        healthBar.reset();
        viewport.setObjectToTrack(sprites[0]);
        std::vector<Drawable*>::iterator bt = sprites.begin();
        while(bt != sprites.end()){
          delete *bt;
          bt = sprites.erase(bt);
        }
        sprites.push_back(new Player("spaceship"));
      	for(unsigned i = 0; i < numberOfUfo; i++){
    		sprites.push_back( new ScaledSprite("ufo",ufoSurface) );
  		}
  		sort(sprites.begin()+1, sprites.end(),ScaledSpriteComparator());
  		viewport.setObjectToTrack(sprites[0]);

      }
      }
    
    draw();
    update();
  }
}
