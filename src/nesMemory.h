#pragma once

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

class NesMemory {
 private:
  // TODO For now this is the only memory until PPU
  std::vector<uint8_t> cpuMemory;

  std::string romPath;
  std::vector<char> rom;

  // ROM Header information
  uint8_t program_size;
  uint8_t character_size;

  enum Nametable : bool { Vertical, Horizontal } arrangement;
  bool persistent;
  bool trainerPresent;

  uint8_t mapper;

 public:
  NesMemory() = default;
  bool loadRom(const std::string& romPath);

  uint8_t& operator[](size_t);
  const uint8_t& operator[](size_t) const;

  void dump() const;
};
