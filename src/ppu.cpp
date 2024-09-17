#include "ppu.h"

#include <SDL2/SDL.h>

#include <cstring>
#include <fstream>
#include <vector>

#include "utils.h"

/*
1 CPU Cycle = 3 PPU Cycles
https://www.nesdev.org/wiki/PPU_rendering
Line By Line Timing
*/

constexpr int VISIBLE_SCANLINE_DOTS = 256;
constexpr int NUM_SCANLINE_CYCLES = 341;
constexpr int SCANLINE_END_CYCLE = 340;

PPU::PPU(Window* window)
    : status(0xA0),
      OAMAddr(0x00),
      data(0x00),
      rv(0x0000),
      rt(0x0000),
      rx(0x00),
      window(window),
      dot(0),
      scanlineState(PPUScanline::PreRender),
      scanline(0),
      evenFrame(true) {}

void PPU::loadRom(std::vector<char>& inRom, bool trainerPresent) {
  characterSize = inRom[5];
  mirroring = (inRom[6] & 0x01) ? Horizontal : Vertical;
  mapper = static_cast<uint8_t>(inRom[6]) >> 4;
  mapper |= static_cast<uint8_t>(inRom[7]) & 0xF0;
  this->rom = &inRom;

  characterStart = 16 + (trainerPresent * 512) + (inRom[4] * 0x4000);

  // TODO fix this
  if (characterSize > 1) {
    warning(("Character size " + std::to_string(characterSize * 8) +
             "KB Not yet supported. Truncating to 8KB")
                .c_str());
    characterSize = 1;
  }

  memcpy(memory.data(), inRom.data() + characterStart, characterSize * 0x2000);
}

bool PPU::doCycle() {
  switch (scanlineState) {
    case PPUScanline::PreRender:
      preRenderStage();
      break;
    case PPUScanline::Render:
      renderStage();
      break;
    case PPUScanline::PostRender:
      postRenderStage();
      break;
    case PPUScanline::VBlank:
      vBlankStage();
      break;
  }
  ++dot;
  // TODO return if send NMI or not
  return false;
}

uint8_t PPU::readctrl() { return latch; }
uint8_t PPU::readmask() { return latch; }

uint8_t PPU::readstatus() {
  latch = (status & 0b11100000) | (latch & 0b00011111);
  status = status & 0b01111111;  // clear vblank bit on read
  writeLatch = false;
  return latch;
}

uint8_t PPU::readOAMAddr() { return latch; }

uint8_t PPU::readOAMData() {
  latch = OAMMemory[OAMAddr];
  return OAMMemory[OAMAddr];
}

uint8_t PPU::readscroll() { return latch; }

uint8_t PPU::readaddr() { return latch; }

uint8_t PPU::readdata() {
  latch = data;
  uint16_t addr = mapAddr(rv);
  data = memory[addr];
  rv += VRamInc;
  return latch;
}

uint8_t PPU::readOAMDMA() { return latch; }

// Writes
void PPU::writectrl(uint8_t val) {
  latch = val;
  // x... GH.. .... .... <- val: .... ..GH
  rt &= ~0x0C00;
  rt |= (val & 0x03) << 10;

  VRamInc = (val & 0x04) ? 32 : 1;
  spritePTAddr = (val & 0x08) ? 0x1000 : 0x0000;
  backgroundPTAddr = (val & 0x10) ? 0x1000 : 0x0000;
  spriteSize = (val & 0x20);
  masterSlaveSel = (val & 0x40);
  generateNMI = (val & 0x80);
}

void PPU::writemask(uint8_t val) {
  latch = val;

  maskGreyscale = (val & 0x01);
  maskShowLeftBackground = (val & 0x02);
  maskShowLeftSprite = (val & 0x04);
  maskShowBackground = (val & 0x08);
  maskShowSprites = (val & 0x10);
  maskEmphRed = (val & 0x20);
  maskEmphGreen = (val & 0x40);
  maskEmphBlue = (val & 0x80);
}

void PPU::writestatus(uint8_t val) { latch = val; }

void PPU::writeOAMAddr(uint8_t val) {
  latch = val;
  OAMAddr = val;
}

void PPU::writeOAMData(uint8_t val) {
  latch = val;
  OAMMemory[OAMAddr++] = val;
}

void PPU::writescroll(uint8_t val) {
  latch = val;
  if (!writeLatch) {
    rt &= ~0x001F;  // clear bits 4-0 and replace with bits 7-3 of val
    rt |= (val >> 3) & 0x001F;
    rx = val & 0x07;
  } else {
    // t: .FGH ..AB CDE. .... <- d: AB CDE FGH
    rt &= ~0x73E0;             // Clear bits that FGH AB and CDE go to
    rt |= (val & 0x07) << 12;  // FGH
    rt |= (val & 0xF8) << 2;   // AB CDE
  }
  writeLatch = !writeLatch;
}

void PPU::writeaddr(uint8_t val) {
  latch = val;
  // High is first
  if (!writeLatch) {
    rt = (rt & 0x00FF) | ((val & 0x003F) << 8);
  } else {
    rt = (rt & 0x7F00) | val;
    rv = rt;
  }
  writeLatch = !writeLatch;
}

void PPU::writedata(uint8_t val) {
  latch = val;
  uint16_t addr = mapAddr(rv);
  memory[addr] = val;
  rv += VRamInc;
}

void PPU::dump() const {
  std::ofstream file("PPU.dump");

  enum DisplayState { Display, Repeat, Skip } ds;
  ds = Display;
  for (size_t pc = 0; pc < 0x4000; pc += 0x10) {
    uint8_t numZeroes = 0;
    std::string line = "";
    for (size_t i = 0; i < 0x10; ++i) {
      uint8_t val = memory[pc + i];
      line += to_hex(val) + " ";
      if (val == 0x00) {
        numZeroes++;
      };
    }

    // Skip all zero lines if they're repeated
    if (numZeroes == 16) {
      if (ds == Display) {
        ds = Repeat;
        goto print;
      } else if (ds == Repeat) {
        ds = Skip;
        file << "\n";
        continue;
      } else {
        continue;
      }
    }
    ds = Display;
  print:
    file << to_hex(static_cast<uint16_t>(pc)) << "\t" << line << "\n";
  }
  file << std::flush;
}

uint16_t PPU::mapAddr(uint16_t addr) {
  if (addr > 0x4000) {
    return addr % 0x4000;
  }
  // Pattern Tables
  if (addr < 0x2000) return addr;

  // Unused portion
  if (addr < 0x3F00 && addr >= 0x3000) {
    return addr;
  }
  // Palette
  if (addr >= 0x3F00) {
    uint16_t temp = addr - 0x3F00;
    temp %= 0x0020;
    return temp + 0x3F00;
  }

  // Nametable
  int nameTableIndex = (addr - 0x2000) / 0x400;
  if (mirroring == Horizontal) {
    if (nameTableIndex == 0 || nameTableIndex == 2) {
      return addr;
    }
    /*
      2400-27FF -> 2000-23FF
      2C00-2FFF -> 2800-2BFF
    */
    return addr - 0x400;
  } else {
    if (nameTableIndex == 0 || nameTableIndex == 1) {
      return addr;
    }
    /*
      2800-2BFF -> 2000-23FF
      2C00-2FFF -> 2400-27FF
    */
    return addr - 0x800;
  }
}

void PPU::preRenderStage() { uint16_t addr = 0x2000 | (rv & 0x0FFF); }

void PPU::renderStage() {}

void PPU::postRenderStage() {}

void PPU::vBlankStage() {}
