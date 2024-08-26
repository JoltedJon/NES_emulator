#include "nesMemory.h"

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <fstream>
#include <iostream>
#include <vector>

#include "utils.h"

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

  cpuMemory = std::vector<uint8_t>(0x10000);
  memcpy(cpuMemory.data() + 0x8000,
         rom.data() + 16 + (trainerPresent * trainerPresent),
         program_size * (16384));
  // dump();
  // exit(0);
  return true;
}

// TODO have correct CPU memory map
uint8_t& NesMemory::operator[](size_t i) {
  if (i >= 0x0800 && i < 0x2000) {
    i %= 0x800;
  } else if (i >= 0x8000) {
    i -= 0x8000;
    i %= program_size * 16384;
    i += 0x8000;
  }
  return cpuMemory[i];
}
const uint8_t& NesMemory::operator[](size_t i) const {
  if (i >= 0x0800 && i < 0x2000) {
    i %= 0x800;
  } else if (i >= 0x8000) {
    i -= 0x8000;
    i %= program_size * 16384;
    i += 0x8000;
  }
  return cpuMemory[i];
}

void NesMemory::dump() const {
  std::ofstream file("NES.dump");

  bool same = false;
  std::string previousLine = "";
  for (size_t pc = 0; pc < cpuMemory.size(); pc += 0x10) {
    std::string line = "";
    for (size_t i = 0; i < 0x10; ++i) {
      line += to_hex(operator[](pc + i)) + " ";
    }
    if (previousLine == line) {
      previousLine = line;
      if (!same) {
        file << "\n";
        same = true;
      }
      continue;
    }
    previousLine = line;
    same = false;
    file << to_hex(static_cast<uint16_t>(pc)) << "\t" << line << "\n";
  }
  file << std::flush;
}
