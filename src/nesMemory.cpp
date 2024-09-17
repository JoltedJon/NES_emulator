#include "nesMemory.h"

#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <vector>

#include "cpu.h"
#include "ppu.h"
#include "utils.h"

// Set of the currently supported Mappers
std::unordered_set<uint16_t> NesMemory::supportedMappers = {0x00};

constexpr size_t ROMLOCATION = 0x3FE0;

bool NesMemory::loadRom(const std::string& romPath, PPU& ppu) {
  NesMemory::romPath = romPath;
  std::ifstream romFile(romPath, std::ifstream::binary | std::ifstream::ate);
  if (romFile.fail()) {
    std::cout << "Failed to Open \"" << romPath << "\"" << std::endl;
    return false;
  }
  std::streamsize size = romFile.tellg();
  romFile.seekg(0, std::ios::beg);

  rom = std::vector<char>(size);
  if (!romFile.read(rom.data(), size)) {
    std::cout << "Failed to Read \"" << romPath << "\"" << std::endl;
    return false;
  }

  constexpr char const NES_BYTES[4] = {'N', 'E', 'S', 0x1A};
  bool headerFound = !memcmp(rom.data(), NES_BYTES, 4);
  if (rom.size() < 16 || !headerFound) {
    std::cout << "\"" << romPath << "\" is not a valid NES Rom" << std::endl;
    return false;
  }

  programSize = rom[4];
  characterSize = rom[5];

  // Flags 6
  mirroring = (rom[6] & 0x01) ? Horizontal : Vertical;
  persistent = rom[6] & 0x02;
  trainerPresent = rom[6] & 0x04;
  mapper = static_cast<uint8_t>(rom[6]) >> 4;

  if (rom[7] & 0x01) {
    std::cout << "VS. Unisystem ROM not yet supported" << std::endl;
    return false;
  }

  mapper |= static_cast<uint8_t>(rom[7]) & 0xF0;

  std::cout << "\n\n";
  std::cout << "Rom Size: " << programSize * 16 << "KB" << std::endl;
  std::cout << "CHR Size: " << characterSize * 8 << "KB" << std::endl;
  std::cout << (mirroring == Horizontal ? "Horizontal" : "Vertical")
            << " Mirroring" << std::endl;
  std::cout << "Persistent Memory " << (persistent ? "Present" : "Not Present")
            << std::endl;
  std::cout << "Trainer " << (trainerPresent ? "Present" : "Not Present")
            << std::endl;
  std::cout << "Mapper Number: " << (int)mapper << std::endl << std::endl;

  if (!supportedMappers.contains(mapper)) {
    std::cout << "Mapper \"" << (int)mapper << "\" Not yet supported"
              << std::endl;
    return false;
  }

  // TODO implement other Mappers
  // Currently only loads ROM for 0x00

  // Construct Memory
  memcpy(cpuMemory.data() + ROMLOCATION,
         rom.data() + 16 + (trainerPresent * 512), programSize * 0x4000);
  this->ppu = &ppu;
  ppu.loadRom(rom, trainerPresent);

  return true;
}

// TODO Support different memory maps from different mappers
uint8_t& NesMemory::operator[](size_t i) {
  if (i < 0x2000) {
    return internalRam[i % 0x0800];
  }

  if (i < 0x4018 && i >= 0x4000) {
    return APUIOMemory[i - 0x4000];
  }

  if (i < 0x4020 && i >= 0x4018) {
    return APUIOMemory[i - 0x4000];
  }

  if (i < 0x8000 && i >= 0x4020) {
    return cpuMemory[i - 0x4020];
  }

  if (i >= 0x8000) {
    i = ((i - 0x8000) % (programSize * 0x4000)) + ROMLOCATION;
    return cpuMemory[i];
  }

  error(("Index: 0x" + to_hex(static_cast<uint16_t>(i)) +
         " Not Implemented Correctly!")
            .c_str());
  exit(1);
}

const uint8_t& NesMemory::operator[](size_t i) const {
  if (i < 0x2000) {
    return internalRam[i % 0x0800];
  }

  if (i < 0x4018 && i >= 0x4000) {
    return APUIOMemory[i - 0x4000];
  }

  if (i < 0x4020 && i >= 0x4018) {
    return APUIOMemory[i - 0x4000];
  }

  if (i < 0x8000 && i >= 0x4020) {
    return cpuMemory[i - 0x4020];
  }

  if (i >= 0x8000) {
    i = ((i - 0x8000) % (programSize * 0x4000)) + ROMLOCATION;
    return cpuMemory[i];
  }
  error(("Index: 0x" + to_hex(static_cast<uint16_t>(i)) +
         " Not Implemented Correctly!")
            .c_str());
  exit(1);
}

void NesMemory::write(uint16_t addr, uint8_t val) {
  if (addr < 0x4000 && addr >= 0x2000) {
    addr %= 0x2008;
  }
  switch (addr) {
    case 0x2000:
      ppu->writectrl(val);
      break;
    case 0x2001:
      ppu->writemask(val);
      break;
    case 0x2002:
      ppu->writestatus(val);
      break;
    case 0x2003:
      ppu->writeOAMAddr(val);
      break;
    case 0x2004:
      ppu->writeOAMData(val);
      break;
    case 0x2005:
      ppu->writescroll(val);
      break;
    case 0x2006:
      ppu->writeaddr(val);
      break;
    case 0x2007:
      ppu->writedata(val);
      break;
    case 0x4014:
      cpu->queueOAM_DMA(val);
      break;
    default:
      operator[](addr) = val;
      return;
  }
}

uint8_t NesMemory::read(uint16_t addr) {
  if (addr < 0x4000 && addr >= 0x2000) {
    addr %= 0x2008;
  }
  switch (addr) {
    case 0x2000:
      return ppu->readctrl();
    case 0x2001:
      return ppu->readmask();
    case 0x2002:
      return ppu->readstatus();
    case 0x2003:
      return ppu->readOAMAddr();
    case 0x2004:
      return ppu->readOAMData();
    case 0x2005:
      return ppu->readscroll();
    case 0x2006:
      return ppu->readaddr();
    case 0x2007:
      return ppu->readdata();
    case 0x4014:
      return ppu->readOAMDMA();
    default:
      return operator[](addr);
  }
}

void NesMemory::dump() {
  std::ofstream file("NES.dump");

  enum DisplayState { Display, Repeat, Skip } ds;
  ds = Display;
  for (size_t pc = 0; pc < 0x10000; pc += 0x10) {
    uint8_t numZeroes = 0;
    std::string line = "";
    for (size_t i = 0; i < 0x10; ++i) {
      uint8_t val = read(pc + i);
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
