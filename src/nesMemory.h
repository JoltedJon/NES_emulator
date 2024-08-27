#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <string>
#include <unordered_set>
#include <vector>

class NesMemory {
 private:
  std::array<uint8_t, 0x800> internalRam;  // 2KB
  std::array<uint8_t*, 0x8> PPURegisters;
  std::array<uint8_t, 0x20> APUIOMemory;  // 0x20 Bytes
  std::array<uint8_t, 0xBFE0> cpuMemory;  // 0xBFE0 bytes

  static std::unordered_set<uint16_t> supportedMappers;

  std::string romPath;
  std::vector<char> rom;

  // ROM Header information
  uint8_t program_size;    // multiply by 16KB -> 0x4000 bytes
  uint8_t character_size;  // multiply by 8KB -> 0x2000 bytes

  enum Nametable : bool { Vertical, Horizontal } arrangement;
  bool persistent;
  bool trainerPresent;

  uint16_t mapper;

 public:
  NesMemory() = default;
  bool loadRom(const std::string& romPath);

  uint8_t& operator[](size_t);
  const uint8_t& operator[](size_t) const;

  void dump() const;

 private:
};
