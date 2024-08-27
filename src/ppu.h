#pragma once

#include <array>
#include <cstdint>
#include <vector>

class PPU {
 private:
  uint8_t ctrl;
  uint8_t mask;
  uint8_t status;
  uint8_t OAMAddr;
  uint8_t OAMData;
  uint8_t scroll;
  uint8_t addr;
  uint8_t data;

  uint8_t OAMDMA;

  uint16_t addrFull;

  uint8_t characterSize;  // multiply by 8KB -> 0x2000 bytes
  enum Nametable : bool { Vertical, Horizontal } mirroring;
  uint16_t mapper;

  std::vector<char> *rom;
  size_t characterStart;

  std::array<uint8_t, 0x4000> memory;

 public:
  PPU();

  void loadRom(std::vector<char> &inRom, bool trainerPresent);

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
  void writeOAMDMA(uint8_t val);

  void dump() const;
};
