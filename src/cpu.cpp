#include "cpu.h"

#include <cstdint>
#include <string>

#include "utils.h"

void CPU::decode(uint8_t byte) {
  switch (byte) {
    // Immediate
    case 0x09:
      i = {Operation::ORA, AddressingMode::Immediate};
      break;
    case 0x29:
      i = {Operation::AND, AddressingMode::Immediate};
      break;
    case 0x49:
      i = {Operation::EOR, AddressingMode::Immediate};
      break;
    case 0x69:
      i = {Operation::ADC, AddressingMode::Immediate};
      break;
    case 0xA0:
      i = {Operation::LDY, AddressingMode::Immediate};
      break;
    case 0xA2:
      i = {Operation::LDX, AddressingMode::Immediate};
      break;
    case 0xA9:
      i = {Operation::LDA, AddressingMode::Immediate};
      break;
    case 0xC0:
      i = {Operation::CPY, AddressingMode::Immediate};
      break;
    case 0xC9:
      i = {Operation::CMP, AddressingMode::Immediate};
      break;
    case 0xE0:
      i = {Operation::CPX, AddressingMode::Immediate};
      break;
    case 0xE9:
      i = {Operation::SBC, AddressingMode::Immediate};
      break;

    // Zero Page
    case 0x05:
      i = {Operation::ORA, AddressingMode::ZeroPage};
      break;
    case 0x24:
      i = {Operation::BIT, AddressingMode::ZeroPage};
      break;
    case 0x25:
      i = {Operation::AND, AddressingMode::ZeroPage};
      break;
    case 0x45:
      i = {Operation::EOR, AddressingMode::ZeroPage};
      break;
    case 0x65:
      i = {Operation::ADC, AddressingMode::ZeroPage};
      break;
    case 0x84:
      i = {Operation::STY, AddressingMode::ZeroPage};
      break;
    case 0x85:
      i = {Operation::STA, AddressingMode::ZeroPage};
      break;
    case 0x86:
      i = {Operation::STX, AddressingMode::ZeroPage};
      break;
    case 0xA4:
      i = {Operation::LDY, AddressingMode::ZeroPage};
      break;
    case 0xA5:
      i = {Operation::LDA, AddressingMode::ZeroPage};
      break;
    case 0xA6:
      i = {Operation::LDX, AddressingMode::ZeroPage};
      break;
    case 0xC4:
      i = {Operation::CPY, AddressingMode::ZeroPage};
      break;
    case 0xC5:
      i = {Operation::CMP, AddressingMode::ZeroPage};
      break;
    case 0xE4:
      i = {Operation::CPX, AddressingMode::ZeroPage};
      break;
    case 0xE5:
      i = {Operation::SBC, AddressingMode::ZeroPage};
      break;
    case 0x06:
      i = {Operation::ASL, AddressingMode::ZeroPage};
      break;
    case 0x26:
      i = {Operation::ROL, AddressingMode::ZeroPage};
      break;
    case 0x46:
      i = {Operation::LSR, AddressingMode::ZeroPage};
      break;
    case 0x66:
      i = {Operation::ROR, AddressingMode::ZeroPage};
      break;
    case 0xC6:
      i = {Operation::DEC, AddressingMode::ZeroPage};
      break;
    case 0xE6:
      i = {Operation::INC, AddressingMode::ZeroPage};
      break;

    // Zero Page, X
    case 0x15:
      i = {Operation::ORA, AddressingMode::ZeroPageX};
      break;
    case 0x35:
      i = {Operation::AND, AddressingMode::ZeroPageX};
      break;
    case 0x55:
      i = {Operation::EOR, AddressingMode::ZeroPageX};
      break;
    case 0x75:
      i = {Operation::ADC, AddressingMode::ZeroPageX};
      break;
    case 0x94:
      i = {Operation::STY, AddressingMode::ZeroPageX};
      break;
    case 0x95:
      i = {Operation::STA, AddressingMode::ZeroPageX};
      break;
    case 0xB4:
      i = {Operation::LDY, AddressingMode::ZeroPageX};
      break;
    case 0xB5:
      i = {Operation::LDA, AddressingMode::ZeroPageX};
      break;
    case 0xD5:
      i = {Operation::CMP, AddressingMode::ZeroPageX};
      break;
    case 0xF5:
      i = {Operation::SBC, AddressingMode::ZeroPageX};
      break;
    case 0x16:
      i = {Operation::ASL, AddressingMode::ZeroPageX};
      break;
    case 0x36:
      i = {Operation::ROL, AddressingMode::ZeroPageX};
      break;
    case 0x56:
      i = {Operation::LSR, AddressingMode::ZeroPageX};
      break;
    case 0x76:
      i = {Operation::ROR, AddressingMode::ZeroPageX};
      break;
    case 0xD6:
      i = {Operation::DEC, AddressingMode::ZeroPageX};
      break;
    case 0xF6:
      i = {Operation::INC, AddressingMode::ZeroPageX};
      break;

    // Zero Page, Y
    case 0xB6:
      i = {Operation::LDX, AddressingMode::ZeroPageY};
      break;
    case 0x96:
      i = {Operation::STX, AddressingMode::ZeroPageY};
      break;

    // Absolute
    case 0x4C:
      i = {Operation::JMP, AddressingMode::Absolute};
      break;
    case 0x6D:
      i = {Operation::ADC, AddressingMode::Absolute};
      break;
    case 0x2D:
      i = {Operation::AND, AddressingMode::Absolute};
      break;
    case 0x2C:
      i = {Operation::BIT, AddressingMode::Absolute};
      break;
    case 0xCD:
      i = {Operation::CMP, AddressingMode::Absolute};
      break;
    case 0xEC:
      i = {Operation::CPX, AddressingMode::Absolute};
      break;
    case 0xCC:
      i = {Operation::CPY, AddressingMode::Absolute};
      break;
    case 0xAD:
      i = {Operation::LDA, AddressingMode::Absolute};
      break;
    case 0xAE:
      i = {Operation::LDX, AddressingMode::Absolute};
      break;
    case 0xAC:
      i = {Operation::LDY, AddressingMode::Absolute};
      break;
    case 0x0D:
      i = {Operation::ORA, AddressingMode::Absolute};
      break;
    case 0xED:
      i = {Operation::SBC, AddressingMode::Absolute};
      break;
    case 0x8D:
      i = {Operation::STA, AddressingMode::Absolute};
      break;
    case 0x8E:
      i = {Operation::STX, AddressingMode::Absolute};
      break;
    case 0x8C:
      i = {Operation::STY, AddressingMode::Absolute};
      break;
    case 0x4D:
      i = {Operation::EOR, AddressingMode::Absolute};
      break;
    case 0x0E:
      i = {Operation::ASL, AddressingMode::Absolute};
      break;
    case 0x20:
      i = {Operation::JSR, AddressingMode::Absolute};
      break;
    case 0x2E:
      i = {Operation::ROL, AddressingMode::Absolute};
      break;
    case 0x4E:
      i = {Operation::LSR, AddressingMode::Absolute};
      break;
    case 0x6E:
      i = {Operation::ROR, AddressingMode::Absolute};
      break;
    case 0xCE:
      i = {Operation::DEC, AddressingMode::Absolute};
      break;
    case 0xEE:
      i = {Operation::INC, AddressingMode::Absolute};
      break;

    // Absolute, X
    case 0x7D:
      i = {Operation::ADC, AddressingMode::AbsoluteX};
      break;
    case 0x3D:
      i = {Operation::AND, AddressingMode::AbsoluteX};
      break;
    case 0xFD:
      i = {Operation::SBC, AddressingMode::AbsoluteX};
      break;
    case 0xDD:
      i = {Operation::CMP, AddressingMode::AbsoluteX};
      break;
    case 0x1D:
      i = {Operation::ORA, AddressingMode::AbsoluteX};
      break;
    case 0x5D:
      i = {Operation::EOR, AddressingMode::AbsoluteX};
      break;
    case 0xBD:
      i = {Operation::LDA, AddressingMode::AbsoluteX};
      break;
    case 0xBC:
      i = {Operation::LDY, AddressingMode::AbsoluteX};
      break;
    case 0x9D:
      i = {Operation::STA, AddressingMode::AbsoluteX};
      break;
    case 0x1E:
      i = {Operation::ASL, AddressingMode::AbsoluteX};
      break;
    case 0x3E:
      i = {Operation::ROL, AddressingMode::AbsoluteX};
      break;
    case 0x5E:
      i = {Operation::LSR, AddressingMode::AbsoluteX};
      break;
    case 0x7E:
      i = {Operation::ROR, AddressingMode::AbsoluteX};
      break;
    case 0xDE:
      i = {Operation::DEC, AddressingMode::AbsoluteX};
      break;
    case 0xFE:
      i = {Operation::INC, AddressingMode::AbsoluteX};
      break;

    // Absolute, Y
    case 0x79:
      i = {Operation::ADC, AddressingMode::AbsoluteY};
      break;
    case 0x39:
      i = {Operation::AND, AddressingMode::AbsoluteY};
      break;
    case 0xD9:
      i = {Operation::CMP, AddressingMode::AbsoluteY};
      break;
    case 0x59:
      i = {Operation::EOR, AddressingMode::AbsoluteY};
      break;
    case 0xB9:
      i = {Operation::LDA, AddressingMode::AbsoluteY};
      break;
    case 0xBE:
      i = {Operation::LDX, AddressingMode::AbsoluteY};
      break;
    case 0x19:
      i = {Operation::ORA, AddressingMode::AbsoluteY};
      break;
    case 0xF9:
      i = {Operation::SBC, AddressingMode::AbsoluteY};
      break;

    case 0x99:
      i = {Operation::STA, AddressingMode::AbsoluteY};
      break;

    // Indexed Indirect
    case 0x61:
      i = {Operation::ADC, AddressingMode::IndexedIndirect};
      break;
    case 0x21:
      i = {Operation::AND, AddressingMode::IndexedIndirect};
      break;
    case 0xC1:
      i = {Operation::CMP, AddressingMode::IndexedIndirect};
      break;
    case 0x41:
      i = {Operation::EOR, AddressingMode::IndexedIndirect};
      break;
    case 0xA1:
      i = {Operation::LDA, AddressingMode::IndexedIndirect};
      break;
    case 0x01:
      i = {Operation::ORA, AddressingMode::IndexedIndirect};
      break;
    case 0xE1:
      i = {Operation::SBC, AddressingMode::IndexedIndirect};
      break;
    case 0x81:
      i = {Operation::STA, AddressingMode::IndexedIndirect};
      break;

    // Indirect Indexed
    case 0x71:
      i = {Operation::ADC, AddressingMode::IndirectIndexed};
      break;
    case 0x31:
      i = {Operation::AND, AddressingMode::IndirectIndexed};
      break;
    case 0xD1:
      i = {Operation::CMP, AddressingMode::IndirectIndexed};
      break;
    case 0x51:
      i = {Operation::EOR, AddressingMode::IndirectIndexed};
      break;
    case 0xB1:
      i = {Operation::LDA, AddressingMode::IndirectIndexed};
      break;
    case 0x11:
      i = {Operation::ORA, AddressingMode::IndirectIndexed};
      break;
    case 0xF1:
      i = {Operation::SBC, AddressingMode::IndirectIndexed};
      break;
    case 0x91:
      i = {Operation::STA, AddressingMode::IndirectIndexed};
      break;

    // Accumulator
    case 0x0A:
      i = {Operation::ASL, AddressingMode::Accumulator};
      break;
    case 0x4A:
      i = {Operation::LSR, AddressingMode::Accumulator};
      break;
    case 0x2A:
      i = {Operation::ROL, AddressingMode::Accumulator};
      break;
    case 0x6A:
      i = {Operation::ROR, AddressingMode::Accumulator};
      break;

    // Relative
    case 0x90:
      i = {Operation::BCC, AddressingMode::Relative};
      break;
    case 0xB0:
      i = {Operation::BCS, AddressingMode::Relative};
      break;
    case 0xF0:
      i = {Operation::BEQ, AddressingMode::Relative};
      break;
    case 0x30:
      i = {Operation::BMI, AddressingMode::Relative};
      break;
    case 0xD0:
      i = {Operation::BNE, AddressingMode::Relative};
      break;
    case 0x10:
      i = {Operation::BPL, AddressingMode::Relative};
      break;
    case 0x50:
      i = {Operation::BVC, AddressingMode::Relative};
      break;
    case 0x70:
      i = {Operation::BVS, AddressingMode::Relative};
      break;

    // Implicit
    case 0x18:
      i = {Operation::CLC, AddressingMode::Implicit};
      break;
    case 0x38:
      i = {Operation::SEC, AddressingMode::Implicit};
      break;
    case 0x58:
      i = {Operation::CLI, AddressingMode::Implicit};
      break;
    case 0x78:
      i = {Operation::SEI, AddressingMode::Implicit};
      break;
    case 0x88:
      i = {Operation::DEY, AddressingMode::Implicit};
      break;
    case 0x8A:
      i = {Operation::TXA, AddressingMode::Implicit};
      break;
    case 0x98:
      i = {Operation::TYA, AddressingMode::Implicit};
      break;
    case 0x9A:
      i = {Operation::TXS, AddressingMode::Implicit};
      break;
    case 0xA8:
      i = {Operation::TAY, AddressingMode::Implicit};
      break;
    case 0xAA:
      i = {Operation::TAX, AddressingMode::Implicit};
      break;
    case 0xB8:
      i = {Operation::CLV, AddressingMode::Implicit};
      break;
    case 0xBA:
      i = {Operation::TSX, AddressingMode::Implicit};
      break;
    case 0xC8:
      i = {Operation::INY, AddressingMode::Implicit};
      break;
    case 0xCA:
      i = {Operation::DEX, AddressingMode::Implicit};
      break;
    case 0xD8:
      i = {Operation::CLD, AddressingMode::Implicit};
      break;
    case 0xE8:
      i = {Operation::INX, AddressingMode::Implicit};
      break;
    case 0xEA:
      i = {Operation::NOP, AddressingMode::Implicit};
      break;
    case 0xF8:
      i = {Operation::SED, AddressingMode::Implicit};
      break;

    case 0x08:
      i = {Operation::PHP, AddressingMode::Implicit};
      break;
    case 0x48:
      i = {Operation::PHA, AddressingMode::Implicit};
      break;
    case 0x28:
      i = {Operation::PLP, AddressingMode::Implicit};
      break;
    case 0x68:
      i = {Operation::PLA, AddressingMode::Implicit};
      break;
    case 0x40:
      i = {Operation::RTI, AddressingMode::Implicit};
      break;
    case 0x60:
      i = {Operation::RTS, AddressingMode::Implicit};
      break;
    case 0x00:
      i = {Operation::BRK, AddressingMode::Implicit};
      break;

    // Indirect
    case 0x6C:
      i = {Operation::JMP, AddressingMode::Indirect};
      break;

    default: {
      std::string message = "Decode Error: PC[" + std::to_string(pc - 1) +
                            "] = " + std::to_string(byte);
      warning(message.c_str());
      i = {Operation::NOP, AddressingMode::Implicit};
      break;
    }
  }
}

void CPU::pushStack(uint8_t val) { memory[0x100 | sp--] = val; }

uint8_t CPU::popStack() { return memory[0x100 | sp]; }

void CPU::executeImplicit() {
  switch (i.o) {
    case Operation::CLC:
      CLC();
      break;
    case Operation::SEC:
      SEC();
      break;
    case Operation::CLI:
      CLI();
      break;
    case Operation::SEI:
      SEI();
      break;
    case Operation::DEY:
      DEY();
      break;
    case Operation::TXA:
      TXA();
      break;
    case Operation::TYA:
      TYA();
      break;
    case Operation::TXS:
      TXS();
      break;
    case Operation::TAY:
      TAY();
      break;
    case Operation::TAX:
      TAX();
      break;
    case Operation::CLV:
      CLV();
      break;
    case Operation::TSX:
      TSX();
      break;
    case Operation::INY:
      INY();
      break;
    case Operation::DEX:
      DEX();
      break;
    case Operation::CLD:
      CLD();
      break;
    case Operation::INX:
      INX();
      break;
    case Operation::NOP:
      break;
    case Operation::SED:
      SED();
      break;
    // These operations take more than 2 cycles and handle stage separately
    case Operation::PHP:
      PHP();
      return;
    case Operation::PHA:
      PHA();
      return;
    case Operation::PLP:
      PLP();
      return;
    case Operation::PLA:
      PLA();
      return;
    case Operation::RTI:
      RTI();
      return;
    case Operation::RTS:
      RTS();
      return;
    case Operation::BRK:
      BRK();
      return;
    default:
      error("Invalid Implicit Operation");
  }

  // All non complex implicit instructions will reach here
  // These reads are put in incase it has an effect on open bus behavior
  (void)memory[pc];
  stage = 0;
}

void CPU::executeAccumulator() {
  switch (i.o) {
    case Operation::ASL:
      ra = ASL(ra);
      break;
    case Operation::LSR:
      ra = LSR(ra);
      break;
    case Operation::ROL:
      ra = ROL(ra);
      break;
    case Operation::ROR:
      ra = ROR(ra);
      break;
    default:
      error("Invalid Accumulator Operation");
  }

  (void)memory[pc];
  stage = 0;
}

void CPU::executeImmediate() {
  uint8_t val = memory[pc++];
  switch (i.o) {
    case Operation::ADC:
      ADC(val);
      break;
    case Operation::AND:
      AND(val);
      break;
    case Operation::CMP:
      CMP(val);
      break;
    case Operation::CPY:
      CPY(val);
      break;
    case Operation::CPX:
      CPX(val);
      break;
    case Operation::EOR:
      EOR(val);
      break;
    case Operation::LDA:
      LDA(val);
      break;
    case Operation::LDX:
      LDX(val);
      break;
    case Operation::LDY:
      LDY(val);
      break;
    case Operation::ORA:
      ORA(val);
      break;
    case Operation::SBC:
      SBC(val);
      break;
    default:
      error("Invalid Immediate Operation");
  }
  stage = 0;
}

template <typename F>
void CPU::readModifyWrite(F func) {
  static uint8_t val = 0;
  switch (stage) {
    case 3:
      val = memory[addr];
      break;
    case 4:
      memory[addr] = val;
      break;
    case 5:
      func(val);
      memory[addr] = func(val);
      stage = 0;
      return;
  }
  stage++;
}

void CPU::executeAbsolute() {
  if (stage == 1) {
    addr = memory[pc++];
    stage++;
    return;
  } else if (stage == 2) {
    addr |= (static_cast<uint16_t>(memory[pc++]) << 8);
    stage++;

    // JMP executes in 3 cycles and can continue past here
    if (i.o != Operation::JMP) return;
  }

  switch (i.o) {
    case Operation::JMP:
      JMP(addr);
      break;

    // Read Instructions
    case Operation::ADC:
      ADC(memory[addr]);
      break;
    case Operation::AND:
      AND(memory[addr]);
      break;
    case Operation::BIT:
      BIT(memory[addr]);
      break;
    case Operation::CMP:
      CMP(memory[addr]);
      break;
    case Operation::CPX:
      CPX(memory[addr]);
      break;
    case Operation::CPY:
      CPY(memory[addr]);
      break;
    case Operation::EOR:
      EOR(memory[addr]);
      break;
    case Operation::LDA:
      LDA(memory[addr]);
      break;
    case Operation::LDX:
      LDX(memory[addr]);
      break;
    case Operation::LDY:
      LDY(memory[addr]);
      break;
    case Operation::ORA:
      ORA(memory[addr]);
      break;
    case Operation::SBC:
      SBC(memory[addr]);
      break;

    // Read, Modify, Write
    case Operation::ASL:
      readModifyWrite([this](uint8_t val) { return ASL(val); });
      return;
    case Operation::LSR:
      readModifyWrite([this](uint8_t val) { return LSR(val); });
      return;
    case Operation::ROL:
      readModifyWrite([this](uint8_t val) { return ROL(val); });
      return;
    case Operation::ROR:
      readModifyWrite([this](uint8_t val) { return ROR(val); });
      return;
    case Operation::INC:
      readModifyWrite([this](uint8_t val) { return INC(val); });
      return;
    case Operation::DEC:
      readModifyWrite([this](uint8_t val) { return DEC(val); });
      return;

    // Write
    case Operation::STA:
      STA(addr);
      break;
    case Operation::STX:
      STX(addr);
      break;
    case Operation::STY:
      STY(addr);
      break;
    default:
      error("Invalid Absolute Operation");
  }
  stage = 0;
}

void CPU::executeZeroPage() {
  if (stage == 1) {
    addr = memory[pc++];
    stage++;
    return;
  }

  switch (i.o) {
    // Read
    case Operation::ADC:
      ADC(memory[addr]);
      break;
    case Operation::AND:
      AND(memory[addr]);
      break;
    case Operation::BIT:
      BIT(memory[addr]);
      break;
    case Operation::CMP:
      CMP(memory[addr]);
      break;
    case Operation::CPX:
      CPX(memory[addr]);
      break;
    case Operation::CPY:
      CPY(memory[addr]);
      break;
    case Operation::LDA:
      LDA(memory[addr]);
      break;
    case Operation::LDX:
      LDX(memory[addr]);
      break;
    case Operation::LDY:
      LDY(memory[addr]);
      break;
    case Operation::EOR:
      EOR(memory[addr]);
      break;
    case Operation::ORA:
      ORA(memory[addr]);
      break;
    case Operation::SBC:
      SBC(memory[addr]);
      break;

    // Write
    case Operation::STA:
      STA(addr);
      break;
    case Operation::STX:
      STX(addr);
      break;
    case Operation::STY:
      STY(addr);
      break;

    // Read, Modify, Write
    case Operation::ASL:
      readModifyWrite([this](uint8_t val) { return ASL(val); });
      return;
    case Operation::LSR:
      readModifyWrite([this](uint8_t val) { return LSR(val); });
      return;
    case Operation::ROL:
      readModifyWrite([this](uint8_t val) { return ROL(val); });
      return;
    case Operation::ROR:
      readModifyWrite([this](uint8_t val) { return ROR(val); });
      return;
    case Operation::INC:
      readModifyWrite([this](uint8_t val) { return INC(val); });
      return;
    case Operation::DEC:
      readModifyWrite([this](uint8_t val) { return DEC(val); });
      return;

    default:
      error("Invalid Zero Page Operation");
  }
  stage = 0;
}

void CPU::executeZeroPageX() { throw NotImplemented(); }
void CPU::executeZeroPageY() { throw NotImplemented(); }
void CPU::executeRelative() { throw NotImplemented(); }
void CPU::executeAbsoluteX() { throw NotImplemented(); }
void CPU::executeAbsoluteY() { throw NotImplemented(); }
void CPU::executeIndirect() { throw NotImplemented(); }
void CPU::executeIndexedIndirect() { throw NotImplemented(); }
void CPU::executeIndirectIndexed() { throw NotImplemented(); }

void CPU::doCycle() {
  // Fetch stage
  if (stage == 0) {
    decode(memory[pc++]);
    stage++;
    return;
  }

  switch (i.am) {
    case AddressingMode::Implicit:
      executeImplicit();
      break;
    case AddressingMode::Accumulator:
      executeAccumulator();
      break;
    case AddressingMode::Immediate:
      executeImmediate();
      break;
    case AddressingMode::ZeroPage:
      executeZeroPage();
      break;
    case AddressingMode::ZeroPageX:
      executeZeroPageX();
      break;
    case AddressingMode::ZeroPageY:
      executeZeroPageY();
      break;
    case AddressingMode::Relative:
      executeRelative();
      break;
    case AddressingMode::Absolute:
      executeAbsolute();
      break;
    case AddressingMode::AbsoluteX:
      executeAbsoluteX();
      break;
    case AddressingMode::AbsoluteY:
      executeAbsoluteY();
      break;
    case AddressingMode::Indirect:
      executeIndirect();
      break;
    case AddressingMode::IndexedIndirect:
      executeIndexedIndirect();
      break;
    case AddressingMode::IndirectIndexed:
      executeIndirectIndexed();
      break;
  }
}

/////////////////////////////////////////////////////////////////////////////////
//                                Instructions //
/////////////////////////////////////////////////////////////////////////////////

// Into Register

void CPU::ADC(uint8_t val) {
  uint16_t result = static_cast<uint16_t>(val) + static_cast<uint16_t>(ra) +
                    static_cast<uint16_t>(rf.carry);
  uint8_t result_8 = static_cast<uint8_t>(result);

  rf.carry = result > 0xFF;
  rf.overflow = ((result_8 ^ ra) & (result_8 ^ val) & 0x80) != 0;

  ra = result_8;
  setZero(ra);
  setSign(ra);
}

void CPU::AND(uint8_t val) {
  ra = ra & val;

  setZero(ra);
  setSign(ra);
}

void CPU::BIT(uint8_t val) {
  rf.zero = ra & val;
  rf.overflow = val & 0x40;
  setSign(val);
}

void CPU::__compare(uint8_t first, uint8_t second) {
  rf.carry = first >= second;
  setZero(first - second);
  setSign(first - second);
}
void CPU::CMP(uint8_t val) { __compare(ra, val); }
void CPU::CPX(uint8_t val) { __compare(rx, val); }
void CPU::CPY(uint8_t val) { __compare(ry, val); }

void CPU::EOR(uint8_t val) {
  ra = ra ^ val;
  setZero(ra);
  setSign(ra);
}

void CPU::LDA(uint8_t val) {
  ra = val;
  setZero(ra);
  setSign(ra);
}

void CPU::LDX(uint8_t val) {
  rx = val;
  setZero(rx);
  setSign(rx);
}

void CPU::LDY(uint8_t val) {
  ry = val;
  setZero(ry);
  setSign(ry);
}

void CPU::ORA(uint8_t val) {
  ra |= val;
  setZero(ra);
  setSign(ra);
}

void CPU::SBC(uint8_t val) {
  uint16_t result = static_cast<uint16_t>(ra) - static_cast<uint16_t>(val) -
                    (1 - static_cast<uint16_t>(rf.carry));
  uint8_t result_8 = static_cast<uint8_t>(result);

  rf.carry = static_cast<int16_t>(result) >= 0;
  rf.overflow = ((result_8 ^ ra) & (result_8 ^ val) & 0x80) != 0;

  ra = result_8;
  setZero(ra);
  setSign(ra);
}

// Into Accumulator or Memory

uint8_t CPU::ASL(uint8_t val) {
  rf.carry = val & 0x80;
  setZero(ra);

  val *= 2;
  setSign(val);
  return val;
}

uint8_t CPU::LSR(uint8_t val) {
  rf.carry = val & 0x80;
  setZero(ra);

  val >>= 1;
  setSign(val);
  return val;
}

uint8_t CPU::ROL(uint8_t val) {
  bool oldCarry = rf.carry;
  rf.carry = val & 0x80;
  val = (val << 1) | oldCarry;

  setZero(ra);
  setSign(val);
  return val;
}

uint8_t CPU::ROR(uint8_t val) {
  uint8_t oldCarry = static_cast<uint8_t>(rf.carry) << 7;
  rf.carry = val & 0x01;
  val = (val >> 1) | (oldCarry);

  setZero(ra);
  setSign(val);
  return val;
}

uint8_t CPU::DEC(uint8_t val) {
  uint8_t result = val - 1;
  setZero(result);
  setSign(result);
  return result;
}

uint8_t CPU::INC(uint8_t val) {
  uint8_t result = val + 1;
  setZero(result);
  setSign(result);
  return result;
}

// Branches

bool CPU::BCC(uint8_t offset) { throw NotImplemented(); }
bool CPU::BCS(uint8_t offset) { throw NotImplemented(); }
bool CPU::BEQ(uint8_t offset) { throw NotImplemented(); }
bool CPU::BMI(uint8_t offset) { throw NotImplemented(); }
bool CPU::BNE(uint8_t offset) { throw NotImplemented(); }
bool CPU::BPL(uint8_t offset) { throw NotImplemented(); }
bool CPU::BVC(uint8_t offset) { throw NotImplemented(); }
bool CPU::BVS(uint8_t offset) { throw NotImplemented(); }

// Unconditional Jumps

void CPU::JMP(uint16_t addr) { pc = addr; }

/*
  #  address R/W description
  --- ------- --- -------------------------------------------------
  1    PC     R  fetch opcode, increment PC
  2    PC     R  fetch low address byte, increment PC
  3  $0100,S  R  internal operation (predecrement S?)
  4  $0100,S  W  push PCH on stack, decrement S
  5  $0100,S  W  push PCL on stack, decrement S
  6    PC     R  copy low address byte to PCL, fetch high address
                  byte to PCH
*/
void CPU::JSR(uint16_t addr) {
  static uint16_t newPC = 0;

  switch (stage) {
    case 1:
      newPC = memory[pc++];
      break;
    case 2:
      (void)memory[0x0100 | sp];
      break;
    case 3:
      pushStack(pc >> 8);
      break;  // Push PC High
    case 4:
      pushStack(pc & 0xFF);
      break;  // Push PC Low
    case 5:
      newPC |= memory[pc] << 8;
      pc = newPC;
      stage = 0;
      return;
  }
  stage++;
}

// Stores

void CPU::STA(uint16_t addr) { memory[addr] = ra; }
void CPU::STX(uint16_t addr) { memory[addr] = rx; }
void CPU::STY(uint16_t addr) { memory[addr] = ry; }

// Implicit

void CPU::CLC() { rf.carry = false; }
void CPU::CLD() { rf.decimalMode = false; }
void CPU::CLI() { rf.irqDisable = false; }
void CPU::CLV() { rf.overflow = false; }

void CPU::DEX() { rx = DEC(rx); }
void CPU::DEY() { ry = DEC(ry); }

void CPU::INX() { rx = INC(rx); }
void CPU::INY() { ry = INC(ry); }

void CPU::SEC() { rf.carry = true; }
void CPU::SED() { rf.decimalMode = true; }
void CPU::SEI() { rf.irqDisable = true; }

void CPU::TAX() {
  rx = ra;
  setZero(rx);
  setSign(rx);
}

void CPU::TAY() {
  ry = ra;
  setZero(ry);
  setSign(ry);
}

void CPU::TSX() {
  rx = sp;
  setZero(rx);
  setSign(rx);
}

void CPU::TXA() {
  ra = rx;
  setZero(ra);
  setSign(ra);
}

void CPU::TXS() { sp = rx; }

void CPU::TYA() {
  ra = ry;
  setZero(ra);
  setSign(ra);
}

// Complex Implicit Instructions
/*
PHA, PHP

  #  address R/W description
  --- ------- --- -----------------------------------------------
  1    PC     R  fetch opcode, increment PC
  2    PC     R  read next instruction byte (and throw it away)
  3  $0100,S  W  push register on stack, decrement S
*/
void CPU::PHA() {
  switch (stage) {
    case 1:
      (void)memory[pc];
      stage++;
    case 2:
      pushStack(ra);
      stage = 0;
    default:
      error("Invalid PHA stage");
  }
}

void CPU::PHP() {
  switch (stage) {
    case 1:
      (void)memory[pc];
      stage++;
      return;
    case 2:
      pushStack(getStatus());
      stage = 0;
      return;
    default:
      error("Invalid PHP stage");
  }
}

/*
PLA, PLP

  #  address R/W description
  --- ------- --- -----------------------------------------------
  1    PC     R  fetch opcode, increment PC
  2    PC     R  read next instruction byte (and throw it away)
  3  $0100,S  R  increment S
  4  $0100,S  R  pull register from stack
*/
void CPU::PLA() {
  switch (stage) {
    case 1:
      (void)memory[pc];
      break;
    case 2:
      sp++;
      break;
    case 3:
      ra = popStack();
      setZero(ra);
      setSign(ra);
      stage = 0;
      return;
    default:
      error("Invalid PLA stage");
  }
  stage++;
}

void CPU::PLP() {
  switch (stage) {
    case 1:
      (void)memory[pc];
      break;
    case 2:
      sp++;
      break;
    case 3: {
      FlagConversion fc;
      fc.byte = popStack();
      rf = fc.f;
      stage = 0;
      return;
    }
    default:
      error("Invalid PLP stage");
  }
  stage++;
}

/*
  #  address R/W description
  --- ------- --- -----------------------------------------------
  1    PC     R  fetch opcode, increment PC
  2    PC     R  read next instruction byte (and throw it away)
  3  $0100,S  R  increment S
  4  $0100,S  R  pull P from stack, increment S
  5  $0100,S  R  pull PCL from stack, increment S
  6  $0100,S  R  pull PCH from stack
*/
void CPU::RTI() {
  switch (stage) {
    case 1:
      (void)memory[pc];
      break;
    case 2:
      sp++;
      break;
    case 3: {
      FlagConversion fc;
      fc.byte = popStack();
      rf = fc.f;
      sp++;
      break;
    }
    case 4:
      pc = popStack();
      sp++;
      break;
    case 5:
      pc = static_cast<uint16_t>(popStack()) << 8;
      stage = 0;
      return;
    default:
      error("Invalid RTI stage");
  }
  stage++;
}

void CPU::RTS() {
  switch (stage) {
    case 1:
      (void)memory[pc];
      break;
    case 2:
      sp++;
      break;
    case 3:
      pc = popStack();
      sp++;
      break;
    case 4:
      pc = static_cast<uint16_t>(popStack()) << 8;
      stage = 0;
      return;
    default:
      error("Invalid RTS  stage");
  }
  stage++;
}

/*
  #  address R/W description
  --- ------- --- -----------------------------------------------
  1    PC     R  fetch opcode, increment PC
  2    PC     R  read next instruction byte (and throw it away),
                  increment PC
  3  $0100,S  W  push PCH on stack, decrement S
  4  $0100,S  W  push PCL on stack, decrement S
  5  $0100,S  W  push P on stack (with B flag set), decrement S
  6   $FFFE   R  fetch PCL
  7   $FFFF   R  fetch PCH
*/
void CPU::BRK() {
  switch (stage) {
    case 1:
      (void)
          memory[pc++];  // Read and discard next instruction then increment pc;
      break;
    case 2:
      pushStack(pc >> 8);  // Push PC high
      break;
    case 3:
      pushStack(pc & 0xFF);  // Push PC low
      break;
    case 4: {
      rf.breakFlag = true;
      pushStack(getStatus());
      break;
    }
    case 5:
      pc = memory[0xFFFE];
      break;
    case 6: {
      uint16_t pcHigh = memory[0xFFFF];
      pc |= pcHigh << 8;
      stage = 0;
      return;
    }
    default:
      error("Invalid BRK stage");
  }
  stage++;
}
