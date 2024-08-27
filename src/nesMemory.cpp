#include "nesMemory.h"

#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <string_view>
#include <vector>

#include "utils.h"

// Set of the currently supported Mappers
std::unordered_set<uint16_t> NesMemory::supportedMappers = {0x00};

constexpr size_t ROMLOCATION = 0x3FE0;

bool NesMemory::loadRom(const std::string& romPath) {
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

  program_size = rom[4];
  character_size = rom[5];

  // Flags 6
  arrangement = (rom[6] & 0x01) ? Horizontal : Vertical;
  persistent = rom[6] & 0x02;
  trainerPresent = rom[6] & 0x04;
  mapper = static_cast<uint8_t>(rom[6]) >> 4;

  if (rom[7] & 0x01) {
    std::cout << "VS. Unisystem ROM not yet supported" << std::endl;
    return false;
  }

  mapper |= static_cast<uint8_t>(rom[7]) & 0xF0;

  std::cout << "\n\n";
  std::cout << "Rom Size: " << program_size * 16 << "KB" << std::endl;
  std::cout << "CHR Size: " << character_size * 8 << "KB" << std::endl;
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

  // Construct Memory

  // TODO implement other Mappers
  // Currently only loads ROM for 0x00

  memcpy(cpuMemory.data() + ROMLOCATION,
         rom.data() + 16 + (trainerPresent * 512), program_size * 0x4000);

  return true;
}

// TODO Support different memory maps from different mappers
uint8_t& NesMemory::operator[](size_t i) {
  if (i < 0x2000) {
    return internalRam[i % 0x0800];
  }

  if (i < 0x4000 && i >= 0x2000) {
    // TODO PPU registers
    return internalRam[0];
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
    i = ((i - 0x8000) % (program_size * 0x4000)) + ROMLOCATION;
    return cpuMemory[i];
  }
  error(
      ("Index: " + std::to_string(i) + " Not Implemented Correctly!").c_str());
  exit(1);
}

const uint8_t& NesMemory::operator[](size_t i) const {
  if (i < 0x2000) {
    return internalRam[i % 0x0800];
  }

  if (i < 0x4000 && i >= 0x2000) {
    // TODO PPU registers
    return internalRam[0];
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
    i = ((i - 0x8000) % (program_size * 0x4000)) + ROMLOCATION;
    return cpuMemory[i];
  }
  error(
      ("Index: " + std::to_string(i) + " Not Implemented Correctly!").c_str());
  exit(1);
}

void NesMemory::dump() const {
  std::ofstream file("NES.dump");

  enum DisplayState { Display, Repeat, Skip } ds;
  ds = Display;
  for (size_t pc = 0; pc < 0x10000; pc += 0x10) {
    uint8_t numZeroes = 0;
    std::string line = "";
    for (size_t i = 0; i < 0x10; ++i) {
      uint8_t val = operator[](pc + i);
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
