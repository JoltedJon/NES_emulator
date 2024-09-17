#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <string>
#include <unordered_set>
#include <vector>

#include "ppu.h"

class CPU;

class NesMemory {
 private:
  std::array<uint8_t, 0x800> internalRam;  // 2KB
  std::array<uint8_t, 0x20> APUIOMemory;   // 0x20 Bytes
  std::array<uint8_t, 0xBFE0> cpuMemory;   // 0xBFE0 bytes

  PPU* ppu;
  CPU* cpu;

  static std::unordered_set<uint16_t> supportedMappers;

  std::string romPath;
  std::vector<char> rom;

  // ROM Header information
  uint8_t programSize;    // multiply by 16KB -> 0x4000 bytes
  uint8_t characterSize;  // multiply by 8KB -> 0x2000 bytes

  enum Nametable : bool { Vertical, Horizontal } mirroring;
  bool persistent;
  bool trainerPresent;

  uint16_t mapper;

 public:
  NesMemory() = default;
  bool loadRom(const std::string& romPath, PPU& ppu);
  inline void setCPU(CPU* inCPU) { cpu = inCPU; }

  uint8_t& operator[](size_t);
  const uint8_t& operator[](size_t) const;

  void write(uint16_t addr, uint8_t val);
  uint8_t read(uint16_t addr);

  void dump();

 private:
};
