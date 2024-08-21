#pragma once

#include <cstdint>

enum Operation {
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

enum AddressingMode {
  Implicit,
  Accumulator,
  Immediate,
  ZeroPage,
  ZeroPageX,
  ZeroPageY,
  Relative,
  Absolute,
  AbsoluteX,
  AbsoluteY,
  Indirect,
  IndexedIndirect,
  IndirectIndexed
};

struct Instruction {
  Operation o;
  AddressingMode am;
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

  Instruction i;

  int stage;     // Used for keeping track of execution of instruction
  uint8_t addr;  // For any addresses that need to be modified per cycle

  char *memory;  // TODO placeholder

 public:
  void doCycle();

 private:
  void decode(uint8_t byte);

  void pushStack(uint8_t val);
  uint8_t popStack();

  inline uint8_t getStatus() {
    FlagConversion fc;
    fc.f = rf;
    return fc.byte;
  }

  inline void setZero(uint8_t val) { rf.zero = val == 0; }
  inline void setSign(uint8_t val) { rf.sign = val & 0x80; }

  void executeImplicit();
  void executeAccumulator();
  void executeImmediate();

  template <typename F>
  void readModifyWrite(F func);

  void executeAbsolute();

  void executeZeroPage();
  void executeZeroPageX();
  void executeZeroPageY();
  void executeRelative();
  void executeAbsoluteX();
  void executeAbsoluteY();
  void executeIndirect();
  void executeIndexedIndirect();
  void executeIndirectIndexed();

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
  uint8_t DEC(uint8_t val);
  uint8_t INC(uint8_t val);

  // Branches
  bool BCC(uint8_t offset);
  bool BCS(uint8_t offset);
  bool BEQ(uint8_t offset);
  bool BMI(uint8_t offset);
  bool BNE(uint8_t offset);
  bool BPL(uint8_t offset);
  bool BVC(uint8_t offset);
  bool BVS(uint8_t offset);

  // Unconditional Jumps
  void JMP(uint16_t addr);
  void JSR(uint16_t addr);

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
};
