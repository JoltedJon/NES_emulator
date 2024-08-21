#include <iostream>

#include "sdlpp.hpp"

int main(int argc, char** argv) {
  sdl::Init init(SDL_INIT_EVERYTHING);
  sdl::Window w("NES Emulator", 50, 50, 500, 500, SDL_WINDOW_RESIZABLE);
  SDL_SetHint(SDL_HINT_RENDER_VSYNC, "1");

  bool done = false;

  sdl::EventHandler e;
  e.quit = [&done](const SDL_QuitEvent&) { done = true; };

  while (!done) {
    while (e.poll()) {
    }
  }

  return 0;
}
