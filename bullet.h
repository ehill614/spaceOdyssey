#ifndef BULLET_H
#define BULLET_H

#include <iostream>
#include "multisprite.h"
#include "gamedata.h"

class Bullet : public MultiSprite {
public:
  Bullet(const string& name, const Vector2f& pos, const Vector2f& vel);
  Bullet(const Bullet&);
  Bullet(const string& name);
  virtual void update(Uint32 ticks);
  bool goneTooFar() const { return tooFar; }
  void reset() {
    tooFar = false;
    distance = 0;
  }

private:
  float distance;
  float maxDistance;
  bool tooFar;
};
#endif