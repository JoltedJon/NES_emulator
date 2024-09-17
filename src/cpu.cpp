#include "cpu.h"

#include <cassert>
#include <cstdint>
#include <string>

#include "utils.h"

// Simulated on power up
CPU::CPU(NesMemory& memory)
    : ra(0),
      rx(0),
      ry(0),
      sp(0),
      rf(),
      state(States::Fetch),
      IRQ(false),
      NMI(false),
      memory(memory),
      cycle(7),
      step(false),
      printLog(false) {
  reset();
}

void CPU::reset() {
  rf.irqDisable = true;
  sp -= 3;
  // Reset Vector
  resetPC();
}

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
      state = States::Execute1;
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
      std::string message =
          "Decode Error: PC: " + to_hex(pc) + " = " + to_hex(byte);
      error(message.c_str());
      memory.dump();
      exit(1);
    }
  }
}

void CPU::pushStack(uint8_t val) { memory.write(0x100 | sp--, val); }

uint8_t CPU::popStack() { return memory.read(0x100 | sp); }

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
      error(("Invalid Implicit Operation - PC: " + to_hex(pc) + " " + opMap[op])
                .c_str());
      memory.dump();
      exit(1);
  }

  // All non complex implicit instructions will reach here
  // These reads are put in incase it has an effect on open bus behavior
  (void)memory.read(pc);
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
      error(("Invalid Accumulator Operation - PC: " + to_hex(pc) + " " +
             opMap[op])
                .c_str());
      memory.dump();
      exit(1);
  }

  (void)memory.read(pc);
  state = States::Fetch;
}

void CPU::executeImmediate() {
  uint8_t val = memory.read(pc++);
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
      error(
          ("Invalid Immediate Operation - PC: " + to_hex(pc) + " " + opMap[op])
              .c_str());
      memory.dump();
      exit(1);
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
        error(("Invalid Branch Operation - PC: " + to_hex(pc) + " " + opMap[op])
                  .c_str());
        memory.dump();
        exit(1);
    }
  }

  switch (state) {
    case States::Execute1:
      (void)memory.read(pc);  // Open bus
      if (((pc & 0xFF) + static_cast<int8_t>(value)) > 0xFF) {
        state = States::Execute2;
      } else {
        pc += static_cast<int8_t>(value);
        state = States::Fetch;
      }
      break;
    case States::Execute2:
      (void)memory.read((pc & 0xFF00) | ((pc + value) & 0x00FF));  // Open bus
      pc += static_cast<int8_t>(value);
      state = States::Fetch;
      break;
    default:
      error(("Invalid Branch Case - PC: " + to_hex(pc) +
             " Cycle: " + std::to_string(cycle))
                .c_str());
      memory.dump();
      exit(1);
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
      memory.write(addr, ASL(value));
      break;
    case Operation::LSR:
      memory.write(addr, LSR(value));
      break;
    case Operation::ROL:
      memory.write(addr, ROL(value));
      break;
    case Operation::ROR:
      memory.write(addr, ROR(value));
      break;
    case Operation::DEC:
      memory.write(addr, DEC(value));
      break;
    case Operation::INC:
      memory.write(addr, INC(value));
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
      JSR();
      return;
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
    case Operation::NMI:
    case Operation::IRQ:
      INT();
      return;

    default:
      error(
          ("Invalid Execute Instruction - PC: " + to_hex(pc) + " " + opMap[op])
              .c_str());
      memory.dump();
      exit(1);
  }
  state = States::Fetch;
}

void CPU::executeDMA() {
  switch (state) {
    case States::OAM_DMA:
      if (OAM_DMA_Cycles % 2 == 0) {
        // Read
        value = memory.read(OAM_DMA_Addr++);
      } else {
        // Write
        memory.write(0x2004, value);
      }
      OAM_DMA_Cycles--;
      if (OAM_DMA_Cycles == 0) state = States::Fetch;
      break;
    default:
      error("Invalid DMA State");
      exit(1);
  }
}

void CPU::doCycle() {
  switch (state) {
    case States::Fetch:
      if (printLog) addDebugInfo();
      if (step) {
        std::string message;
        std::cin >> message;
        if (std::cin.eof()) exit(0);
        if (message == "r") step = false;
      }

      if (NMI) {
        op = Operation::NMI;
        state = States::Execute1;
        (void)memory.read(pc);
        break;
      } else if (IRQ && !rf.irqDisable) {
        op = Operation::IRQ;
        state = States::Execute1;
        (void)memory.read(pc);
        break;
      }
      // TODO, DMC DMA has higher Priority than OAM DMA
      else if (OAM_DMA_Cycles > 0) {
        // Need allignment cycle if on odd cycle
        if (cycle % 2 == 0) state = States::OAM_DMA;
        (void)memory.read(pc);
        break;
      }

      decode(memory.read(pc++));  // State transition in Decode
      assert(state != States::Fetch);
      break;
    case States::Accumulator:
      executeAccumulator();
      break;
    case States::Immediate:
      executeImmediate();
      break;
    case States::Branch:
      value = memory.read(pc++);
      if (!executeBranch()) {
        state = States::Fetch;
      } else {
        state = States::Execute1;
      }
      break;
    case States::Zero:
      addr = memory.read(pc++);
      if (is_in(op, Operation::ASL, Operation::LSR, Operation::ROL,
                Operation::ROR, Operation::INC, Operation::DEC)) {
        state = States::RMWStall1;
      } else if (is_in(op, Operation::STA, Operation::STX, Operation::STY)) {
        state = States::Execute1;
      } else {
        state = States::Read;
      }
      break;
    case States::ZeroX:
      (void)memory.read(pc++);
      state = States::ZeroXY;
      value = rx;
      break;
    case States::ZeroY:
      (void)memory.read(pc++);
      state = States::ZeroXY;
      value = ry;
      break;
    case States::ZeroXY:
      addr = (memory.read(pc - 1) + value) & 0xFF;
      if (is_in(op, Operation::ASL, Operation::LSR, Operation::ROL,
                Operation::ROR, Operation::INC, Operation::DEC)) {
        state = States::RMWStall1;
      } else if (is_in(op, Operation::STA, Operation::STX, Operation::STY)) {
        state = States::Execute1;
      } else {
        state = States::Read;
      }
      break;
    case States::Abs1:
      addr = memory.read(pc++);
      state = States::Abs2;
      break;
    case States::Abs2:
      addr |= static_cast<uint16_t>(memory.read(pc++)) << 8;
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
      break;
    case States::RMWStall1:
      value = memory.read(addr);
      state = States::RMWStall2;
      break;
    case States::RMWStall2:
      assert(memory.read(addr) == value);
      memory.write(addr, value);
      state = States::Execute1;
      break;
    case States::AbsX:
      addr = memory.read(pc++);
      state = States::AbsXY;
      value = rx;
      break;
    case States::AbsY:
      addr = memory.read(pc++);
      state = States::AbsXY;
      value = ry;
      break;
    case States::AbsXY:
      addr |= static_cast<uint16_t>(memory.read(pc++)) << 8;
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
      (void)memory.read((addr & 0xFF00) |
                        ((addr + value) & 0x00FF));  // Open bus behavior
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
      value = memory.read(pc++);
      state = States::Indexed2;
      break;
    case States::Indexed2:
      (void)memory.read(value);
      value = (value + rx) & 0xFF;
      state = States::Indexed3;
      break;
    case States::Indexed3:
      addr = memory.read(value);  // Value is address Low
      state = States::Indexed4;
      break;
    case States::Indexed4:
      addr |= (static_cast<uint16_t>(memory.read((value + 1) & 0xFF)) << 8);
      if (op == Operation::STA) {
        state = States::Execute1;
      } else {
        state = States::Read;
      }
      break;
    case States::IndirectIndexed1:
      value = memory.read(pc++);  // Value is pointer
      state = States::IndirectIndexed2;
      break;
    case States::IndirectIndexed2:
      addr = memory.read(value);
      state = States::IndirectIndexed3;
      break;
    case States::IndirectIndexed3:
      addr |= static_cast<uint16_t>(memory.read((value + 1) & 0xFF)) << 8;
      if (((addr & 0xFF) + ry) > 0xFF || (op == Operation::STA)) {
        state = States::IndirectIndexedFix;
      } else {
        addr += ry;
        state = States::Read;
      }
      break;
    case States::IndirectIndexedFix:
      (void)memory.read((addr & 0xFF00) | ((addr + ry) & 0x00FF));  // Open bus
      addr += ry;
      state = States::Read;
      break;
    case States::Indirect1:
      addr = memory.read(pc++);
      state = States::Indirect2;
      break;
    case States::Indirect2:
      addr |= static_cast<uint16_t>(memory.read(pc++)) << 8;
      state = States::Indirect3;
      break;
    case States::Indirect3:
      value = memory.read(addr);
      state = States::Indirect4;
      break;
    case States::Indirect4:
      pc = (static_cast<uint16_t>(
                memory.read((addr & 0xFF00) | ((addr + 1) & 0x00FF)))
            << 8) |
           value;
      state = States::Fetch;
      break;
    case States::Read:
      value = memory.read(addr);
      state = States::Execute1;
      // Fall through to Execute Instruction
    case States::Execute1:
    case States::Execute2:
    case States::Execute3:
    case States::Execute4:
    case States::Execute5:
    case States::Execute6:
      executeInstruction();
      break;
    default:
      error(("Invalid State: " + stateMap[state]).c_str());
      memory.dump();
      exit(1);
  }
  ++cycle;
}

/////////////////////////////////////////////////////////////////////////////////
//                                Instructions
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
  rf.zero = (ra & val) == 0;
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
  uint8_t result_8 = ra - val - (1 - static_cast<uint8_t>(rf.carry));

  rf.carry = static_cast<int16_t>(result) >= 0;
  rf.overflow = ((result_8 ^ ra) & (result_8 ^ ~val) & 0x80) != 0;

  ra = result_8;
  setZero(ra);
  setSign(ra);
}

// Into Accumulator or Memory

uint8_t CPU::ASL(uint8_t val) {
  rf.carry = val & 0x80;

  val *= 2;
  setZero(val);
  setSign(val);
  return val;
}

uint8_t CPU::LSR(uint8_t val) {
  rf.carry = val & 0x01;

  val >>= 1;
  setZero(val);
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
void CPU::JSR() {
  switch (state) {
    case States::Execute1:
      addr = memory.read(pc++);
      state = States::Execute2;
      break;
    case States::Execute2:
      (void)memory.read(0x0100 | sp);
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
      addr |= static_cast<uint16_t>(memory.read(pc)) << 8;
      pc = addr;
      state = States::Fetch;
      return;
    default:
      error("Invalid JSR state");
      memory.dump();
      exit(1);
  }
}

// Stores

void CPU::STA(uint16_t addr) { memory.write(addr, ra); }
void CPU::STX(uint16_t addr) { memory.write(addr, rx); }
void CPU::STY(uint16_t addr) { memory.write(addr, ry); }

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
      (void)memory.read(pc);
      state = States::Execute2;
      break;
    case States::Execute2:
      pushStack(ra);
      state = States::Fetch;
      break;
    default:
      error("Invalid PHA stage");
      memory.dump();
      exit(1);
  }
}

void CPU::PHP() {
  switch (state) {
    case States::Execute1:
      (void)memory.read(pc);
      state = States::Execute2;
      return;
    case States::Execute2:
      rf.breakFlag = true;
      pushStack(getStatus());
      rf.breakFlag = false;
      state = States::Fetch;
      return;
    default:
      error("Invalid PHP stage");
      memory.dump();
      exit(1);
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
      (void)memory.read(pc);
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
      memory.dump();
      exit(1);
  }
}

void CPU::PLP() {
  switch (state) {
    case States::Execute1:
      (void)memory.read(pc);
      state = States::Execute2;
      break;
    case States::Execute2:
      sp++;
      state = States::Execute3;
      break;
    case States::Execute3:
      setStatus(popStack());
      state = States::Fetch;
      return;
    default:
      error("Invalid PLP stage");
      memory.dump();
      exit(1);
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
      (void)memory.read(pc);
      state = States::Execute2;
      break;
    case States::Execute2:
      sp++;
      state = States::Execute3;
      break;
    case States::Execute3:
      setStatus(popStack());
      sp++;
      state = States::Execute4;
      break;
    case States::Execute4:
      pc = popStack();
      sp++;
      state = States::Execute5;
      break;
    case States::Execute5:
      pc |= static_cast<uint16_t>(popStack()) << 8;
      state = States::Fetch;
      return;
    default:
      error("Invalid RTI stage");
      memory.dump();
      exit(1);
  }
}

void CPU::RTS() {
  switch (state) {
    case States::Execute1:
      (void)memory.read(pc);
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
      pc |= static_cast<uint16_t>(popStack()) << 8;
      state = States::Execute5;
      break;
    case States::Execute5:
      ++pc;
      state = States::Fetch;
      return;
    default:
      error("Invalid RTS  stage");
      memory.dump();
      exit(1);
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
      (void)memory.read(
          pc++);  // Read and discard next instruction then increment pc;
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
      pc = memory.read(0xFFFE);
      state = States::Execute6;
      break;
    case States::Execute6: {
      uint16_t pcHigh = memory.read(0xFFFF);
      pc |= pcHigh << 8;
      state = States::Fetch;
      return;
    }
    default:
      error("Invalid BRK stage");
      if (printLog) memory.dump();
      exit(1);
  }
}

// TODO interrupt hijacking
void CPU::INT() {
  switch (state) {
    case States::Execute1:
      (void)memory.read(pc);
      state = States::Execute2;
      break;
    case States::Execute2:
      pushStack(pc >> 8);
      state = States::Execute3;
      break;
    case States::Execute3:
      pushStack(pc & 0x00FF);
      state = States::Execute4;
      break;
    case States::Execute4:
      rf.breakFlag = false;
      pushStack(getStatus());
      state = States::Execute5;
    case States::Execute5:
      if (op == Operation::IRQ) rf.irqDisable = true;
      pc = (op == Operation::NMI) ? memory.read(0xFFFA) : memory.read(0xFFFE);
      state = States::Execute6;
    case States::Execute6: {
      if (op == Operation::NMI) NMI = false;
      uint16_t PCH =
          (op == Operation::NMI) ? memory.read(0xFFFB) : memory.read(0xFFFF);
      pc = PCH << 8;
      state = States::Fetch;
      break;
    }
    default:
      error("Invalid Interrupt State");
      exit(1);
  }
}

// Debug information

std::unordered_map<Operation, std::string> CPU::opMap = {
    {Operation::ADC, "ADC"}, {Operation::AND, "AND"}, {Operation::ASL, "ASL"},
    {Operation::BCC, "BCC"}, {Operation::BCS, "BCS"}, {Operation::BEQ, "BEQ"},
    {Operation::BIT, "BIT"}, {Operation::BMI, "BMI"}, {Operation::BNE, "BNE"},
    {Operation::BPL, "BPL"}, {Operation::BRK, "BRK"}, {Operation::BVC, "BVC"},
    {Operation::BVS, "BVS"}, {Operation::CLC, "CLC"}, {Operation::CLD, "CLD"},
    {Operation::CLI, "CLI"}, {Operation::CLV, "CLV"}, {Operation::CMP, "CMP"},
    {Operation::CPX, "CPX"}, {Operation::CPY, "CPY"}, {Operation::DEC, "DEC"},
    {Operation::DEX, "DEX"}, {Operation::DEY, "DEY"}, {Operation::EOR, "EOR"},
    {Operation::INC, "INC"}, {Operation::INX, "INX"}, {Operation::INY, "INY"},
    {Operation::JMP, "JMP"}, {Operation::JSR, "JSR"}, {Operation::LDA, "LDA"},
    {Operation::LDX, "LDX"}, {Operation::LDY, "LDY"}, {Operation::LSR, "LSR"},
    {Operation::NOP, "NOP"}, {Operation::ORA, "ORA"}, {Operation::PHA, "PHA"},
    {Operation::PHP, "PHP"}, {Operation::PLA, "PLA"}, {Operation::PLP, "PLP"},
    {Operation::ROL, "ROL"}, {Operation::ROR, "ROR"}, {Operation::RTI, "RTI"},
    {Operation::RTS, "RTS"}, {Operation::SBC, "SBC"}, {Operation::SEC, "SEC"},
    {Operation::SED, "SED"}, {Operation::SEI, "SEI"}, {Operation::STA, "STA"},
    {Operation::STX, "STX"}, {Operation::STY, "STY"}, {Operation::TAX, "TAX"},
    {Operation::TAY, "TAY"}, {Operation::TSX, "TSX"}, {Operation::TXA, "TXA"},
    {Operation::TXS, "TXS"}, {Operation::TYA, "TYA"}};
std::unordered_map<States, std::string> CPU::stateMap = {
    {States::Fetch, "Fetch"},
    {States::Abs1, "Abs1"},
    {States::Abs2, "Abs2"},
    {States::AbsX, "AbsX"},
    {States::AbsY, "AbsY"},
    {States::AbsXY, "AbsXY"},
    {States::AbsFix, "AbsFix"},
    {States::Zero, "Zero"},
    {States::ZeroX, "ZeroX"},
    {States::ZeroY, "ZeroY"},
    {States::ZeroXY, "ZeroXY"},
    {States::Indexed1, "Indexed1"},
    {States::Indexed2, "Indexed2"},
    {States::Indexed3, "Indexed3"},
    {States::Indexed4, "Indexed4"},
    {States::IndirectIndexed1, "IndirectIndexed1"},
    {States::IndirectIndexed2, "IndirectIndexed2"},
    {States::IndirectIndexed3, "IndirectIndexed3"},
    {States::IndirectIndexedFix, "IndirectIndexedFix"},
    {States::Indirect1, "Indirect1"},
    {States::Indirect2, "Indirect2"},
    {States::Indirect3, "Indirect3"},
    {States::Indirect4, "Indirect4"},
    {States::RMWStall1, "RMWStall1"},
    {States::RMWStall2, "RMWStall2"},
    {States::Accumulator, "Accumulator"},
    {States::Immediate, "Immediate"},
    {States::Branch, "Branch"},
    {States::Read, "Read"},
    {States::Execute1, "Execute1"},
    {States::Execute2, "Execute2"},
    {States::Execute3, "Execute3"},
    {States::Execute4, "Execute4"},
    {States::Execute5, "Execute5"},
    {States::Execute6, "Execute6"}};

void CPU::dbgImp(const std::string opStr) { debugInfo += "       " + opStr; }

void CPU::dbgZP(const std::string opStr) {
  std::string b1 = to_hex(memory.read(pc + 1));
  std::string zp = to_hex(memory.read(memory.read(pc + 1)));
  debugInfo += b1 + "     " + opStr + " $" + b1 + " = " + zp;
}

void CPU::dbgZPX(const std::string opStr) {
  std::string b1 = to_hex(memory.read(pc + 1));
  uint8_t addr = memory.read(pc + 1) + rx;
  std::string zp = to_hex(memory.read(addr));
  debugInfo +=
      b1 + "     " + opStr + " $" + b1 + ",X @ " + to_hex(addr) + " = " + zp;
}

void CPU::dbgZPY(const std::string opStr) {
  std::string b1 = to_hex(memory.read(pc + 1));
  uint8_t addr = memory.read(pc + 1) + ry;
  std::string zp = to_hex(memory.read(addr));
  debugInfo +=
      b1 + "     " + opStr + " $" + b1 + ",Y @ " + to_hex(addr) + " = " + zp;
}

void CPU::dbgImm(const std::string opStr) {
  std::string b1 = to_hex(memory.read(pc + 1));
  debugInfo += b1 + "     " + opStr + " #$" + b1;
}

void CPU::dbgAcc(const std::string opStr) {
  debugInfo += "       " + opStr + " A";
}

void CPU::dbgAbs(const std::string opStr) {
  std::string b1 = to_hex(memory.read(pc + 1));
  std::string b2 = to_hex(memory.read(pc + 2));
  uint16_t addr =
      (static_cast<uint16_t>(memory.read(pc + 2)) << 8) | (memory.read(pc + 1));
  debugInfo += b1 + " " + b2 + "  " + opStr + " $" + b2 + b1;

  if ((opStr == "STX") || (opStr == "STA") || (opStr == "STY") ||
      (opStr == "LDX") || (opStr == "LDA") || (opStr == "LDY") ||
      (opStr == "BIT") || (opStr == "ORA")) {
    debugInfo += " = " + to_hex(memory.read(addr));
  }
}

void CPU::dbgMem(const std::string opStr) {
  std::string b1 = to_hex(memory.read(pc + 1));
  std::string b2 = to_hex(memory.read(pc + 2));
  uint16_t addr =
      (static_cast<uint16_t>(memory.read(pc + 2)) << 8) | (memory.read(pc + 1));
  debugInfo += b1 + " " + b2 + "  " + opStr + " $" + b2 + b1;
  debugInfo += " = " + to_hex(memory.read(addr));
}

void CPU::dbgAbsY(const std::string opStr) {
  std::string b1 = to_hex(memory.read(pc + 1));
  std::string b2 = to_hex(memory.read(pc + 2));
  uint16_t addr1 =
      (static_cast<uint16_t>(memory.read(pc + 2)) << 8) | (memory.read(pc + 1));
  uint16_t addr2 = addr1 + ry;
  debugInfo += b1 + " " + b2 + "  " + opStr + " $" + b2 + b1 + ",Y @ " +
               to_hex(addr2) + " = " + to_hex(memory.read(addr2));
}

void CPU::dbgAbsX(const std::string opStr) {
  std::string b1 = to_hex(memory.read(pc + 1));
  std::string b2 = to_hex(memory.read(pc + 2));
  uint16_t addr1 =
      (static_cast<uint16_t>(memory.read(pc + 2)) << 8) | (memory.read(pc + 1));
  uint16_t addr2 = addr1 + rx;
  debugInfo += b1 + " " + b2 + "  " + opStr + " $" + b2 + b1 + ",X @ " +
               to_hex(addr2) + " = " + to_hex(memory.read(addr2));
}

void CPU::dbgBr(const std::string opStr) {
  std::string b1 = to_hex(memory.read(pc + 1));
  std::string offset = to_hex(
      static_cast<uint16_t>(pc + 2 + static_cast<int8_t>(memory.read(pc + 1))));
  debugInfo += b1 + "     " + opStr + " $" + offset;
}

void CPU::dbgXInd(const std::string opStr) {
  std::string b1 = to_hex(memory.read(pc + 1));
  uint8_t addr1 = memory.read(pc + 1) + rx;
  uint16_t addr2 = (memory.read((addr1 + 1) & 0xFF) << 8) | memory.read(addr1);
  std::string b2 = to_hex(memory.read(addr2));

  debugInfo += b1 + "     " + opStr + " ($" + b1 + ",X) @ " + to_hex(addr1) +
               " = " + to_hex(addr2) + " = " + b2;
}

void CPU::dbgYInd(const std::string opStr) {
  std::string b1 = to_hex(memory.read(pc + 1));
  uint8_t addr1 = memory.read(pc + 1);
  uint16_t addr2 = (memory.read((addr1 + 1) & 0xFF) << 8) | memory.read(addr1);
  uint16_t addr3 = addr2 + ry;
  std::string b2 = to_hex(memory.read(addr3));

  debugInfo += b1 + "     " + opStr + " ($" + b1 + "),Y = " + to_hex(addr2) +
               " @ " + to_hex(addr3) + " = " + b2;
}

void CPU::dbgInd(const std::string opStr) {
  std::string b1 = to_hex(memory.read(pc + 1));
  std::string b2 = to_hex(memory.read(pc + 2));
  uint16_t addr =
      (static_cast<uint16_t>(memory.read(pc + 2)) << 8) | memory.read(pc + 1);
  std::string b3 = to_hex(memory.read(addr));
  std::string b4 = to_hex(memory.read((addr & 0xFF00) | ((addr + 1) & 0x00FF)));

  debugInfo +=
      b1 + " " + b2 + "  " + opStr + " ($" + to_hex(addr) + ") = " + b4 + b3;
}

void CPU::addDebugInfo() {
  debugInfo = to_hex(pc) + "  " + to_hex(memory.read(pc)) + " ";
  switch (memory.read(pc)) {
    case 0x0:
      dbgImp("BRK");
      break;
    case 0x1:
      dbgXInd("ORA");
      break;
    case 0x2:
      // TODO Illegal instructions
      debugInfo += "Illegal";
      break;
    case 0x3:
      // TODO Illegal instructions
      debugInfo += "Illegal";
      break;
    case 0x4:
      // TODO Illegal instructions
      debugInfo += "Illegal";
      break;
    case 0x5:
      dbgZP("ORA");
      break;
    case 0x6:
      dbgZP("ASL");
      break;
    case 0x7:
      // TODO Illegal instructions
      debugInfo += "Illegal";
      break;
    case 0x8:
      dbgImp("PHP");
      break;
    case 0x9:
      dbgImm("ORA");
      break;
    case 0xa:
      dbgAcc("ASL");
      break;
    case 0xb:
      // TODO Illegal instructions
      debugInfo += "Illegal";
      break;
    case 0xc:
      // TODO Illegal instructions
      debugInfo += "Illegal";
      break;
    case 0xd:
      dbgMem("ORA");
      break;
    case 0xe:
      dbgMem("ASL");
      break;
    case 0xf:
      // TODO Illegal instructions
      debugInfo += "Illegal";
      break;
    case 0x10:
      dbgBr("BPL");
      break;
    case 0x11:
      dbgYInd("ORA");
      break;
    case 0x12:
      // TODO Illegal instructions
      debugInfo += "Illegal";
      break;
    case 0x13:
      // TODO Illegal instructions
      debugInfo += "Illegal";
      break;
    case 0x14:
      // TODO Illegal instructions
      debugInfo += "Illegal";
      break;
    case 0x15:
      dbgZPX("ORA");
      break;
    case 0x16:
      dbgZPX("ASL");
      break;
    case 0x17:
      // TODO Illegal instructions
      debugInfo += "Illegal";
      break;
    case 0x18:
      dbgImp("CLC");
      break;
    case 0x19:
      dbgAbsY("ORA");
      break;
    case 0x1a:
      // TODO Illegal instructions
      debugInfo += "Illegal";
      break;
    case 0x1b:
      // TODO Illegal instructions
      debugInfo += "Illegal";
      break;
    case 0x1c:
      // TODO Illegal instructions
      debugInfo += "Illegal";
      break;
    case 0x1d:
      dbgAbsX("ORA");
      break;
    case 0x1e:
      dbgAbsX("ASL");
      break;
    case 0x1f:
      // TODO Illegal instructions
      debugInfo += "Illegal";
      break;
    case 0x20:
      dbgAbs("JSR");
      break;
    case 0x21:
      dbgXInd("AND");
      break;
    case 0x22:
      // TODO Illegal instructions
      debugInfo += "Illegal";
      break;
    case 0x23:
      // TODO Illegal instructions
      debugInfo += "Illegal";
      break;
    case 0x24:
      dbgZP("BIT");
      break;
    case 0x25:
      dbgZP("AND");
      break;
    case 0x26:
      dbgZP("ROL");
      break;
    case 0x27:
      // TODO Illegal instructions
      debugInfo += "Illegal";
      break;
    case 0x28:
      dbgImp("PLP");
      break;
    case 0x29:
      dbgImm("AND");
      break;
    case 0x2a:
      dbgAcc("ROL");
      break;
    case 0x2b:
      // TODO Illegal instructions
      debugInfo += "Illegal";
      break;
    case 0x2c:
      dbgMem("BIT");
      break;
    case 0x2d:
      dbgMem("AND");
      break;
    case 0x2e:
      dbgMem("ROL");
      break;
    case 0x2f:
      // TODO Illegal instructions
      debugInfo += "Illegal";
      break;
    case 0x30:
      dbgBr("BMI");
      break;
    case 0x31:
      dbgYInd("AND");
      break;
    case 0x32:
      // TODO Illegal instructions
      debugInfo += "Illegal";
      break;
    case 0x33:
      // TODO Illegal instructions
      debugInfo += "Illegal";
      break;
    case 0x34:
      // TODO Illegal instructions
      debugInfo += "Illegal";
      break;
    case 0x35:
      dbgZPX("AND");
      break;
    case 0x36:
      dbgZPX("ROL");
      break;
    case 0x37:
      // TODO Illegal instructions
      debugInfo += "Illegal";
      break;
    case 0x38:
      dbgImp("SEC");
      break;
    case 0x39:
      dbgAbsY("AND");
      break;
    case 0x3a:
      // TODO Illegal instructions
      debugInfo += "Illegal";
      break;
    case 0x3b:
      // TODO Illegal instructions
      debugInfo += "Illegal";
      break;
    case 0x3c:
      // TODO Illegal instructions
      debugInfo += "Illegal";
      break;
    case 0x3d:
      dbgAbsX("AND");
      break;
    case 0x3e:
      dbgAbsX("ROL");
      break;
    case 0x3f:
      // TODO Illegal instructions
      debugInfo += "Illegal";
      break;
    case 0x40:
      dbgImp("RTI");
      break;
    case 0x41:
      dbgXInd("EOR");
      break;
    case 0x42:
      // TODO Illegal instructions
      debugInfo += "Illegal";
      break;
    case 0x43:
      // TODO Illegal instructions
      debugInfo += "Illegal";
      break;
    case 0x44:
      // TODO Illegal instructions
      debugInfo += "Illegal";
      break;
    case 0x45:
      dbgZP("EOR");
      break;
    case 0x46:
      dbgZP("LSR");
      break;
    case 0x47:
      // TODO Illegal instructions
      debugInfo += "Illegal";
      break;
    case 0x48:
      dbgImp("PHA");
      break;
    case 0x49:
      dbgImm("EOR");
      break;
    case 0x4a:
      dbgAcc("LSR");
      break;
    case 0x4b:
      // TODO Illegal instructions
      debugInfo += "Illegal";
      break;
    case 0x4c:
      dbgAbs("JMP");
      break;
    case 0x4d:
      dbgMem("EOR");
      break;
    case 0x4e:
      dbgMem("LSR");
      break;
    case 0x4f:
      // TODO Illegal instructions
      debugInfo += "Illegal";
      break;
    case 0x50:
      dbgBr("BVC");
      break;
    case 0x51:
      dbgYInd("EOR");
      break;
    case 0x52:
      // TODO Illegal instructions
      debugInfo += "Illegal";
      break;
    case 0x53:
      // TODO Illegal instructions
      debugInfo += "Illegal";
      break;
    case 0x54:
      // TODO Illegal instructions
      debugInfo += "Illegal";
      break;
    case 0x55:
      dbgZPX("EOR");
      break;
    case 0x56:
      dbgZPX("LSR");
      break;
    case 0x57:
      // TODO Illegal instructions
      debugInfo += "Illegal";
      break;
    case 0x58:
      dbgImp("CLI");
      break;
    case 0x59:
      dbgAbsY("EOR");
      break;
    case 0x5a:
      // TODO Illegal instructions
      debugInfo += "Illegal";
      break;
    case 0x5b:
      // TODO Illegal instructions
      debugInfo += "Illegal";
      break;
    case 0x5c:
      // TODO Illegal instructions
      debugInfo += "Illegal";
      break;
    case 0x5d:
      dbgAbsX("EOR");
      break;
    case 0x5e:
      dbgAbsX("LSR");
      break;
    case 0x5f:
      // TODO Illegal instructions
      debugInfo += "Illegal";
      break;
    case 0x60:
      dbgImp("RTS");
      break;
    case 0x61:
      dbgXInd("ADC");
      break;
    case 0x62:
      // TODO Illegal instructions
      debugInfo += "Illegal";
      break;
    case 0x63:
      // TODO Illegal instructions
      debugInfo += "Illegal";
      break;
    case 0x64:
      // TODO Illegal instructions
      debugInfo += "Illegal";
      break;
    case 0x65:
      dbgZP("ADC");
      break;
    case 0x66:
      dbgZP("ROR");
      break;
    case 0x67:
      // TODO Illegal instructions
      debugInfo += "Illegal";
      break;
    case 0x68:
      dbgImp("PLA");
      break;
    case 0x69:
      dbgImm("ADC");
      break;
    case 0x6a:
      dbgAcc("ROR");
      break;
    case 0x6b:
      // TODO Illegal instructions
      debugInfo += "Illegal";
      break;
    case 0x6c:
      dbgInd("JMP");
      break;
    case 0x6d:
      dbgMem("ADC");
      break;
    case 0x6e:
      dbgMem("ROR");
      break;
    case 0x6f:
      // TODO Illegal instructions
      debugInfo += "Illegal";
      break;
    case 0x70:
      dbgBr("BVS");
      break;
    case 0x71:
      dbgYInd("ADC");
      break;
    case 0x72:
      // TODO Illegal instructions
      debugInfo += "Illegal";
      break;
    case 0x73:
      // TODO Illegal instructions
      debugInfo += "Illegal";
      break;
    case 0x74:
      // TODO Illegal instructions
      debugInfo += "Illegal";
      break;
    case 0x75:
      dbgZPX("ADC");
      break;
    case 0x76:
      dbgZPX("ROR");
      break;
    case 0x77:
      // TODO Illegal instructions
      debugInfo += "Illegal";
      break;
    case 0x78:
      dbgImp("SEI");
      break;
    case 0x79:
      dbgAbsY("ADC");
      break;
    case 0x7a:
      // TODO Illegal instructions
      debugInfo += "Illegal";
      break;
    case 0x7b:
      // TODO Illegal instructions
      debugInfo += "Illegal";
      break;
    case 0x7c:
      // TODO Illegal instructions
      debugInfo += "Illegal";
      break;
    case 0x7d:
      dbgAbsX("ADC");
      break;
    case 0x7e:
      dbgAbsX("ROR");
      break;
    case 0x7f:
      // TODO Illegal instructions
      debugInfo += "Illegal";
      break;
    case 0x80:
      // TODO Illegal instructions
      debugInfo += "Illegal";
      break;
    case 0x81:
      dbgXInd("STA");
      break;
    case 0x82:
      // TODO Illegal instructions
      debugInfo += "Illegal";
      break;
    case 0x83:
      // TODO Illegal instructions
      debugInfo += "Illegal";
      break;
    case 0x84:
      dbgZP("STY");
      break;
    case 0x85:
      dbgZP("STA");
      break;
    case 0x86:
      dbgZP("STX");
      break;
    case 0x87:
      // TODO Illegal instructions
      debugInfo += "Illegal";
      break;
    case 0x88:
      dbgImp("DEY");
      break;
    case 0x89:
      // TODO Illegal instructions
      debugInfo += "Illegal";
      break;
    case 0x8a:
      dbgImp("TXA");
      break;
    case 0x8b:
      // TODO Illegal instructions
      debugInfo += "Illegal";
      break;
    case 0x8c:
      dbgMem("STY");
      break;
    case 0x8d:
      dbgMem("STA");
      break;
    case 0x8e:
      dbgMem("STX");
      break;
    case 0x8f:
      // TODO Illegal instructions
      debugInfo += "Illegal";
      break;
    case 0x90:
      dbgBr("BCC");
      break;
    case 0x91:
      dbgYInd("STA");
      break;
    case 0x92:
      // TODO Illegal instructions
      debugInfo += "Illegal";
      break;
    case 0x93:
      // TODO Illegal instructions
      debugInfo += "Illegal";
      break;
    case 0x94:
      dbgZPX("STY");
      break;
    case 0x95:
      dbgZPX("STA");
      break;
    case 0x96:
      dbgZPY("STX");
      break;
    case 0x97:
      // TODO Illegal instructions
      debugInfo += "Illegal";
      break;
    case 0x98:
      dbgImp("TYA");
      break;
    case 0x99:
      dbgAbsY("STA");
      break;
    case 0x9a:
      dbgImp("TXS");
      break;
    case 0x9b:
      // TODO Illegal instructions
      debugInfo += "Illegal";
      break;
    case 0x9c:
      // TODO Illegal instructions
      debugInfo += "Illegal";
      break;
    case 0x9d:
      dbgAbsX("STA");
      break;
    case 0x9e:
      // TODO Illegal instructions
      debugInfo += "Illegal";
      break;
    case 0x9f:
      // TODO Illegal instructions
      debugInfo += "Illegal";
      break;
    case 0xa0:
      dbgImm("LDY");
      break;
    case 0xa1:
      dbgXInd("LDA");
      break;
    case 0xa2:
      dbgImm("LDX");
      break;
    case 0xa3:
      // TODO Illegal instructions
      debugInfo += "Illegal";
      break;
    case 0xa4:
      dbgZP("LDY");
      break;
    case 0xa5:
      dbgZP("LDA");
      break;
    case 0xa6:
      dbgZP("LDX");
      break;
    case 0xa7:
      // TODO Illegal instructions
      debugInfo += "Illegal";
      break;
    case 0xa8:
      dbgImp("TAY");
      break;
    case 0xa9:
      dbgImm("LDA");
      break;
    case 0xaa:
      dbgImp("TAX");
      break;
    case 0xab:
      // TODO Illegal instructions
      debugInfo += "Illegal";
      break;
    case 0xac:
      dbgMem("LDY");
      break;
    case 0xad:
      dbgMem("LDA");
      break;
    case 0xae:
      dbgMem("LDX");
      break;
    case 0xaf:
      // TODO Illegal instructions
      debugInfo += "Illegal";
      break;
    case 0xb0:
      dbgBr("BCS");
      break;
    case 0xb1:
      dbgYInd("LDA");
      break;
    case 0xb2:
      // TODO Illegal instructions
      debugInfo += "Illegal";
      break;
    case 0xb3:
      // TODO Illegal instructions
      debugInfo += "Illegal";
      break;
    case 0xb4:
      dbgZPX("LDY");
      break;
    case 0xb5:
      dbgZPX("LDA");
      break;
    case 0xb6:
      dbgZPY("LDX");
      break;
    case 0xb7:
      // TODO Illegal instructions
      debugInfo += "Illegal";
      break;
    case 0xb8:
      dbgImp("CLV");
      break;
    case 0xb9:
      dbgAbsY("LDA");
      break;
    case 0xba:
      dbgImp("TSX");
      break;
    case 0xbb:
      // TODO Illegal instructions
      debugInfo += "Illegal";
      break;
    case 0xbc:
      dbgAbsX("LDY");
      break;
    case 0xbd:
      dbgAbsX("LDA");
      break;
    case 0xbe:
      dbgAbsY("LDX");
      break;
    case 0xbf:
      // TODO Illegal instructions
      debugInfo += "Illegal";
      break;
    case 0xc0:
      dbgImm("CPY");
      break;
    case 0xc1:
      dbgXInd("CMP");
      break;
    case 0xc2:
      // TODO Illegal instructions
      debugInfo += "Illegal";
      break;
    case 0xc3:
      // TODO Illegal instructions
      debugInfo += "Illegal";
      break;
    case 0xc4:
      dbgZP("CPY");
      break;
    case 0xc5:
      dbgZP("CMP");
      break;
    case 0xc6:
      dbgZP("DEC");
      break;
    case 0xc7:
      // TODO Illegal instructions
      debugInfo += "Illegal";
      break;
    case 0xc8:
      dbgImp("INY");
      break;
    case 0xc9:
      dbgImm("CMP");
      break;
    case 0xca:
      dbgImp("DEX");
      break;
    case 0xcb:
      // TODO Illegal instructions
      debugInfo += "Illegal";
      break;
    case 0xcc:
      dbgMem("CPY");
      break;
    case 0xcd:
      dbgMem("CMP");
      break;
    case 0xce:
      dbgMem("DEC");
      break;
    case 0xcf:
      // TODO Illegal instructions
      debugInfo += "Illegal";
      break;
    case 0xd0:
      dbgBr("BNE");
      break;
    case 0xd1:
      dbgYInd("CMP");
      break;
    case 0xd2:
      // TODO Illegal instructions
      debugInfo += "Illegal";
      break;
    case 0xd3:
      // TODO Illegal instructions
      debugInfo += "Illegal";
      break;
    case 0xd4:
      // TODO Illegal instructions
      debugInfo += "Illegal";
      break;
    case 0xd5:
      dbgZPX("CMP");
      break;
    case 0xd6:
      dbgZPX("DEC");
      break;
    case 0xd7:
      // TODO Illegal instructions
      debugInfo += "Illegal";
      break;
    case 0xd8:
      dbgImp("CLD");
      break;
    case 0xd9:
      dbgAbsY("CMP");
      break;
    case 0xda:
      // TODO Illegal instructions
      debugInfo += "Illegal";
      break;
    case 0xdb:
      // TODO Illegal instructions
      debugInfo += "Illegal";
      break;
    case 0xdc:
      // TODO Illegal instructions
      debugInfo += "Illegal";
      break;
    case 0xdd:
      dbgAbsX("CMP");
      break;
    case 0xde:
      dbgAbsX("DEC");
      break;
    case 0xdf:
      // TODO Illegal instructions
      debugInfo += "Illegal";
      break;
    case 0xe0:
      dbgImm("CPX");
      break;
    case 0xe1:
      dbgXInd("SBC");
      break;
    case 0xe2:
      // TODO Illegal instructions
      debugInfo += "Illegal";
      break;
    case 0xe3:
      // TODO Illegal instructions
      debugInfo += "Illegal";
      break;
    case 0xe4:
      dbgZP("CPX");
      break;
    case 0xe5:
      dbgZP("SBC");
      break;
    case 0xe6:
      dbgZP("INC");
      break;
    case 0xe7:
      // TODO Illegal instructions
      debugInfo += "Illegal";
      break;
    case 0xe8:
      dbgImp("INX");
      break;
    case 0xe9:
      dbgImm("SBC");
      break;
    case 0xea:
      dbgImp("NOP");
      break;
    case 0xeb:
      // TODO Illegal instructions
      debugInfo += "Illegal";
      break;
    case 0xec:
      dbgMem("CPX");
      break;
    case 0xed:
      dbgMem("SBC");
      break;
    case 0xee:
      dbgMem("INC");
      break;
    case 0xef:
      // TODO Illegal instructions
      debugInfo += "Illegal";
      break;
    case 0xf0:
      dbgBr("BEQ");
      break;
    case 0xf1:
      dbgYInd("SBC");
      break;
    case 0xf2:
      // TODO Illegal instructions
      debugInfo += "Illegal";
      break;
    case 0xf3:
      // TODO Illegal instructions
      debugInfo += "Illegal";
      break;
    case 0xf4:
      // TODO Illegal instructions
      debugInfo += "Illegal";
      break;
    case 0xf5:
      dbgZPX("SBC");
      break;
    case 0xf6:
      dbgZPX("INC");
      break;
    case 0xf7:
      // TODO Illegal instructions
      debugInfo += "Illegal";
      break;
    case 0xf8:
      dbgImp("SED");
      break;
    case 0xf9:
      dbgAbsY("SBC");
      break;
    case 0xfa:
      // TODO Illegal instructions
      debugInfo += "Illegal";
      break;
    case 0xfb:
      // TODO Illegal instructions
      debugInfo += "Illegal";
      break;
    case 0xfc:
      // TODO Illegal instructions
      debugInfo += "Illegal";
      break;
    case 0xfd:
      dbgAbsX("SBC");
      break;
    case 0xfe:
      dbgAbsX("INC");
      break;
    case 0xff:
      // TODO Illegal instructions
      debugInfo += "Illegal";
      break;
  }

  size_t length = 48;
  while (debugInfo.length() < length) {
    debugInfo += ' ';
  }

  debugInfo += "A:" + to_hex(ra) + " ";
  debugInfo += "X:" + to_hex(rx) + " ";
  debugInfo += "Y:" + to_hex(ry) + " ";
  debugInfo += "P:" + to_hex(getStatus()) + " ";
  debugInfo += "SP:" + to_hex(sp) + " ";
  // debugInfo += "PPU:  0,  0 ";  // TODO PPU debug info
  debugInfo += "CYC:" + std::to_string(cycle);

  if (!log.is_open()) {
    log.open("NES.log");
  }
  log << debugInfo << std::endl;
}
