#include <iostream>
#include <algorithm>
#include <sstream>
#include <string>
#include <random>
#include <iomanip>
#include "sprite.h"
#include "smartSprite.h"
#include "multisprite.h"
#include "twoWaySprite.h"
#include "gamedata.h"
#include "engine.h"
#include "frameGenerator.h"
#include "player.h"
#include "hud.h"
#include "collisionStrategy.h"

Engine::~Engine() { 
  std::cout << "Terminating program" << std::endl;
  for ( Drawable* sprite : sprites ) {
    delete sprite;
  }

  for ( CollisionStrategy* strategy : strategies ) {
    delete strategy;
  }
  delete player;
  //delete sound;
}

Engine::Engine() :
  rc( RenderContext::getInstance() ),
  io( IoMod::getInstance() ),
  clock( Clock::getInstance() ),
  soundFX(new SDLSoundFX),
  renderer( rc->getRenderer() ),
  space("space", Gamedata::getInstance().getXmlInt("space/factor") ),
  planets("planets", Gamedata::getInstance().getXmlInt("planets/factor")),
  earth("earth", Gamedata::getInstance().getXmlInt("earth/factor")),
  menuu("menu", Gamedata::getInstance().getXmlInt("menu/factor")),
  win("win", Gamedata::getInstance().getXmlInt("win/factor")),
  viewport( Viewport::getInstance() ),
  player(new Player("roadster")),
  theHUD(new Hud("theHUD")),
  sprites(),
  strategies(),
  currentStrategy(0),
  currentSprite(0),
  collision(false),
  explosion(nullptr),
  doneExploding(false),
  godMode(false)
{
  //sound->stopMusic();
  int n = Gamedata::getInstance().getXmlInt("meteoR/numStars");
  //int showing = Gamedata::getInstance().getXmlInt("meteoR/numStars");
  sprites.reserve(n);
  Vector2f pos = player->getPosition();
  int w = player->getScaledWidth();
  int h = player->getScaledHeight();
  for (int i = 0; i < n; ++i) {
    SmartSprite* smart = new SmartSprite("meteoR", pos, w, h);
    sprites.push_back( smart );
    player->attach( smart );
  }
  

  strategies.push_back( new RectangularCollisionStrategy );

  Viewport::getInstance().setObjectToTrack(player->getPlayer());
  std::cout << "Loading complete" << std::endl;
}

bool Engine::menu(){
  SDL_Event event;
  const Uint8* keystate;
  bool done = false;
  Uint32 ticks = clock.getElapsedTicks();
  menuu.draw();
  viewport.draw();
  SDL_RenderPresent(renderer);
  clock.pause();
  //FrameGenerator frameGen;
  while ( !done ) {
    update(ticks);
    // The next loop polls for events, guarding against key bounce:
    while ( SDL_PollEvent(&event) ) {
      keystate = SDL_GetKeyboardState(NULL);
      if (event.type ==  SDL_QUIT) { done = true; break; }
      if(event.type == SDL_KEYDOWN) {
        if (keystate[SDL_SCANCODE_ESCAPE] || keystate[SDL_SCANCODE_Q] || keystate[SDL_SCANCODE_SPACE]) {
          clock.unpause();
          done = true;
          return false;
          //break;
        }
      }
    }

    // In this section of the event loop we allow key bounce:
    ticks = clock.getElapsedTicks();
    if ( ticks > 0 ) {
      clock.incrFrame();
    }
  }
  return true;
}

void Engine::draw() const {
  space.draw();
  planets.draw();
  earth.draw();
  theHUD->draw();

  for ( const Drawable* sprite : sprites ) {
    sprite->draw();
  }
  std::stringstream strm;
  strategies[currentStrategy]->draw();


///////////Below is FPS && BULLET/FREE COUNT///////////////

  /*
  strm<<clock.getFps();
  string fps(strm.str());
  io.writeText(fps,25,350);

  io.writeText("Elizabeth Hill", 25, 400, {0, 255, 255, 255});

  std::stringstream bl;
  bl<<"Lazer List: "<< player->bulletCount();
  io.writeText(bl.str(),25,250);

  std::stringstream fl;
  fl<<"Free List: "<< player->freeCount();
  io.writeText(fl.str(), 25, 300, {0, 255, 255, 255});
  */
  //////////////////////////////////////////////////////////


  std::stringstream bl;
  bl<< (player->getNumLeft() -1);
  io.writeText(bl.str(),25,400);


//  player->draw();
  if ( explosion ) explosion->draw();
  else   player->draw();
  if( doneExploding == true){
    io.writeText("Press R to restart", 250, 200);
    clock.pause();
  }
  viewport.draw();
  SDL_RenderPresent(renderer);
}

void Engine::checkForCollisions() {
  auto it = sprites.begin();
  while ( it != sprites.end() ) {
    bool flag = strategies[currentStrategy]->execute(*player, **it);
    if ( flag ) {
      SmartSprite* doa = *it;
      player->detach(doa);
      it = sprites.erase(it);
      explode();
    }
    else ++it;
  }
//  std::cout << "DONE Checking for collisions" << std::endl;
}

void Engine::explode() {
  if ( !explosion ) {
    Sprite sprite(player->getName(), player->getPosition(), player->getVelocity(), player->getImage());
    explosion = new ExplodingSprite(sprite);
  }
}

void Engine::update(Uint32 ticks) {
  if (godMode == false){
    checkForCollisions();
  }
  player->update(ticks);
  if ((player->getMeteorSize()< 4 ) && (player->getNumLeft() >4)){
    Vector2f pos = player->getPosition();
    int w = player->getScaledWidth();
    int h = player->getScaledHeight();
      SmartSprite* smart = new SmartSprite("meteoR", pos, w, h);
      sprites.push_back( smart );
      player->attach( smart );
  }

  if(player->getNumLeft() == 1){
    clock.pause();
    win.draw();
    viewport.draw();
    SDL_RenderPresent(renderer);
  }

    if ( explosion ) {
    explosion->update(ticks);
    if ( explosion->chunkCount() == 0 ) {
      delete explosion;
      explosion = nullptr;
      doneExploding = true;
    }
    return;
  }

  auto smart = sprites.begin();
  while ( smart != sprites.end() ) {
    (*smart)->update( ticks );
    if ( (*smart)->imDoneExploding() ) {
      smart = sprites.erase(smart);
    }
    else ++smart;
  }
  space.update();
  planets.update();
  earth.update();
  theHUD->update(ticks);
  viewport.update(); // always update viewport last
}

void Engine::switchSprite(){
  ++currentSprite;
  currentSprite = currentSprite % 2;
  if ( currentSprite ) {
    Viewport::getInstance().setObjectToTrack(player->getPlayer());
  }
  else {
    Viewport::getInstance().setObjectToTrack(sprites[0]);
  }
}

bool Engine::play() {
  SDL_Event event;
  const Uint8* keystate;
  bool done = false;
  Uint32 ticks = clock.getElapsedTicks();
  //FrameGenerator frameGen;

  while ( !done ) {
    // The next loop polls for events, guarding against key bounce:
    while ( SDL_PollEvent(&event) ) {
      keystate = SDL_GetKeyboardState(NULL);
      if (event.type ==  SDL_QUIT) { done = true; break; }
      if(event.type == SDL_KEYDOWN) {
        if (keystate[SDL_SCANCODE_ESCAPE] || keystate[SDL_SCANCODE_Q]) {
          done = true;
          break;
        }
        if ( keystate[SDL_SCANCODE_P] ) {
          if ( clock.isPaused() ) clock.unpause();
          else clock.pause();
        }
        if (keystate[SDL_SCANCODE_R]){
          clock.unpause();
          return true;
        }
        if ( keystate[SDL_SCANCODE_T] ) {
          switchSprite();
        }
        if ( keystate[SDL_SCANCODE_SPACE] ) {
          player->shoot();
          (*soundFX)[0];
        }
        if ( keystate[SDL_SCANCODE_G] ) {
          if (godMode) godMode =false;
          else if (!godMode) godMode = true; 
        }
        else if (keystate[SDL_SCANCODE_F1]) {
          theHUD->switchHud();
        }
      }
    }

    // In this section of the event loop we allow key bounce:

    ticks = clock.getElapsedTicks();
    if ( ticks > 0 ) {
      clock.incrFrame();
            if (keystate[SDL_SCANCODE_A]) {
        static_cast<Player*>(player)->left();
      }
      if (keystate[SDL_SCANCODE_D]) {
        static_cast<Player*>(player)->right();
      }
      if (keystate[SDL_SCANCODE_W]) {
        static_cast<Player*>(player)->up();
      }
      if (keystate[SDL_SCANCODE_S]) {
        static_cast<Player*>(player)->down();
      }
      draw();

      draw();
      update(ticks);
    }
  }return false;
}
