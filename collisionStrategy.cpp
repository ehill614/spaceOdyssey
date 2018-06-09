#include <vector>
#include <algorithm>
#include "ioMod.h"
#include "collisionStrategy.h"
#include "viewport.h"
#include "renderContext.h"


void RectangularCollisionStrategy::draw() const {
  //IOmod::getInstance().writeText("Strategy: Rectangular", 500, 30);
}

bool RectangularCollisionStrategy::execute(
      const Player& sprite1, const Drawable& sprite2) const {
  float left1 = sprite1.getX();
  float left2 = sprite2.getX();

  float right1 = left1+sprite1.getScaledWidth();
  float right2 = left2+sprite2.getScaledWidth();
  if ( right1 < left2 ) return false;
  if ( left1 > right2 ) return false;
  float top1 = sprite1.getY();
  float top2 = sprite2.getY();
  float bottom1 = top1+sprite1.getScaledHeight();
  float bottom2 = top2+sprite2.getScaledHeight();
  if ( bottom1 < top2 ) return false;
  if ( bottom2 < top1 ) return false;
  return true;
}

bool KillCollisionStrategy::executed(
  const Drawable& sprite1, const Drawable& sprite2) const {
  float left1 = sprite1.getX();
  float left2 = sprite2.getX();

  float right1 = left1+sprite1.getScaledWidth();
  float right2 = left2+sprite2.getScaledWidth();
  if ( right1 < left2 ) return false;
  if ( left1 > right2 ) return false;
  float top1 = sprite1.getY();
  float top2 = sprite2.getY();
  float bottom1 = top1+sprite1.getScaledHeight();
  float bottom2 = top2+sprite2.getScaledHeight();
  if ( bottom1 < top2 ) return false;
  if ( bottom2 < top1 ) return false;
  return true;
}

void KillCollisionStrategy::drawed() const {
  //IOmod::getInstance().writeText("Strategy: Rectangular", 500, 30);
}
