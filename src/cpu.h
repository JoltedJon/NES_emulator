#pragma once

#include <cstdint>
#include <fstream>
#include <string>
#include <unordered_map>

#include "nesMemory.h"

enum class Operation {
  ADC,
  AND,
  ASL,
  BCC,
  BCS,
  BEQ,
  BIT,
  BMI,
  BNE,
  BPL,
  BRK,
  BVC,
  BVS,
  CLC,
  CLD,
  CLI,
  CLV,
  CMP,
  CPX,
  CPY,
  DEC,
  DEX,
  DEY,
  EOR,
  INC,
  INX,
  INY,
  JMP,
  JSR,
  LDA,
  LDX,
  LDY,
  LSR,
  NOP,
  ORA,
  PHA,
  PHP,
  PLA,
  PLP,
  ROL,
  ROR,
  RTI,
  RTS,
  SBC,
  SEC,
  SED,
  SEI,
  STA,
  STX,
  STY,
  TAX,
  TAY,
  TSX,
  TXA,
  TXS,
  TYA
};

enum class States {
  Fetch,

  Abs1,
  Abs2,

  AbsX,
  AbsY,
  AbsXY,
  AbsFix,

  Zero,
  ZeroX,
  ZeroY,
  ZeroXY,

  Indexed1,
  Indexed2,
  Indexed3,
  Indexed4,

  IndirectIndexed1,
  IndirectIndexed2,
  IndirectIndexed3,
  IndirectIndexedFix,

  Indirect1,  // Only for JMP
  Indirect2,
  Indirect3,
  Indirect4,

  RMWStall1,
  RMWStall2,

  Accumulator,
  Immediate,

  Branch,

  Read,

  Execute1,
  Execute2,
  Execute3,
  Execute4,
  Execute5,
  Execute6,
};

struct StatusFlags {
  bool carry : 1;
  bool zero : 1;
  bool irqDisable : 1;
  bool decimalMode : 1;
  bool breakFlag : 1;
  bool __ : 1;  // Unused bit set to true always
  bool overflow : 1;
  bool sign : 1;

  StatusFlags()
      : carry(),
        zero(),
        irqDisable(),
        decimalMode(),
        breakFlag(),
        __(true),
        overflow(),
        sign() {}
};

union FlagConversion {
  StatusFlags f;
  uint8_t byte;

  FlagConversion() : f() {}
};

class CPU {
 private:
  // Registers
  uint8_t ra;   // Accumulator
  uint8_t rx;   // Register x
  uint8_t ry;   // Register y
  uint16_t pc;  // Program Counter
  uint8_t sp;   // Stack Pointer
  StatusFlags rf;

  Operation op;

  States state;

  uint16_t addr;  // For any addresses that need to be modified per cycle
  uint8_t value;

  // char* memory;  // TODO placeholder
  NesMemory& memory;

  // Debug
  uint64_t cycle;
  std::string debugInfo;
  std::ofstream log;
  bool step;
  bool printLog;

  static std::unordered_map<Operation, std::string> opMap;
  static std::unordered_map<States, std::string> stateMap;

  // Functions
 public:
  CPU(NesMemory& memory);
  void reset();

  void doCycle();

  // Debug
  inline void setPC(uint16_t newPC) { pc = newPC; }
  inline void resetPC() {
    pc = memory[0xFFFC] | (static_cast<uint16_t>(memory[0xFFFD]) << 8);
  }
  inline const uint16_t getPC() const { return pc; }
  inline void toggleStep() { step = !step; }
  inline const bool getStep() const { return step; }
  inline void toggleLog() { printLog = !printLog; }
  inline const bool getLog() const { return printLog; }
  inline const uint64_t getCycle() const { return cycle; }

 private:
  void decode(uint8_t byte);

  void pushStack(uint8_t val);
  uint8_t popStack();

  inline uint8_t getStatus() const {
    FlagConversion fc;
    fc.f = rf;
    return fc.byte;
  }
  inline void setStatus(uint8_t byte) {
    rf.carry = byte & 0x01;
    rf.zero = byte & 0x02;
    rf.irqDisable = byte & 0x04;
    rf.decimalMode = byte & 0x08;
    rf.breakFlag = false;
    rf.__ = true;
    rf.overflow = byte & 0x40;
    rf.sign = byte & 0x80;
  }

  inline void setZero(uint8_t val) { rf.zero = val == 0; }
  inline void setSign(uint8_t val) { rf.sign = val & 0x80; }

  void executeImplicit();
  void executeAccumulator();
  void executeImmediate();
  bool executeBranch();

  void executeInstruction();

  void __compare(uint8_t first, uint8_t second);

  // Instructions

  // Into Register
  void ADC(uint8_t val);
  void AND(uint8_t val);
  void BIT(uint8_t val);
  void CMP(uint8_t val);
  void CPX(uint8_t val);
  void CPY(uint8_t val);
  void EOR(uint8_t val);
  void LDA(uint8_t val);
  void LDX(uint8_t val);
  void LDY(uint8_t val);
  void ORA(uint8_t val);
  void SBC(uint8_t val);

  // Into Accumulator or Memory
  uint8_t ASL(uint8_t val);
  uint8_t LSR(uint8_t val);
  uint8_t ROL(uint8_t val);
  uint8_t ROR(uint8_t val);

  // Into Memory
  uint8_t DEC(uint8_t val);
  uint8_t INC(uint8_t val);

  // Branches
  bool BCC();
  bool BCS();
  bool BEQ();
  bool BMI();
  bool BNE();
  bool BPL();
  bool BVC();
  bool BVS();

  // Unconditional Jumps
  void JMP(uint16_t addr);
  void JSR();

  // Stores
  void STA(uint16_t addr);
  void STX(uint16_t addr);
  void STY(uint16_t addr);

  // Implied

  void CLC();
  void CLD();
  void CLI();
  void CLV();

  void DEX();
  void DEY();

  void INX();
  void INY();

  void SEC();
  void SED();
  void SEI();

  void TAX();
  void TAY();
  void TSX();
  void TXA();
  void TXS();
  void TYA();

  // Complex Implied
  void BRK();
  void PHA();
  void PHP();
  void PLA();
  void PLP();
  void RTI();
  void RTS();

  // Debug Information
  void addDebugInfo();
  void dbgImp(const std::string opStr);
  void dbgZP(const std::string opStr);
  void dbgZPX(const std::string opStr);
  void dbgZPY(const std::string opStr);
  void dbgImm(const std::string opStr);
  void dbgAcc(const std::string opStr);
  void dbgAbs(const std::string opStr);
  void dbgMem(const std::string opstr);
  void dbgAbsY(const std::string opStr);
  void dbgAbsX(const std::string opStr);
  void dbgBr(const std::string opStr);
  void dbgXInd(const std::string opStr);
  void dbgYInd(const std::string opStr);
  void dbgInd(const std::string opStr);
};
