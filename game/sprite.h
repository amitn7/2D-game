#ifndef SPRITE__H
#define SPRITE__H
#include <string>
#include "drawable.h"


class Sprite :public Drawable {
public:
  Sprite(const std::string&, 
         const Vector2f& pos, const Vector2f& vel, const Frame*);
  Sprite(const std::string&, const Vector2f& pos, const Vector2f& vel);
  Sprite(const Sprite& s);
  Sprite(const std::string&);
  virtual ~Sprite() {  } 
  Sprite& operator=(const Sprite&);

  virtual const Frame* getFrame() const { return frame; }
  virtual void draw() const;

  virtual void update(Uint32 ticks);
  void setFlip(bool f) { rotatingSprite = f; }

protected:
  const Frame * frame;
  int frameWidth;
  int frameHeight;
  int worldWidth;
  int worldHeight;
  bool rotatingSprite;
  int getDistance(const Sprite*) const;
};
#endif
