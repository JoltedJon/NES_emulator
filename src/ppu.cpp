#include "ppu.h"

#include <cstring>
#include <fstream>
#include <vector>

#include "utils.h"

PPU::PPU()
    : ctrl(0x00),
      mask(0x00),
      status(0xA0),
      OAMAddr(0x00),
      OAMData(0x00),
      scroll(0x00),
      addr(0x00),
      data(0x00),
      OAMDMA(0x00) {}

void PPU::loadRom(std::vector<char> &inRom, bool trainerPresent) {
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

uint8_t PPU::readctrl() { return 0x00; }
uint8_t PPU::readmask() { return 0x00; }
uint8_t PPU::readstatus() { return 0x00; }
uint8_t PPU::readOAMAddr() { return 0x00; }
uint8_t PPU::readOAMData() { return 0x00; }
uint8_t PPU::readscroll() { return 0x00; }
uint8_t PPU::readaddr() { return 0x00; }
uint8_t PPU::readdata() { return 0x00; }
uint8_t PPU::readOAMDMA() { return 0x00; }

void PPU::writectrl(uint8_t val) {}
void PPU::writemask(uint8_t val) {}
void PPU::writestatus(uint8_t val) {}
void PPU::writeOAMAddr(uint8_t val) {}
void PPU::writeOAMData(uint8_t val) {}
void PPU::writescroll(uint8_t val) {}
void PPU::writeaddr(uint8_t val) {}
void PPU::writedata(uint8_t val) {}
void PPU::writeOAMDMA(uint8_t val) {}

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
