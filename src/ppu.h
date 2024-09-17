#pragma once

#include <array>
#include <cstdint>
#include <vector>

#include "window.h"

struct Color {
  uint8_t r;
  uint8_t g;
  uint8_t b;
};

class PPU {
 private:
  // MMIO Registers

  // Control - Write Only
  uint8_t VRamInc;
  uint16_t spritePTAddr;
  uint16_t backgroundPTAddr;
  bool spriteSize;      // false = 8x8, true = 8x16
  bool masterSlaveSel;  // TODO not sure what this does
  bool generateNMI;     // Generates at the start of VBlank interval

  // Mask - Write Only
  bool maskGreyscale;
  bool maskShowLeftBackground;
  bool maskShowLeftSprite;
  bool maskShowBackground;
  bool maskShowSprites;
  bool maskEmphRed;
  bool maskEmphGreen;
  bool maskEmphBlue;

  uint8_t status;   // Read only
  uint8_t OAMAddr;  // Write only

  uint8_t data;

  uint8_t latch;

  uint16_t rv;
  uint16_t rt;
  uint8_t rx;

  bool writeLatch;

  // Memory
  std::array<uint8_t, 0x4000> memory;
  std::array<uint8_t, 256> OAMMemory;

  std::array<Color, 64> palette = {
      {{0x80, 0x80, 0x80}, {0x00, 0x3D, 0xA6}, {0x00, 0x12, 0xB0},
       {0x44, 0x00, 0x96}, {0xA1, 0x00, 0x5E}, {0xC7, 0x00, 0x28},
       {0xBA, 0x06, 0x00}, {0x8C, 0x17, 0x00}, {0x5C, 0x2F, 0x00},
       {0x10, 0x45, 0x00}, {0x05, 0x4A, 0x00}, {0x00, 0x47, 0x2E},
       {0x00, 0x41, 0x66}, {0x00, 0x00, 0x00}, {0x05, 0x05, 0x05},
       {0x05, 0x05, 0x05}, {0xC7, 0xC7, 0xC7}, {0x00, 0x77, 0xFF},
       {0x21, 0x55, 0xFF}, {0x82, 0x37, 0xFA}, {0xEB, 0x2F, 0xB5},
       {0xFF, 0x29, 0x50}, {0xFF, 0x22, 0x00}, {0xD6, 0x32, 0x00},
       {0xC4, 0x62, 0x00}, {0x35, 0x80, 0x00}, {0x05, 0x8F, 0x00},
       {0x00, 0x8A, 0x55}, {0x00, 0x99, 0xCC}, {0x21, 0x21, 0x21},
       {0x09, 0x09, 0x09}, {0x09, 0x09, 0x09}, {0xFF, 0xFF, 0xFF},
       {0x0F, 0xD7, 0xFF}, {0x69, 0xA2, 0xFF}, {0xD4, 0x80, 0xFF},
       {0xFF, 0x45, 0xF3}, {0xFF, 0x61, 0x8B}, {0xFF, 0x88, 0x33},
       {0xFF, 0x9C, 0x12}, {0xFA, 0xBC, 0x20}, {0x9F, 0xE3, 0x0E},
       {0x2B, 0xF0, 0x35}, {0x0C, 0xF0, 0xA4}, {0x05, 0xFB, 0xFF},
       {0x5E, 0x5E, 0x5E}, {0x0D, 0x0D, 0x0D}, {0x0D, 0x0D, 0x0D},
       {0xFF, 0xFF, 0xFF}, {0xA6, 0xFC, 0xFF}, {0xB3, 0xEC, 0xFF},
       {0xDA, 0xAB, 0xEB}, {0xFF, 0xA8, 0xF9}, {0xFF, 0xAB, 0xB3},
       {0xFF, 0xD2, 0xB0}, {0xFF, 0xEF, 0xA6}, {0xFF, 0xF7, 0x9C},
       {0xD7, 0xE8, 0x95}, {0xA6, 0xED, 0xAF}, {0xA2, 0xF2, 0xDA},
       {0x99, 0xFF, 0xFC}, {0xDD, 0xDD, 0xDD}, {0x11, 0x11, 0x11},
       {0x11, 0x11, 0x11}}};

  // Header information
  uint8_t characterSize;  // multiply by 8KB -> 0x2000 bytes
  enum Nametable : bool { Vertical, Horizontal } mirroring;
  uint16_t mapper;

  std::vector<char>* rom;
  size_t characterStart;

  // SDL Window wrapper
  Window* window;

  // Rendering
  int dot;
  enum class PPUScanline {
    PreRender,
    Render,
    PostRender,
    VBlank,
  } scanlineState;
  int scanline;

  bool evenFrame;
  bool vBlank;
  bool spriteZeroHit;

  // Internal to rendering
  std::array<uint8_t, 2> bgShiftLow;
  std::array<uint8_t, 2> bgShiftHigh;

  std::array<uint8_t, 8> spriteShiftLow;
  std::array<uint8_t, 8> spriteShiftHigh;
  std::array<uint8_t, 8> spriteAttributeShift;

 public:
  PPU(Window* window);

  void loadRom(std::vector<char>& inRom, bool trainerPresent);

  bool doCycle();

  inline void display() { window->displayPatternTable(memory.data()); }

  // Memory Mapped IO
  uint8_t readctrl();
  uint8_t readmask();
  uint8_t readstatus();
  uint8_t readOAMAddr();
  uint8_t readOAMData();
  uint8_t readscroll();
  uint8_t readaddr();
  uint8_t readdata();
  uint8_t readOAMDMA();

  void writectrl(uint8_t val);
  void writemask(uint8_t val);
  void writestatus(uint8_t val);
  void writeOAMAddr(uint8_t val);
  void writeOAMData(uint8_t val);
  void writescroll(uint8_t val);
  void writeaddr(uint8_t val);
  void writedata(uint8_t val);

  void dump() const;

 private:
  uint16_t mapAddr(uint16_t addr);

  void preRenderStage();
  void renderStage();
  void postRenderStage();
  void vBlankStage();
};
