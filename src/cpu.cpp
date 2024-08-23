#include "cpu.h"

#include <cassert>
#include <cstdint>
#include <string>

#include "utils.h"

void CPU::decode(uint8_t byte) {
  switch (byte) {
    // Immediate
    case 0x09:
      op = Operation::ORA;
      state = States::Immediate;
      break;
    case 0x29:
      op = Operation::AND;
      state = States::Immediate;
      break;
    case 0x49:
      op = Operation::EOR;
      state = States::Immediate;
      break;
    case 0x69:
      op = Operation::ADC;
      state = States::Immediate;
      break;
    case 0xA0:
      op = Operation::LDY;
      state = States::Immediate;
      break;
    case 0xA2:
      op = Operation::LDX;
      state = States::Immediate;
      break;
    case 0xA9:
      op = Operation::LDA;
      state = States::Immediate;
      break;
    case 0xC0:
      op = Operation::CPY;
      state = States::Immediate;
      break;
    case 0xC9:
      op = Operation::CMP;
      state = States::Immediate;
      break;
    case 0xE0:
      op = Operation::CPX;
      state = States::Immediate;
      break;
    case 0xE9:
      op = Operation::SBC;
      state = States::Immediate;
      break;

    // Zero Page
    case 0x05:
      op = Operation::ORA;
      state = States::Zero;
      break;
    case 0x24:
      op = Operation::BIT;
      state = States::Zero;
      break;
    case 0x25:
      op = Operation::AND;
      state = States::Zero;
      break;
    case 0x45:
      op = Operation::EOR;
      state = States::Zero;
      break;
    case 0x65:
      op = Operation::ADC;
      state = States::Zero;
      break;
    case 0x84:
      op = Operation::STY;
      state = States::Zero;
      break;
    case 0x85:
      op = Operation::STA;
      state = States::Zero;
      break;
    case 0x86:
      op = Operation::STX;
      state = States::Zero;
      break;
    case 0xA4:
      op = Operation::LDY;
      state = States::Zero;
      break;
    case 0xA5:
      op = Operation::LDA;
      state = States::Zero;
      break;
    case 0xA6:
      op = Operation::LDX;
      state = States::Zero;
      break;
    case 0xC4:
      op = Operation::CPY;
      state = States::Zero;
      break;
    case 0xC5:
      op = Operation::CMP;
      state = States::Zero;
      break;
    case 0xE4:
      op = Operation::CPX;
      state = States::Zero;
      break;
    case 0xE5:
      op = Operation::SBC;
      state = States::Zero;
      break;
    case 0x06:
      op = Operation::ASL;
      state = States::Zero;
      break;
    case 0x26:
      op = Operation::ROL;
      state = States::Zero;
      break;
    case 0x46:
      op = Operation::LSR;
      state = States::Zero;
      break;
    case 0x66:
      op = Operation::ROR;
      state = States::Zero;
      break;
    case 0xC6:
      op = Operation::DEC;
      state = States::Zero;
      break;
    case 0xE6:
      op = Operation::INC;
      state = States::Zero;
      break;

    // Zero Page, X
    case 0x15:
      op = Operation::ORA;
      state = States::ZeroX;
      break;
    case 0x35:
      op = Operation::AND;
      state = States::ZeroX;
      break;
    case 0x55:
      op = Operation::EOR;
      state = States::ZeroX;
      break;
    case 0x75:
      op = Operation::ADC;
      state = States::ZeroX;
      break;
    case 0x94:
      op = Operation::STY;
      state = States::ZeroX;
      break;
    case 0x95:
      op = Operation::STA;
      state = States::ZeroX;
      break;
    case 0xB4:
      op = Operation::LDY;
      state = States::ZeroX;
      break;
    case 0xB5:
      op = Operation::LDA;
      state = States::ZeroX;
      break;
    case 0xD5:
      op = Operation::CMP;
      state = States::ZeroX;
      break;
    case 0xF5:
      op = Operation::SBC;
      state = States::ZeroX;
      break;
    case 0x16:
      op = Operation::ASL;
      state = States::ZeroX;
      break;
    case 0x36:
      op = Operation::ROL;
      state = States::ZeroX;
      break;
    case 0x56:
      op = Operation::LSR;
      state = States::ZeroX;
      break;
    case 0x76:
      op = Operation::ROR;
      state = States::ZeroX;
      break;
    case 0xD6:
      op = Operation::DEC;
      state = States::ZeroX;
      break;
    case 0xF6:
      op = Operation::INC;
      state = States::ZeroX;
      break;

    // Zero Page, Y
    case 0xB6:
      op = Operation::LDX;
      state = States::ZeroY;
      break;
    case 0x96:
      op = Operation::STX;
      state = States::ZeroY;
      break;

    // Absolute
    case 0x4C:
      op = Operation::JMP;
      state = States::Abs1;
      break;
    case 0x6D:
      op = Operation::ADC;
      state = States::Abs1;
      break;
    case 0x2D:
      op = Operation::AND;
      state = States::Abs1;
      break;
    case 0x2C:
      op = Operation::BIT;
      state = States::Abs1;
      break;
    case 0xCD:
      op = Operation::CMP;
      state = States::Abs1;
      break;
    case 0xEC:
      op = Operation::CPX;
      state = States::Abs1;
      break;
    case 0xCC:
      op = Operation::CPY;
      state = States::Abs1;
      break;
    case 0xAD:
      op = Operation::LDA;
      state = States::Abs1;
      break;
    case 0xAE:
      op = Operation::LDX;
      state = States::Abs1;
      break;
    case 0xAC:
      op = Operation::LDY;
      state = States::Abs1;
      break;
    case 0x0D:
      op = Operation::ORA;
      state = States::Abs1;
      break;
    case 0xED:
      op = Operation::SBC;
      state = States::Abs1;
      break;
    case 0x8D:
      op = Operation::STA;
      state = States::Abs1;
      break;
    case 0x8E:
      op = Operation::STX;
      state = States::Abs1;
      break;
    case 0x8C:
      op = Operation::STY;
      state = States::Abs1;
      break;
    case 0x4D:
      op = Operation::EOR;
      state = States::Abs1;
      break;
    case 0x0E:
      op = Operation::ASL;
      state = States::Abs1;
      break;
    case 0x20:
      op = Operation::JSR;
      state = States::Abs1;
      break;
    case 0x2E:
      op = Operation::ROL;
      state = States::Abs1;
      break;
    case 0x4E:
      op = Operation::LSR;
      state = States::Abs1;
      break;
    case 0x6E:
      op = Operation::ROR;
      state = States::Abs1;
      break;
    case 0xCE:
      op = Operation::DEC;
      state = States::Abs1;
      break;
    case 0xEE:
      op = Operation::INC;
      state = States::Abs1;
      break;

    // Absolute, X
    case 0x7D:
      op = Operation::ADC;
      state = States::AbsX;
      break;
    case 0x3D:
      op = Operation::AND;
      state = States::AbsX;
      break;
    case 0xFD:
      op = Operation::SBC;
      state = States::AbsX;
      break;
    case 0xDD:
      op = Operation::CMP;
      state = States::AbsX;
      break;
    case 0x1D:
      op = Operation::ORA;
      state = States::AbsX;
      break;
    case 0x5D:
      op = Operation::EOR;
      state = States::AbsX;
      break;
    case 0xBD:
      op = Operation::LDA;
      state = States::AbsX;
      break;
    case 0xBC:
      op = Operation::LDY;
      state = States::AbsX;
      break;
    case 0x9D:
      op = Operation::STA;
      state = States::AbsX;
      break;
    case 0x1E:
      op = Operation::ASL;
      state = States::AbsX;
      break;
    case 0x3E:
      op = Operation::ROL;
      state = States::AbsX;
      break;
    case 0x5E:
      op = Operation::LSR;
      state = States::AbsX;
      break;
    case 0x7E:
      op = Operation::ROR;
      state = States::AbsX;
      break;
    case 0xDE:
      op = Operation::DEC;
      state = States::AbsX;
      break;
    case 0xFE:
      op = Operation::INC;
      state = States::AbsX;
      break;

    // Absolute, Y
    case 0x79:
      op = Operation::ADC;
      state = States::AbsY;
      break;
    case 0x39:
      op = Operation::AND;
      state = States::AbsY;
      break;
    case 0xD9:
      op = Operation::CMP;
      state = States::AbsY;
      break;
    case 0x59:
      op = Operation::EOR;
      state = States::AbsY;
      break;
    case 0xB9:
      op = Operation::LDA;
      state = States::AbsY;
      break;
    case 0xBE:
      op = Operation::LDX;
      state = States::AbsY;
      break;
    case 0x19:
      op = Operation::ORA;
      state = States::AbsY;
      break;
    case 0xF9:
      op = Operation::SBC;
      state = States::AbsY;
      break;
    case 0x99:
      op = Operation::STA;
      state = States::AbsY;
      break;

    // Indexed Indirect
    case 0x61:
      op = Operation::ADC;
      state = States::Indexed1;
      break;
    case 0x21:
      op = Operation::AND;
      state = States::Indexed1;
      break;
    case 0xC1:
      op = Operation::CMP;
      state = States::Indexed1;
      break;
    case 0x41:
      op = Operation::EOR;
      state = States::Indexed1;
      break;
    case 0xA1:
      op = Operation::LDA;
      state = States::Indexed1;
      break;
    case 0x01:
      op = Operation::ORA;
      state = States::Indexed1;
      break;
    case 0xE1:
      op = Operation::SBC;
      state = States::Indexed1;
      break;
    case 0x81:
      op = Operation::STA;
      state = States::Indexed1;
      break;

    // Indirect Indexed
    case 0x71:
      op = Operation::ADC;
      state = States::IndirectIndexed1;
      break;
    case 0x31:
      op = Operation::AND;
      state = States::IndirectIndexed1;
      break;
    case 0xD1:
      op = Operation::CMP;
      state = States::IndirectIndexed1;
      break;
    case 0x51:
      op = Operation::EOR;
      state = States::IndirectIndexed1;
      break;
    case 0xB1:
      op = Operation::LDA;
      state = States::IndirectIndexed1;
      break;
    case 0x11:
      op = Operation::ORA;
      state = States::IndirectIndexed1;
      break;
    case 0xF1:
      op = Operation::SBC;
      state = States::IndirectIndexed1;
      break;
    case 0x91:
      op = Operation::STA;
      state = States::IndirectIndexed1;
      break;

    // Accumulator
    case 0x0A:
      op = Operation::ASL;
      state = States::Accumulator;
      break;
    case 0x4A:
      op = Operation::LSR;
      state = States::Accumulator;
      break;
    case 0x2A:
      op = Operation::ROL;
      state = States::Accumulator;
      break;
    case 0x6A:
      op = Operation::ROR;
      state = States::Accumulator;
      break;

    // Relative
    case 0x90:
      op = Operation::BCC;
      state = States::Branch;
      break;
    case 0xB0:
      op = Operation::BCS;
      state = States::Branch;
      break;
    case 0xF0:
      op = Operation::BEQ;
      state = States::Branch;
      break;
    case 0x30:
      op = Operation::BMI;
      state = States::Branch;
      break;
    case 0xD0:
      op = Operation::BNE;
      state = States::Branch;
      break;
    case 0x10:
      op = Operation::BPL;
      state = States::Branch;
      break;
    case 0x50:
      op = Operation::BVC;
      state = States::Branch;
      break;
    case 0x70:
      op = Operation::BVS;
      state = States::Branch;
      break;

    // Implicit
    case 0x18:
      op = Operation::CLC;
      state = States::Execute1;
      break;
    case 0x38:
      op = Operation::SEC;
      state = States::Execute1;
      break;
    case 0x58:
      op = Operation::CLI;
      state = States::Execute1;
      break;
    case 0x78:
      op = Operation::SEI;
      state = States::Execute1;
      break;
    case 0x88:
      op = Operation::DEY;
      state = States::Execute1;
      break;
    case 0x8A:
      op = Operation::TXA;
      state = States::Execute1;
      break;
    case 0x98:
      op = Operation::TYA;
      state = States::Execute1;
      break;
    case 0x9A:
      op = Operation::TXS;
      state = States::Execute1;
      break;
    case 0xA8:
      op = Operation::TAY;
      state = States::Execute1;
      break;
    case 0xAA:
      op = Operation::TAX;
      state = States::Execute1;
      break;
    case 0xB8:
      op = Operation::CLV;
      state = States::Execute1;
      break;
    case 0xBA:
      op = Operation::TSX;
      state = States::Execute1;
      break;
    case 0xC8:
      op = Operation::INY;
      state = States::Execute1;
      break;
    case 0xCA:
      op = Operation::DEX;
      state = States::Execute1;
      break;
    case 0xD8:
      op = Operation::CLD;
      state = States::Execute1;
      break;
    case 0xE8:
      op = Operation::INX;
      state = States::Execute1;
      break;
    case 0xEA:
      op = Operation::NOP;
      state = States::Execute1;
      break;
    case 0xF8:
      op = Operation::SED;
      state = States::Execute1;
      break;
    case 0x08:
      op = Operation::PHP;
      state = States::Execute1;
      break;
    case 0x48:
      op = Operation::PHA;
      state = States::Execute1;
      break;
    case 0x28:
      op = Operation::PLP;
      state = States::Execute1;
      break;
    case 0x68:
      op = Operation::PLA;
      state = States::Execute1;
      break;
    case 0x40:
      op = Operation::RTI;
      state = States::Execute1;
      break;
    case 0x60:
      op = Operation::RTS;
      state = States::Execute1;
      break;
    case 0x00:
      op = Operation::BRK;
      state = States::Execute1;
      break;

    // Indirect
    case 0x6C:
      op = Operation::JMP;
      state = States::Indirect1;
      break;

    default: {
      std::string message = "Decode Error: PC[" + std::to_string(pc - 1) +
                            "] = " + std::to_string(byte);
      warning(message.c_str());
      op = Operation::NOP;
      state = States::Execute1;
      break;
    }
  }
}

void CPU::pushStack(uint8_t val) { memory[0x100 | sp--] = val; }

uint8_t CPU::popStack() { return memory[0x100 | sp]; }

void CPU::executeImplicit() {
  switch (op) {
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
    // These operations take more than 2 cycles and handle state separately
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
  state = States::Fetch;
}

void CPU::executeAccumulator() {
  switch (op) {
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
  state = States::Fetch;
}

void CPU::executeImmediate() {
  uint8_t val = memory[pc++];
  switch (op) {
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
  state = States::Fetch;
}

bool CPU::executeBranch() {
  if (state == States::Branch) {
    switch (op) {
      case Operation::BCC:
        return BCC();
      case Operation::BCS:
        return BCS();
      case Operation::BEQ:
        return BEQ();
      case Operation::BMI:
        return BMI();
      case Operation::BNE:
        return BNE();
      case Operation::BPL:
        return BPL();
      case Operation::BVC:
        return BVC();
      case Operation::BVS:
        return BVS();
      default:
        error("Not a branch instruction");
    }
  }

  switch (state) {
    case States::Execute1:
      (void)memory[pc];  // Open bus
      if (((pc & 0xFF) + static_cast<int8_t>(value)) > 0xFF) {
        state = States::Execute2;
      } else {
        pc += static_cast<int8_t>(value);
        state = States::Fetch;
      }
      break;
    case States::Execute2:
      (void)memory[(pc & 0xFF00) | ((pc + value) & 0x00FF)];  // Open bus
      pc += static_cast<int8_t>(value);
      state = States::Fetch;
      break;
    default:
      error("Invalid Branch Case");
  }

  return false;
}

void CPU::executeInstruction() {
  // Value and Addr should be set before this function is set
  switch (op) {
    // Into Registers only
    case Operation::ADC:
      ADC(value);
      break;
    case Operation::AND:
      AND(value);
      break;
    case Operation::BIT:
      BIT(value);
      break;
    case Operation::CMP:
      CMP(value);
      break;
    case Operation::CPX:
      CPX(value);
      break;
    case Operation::CPY:
      CPY(value);
      break;
    case Operation::EOR:
      EOR(value);
      break;
    case Operation::LDA:
      LDA(value);
      break;
    case Operation::LDX:
      LDX(value);
      break;
    case Operation::LDY:
      LDY(value);
      break;
    case Operation::ORA:
      ORA(value);
      break;
    case Operation::SBC:
      SBC(value);
      break;
    // Into Memory (Accumulator ones handled specifically in DoCycle)
    case Operation::ASL:
      memory[addr] = ASL(value);
      break;
    case Operation::LSR:
      memory[addr] = LSR(value);
      break;
    case Operation::ROL:
      memory[addr] = ROL(value);
      break;
    case Operation::ROR:
      memory[addr] = ROR(value);
      break;
    case Operation::DEC:
      memory[addr] = DEC(value);
      break;
    case Operation::INC:
      memory[addr] = INC(value);
      break;
    // Stores
    case Operation::STA:
      STA(addr);
      break;
    case Operation::STX:
      STX(addr);
      break;
    case Operation::STY:
      STY(addr);
      break;
    // Jumps
    case Operation::JMP:
      JMP(addr);
      break;
    case Operation::JSR:
      JSR(addr);
      break;
    // Implicit
    case Operation::CLC:
    case Operation::SEC:
    case Operation::CLI:
    case Operation::SEI:
    case Operation::DEY:
    case Operation::TXA:
    case Operation::TYA:
    case Operation::TXS:
    case Operation::TAY:
    case Operation::TAX:
    case Operation::CLV:
    case Operation::TSX:
    case Operation::INY:
    case Operation::DEX:
    case Operation::CLD:
    case Operation::INX:
    case Operation::NOP:
    case Operation::SED:
    case Operation::PHP:
    case Operation::PHA:
    case Operation::PLP:
    case Operation::PLA:
    case Operation::RTI:
    case Operation::RTS:
    case Operation::BRK:
      executeImplicit();
      return;
    case Operation::BCC:
    case Operation::BCS:
    case Operation::BEQ:
    case Operation::BMI:
    case Operation::BNE:
    case Operation::BPL:
    case Operation::BVC:
    case Operation::BVS:
      executeBranch();
      return;

    default:
      error("Invalid Instruction in Execute Instruction");
  }
  state = States::Fetch;
}

void CPU::doCycle() {
  switch (state) {
    case States::Fetch:
      decode(memory[pc++]);  // State transition in Decode
      assert(state != States::Fetch);
      return;
    case States::Accumulator:
      executeAccumulator();
      return;
    case States::Immediate:
      executeImmediate();
      return;
    case States::Branch:
      value = memory[pc++];
      if (!executeBranch()) {
        state = States::Fetch;
      } else {
        state = States::Execute1;
      }
      break;
    case States::Zero:
      addr = memory[pc++];
      if (is_in(op, Operation::ASL, Operation::LSR, Operation::ROL,
                Operation::ROR, Operation::INC, Operation::DEC)) {
        state = States::RMWStall1;
      } else if (is_in(op, Operation::STA, Operation::STX, Operation::STY)) {
        state = States::Execute1;
      } else {
        state = States::Read;
      }
      return;
    case States::ZeroX:
      addr = memory[pc++];
      state = States::ZeroXY;
      value = rx;
      break;
    case States::ZeroY:
      addr = memory[pc++];
      state = States::ZeroXY;
      value = ry;
    case States::ZeroXY:
      addr = (memory[addr] + value) & 0xFF;
      if (is_in(op, Operation::ASL, Operation::LSR, Operation::ROL,
                Operation::ROR, Operation::INC, Operation::DEC)) {
        state = States::RMWStall1;
      } else if (is_in(op, Operation::STA, Operation::STX, Operation::STY)) {
        state = States::Execute1;
      } else {
        state = States::Read;
      }
      return;
    case States::Abs1:
      addr = memory[pc++];
      state = States::Abs2;
      return;
    case States::Abs2:
      addr |= static_cast<uint16_t>(memory[pc++]) << 8;
      if (op == Operation::JMP) {
        executeInstruction();
      } else if (is_in(op, Operation::ASL, Operation::LSR, Operation::ROL,
                       Operation::ROR, Operation::INC, Operation::DEC)) {
        state = States::RMWStall1;
      } else if (is_in(op, Operation::STA, Operation::STX, Operation::STY)) {
        state = States::Execute1;
      } else {
        state = States::Read;
      }
      return;
    case States::RMWStall1:
      value = memory[addr];
      state = States::RMWStall2;
      break;
    case States::RMWStall2:
      assert(memory[addr] == value);
      memory[addr] = value;
      state = States::Execute1;
      break;
    case States::AbsX:
      addr = memory[pc++];
      state = States::AbsXY;
      value = rx;
      break;
    case States::AbsY:
      addr = memory[pc++];
      state = States::AbsXY;
      value = ry;
      break;
    case States::AbsXY:
      addr |= static_cast<uint16_t>(memory[pc++]) << 8;
      // Writes cannot be undone so have to ensure address is completely correct
      // before doing write
      if (((addr & 0xFF) + value) > 0xFF ||
          is_in(op, Operation::STA, Operation::STX, Operation::STY,
                Operation::ASL, Operation::LSR, Operation::ROL, Operation::ROR,
                Operation::INC, Operation::DEC)) {
        state = States::AbsFix;
      } else {
        state = States::Read;
        addr += value;
      }
      break;
    case States::AbsFix:
      (void)memory[(addr & 0xFF00) |
                   ((addr + value) & 0x00FF)];  // Open bus behavior
      addr += value;
      if (is_in(op, Operation::ASL, Operation::LSR, Operation::ROL,
                Operation::ROR, Operation::INC, Operation::DEC)) {
        state = States::RMWStall1;
      } else if (is_in(op, Operation::STA, Operation::STX, Operation::STY)) {
        state = States::Execute1;
      } else {
        state = States::Read;
      }
      break;
    case States::Indexed1:
      addr = memory[pc++];
      state = States::Indexed2;
      break;
    case States::Indexed2:
      addr = (memory[addr] + rx) & 0xFF;  // Only do zero page operations
      state = States::Indexed3;
      break;
    case States::Indexed3:
      value = memory[addr];  // Value is address Low
      state = States::Indexed4;
      break;
    case States::Indexed4:
      addr = (static_cast<uint16_t>(memory[addr + 1]) << 8) | value;
      if (op == Operation::STA) {
        state = States::Execute1;
      } else {
        state = States::Read;
      }
      break;
    case States::IndirectIndexed1:
      value = memory[pc++];  // Value is pointer
      state = States::IndirectIndexed2;
      break;
    case States::IndirectIndexed2:
      addr = memory[value];
      state = States::IndirectIndexed3;
      break;
    case States::IndirectIndexed3:
      addr |= memory[(value + 1) & 0xFF];
      if (((addr & 0xFF) + ry) > 0xFF || (op == Operation::STA)) {
        state = States::IndirectIndexedFix;
      } else {
        addr += ry;
        state = States::Read;
      }
      break;
    case States::IndirectIndexedFix:
      (void)memory[(addr & 0xFF00) | ((addr + ry) & 0x00FF)];  // Open bus
      addr += ry;
      state = States::Read;
      break;
    case States::Indirect1:
      addr = memory[pc++];
      state = States::Indirect2;
      break;
    case States::Indirect2:
      addr |= static_cast<uint16_t>(memory[pc++]) << 8;
      state = States::Indirect3;
      break;
    case States::Indirect3:
      value = memory[addr];
      state = States::Indirect4;
      break;
    case States::Indirect4:
      pc = (static_cast<uint16_t>(
                memory[(addr & 0xFF00) | ((addr + 1) & 0x00FF)])
            << 8) |
           value;
      state = States::Fetch;
      break;
    case States::Read:
      value = memory[addr];
      state = States::Execute1;
      // Fall through to Execute Instruction
    case States::Execute1:
    case States::Execute2:
    case States::Execute3:
    case States::Execute4:
    case States::Execute5:
    case States::Execute6:
      executeInstruction();
      return;
    default:
      throw NotImplemented();
  }
}

/////////////////////////////////////////////////////////////////////////////////
//                                Instructions
/////////////////////////////////////////////////////////////////////////////////
#pragma region Instructions

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

bool CPU::BCC() { return !rf.carry; }
bool CPU::BCS() { return rf.carry; }
bool CPU::BEQ() { return rf.zero; }
bool CPU::BMI() { return rf.sign; }
bool CPU::BNE() { return !rf.zero; }
bool CPU::BPL() { return !rf.sign; }
bool CPU::BVC() { return !rf.overflow; }
bool CPU::BVS() { return rf.overflow; }

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

  switch (state) {
    case States::Execute1:
      newPC = memory[pc++];
      state = States::Execute2;
      break;
    case States::Execute2:
      (void)memory[0x0100 | sp];
      state = States::Execute3;
      break;
    case States::Execute3:
      pushStack(pc >> 8);
      state = States::Execute4;
      break;  // Push PC High
    case States::Execute4:
      pushStack(pc & 0xFF);
      state = States::Execute5;
      break;  // Push PC Low
    case States::Execute5:
      newPC |= memory[pc] << 8;
      pc = newPC;
      state = States::Fetch;
      return;
    default:
      error("Invalid JSR state");
  }
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
  switch (state) {
    case States::Execute1:
      (void)memory[pc];
      state = States::Execute2;
      break;
    case States::Execute2:
      pushStack(ra);
      state = States::Fetch;
      break;
    default:
      error("Invalid PHA stage");
  }
}

void CPU::PHP() {
  switch (state) {
    case States::Execute1:
      (void)memory[pc];
      state = States::Execute2;
      return;
    case States::Execute2:
      pushStack(getStatus());
      state = States::Fetch;
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
  switch (state) {
    case States::Execute1:
      (void)memory[pc];
      state = States::Execute2;
      break;
    case States::Execute2:
      sp++;
      state = States::Execute3;
      break;
    case States::Execute3:
      ra = popStack();
      setZero(ra);
      setSign(ra);
      state = States::Fetch;
      return;
    default:
      error("Invalid PLA stage");
  }
}

void CPU::PLP() {
  switch (state) {
    case States::Execute1:
      (void)memory[pc];
      state = States::Execute2;
      break;
    case States::Execute2:
      sp++;
      state = States::Execute3;
      break;
    case States::Execute3: {
      FlagConversion fc;
      fc.byte = popStack();
      rf = fc.f;
      state = States::Fetch;
      return;
    }
    default:
      error("Invalid PLP stage");
  }
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
  switch (state) {
    case States::Execute1:
      (void)memory[pc];
      state = States::Execute2;
      break;
    case States::Execute2:
      sp++;
      state = States::Execute3;
      break;
    case States::Execute3: {
      FlagConversion fc;
      fc.byte = popStack();
      rf = fc.f;
      sp++;
      state = States::Execute4;
      break;
    }
    case States::Execute4:
      pc = popStack();
      sp++;
      state = States::Execute5;
      break;
    case States::Execute5:
      pc = static_cast<uint16_t>(popStack()) << 8;
      state = States::Fetch;
      return;
    default:
      error("Invalid RTI stage");
  }
}

void CPU::RTS() {
  switch (state) {
    case States::Execute1:
      (void)memory[pc];
      state = States::Execute2;
      break;
    case States::Execute2:
      sp++;
      state = States::Execute3;
      break;
    case States::Execute3:
      pc = popStack();
      sp++;
      state = States::Execute4;
      break;
    case States::Execute4:
      pc = static_cast<uint16_t>(popStack()) << 8;
      state = States::Fetch;
      return;
    default:
      error("Invalid RTS  stage");
  }
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
  switch (state) {
    case States::Execute1:
      (void)
          memory[pc++];  // Read and discard next instruction then increment pc;
      state = States::Execute2;
      break;
    case States::Execute2:
      pushStack(pc >> 8);  // Push PC high
      state = States::Execute3;
      break;
    case States::Execute3:
      pushStack(pc & 0xFF);  // Push PC low
      state = States::Execute4;
      break;
    case States::Execute4:
      rf.breakFlag = true;
      pushStack(getStatus());
      state = States::Execute5;
      break;
    case States::Execute5:
      pc = memory[0xFFFE];
      state = States::Execute6;
      break;
    case States::Execute6: {
      uint16_t pcHigh = memory[0xFFFF];
      pc |= pcHigh << 8;
      state = States::Fetch;
      return;
    }
    default:
      error("Invalid BRK stage");
  }
}

#pragma endregion
