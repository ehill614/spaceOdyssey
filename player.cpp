#include "player.h"
#include "gamedata.h"
#include "smartSprite.h"
#include "renderContext.h"
#include "collisionStrategy.h"


 

Player::Player( const std::string& name) :
  player(name),
  initialVelocity(player.getVelocity()),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
  bulletName( Gamedata::getInstance().getXmlStr(name+"/bullet") ),
  bullets(),
  minSpeed( Gamedata::getInstance().getXmlInt(bulletName+"/speedX") ),
  bulletInterval(Gamedata::getInstance().getXmlInt(bulletName+"/interval")),
  timeSinceLastFrame(0),
  bulletList(),
  freeList(),
  strategies(),
  currentStrategy(0),
  facing(RIGHT),
  isShot(false),
  numLeft( Gamedata::getInstance().getXmlInt("numberOfMeteors")),
  observers()
{ 
  strategies.push_back( new KillCollisionStrategy );

}

void Player::stop() { 
  player.setVelocity( Vector2f(0, 0) );
}
void Player::detach( SmartSprite* o ) {
  std::list<SmartSprite*>::iterator ptr = observers.begin();
  while ( ptr != observers.end() ) {
    if ( *ptr == o ) {
      delete *ptr;
      ptr = observers.erase(ptr);
      return;
    }
    ++ptr;
  }
}

void Player::checkForBeam() {
//  std::cout << "Checking for Beam" << std::endl;
  auto observer = observers.begin();
  while ( observer != observers.end() ) {
    auto it = bulletList.begin();
    while ( it != bulletList.end() ) {
      if ( strategies[currentStrategy]->executed(*it, **observer) ) {
        numLeft--;
        (*observer)->explode();
        observer = observers.erase(observer);
        it = bulletList.erase(it);
        break;
      }
      else ++it;
    }
    ++observer;
  }
//  std::cout << "DONE Checking for Beam" << std::endl;
}

void Player::right() { 
  facing = RIGHT;
  if ( player.getX() < worldWidth-getScaledWidth()) {
    player.setVelocityX(initialVelocity[0]);
  }
} 
void Player::left()  { 
  facing = LEFT;
  if ( player.getX() > 0) {
    player.setVelocityX(-initialVelocity[0]);
  }
} 
void Player::up()    { 
  if ( player.getY() > 0) {
    player.setVelocityY( -initialVelocity[1] );
  }
} 
void Player::down()  { 
  if ( player.getY() < worldHeight-getScaledHeight()) {
    player.setVelocityY( initialVelocity[1] );
  }
}

void Player::shoot() { 
  if ( timeSinceLastFrame < bulletInterval ) return;
  float deltaX = getScaledWidth();
  float deltaY = getScaledHeight()/2;
    if (freeList.empty()){

    Bullet l(bulletName);
    bulletList.push_back(l);
  }else{
    Bullet la = freeList.front();
    freeList.pop_front();
    la.reset();
    if (facing == RIGHT){
      la.setVelocity(getVelocity() + Vector2f(minSpeed, 0));
      la.setPosition(getPosition() +Vector2f(deltaX, deltaY));
    }else if(facing == LEFT){
      la.setVelocity(getVelocity() - Vector2f(minSpeed, 0));
      la.setPosition(getPosition() -Vector2f(deltaX - Gamedata::getInstance().getXmlInt("roadster/imageWidth"), deltaY - Gamedata::getInstance().getXmlInt("roadster/imageHeight")));
    }
    bulletList.push_back(la);
  }
  
}

void Player::draw() const { 
  player.draw();
  for ( const Bullet& bullet : bulletList ) {
    bullet.draw();
  }
}


void Player::update(Uint32 ticks) { 
  player.update(ticks);
  checkForBeam();
  timeSinceLastFrame += ticks;
  std::list<SmartSprite*>::iterator ptr = observers.begin();
  while ( ptr != observers.end() ) {
    (*ptr)->setPlayerPos( getPosition() );
    ++ptr;
  }
  for ( Bullet& bullet : bulletList ) {
    bullet.update(ticks);
  }
  std::list<Bullet>::iterator ptr2 = bulletList.begin();
  while( ptr2 != bulletList.end()){
    ptr2->update(ticks);
    if(ptr2->goneTooFar()){
      freeList.push_back(*ptr2);
      ptr2 = bulletList.erase(ptr2);
    }else{
      ++ptr2;
    }
  }
  stop();

}
