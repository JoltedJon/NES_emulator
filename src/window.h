#pragma once

#include <SDL2/SDL.h>

#include <array>
#include <cstdint>

#include "SDL_pixels.h"
#include "SDL_render.h"
#include "SDL_video.h"

constexpr size_t NESWIDTH = 256;
constexpr size_t NESHEIGHT = 240;

struct Frame {
 private:
  SDL_PixelFormat* format;

 public:
  std::array<uint32_t, (NESWIDTH * NESHEIGHT)> pixels;

  Frame(SDL_Window* window)
      : format(SDL_AllocFormat(SDL_GetWindowPixelFormat(window))) {}
  ~Frame() { SDL_FreeFormat(format); }
  inline void setPixel(size_t x, size_t y, uint8_t r, uint8_t g, uint8_t b,
                       uint8_t a) {
    pixels[x + (y * NESWIDTH)] = SDL_MapRGBA(format, r, g, b, a);
  }
};

class Window {
 private:
  int screenWidth;
  int screenHeight;

  SDL_Window* window;
  SDL_Renderer* renderer;
  SDL_Surface* screen;

 public:
  Window();
  ~Window();

  void drawFrame(const Frame& f);

  void displayPatternTable(uint8_t* patternTable);

  void poll();

 private:
};
