#include <cmath>
#include "drawable.h"
#include "player.h"

class CollisionStrategy {
public:
  virtual bool execute(const Player&, const Drawable&) const = 0;
  virtual void draw() const = 0;
  virtual ~CollisionStrategy() {}
};

class RectangularCollisionStrategy : public CollisionStrategy {
public:
  RectangularCollisionStrategy() {}
  virtual bool execute(const Player&, const Drawable&) const;
  virtual void draw() const;
};

class KillStrategy {
public:
  virtual bool executed(const Drawable&, const Drawable&) const = 0;
  virtual void drawed() const = 0;
  virtual ~KillStrategy() {}
};

class KillCollisionStrategy : public KillStrategy {
public:
  KillCollisionStrategy() {}
  virtual bool executed(const Drawable&, const Drawable&) const;
  virtual void drawed() const;
};

