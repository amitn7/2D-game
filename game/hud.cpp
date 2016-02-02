#include<iostream>
#include "aaline.h"
#include "hud.h"
#include "gamedata.h"
#include<string>

hud::hud(): 
  HUD_HEIGHT(Gamedata::getInstance().getXmlInt("hudDetails/hudDimensions/y")),
  HUD_WIDTH(Gamedata::getInstance().getXmlInt("hudDetails/hudDimensions/x")),
  x(Gamedata::getInstance().getXmlInt("hudDetails/hudLocation/x")),
  y(Gamedata::getInstance().getXmlInt("hudDetails/hudLocation/y")),
  secondsLocation(Gamedata::getInstance().getXmlInt("textPositioning/secondsLocation/x"),
           Gamedata::getInstance().getXmlInt("textPositioning/secondsLocation/y")),
  fpsLocation(Gamedata::getInstance().getXmlInt("textPositioning/fpsLocation/x"),
           Gamedata::getInstance().getXmlInt("textPositioning/fpsLocation/y")),
  playerLocation(Gamedata::getInstance().getXmlInt("textPositioning/playerMovementLocation/x"),
           Gamedata::getInstance().getXmlInt("textPositioning/playerMovementLocation/y")),
  togHudLocation(Gamedata::getInstance().getXmlInt("textPositioning/toggleHUDLocation/x"),
           Gamedata::getInstance().getXmlInt("textPositioning/toggleHUDLocation/y")),
  healthBarLocation(Gamedata::getInstance().getXmlInt("textPositioning/resetHealthLocation/x"),
           Gamedata::getInstance().getXmlInt("textPositioning/resetHealthLocation/y")),
  activeListLocation(Gamedata::getInstance().getXmlInt("textPositioning/activeListLocation/x"),
           Gamedata::getInstance().getXmlInt("textPositioning/activeListLocation/y")),
  freeListLocation(Gamedata::getInstance().getXmlInt("textPositioning/freeListLocation/x"),
           Gamedata::getInstance().getXmlInt("textPositioning/freeListLocation/y"))
  {}

void hud::drawHUD(SDL_Surface* const & screen, const IOManager & io, int seconds, int fps, int activeBullet, int freeBullet)const{
  const Uint32 GREEN  = SDL_MapRGB(screen->format,0,0xff,0);
  Draw_AALine(screen,x,y+HUD_HEIGHT/2,x+HUD_WIDTH,y+HUD_HEIGHT/2,HUD_HEIGHT,0xff,0,0xff,0xff/2);
  io.printMessageValueAt("Seconds: ", seconds, secondsLocation[0], secondsLocation[1]);
  io.printMessageValueAt("fps: ", fps, fpsLocation[0], fpsLocation[1]);
  io.printMessageAt("Use A to move the player left", playerLocation[0], playerLocation[1]);
  io.printMessageAt("Use D to move the player right", playerLocation[0], playerLocation[1]+20);
  io.printMessageAt("Use S to move the player down", playerLocation[0], playerLocation[1]+40);
  io.printMessageAt("Use W to move the player up", playerLocation[0], playerLocation[1]+60);
  io.printMessageAt("Use C to flip player left", playerLocation[0], playerLocation[1]+80);
  io.printMessageAt("Use Z to flip player right", playerLocation[0], playerLocation[1]+100);
  io.printMessageAt("Use X to flip player Up ", playerLocation[0], playerLocation[1]+120);
  io.printMessageAt("Use Space to shoot", playerLocation[0], playerLocation[1]+140);
  io.printMessageAt("Press G to toggle God Mode", playerLocation[0], playerLocation[1]+160);
  io.printMessageAt("Press F1 to toggle HUD", togHudLocation[0], togHudLocation[1]);
  io.printMessageAt("Press R to reset game",healthBarLocation[0], healthBarLocation[1]);
  io.printMessageValueAt("Active Bullet List: ", activeBullet, activeListLocation[0], activeListLocation[1]);
  io.printMessageValueAt("Free List: ", freeBullet, freeListLocation[0], freeListLocation[1]);
  io.printMessageAt("score atleast 6000 to WIN",healthBarLocation[0], healthBarLocation[1]+60);
  io.printMessageAt("press K to reset Health Bar",healthBarLocation[0], healthBarLocation[1]+80);
  Draw_AALine(screen, x,y,x+HUD_WIDTH,y, GREEN);
}


