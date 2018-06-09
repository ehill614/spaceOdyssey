#include <iostream>
#include <cmath>
#include "bullet.h"
#include "renderContext.h"


Bullet::Bullet( const string& name, const Vector2f& pos, const Vector2f& vel) :
    MultiSprite(name, pos, vel, ImageFactory::getInstance().getImages(name)), 
    distance(0), 
    maxDistance(Gamedata::getInstance().getXmlInt(name+"/distance")), 
    tooFar(false) 
  {}

Bullet::Bullet(const string& name) :
    MultiSprite(name), 
    distance(0), 
    maxDistance(Gamedata::getInstance().getXmlInt(name+"/distance")), 
    tooFar(false) 
  {}

Bullet::Bullet(const Bullet& laz):
	MultiSprite(laz),
	distance(laz.distance),
	maxDistance(laz.maxDistance),
	tooFar(laz.tooFar)
{}

/* MultiSprite::MultiSprite(const std::string& name, const Vector2f& pos, const Vector2f& vel):
	Drawable(name, pos, vel),
	images(RenderContext::getInstance()->getImages(name)),
	currentFrame(0),
	numberOfFrames( Gamedata::getInstance().getXmlInt(name+"/frames")),
	frameInterval( Gamedata::getInstance().getXmlInt(name+"/frameSpeed")),
	timeSinceLastFrame(0),
	worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
	worldHeight(Gamedata::getInstance().getXmlInt("world/height"))
{}*/
void Bullet::update(Uint32 ticks) { 
  Vector2f pos = getPosition();
  MultiSprite::update(ticks);
  	if( getY() <0 || getY() + getScaledHeight()> worldHeight){
		tooFar = true;
	}
	if (getX() <0 || getX() >worldWidth){
		tooFar = true;
	}
  distance += ( hypot(getX()-pos[0], getY()-pos[1]) );
  if (distance > maxDistance) tooFar = true;
}

