#include "window.h"

#include <cstdint>
#include <vector>

#include "SDL.h"
#include "SDL_error.h"
#include "SDL_render.h"
#include "SDL_stdinc.h"
#include "SDL_surface.h"
#include "SDL_video.h"
#include "utils.h"

Window::Window()
    : screenWidth(NESWIDTH),
      screenHeight(NESHEIGHT),
      window(nullptr),
      renderer(nullptr),
      screen(nullptr) {
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    error(("SDL could not initalize! SDL Error: " + std::string(SDL_GetError()))
              .c_str());
  }

  window = SDL_CreateWindow("NES Emulator", SDL_WINDOWPOS_UNDEFINED,
                            SDL_WINDOWPOS_UNDEFINED, screenWidth, screenHeight,
                            SDL_WINDOW_SHOWN);
  if (window == nullptr) {
    error(("Window could not be created! SDL_Error: " +
           std::string(SDL_GetError()))
              .c_str());
  }

  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  if (renderer == nullptr) {
    error(("Renderer could not be created! SDL_Error: " +
           std::string(SDL_GetError()))
              .c_str());
  }

  screen = SDL_GetWindowSurface(window);

  SDL_FillRect(screen, NULL,
               SDL_MapRGBA(screen->format, 0xDD, 0x00, 0x30, 0xFF));

  SDL_UpdateWindowSurface(window);
}

Window::~Window() {
  // Deletes window and surface
  SDL_DestroyWindow(window);
  SDL_Quit();
}

void Window::drawFrame(const Frame& f) {
  SDL_LockSurface(screen);
  SDL_memcpy(screen->pixels, f.pixels.data(),
             NESWIDTH * NESHEIGHT * sizeof(uint32_t));

  SDL_UnlockSurface(screen);

  SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, screen);

  int windowWidth, windowHeight;
  SDL_GetWindowSize(window, &windowWidth, &windowHeight);

  SDL_RenderClear(renderer);

  SDL_Rect dstRect = {0, 0, windowWidth, windowHeight};
  SDL_RenderCopy(renderer, texture, nullptr, &dstRect);
  SDL_RenderPresent(renderer);
  SDL_DestroyTexture(texture);
}

void Window::poll() {
  SDL_Event e;
  while (SDL_PollEvent(&e)) {
    if (e.type == SDL_QUIT) exit(0);
  }
}

void Window::displayPatternTable(uint8_t* patternTable) {
  enum PixelFormat { TRANSPARENT = 0, COLOR1 = 1, COLOR2 = 2, COLOR3 = 3 };

  int x = 0;
  int y = 0;

  Frame f(window);
  memset(f.pixels.data(), 0, f.pixels.size() * sizeof(uint32_t));

  int num = 0;
  // Pattern Table 1
  for (size_t i = 0; i < 0x1000; i += 16) {
    std::vector<PixelFormat> pixels;
    for (int byte = 0; byte < 8; ++byte) {
      for (int bit = 0; bit < 8; ++bit) {
        uint8_t bit1 = (patternTable[i + byte] >> (7 - bit)) & 0x01;
        uint8_t bit2 = ((patternTable[i + byte + 8] >> (7 - bit)) & 0x01) << 1;
        pixels.push_back(static_cast<PixelFormat>(bit1 | bit2));
      }
    }

    for (auto pixel : pixels) {
      switch (pixel) {
        case TRANSPARENT:
          f.setPixel(x, y, 0xFF, 0xFF, 0xFF, 0x00);
          break;
        case COLOR1:
          f.setPixel(x, y, 0xB6, 0xB6, 0xB6, 0xFF);
          break;
        case COLOR2:
          f.setPixel(x, y, 0x67, 0x67, 0x67, 0xFF);
          break;
        case COLOR3:
          f.setPixel(x, y, 0x00, 0x00, 0x00, 0xFF);
          break;
      }

      ++x;
      if (x % 8 == 0) {
        x -= 8;
        y++;
      }
    }

    ++num;
    if (num == 16) {
      num = 0;
      x = 0;
      y += 8;
    } else if (x + 8 >= screenWidth) {
      x = 0;
      y += 8;
    } else {
      y -= 8;
      x += 8;
    }
    if (y >= screenHeight) {
      break;
    }
  }

  // Pattern Table 2
  y = 0;
  x = 128;
  for (size_t i = 0x1000; i < 0x2000; i += 16) {
    std::vector<PixelFormat> pixels;
    for (int byte = 0; byte < 8; ++byte) {
      for (int bit = 0; bit < 8; ++bit) {
        uint8_t bit1 = (patternTable[i + byte] >> (7 - bit)) & 0x01;
        uint8_t bit2 = ((patternTable[i + byte + 8] >> (7 - bit)) & 0x01) << 1;
        pixels.push_back(static_cast<PixelFormat>(bit1 | bit2));
      }
    }

    for (auto pixel : pixels) {
      switch (pixel) {
        case TRANSPARENT:
          f.setPixel(x, y, 0xFF, 0xFF, 0xFF, 0x00);
          break;
        case COLOR1:
          f.setPixel(x, y, 0xB6, 0xB6, 0xB6, 0xFF);
          break;
        case COLOR2:
          f.setPixel(x, y, 0x67, 0x67, 0x67, 0xFF);
          break;
        case COLOR3:
          f.setPixel(x, y, 0x00, 0x00, 0x00, 0xFF);
          break;
      }

      ++x;
      if (x % 8 == 0) {
        x -= 8;
        y++;
      }
    }

    if (x + 8 >= screenWidth) {
      x = 128;
      y += 8;
    } else {
      y -= 8;
      x += 8;
    }
    if (y >= screenHeight) {
      break;
    }
  }

  drawFrame(f);
}
