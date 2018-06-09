#include <vector>
#include <SDL.h>
#include "ioMod.h"
#include "renderContext.h"
#include "clock.h"
#include "world.h"
#include "viewport.h"
#include "explodingSprite.h" 
#include "soundFX.h"


class CollisionStrategy;
class SmartSprite;
class Player;
class Hud;

class Engine {
public:
  Engine ();
  Engine(const Engine&) = delete;
  Engine& operator=(const Engine&) = delete;
  ~Engine ();
  bool play();
  bool menu();
  //void drawMenu();
  void switchSprite();

  void explode();
  
  bool isExploding() const { return explosion; }
  bool imDoneExploding() const { return doneExploding; }

private:
  const RenderContext* rc;
  const IoMod& io;
  Clock& clock;
  SDLSoundFX *soundFX;

  SDL_Renderer * const renderer;
  World space;
  World planets;
  World earth;
  World menuu;
  World win;

  Viewport& viewport;

  Player*   player;
  Hud* theHUD;
  std::vector<SmartSprite*> sprites;
  std::vector<CollisionStrategy*> strategies;
  int currentStrategy;
  int currentSprite;
  bool collision;

  ExplodingSprite* explosion;
  bool doneExploding;
  bool godMode;

  void draw() const;
  void update(Uint32);

  void printScales() const;
  void checkForCollisions();
};
