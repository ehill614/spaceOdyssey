//hud.cpp
#include <iostream>
#include <SDL2/SDL.h>

#include <cmath>
#include <random>
#include <functional>
#include "gamedata.h"
#include "hud.h"
#include "renderContext.h"
#include "viewport.h"


Hud::Hud(const string& n, const Vector2f& pos, const Vector2f& vel,
               const Image* img):
  Drawable(n, pos, vel), 
  image( img ),
  io( IoMod::getInstance() ),
  hudUp(true),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height"))
{ }

Hud::Hud(const std::string& name) :
  Drawable(name,
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/startLoc/x"), 
                    Gamedata::getInstance().getXmlInt(name+"/startLoc/y")), 
           Vector2f(
                    Gamedata::getInstance().getXmlInt(name+"/speedX"), 
                    Gamedata::getInstance().getXmlInt(name+"/speedY")) 
           ),
  image( RenderContext::getInstance()->getImage(name) ),
  io( IoMod::getInstance() ),
  hudUp(true),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height"))
{ }

Hud::Hud(const Hud& s) :
  Drawable(s), 
  image(s.image),
  io( IoMod::getInstance() ),
  hudUp(true),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height"))
{ }

Hud& Hud::operator=(const Hud& rhs) {
  Drawable::operator=( rhs );
  image = rhs.image;
  worldWidth = rhs.worldWidth;
  worldHeight = rhs.worldHeight;
  return *this;
}

inline namespace{
  constexpr float SCALE_EPSILON = 2e-7;
}

void Hud::draw() const { 
  if(getScale() < SCALE_EPSILON) return;
  	if (hudUp == true){
  		image->draw(getX(), getY(), getScale()); 
  		io.writeText("W.up|A.left|S.down|D.right|Space.shoot|G.god|F1|HUD", 25, 0, {0, 255, 255, 255});
  }else{
  }

}

void Hud::switchHud(){
	if (hudUp == true){
		hudUp = false;
	}
	else if(hudUp == false){
		hudUp = true;
	}
}

void Hud::update(Uint32 ticks) { 
  ticks++;
  setPosition(Viewport::getInstance().getPosition());

  if ( getY() < 0) {
    setVelocityY( std::abs( getVelocityY() ) );
  }
  if ( getY() > worldHeight-getScaledHeight()) {
    setVelocityY( -std::abs( getVelocityY() ) );
  }

  if ( getX() < 0) {
    setVelocityX( std::abs( getVelocityX() ) );
  }
  if ( getX() > worldWidth-getScaledWidth()) {
    setVelocityX( -std::abs( getVelocityX() ) );
  }  
}
