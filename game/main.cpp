// Amit Nanda        Game Construction
#include "manager.h"

#include "smartSprite.h"
float SmartSprite::safeDistance  = Gamedata::getInstance().getXmlInt("safeDistance");
int main(int, char*[]) {
   try {
      Manager manager;
      manager.play();
   }
   catch (const string& msg) { std::cout << msg << std::endl; }
   catch (...) {
      std::cout << "Oops, someone threw an exception!" << std::endl;
   }
   return 0;
}
