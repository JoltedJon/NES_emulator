#include <cstdint>
#include <cstring>
#include <iostream>
#include <vector>

#include "SDL_timer.h"
#include "cpu.h"
#include "nesMemory.h"
#include "ppu.h"
#include "utils.h"
#include "window.h"

int main(int argc, char** argv) {
  Window win;

  NesMemory memory;
  PPU ppu(&win);

  std::string romPath;
  std::vector<char> rom;

LoadRom:
  while (true) {
    romPath = "";
    // std::cout << "Load Rom: ";
    // std::cin >> romPath;
    romPath = "roms/nestest.nes";

    if (std::cin.eof() || romPath == "") {
      std::cout << std::endl;
      return 0;
    }

    if (!memory.loadRom(romPath, ppu)) {
      continue;
    }
    break;
  }

  while (true) {
    ppu.display();
    SDL_Delay(30);
    win.poll();
  }

  CPU cpu(memory);

  std::string message;
  while (true) {
    std::cout << "[1] Load New ROM\n";
    std::cout << "[2] Set Starting PC [" << to_hex(cpu.getPC()) << "]\n";
    std::cout << "[3] Run ROM\n";
    std::cout << "[4] Toggle Instruction Step Mode, ["
              << (cpu.getStep() ? "Enabled" : "Disabled") << "]\n";
    std::cout << "[5] Toggle Logging, ["
              << (cpu.getLog() ? "Enabled" : "Disabled") << "]\n";

    std::cout << "\n> ";
    std::cin >> message;
    if (std::cin.eof()) return 0;

    if (message == "1") goto LoadRom;
    if (message == "2") {
      std::cout << "Set PC: ";
      std::cin >> message;
      uint16_t newPC = stoi(message, nullptr, 16);
      cpu.setPC(newPC);
    } else if (message == "3") {
      break;
    } else if (message == "4") {
      cpu.toggleStep();
    } else if (message == "5") {
      cpu.toggleLog();
    }
  }

  while (true) {
    cpu.doCycle();
  }
  return 0;
}
