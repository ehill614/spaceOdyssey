#ifndef PLAYER__H
#define PLAYER__H
#include <SDL.h>
#include <string>
#include <list>
#include <vector>
#include <cmath>

#include "twoWaySprite.h"
#include "bullet.h"

class SmartSprite;
class KillStrategy;
 


class Player {
public:
  Player(const std::string&);
  Player(const Player&);

  void draw() const;
  void update(Uint32 ticks);
  void attach( SmartSprite* o ) { observers.push_back(o); } 
  void detach( SmartSprite* o );
  const twoWaySprite* getPlayer() const { return &player; }
  unsigned int bulletCount() const {return bulletList.size();}
  unsigned int freeCount() const {return freeList.size();}
  unsigned int getMeteorSize() const {return observers.size();}
  int getNumLeft() {return numLeft;}
  bool shooting() const { return !bulletList.empty();}
  void checkForBeam();

  const std::string& getName() const { return player.getName(); }
  const Vector2f& getPosition() const { return player.getPosition(); }
  const Vector2f& getVelocity() const { return player.getVelocity(); }

  int getX() const { return player.getX(); }
  int getY() const { return player.getY(); }
  const Image* getImage() const { 
    return player.getImage();
  }
  int getScaledWidth()  const { 
    return player.getScaledWidth();
  } 
  int getScaledHeight()  const { 
    return player.getScaledHeight();
  } 
  const SDL_Surface* getSurface() const { 
    return player.getSurface();
  }

  void right();
  void left();
  void up();
  void down();
  void shoot();
  void stop();

  Player& operator=(const Player&) = delete;

private:
  enum FACE{LEFT, RIGHT};
  twoWaySprite player;
  Vector2f initialVelocity;
  int worldWidth;
  int worldHeight;
  std::string bulletName;
  std::list<Bullet> bullets;
  float minSpeed;
  float bulletInterval;
  float timeSinceLastFrame;
  std::list<Bullet> bulletList; //maybe pass in <Lazer*>
  std::list<Bullet> freeList;
  std::vector<KillStrategy*> strategies;
  int currentStrategy;
  FACE facing;
  bool isShot;
  int numLeft;

  std::list<SmartSprite*> observers;

};
#endif
