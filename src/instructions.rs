
enum Addressing {
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
  IndirectIndexed,
}

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
  TYA,
}

pub struct Instruction {
  operation: Operation,
  mode: Addressing,
}

impl Instruction {
  pub fn decode_inst(byte: u8) -> Self {
    match byte {
      // ADC
      0x69 => Self { operation: Operation::ADC, mode: Addressing::Immediate},
      0x65 => Self { operation: Operation::ADC, mode: Addressing::ZeroPage},
      0x75 => Self { operation: Operation::ADC, mode: Addressing::ZeroPageX},
      0x6D => Self { operation: Operation::ADC, mode: Addressing::Absolute},
      0x7D => Self { operation: Operation::ADC, mode: Addressing::AbsoluteX},
      0x79 => Self { operation: Operation::ADC, mode: Addressing::AbsoluteY},
      0x61 => Self { operation: Operation::ADC, mode: Addressing::IndexedIndirect},
      0x71 => Self { operation: Operation::ADC, mode: Addressing::IndirectIndexed},

      // AND
      0x29 => Self { operation: Operation::AND, mode: Addressing::Immediate},
      0x25 => Self { operation: Operation::AND, mode: Addressing::ZeroPage},
      0x35 => Self { operation: Operation::AND, mode: Addressing::ZeroPageX},
      0x2D => Self { operation: Operation::AND, mode: Addressing::Absolute},
      0x3D => Self { operation: Operation::AND, mode: Addressing::AbsoluteX},
      0x39 => Self { operation: Operation::AND, mode: Addressing::AbsoluteY},
      0x21 => Self { operation: Operation::AND, mode: Addressing::IndexedIndirect},
      0x31 => Self { operation: Operation::AND, mode: Addressing::IndirectIndexed},

      // ASL
      0x0A => Self { operation: Operation::ASL, mode: Addressing::Accumulator},
      0x06 => Self { operation: Operation::ASL, mode: Addressing::ZeroPage},
      0x16 => Self { operation: Operation::ASL, mode: Addressing::ZeroPageX},
      0x0E => Self { operation: Operation::ASL, mode: Addressing::Absolute},
      0x1E => Self { operation: Operation::ASL, mode: Addressing::AbsoluteX},

      // BCC
      0x90 => Self { operation: Operation::BCC, mode: Addressing::Relative},

      // BCS
      0xB0 => Self { operation: Operation::BCS, mode: Addressing::Relative},

      // BEQ
      0xF0 => Self { operation: Operation::BEQ, mode: Addressing::Relative},

      // BIT
      0x24 => Self { operation: Operation::BIT, mode: Addressing::ZeroPage},
      0x2C => Self { operation: Operation::BIT, mode: Addressing::Absolute},

      // BMI
      0x30 => Self { operation: Operation::BMI, mode: Addressing::Relative},

      // BNE
      0xD0 => Self { operation: Operation::BNE, mode: Addressing::Relative},

      // BPL
      0x10 => Self { operation: Operation::BPL, mode: Addressing::Relative},

      // BRK
      0x00 => Self { operation: Operation::BRK, mode: Addressing::Implicit},

      // BVC
      0x50 => Self { operation: Operation::BVC, mode: Addressing::Relative},

      // BVS
      0x70 => Self { operation: Operation::BVS, mode: Addressing::Relative},

      // CLC
      0x18 => Self { operation: Operation::CLC, mode: Addressing::Implicit},

      // CLD
      0xD8 => Self { operation: Operation::CLD, mode: Addressing::Implicit},

      // CLI
      0x58 => Self { operation: Operation::CLI, mode: Addressing::Implicit},

      // CLV
      0xB8 => Self { operation: Operation::CLV, mode: Addressing::Implicit},

      // CMP
      0xC9 => Self { operation: Operation::CMP, mode: Addressing::Immediate},
      0xC5 => Self { operation: Operation::CMP, mode: Addressing::ZeroPage},
      0xD5 => Self { operation: Operation::CMP, mode: Addressing::ZeroPageX},
      0xCD => Self { operation: Operation::CMP, mode: Addressing::Absolute},
      0xDD => Self { operation: Operation::CMP, mode: Addressing::AbsoluteX},
      0xD9 => Self { operation: Operation::CMP, mode: Addressing::AbsoluteY},
      0xC1 => Self { operation: Operation::CMP, mode: Addressing::IndexedIndirect},
      0xD1 => Self { operation: Operation::CMP, mode: Addressing::IndirectIndexed},

      // CPX
      0xE0 => Self { operation: Operation::CPX, mode: Addressing::Immediate},
      0xE4 => Self { operation: Operation::CPX, mode: Addressing::ZeroPage},
      0xEC => Self { operation: Operation::CPX, mode: Addressing::Absolute},

      // CPY
      0xC0 => Self { operation: Operation::CPY, mode: Addressing::Immediate},
      0xC4 => Self { operation: Operation::CPY, mode: Addressing::ZeroPage},
      0xCC => Self { operation: Operation::CPY, mode: Addressing::Absolute},

      // DEC
      0xC6 => Self { operation: Operation::DEC, mode: Addressing::ZeroPage},
      0xD6 => Self { operation: Operation::DEC, mode: Addressing::ZeroPageX},
      0xCE => Self { operation: Operation::DEC, mode: Addressing::Absolute},
      0xDE => Self { operation: Operation::DEC, mode: Addressing::AbsoluteX},

      // DEX
      0xCA => Self { operation: Operation::DEX, mode: Addressing::Implicit},

      // DEY
      0x88 => Self { operation: Operation::DEY, mode: Addressing::Implicit},

      // EOR
      0x49 => Self { operation: Operation::EOR, mode: Addressing::Immediate},
      0x45 => Self { operation: Operation::EOR, mode: Addressing::ZeroPage},
      0x55 => Self { operation: Operation::EOR, mode: Addressing::ZeroPageX},
      0x4D => Self { operation: Operation::EOR, mode: Addressing::Absolute},
      0x5D => Self { operation: Operation::EOR, mode: Addressing::AbsoluteX},
      0x59 => Self { operation: Operation::EOR, mode: Addressing::AbsoluteY},
      0x41 => Self { operation: Operation::EOR, mode: Addressing::IndexedIndirect},
      0x51 => Self { operation: Operation::EOR, mode: Addressing::IndirectIndexed},

      // INC
      0xE6 => Self { operation: Operation::INC, mode: Addressing::ZeroPage},
      0xF6 => Self { operation: Operation::INC, mode: Addressing::ZeroPageX},
      0xEE => Self { operation: Operation::INC, mode: Addressing::Absolute},
      0xFE => Self { operation: Operation::INC, mode: Addressing::AbsoluteX},

      // INX
      0xE8 => Self { operation: Operation::INX, mode: Addressing::Implicit},

      // INY
      0xC8 => Self { operation: Operation::INY, mode: Addressing::Implicit},

      // JMP
      0x4C => Self { operation: Operation::JMP, mode: Addressing::Absolute},
      0x6C => Self { operation: Operation::JMP, mode: Addressing::Indirect},

      // JSR
      0x20 => Self { operation: Operation::JSR, mode: Addressing::Absolute},

      // LDA
      0xA9 => Self { operation: Operation::LDA, mode: Addressing::Immediate},
      0xA5 => Self { operation: Operation::LDA, mode: Addressing::ZeroPage},
      0xB5 => Self { operation: Operation::LDA, mode: Addressing::ZeroPageX},
      0xAD => Self { operation: Operation::LDA, mode: Addressing::Absolute},
      0xBD => Self { operation: Operation::LDA, mode: Addressing::AbsoluteX},
      0xB9 => Self { operation: Operation::LDA, mode: Addressing::AbsoluteY},
      0xA1 => Self { operation: Operation::LDA, mode: Addressing::IndexedIndirect},
      0xB1 => Self { operation: Operation::LDA, mode: Addressing::IndirectIndexed},

      // LDX
      0xA2 => Self { operation: Operation::LDX, mode: Addressing::Immediate},
      0xA6 => Self { operation: Operation::LDX, mode: Addressing::ZeroPage},
      0xB6 => Self { operation: Operation::LDX, mode: Addressing::ZeroPageX},
      0xAE => Self { operation: Operation::LDX, mode: Addressing::Absolute},
      0xBE => Self { operation: Operation::LDX, mode: Addressing::AbsoluteX},

      // LDY
      0xA0 => Self { operation: Operation::LDY, mode: Addressing::Immediate},
      0xA4 => Self { operation: Operation::LDY, mode: Addressing::ZeroPage},
      0xB4 => Self { operation: Operation::LDY, mode: Addressing::ZeroPageX},
      0xAC => Self { operation: Operation::LDY, mode: Addressing::Absolute},
      0xBC => Self { operation: Operation::LDY, mode: Addressing::AbsoluteX},

      // LSR
      0x4A => Self { operation: Operation::LSR, mode: Addressing::Accumulator},
      0x46 => Self { operation: Operation::LSR, mode: Addressing::ZeroPage},
      0x56 => Self { operation: Operation::LSR, mode: Addressing::ZeroPageX},
      0x4E => Self { operation: Operation::LSR, mode: Addressing::Absolute},
      0x5E => Self { operation: Operation::LSR, mode: Addressing::AbsoluteX},

      // NOP
      0xEA => Self { operation: Operation::NOP, mode: Addressing::Implicit},

      // ORA
      0x09 => Self { operation: Operation::ORA, mode: Addressing::Immediate},
      0x05 => Self { operation: Operation::ORA, mode: Addressing::ZeroPage},
      0x15 => Self { operation: Operation::ORA, mode: Addressing::ZeroPageX},
      0x0D => Self { operation: Operation::ORA, mode: Addressing::Absolute},
      0x1D => Self { operation: Operation::ORA, mode: Addressing::AbsoluteX},
      0x19 => Self { operation: Operation::ORA, mode: Addressing::AbsoluteY},
      0x01 => Self { operation: Operation::ORA, mode: Addressing::IndexedIndirect},
      0x11 => Self { operation: Operation::ORA, mode: Addressing::IndirectIndexed},

      // PHA
      0x48 => Self { operation: Operation::PHA, mode: Addressing::Implicit},

      // PHP
      0x08 => Self { operation: Operation::PHP, mode: Addressing::Implicit},

      // PLA
      0x68 => Self { operation: Operation::PLA, mode: Addressing::Implicit},

      // PLP
      0x28 => Self { operation: Operation::PLP, mode: Addressing::Implicit},

      // ROL
      0x2A => Self { operation: Operation::ROL, mode: Addressing::Accumulator},
      0x26 => Self { operation: Operation::ROL, mode: Addressing::ZeroPage},
      0x36 => Self { operation: Operation::ROL, mode: Addressing::ZeroPageX},
      0x2E => Self { operation: Operation::ROL, mode: Addressing::Absolute},
      0x3E => Self { operation: Operation::ROL, mode: Addressing::AbsoluteX},

      // ROR
      0x6A => Self { operation: Operation::ROR, mode: Addressing::Accumulator},
      0x66 => Self { operation: Operation::ROR, mode: Addressing::ZeroPage},
      0x76 => Self { operation: Operation::ROR, mode: Addressing::ZeroPageX},
      0x6E => Self { operation: Operation::ROR, mode: Addressing::Absolute},
      0x7E => Self { operation: Operation::ROR, mode: Addressing::AbsoluteX},

      // RTI
      0x40 => Self { operation: Operation::RTI, mode: Addressing::Implicit},

      // RTS
      0x60 => Self { operation: Operation::RTS, mode: Addressing::Implicit},

      // SBC
      0xE9 => Self { operation: Operation::SBC, mode: Addressing::Immediate},
      0xE5 => Self { operation: Operation::SBC, mode: Addressing::ZeroPage},
      0xF5 => Self { operation: Operation::SBC, mode: Addressing::ZeroPageX},
      0xED => Self { operation: Operation::SBC, mode: Addressing::Absolute},
      0xFD => Self { operation: Operation::SBC, mode: Addressing::AbsoluteX},
      0xF9 => Self { operation: Operation::SBC, mode: Addressing::AbsoluteY},
      0xE1 => Self { operation: Operation::SBC, mode: Addressing::IndexedIndirect},
      0xF1 => Self { operation: Operation::SBC, mode: Addressing::IndirectIndexed},

      // SEC
      0x38 => Self { operation: Operation::SEC, mode: Addressing::Implicit},

      // SED
      0xF8 => Self { operation: Operation::SED, mode: Addressing::Implicit},

      // SEI
      0x78 => Self { operation: Operation::SEI, mode: Addressing::Implicit},

      // STA
      0x85 => Self { operation: Operation::STA, mode: Addressing::ZeroPage},
      0x95 => Self { operation: Operation::STA, mode: Addressing::ZeroPageX},
      0x8D => Self { operation: Operation::STA, mode: Addressing::Absolute},
      0x9D => Self { operation: Operation::STA, mode: Addressing::AbsoluteX},
      0x99 => Self { operation: Operation::STA, mode: Addressing::AbsoluteY},
      0x81 => Self { operation: Operation::STA, mode: Addressing::IndexedIndirect},
      0x91 => Self { operation: Operation::STA, mode: Addressing::IndirectIndexed},

      // STX
      0x86 => Self { operation: Operation::STX, mode: Addressing::ZeroPage},
      0x96 => Self { operation: Operation::STX, mode: Addressing::ZeroPageY},
      0x8E => Self { operation: Operation::STX, mode: Addressing::Absolute},

      // STY
      0x84 => Self { operation: Operation::STY, mode: Addressing::ZeroPage},
      0x94 => Self { operation: Operation::STY, mode: Addressing::ZeroPageX},
      0x8C => Self { operation: Operation::STY, mode: Addressing::Absolute},

      // TAX
      0xAA => Self { operation: Operation::TAX, mode: Addressing::Implicit},

      // TAY
      0xA8 => Self { operation: Operation::TAY, mode: Addressing::Implicit},

      // TSX
      0xBA => Self { operation: Operation::TSX, mode: Addressing::Implicit},

      // TXA
      0x8A => Self { operation: Operation::TXA, mode: Addressing::Implicit},

      // TXS
      0x9A => Self { operation: Operation::TXS, mode: Addressing::Implicit},

      // TYA
      0x98 => Self { operation: Operation::TYA, mode: Addressing::Implicit},

      _ => panic!("Instruction Decode: Bytes 0x{} not a valid opcode", byte),
    }
  }
}
