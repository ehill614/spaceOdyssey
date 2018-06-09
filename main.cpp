// Brian Malloy        Data-Driven Object oriented Game Construction
#include "engine.h"
#include "SDL.h"
#include "sound.h"

//ImageFactory*  ImageFactory::instance = NULL;
RenderContext* RenderContext::instance = NULL;

int main(int, char*[]) {
   SDLSound sound;
   bool keepPlaying = true;
   bool menuOn = true;
   try {
      while (keepPlaying){
         Engine* engine = new Engine;
         while (menuOn){
            menuOn = engine->menu();
         }
         keepPlaying = engine->play();
         delete engine;
      }delete RenderContext::getInstance();
      SDL_Quit();
   }
   catch (const string& msg) { std::cout << msg << std::endl; }
   catch (...) {
      std::cout << "Oops, someone threw an exception!" << std::endl;
   }
   return 0;
}
