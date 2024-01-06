enum Addressing {
  Implicit,
  Accumulator,
  Immediate,
  Zero_Page,
  Zero_Page_X,
  Zero_Page_Y,
  Relative,
  Absolute,
  Absolute_X,
  Absolute_Y,
  Indirect,
  Indexed_Indirect,
  Indirect_Indexed,
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

struct Instruction {
  operation: Operation,
  mode: Addressing,

  cycle: u8,
}

impl Default for Instruction {
  fn default() -> Self {
    Self { operation: Operation::ADC, mode: Addressing::Immediate, cycle: 0 }
  }
}

impl Instruction {
  fn decode_inst(byte: u8) -> Self {
    match byte {
      // ADC
      0x69 => Self { operation: Operation::ADC, mode: Addressing::Immediate, ..Self::default() },
      0x65 => Self { operation: Operation::ADC, mode: Addressing::Zero_Page, ..Self::default() },
      0x75 => Self { operation: Operation::ADC, mode: Addressing::Zero_Page_X, ..Self::default() },
      0x6D => Self { operation: Operation::ADC, mode: Addressing::Absolute, ..Self::default() },
      0x7D => Self { operation: Operation::ADC, mode: Addressing::Absolute_X, ..Self::default() },
      0x79 => Self { operation: Operation::ADC, mode: Addressing::Absolute_Y, ..Self::default() },
      0x61 => Self { operation: Operation::ADC, mode: Addressing::Indexed_Indirect, ..Self::default() },
      0x71 => Self { operation: Operation::ADC, mode: Addressing::Indirect_Indexed, ..Self::default() },

      // AND
      0x29 => Self { operation: Operation::AND, mode: Addressing::Immediate, ..Self::default() },
      0x25 => Self { operation: Operation::AND, mode: Addressing::Zero_Page, ..Self::default() },
      0x35 => Self { operation: Operation::AND, mode: Addressing::Zero_Page_X, ..Self::default() },
      0x2D => Self { operation: Operation::AND, mode: Addressing::Absolute, ..Self::default() },
      0x3D => Self { operation: Operation::AND, mode: Addressing::Absolute_X, ..Self::default() },
      0x39 => Self { operation: Operation::AND, mode: Addressing::Absolute_Y, ..Self::default() },
      0x21 => Self { operation: Operation::AND, mode: Addressing::Indexed_Indirect, ..Self::default() },
      0x31 => Self { operation: Operation::AND, mode: Addressing::Indirect_Indexed, ..Self::default() },

      // ASL
      0x0A => Self { operation: Operation::ASL, mode: Addressing::Accumulator, ..Self::default() },
      0x06 => Self { operation: Operation::ASL, mode: Addressing::Zero_Page, ..Self::default() },
      0x16 => Self { operation: Operation::ASL, mode: Addressing::Zero_Page_X, ..Self::default() },
      0x0E => Self { operation: Operation::ASL, mode: Addressing::Absolute, ..Self::default() },
      0x1E => Self { operation: Operation::ASL, mode: Addressing::Absolute_X, ..Self::default() },

      // BCC
      0x90 => Self { operation: Operation::BCC, mode: Addressing::Relative, ..Self::default() },

      // BCS
      0xB0 => Self { operation: Operation::BCS, mode: Addressing::Relative, ..Self::default() },

      // BEQ
      0xF0 => Self { operation: Operation::BEQ, mode: Addressing::Relative, ..Self::default() },

      // BIT
      0x24 => Self { operation: Operation::BIT, mode: Addressing::Zero_Page, ..Self::default() },
      0x2C => Self { operation: Operation::BIT, mode: Addressing::Absolute, ..Self::default() },

      // BMI
      0x30 => Self { operation: Operation::BMI, mode: Addressing::Relative, ..Self::default() },

      // BNE
      0xD0 => Self { operation: Operation::BNE, mode: Addressing::Relative, ..Self::default() },

      // BPL
      0x10 => Self { operation: Operation::BPL, mode: Addressing::Relative, ..Self::default() },

      // BRK
      0x00 => Self { operation: Operation::BRK, mode: Addressing::Implicit, ..Self::default() },

      // BVC
      0x50 => Self { operation: Operation::BVC, mode: Addressing::Relative, ..Self::default() },

      // BVS
      0x70 => Self { operation: Operation::BVS, mode: Addressing::Relative, ..Self::default() },

      // CLD
      0xD8 => Self { operation: Operation::CLD, mode: Addressing::Implicit, ..Self::default() },

      // CLI
      0x58 => Self { operation: Operation::CLI, mode: Addressing::Implicit, ..Self::default() },

      // CLV
      0xB8 => Self { operation: Operation::CLV, mode: Addressing::Implicit, ..Self::default() },

      // CMP
      0xC9 => Self { operation: Operation::CMP, mode: Addressing::Immediate, ..Self::default() },
      0xC5 => Self { operation: Operation::CMP, mode: Addressing::Zero_Page, ..Self::default() },
      0xD5 => Self { operation: Operation::CMP, mode: Addressing::Zero_Page_X, ..Self::default() },
      0xCD => Self { operation: Operation::CMP, mode: Addressing::Absolute, ..Self::default() },
      0xDD => Self { operation: Operation::CMP, mode: Addressing::Absolute_X, ..Self::default() },
      0xD9 => Self { operation: Operation::CMP, mode: Addressing::Absolute_Y, ..Self::default() },
      0xC1 => Self { operation: Operation::CMP, mode: Addressing::Indexed_Indirect, ..Self::default() },
      0xD1 => Self { operation: Operation::CMP, mode: Addressing::Indirect_Indexed, ..Self::default() },

      // CPX
      0xE0 => Self { operation: Operation::CPX, mode: Addressing::Immediate, ..Self::default() },
      0xE4 => Self { operation: Operation::CPX, mode: Addressing::Zero_Page, ..Self::default() },
      0xEC => Self { operation: Operation::CPX, mode: Addressing::Absolute, ..Self::default() },

      // CPY
      0xC0 => Self { operation: Operation::CPY, mode: Addressing::Immediate, ..Self::default() },
      0xC4 => Self { operation: Operation::CPY, mode: Addressing::Zero_Page, ..Self::default() },
      0xCC => Self { operation: Operation::CPY, mode: Addressing::Absolute, ..Self::default() },

      // DEC
      0xC6 => Self { operation: Operation::DEC, mode: Addressing::Zero_Page, ..Self::default() },
      0xD6 => Self { operation: Operation::DEC, mode: Addressing::Zero_Page_X, ..Self::default() },
      0xCE => Self { operation: Operation::DEC, mode: Addressing::Absolute, ..Self::default() },
      0xDE => Self { operation: Operation::DEC, mode: Addressing::Absolute_X, ..Self::default() },

      // DEX
      0xCA => Self { operation: Operation::DEX, mode: Addressing::Implicit, ..Self::default() },

      // DEY
      0x88 => Self { operation: Operation::DEY, mode: Addressing::Implicit, ..Self::default() },

      // EOR
      0x49 => Self { operation: Operation::EOR, mode: Addressing::Immediate, ..Self::default() },
      0x45 => Self { operation: Operation::EOR, mode: Addressing::Zero_Page, ..Self::default() },
      0x55 => Self { operation: Operation::EOR, mode: Addressing::Zero_Page_X, ..Self::default() },
      0x4D => Self { operation: Operation::EOR, mode: Addressing::Absolute, ..Self::default() },
      0x5D => Self { operation: Operation::EOR, mode: Addressing::Absolute_X, ..Self::default() },
      0x59 => Self { operation: Operation::EOR, mode: Addressing::Absolute_Y, ..Self::default() },
      0x41 => Self { operation: Operation::EOR, mode: Addressing::Indexed_Indirect, ..Self::default() },
      0x51 => Self { operation: Operation::EOR, mode: Addressing::Indirect_Indexed, ..Self::default() },

      // INC
      0xE6 => Self { operation: Operation::INC, mode: Addressing::Zero_Page, ..Self::default() },
      0xF6 => Self { operation: Operation::INC, mode: Addressing::Zero_Page_X, ..Self::default() },
      0xEE => Self { operation: Operation::INC, mode: Addressing::Absolute, ..Self::default() },
      0xFE => Self { operation: Operation::INC, mode: Addressing::Absolute_X, ..Self::default() },

      // INX
      0xE8 => Self { operation: Operation::INX, mode: Addressing::Implicit, ..Self::default() },

      // INY
      0xC8 => Self { operation: Operation::INY, mode: Addressing::Implicit, ..Self::default() },

      // JMP
      0x4C => Self { operation: Operation::JMP, mode: Addressing::Absolute, ..Self::default() },
      0x6C => Self { operation: Operation::JMP, mode: Addressing::Indirect, ..Self::default() },

      // JSR
      0x20 => Self { operation: Operation::JSR, mode: Addressing::Absolute, ..Self::default() },

      // LDA
      0xA9 => Self { operation: Operation::LDA, mode: Addressing::Immediate, ..Self::default() },
      0xA5 => Self { operation: Operation::LDA, mode: Addressing::Zero_Page, ..Self::default() },
      0xB5 => Self { operation: Operation::LDA, mode: Addressing::Zero_Page_X, ..Self::default() },
      0xAD => Self { operation: Operation::LDA, mode: Addressing::Absolute, ..Self::default() },
      0xBD => Self { operation: Operation::LDA, mode: Addressing::Absolute_X, ..Self::default() },
      0xB9 => Self { operation: Operation::LDA, mode: Addressing::Absolute_Y, ..Self::default() },
      0xA1 => Self { operation: Operation::LDA, mode: Addressing::Indexed_Indirect, ..Self::default() },
      0xB1 => Self { operation: Operation::LDA, mode: Addressing::Indirect_Indexed, ..Self::default() },

      // LDX
      0xA2 => Self { operation: Operation::LDX, mode: Addressing::Immediate, ..Self::default() },
      0xA6 => Self { operation: Operation::LDX, mode: Addressing::Zero_Page, ..Self::default() },
      0xB6 => Self { operation: Operation::LDX, mode: Addressing::Zero_Page_X, ..Self::default() },
      0xAE => Self { operation: Operation::LDX, mode: Addressing::Absolute, ..Self::default() },
      0xBE => Self { operation: Operation::LDX, mode: Addressing::Absolute_X, ..Self::default() },

      // LDY
      0xA0 => Self { operation: Operation::LDY, mode: Addressing::Immediate, ..Self::default() },
      0xA4 => Self { operation: Operation::LDY, mode: Addressing::Zero_Page, ..Self::default() },
      0xB4 => Self { operation: Operation::LDY, mode: Addressing::Zero_Page_X, ..Self::default() },
      0xAC => Self { operation: Operation::LDY, mode: Addressing::Absolute, ..Self::default() },
      0xBC => Self { operation: Operation::LDY, mode: Addressing::Absolute_X, ..Self::default() },

      // LSR
      0x4A => Self { operation: Operation::LSR, mode: Addressing::Accumulator, ..Self::default() },
      0x46 => Self { operation: Operation::LSR, mode: Addressing::Zero_Page, ..Self::default() },
      0x56 => Self { operation: Operation::LSR, mode: Addressing::Zero_Page_X, ..Self::default() },
      0x4E => Self { operation: Operation::LSR, mode: Addressing::Absolute, ..Self::default() },
      0x5E => Self { operation: Operation::LSR, mode: Addressing::Absolute_X, ..Self::default() },

      // NOP
      0xEA => Self { operation: Operation::NOP, mode: Addressing::Implicit, ..Self::default() },

      // ORA
      0x09 => Self { operation: Operation::ORA, mode: Addressing::Immediate, ..Self::default() },
      0x05 => Self { operation: Operation::ORA, mode: Addressing::Zero_Page, ..Self::default() },
      0x15 => Self { operation: Operation::ORA, mode: Addressing::Zero_Page_X, ..Self::default() },
      0x0D => Self { operation: Operation::ORA, mode: Addressing::Absolute, ..Self::default() },
      0x1D => Self { operation: Operation::ORA, mode: Addressing::Absolute_X, ..Self::default() },
      0x19 => Self { operation: Operation::ORA, mode: Addressing::Absolute_Y, ..Self::default() },
      0x01 => Self { operation: Operation::ORA, mode: Addressing::Indexed_Indirect, ..Self::default() },
      0x11 => Self { operation: Operation::ORA, mode: Addressing::Indirect_Indexed, ..Self::default() },

      // PHA
      0x48 => Self { operation: Operation::PHA, mode: Addressing::Implicit, ..Self::default() },

      // PHP
      0x08 => Self { operation: Operation::PHP, mode: Addressing::Implicit, ..Self::default() },

      // PLA
      0x68 => Self { operation: Operation::PLA, mode: Addressing::Implicit, ..Self::default() },

      // PLP
      0x28 => Self { operation: Operation::PLP, mode: Addressing::Implicit, ..Self::default() },

      // ROL
      0x2A => Self { operation: Operation::ROL, mode: Addressing::Accumulator, ..Self::default() },
      0x26 => Self { operation: Operation::ROL, mode: Addressing::Zero_Page, ..Self::default() },
      0x36 => Self { operation: Operation::ROL, mode: Addressing::Zero_Page_X, ..Self::default() },
      0x2E => Self { operation: Operation::ROL, mode: Addressing::Absolute, ..Self::default() },
      0x3E => Self { operation: Operation::ROL, mode: Addressing::Absolute_X, ..Self::default() },

      // ROR
      0x6A => Self { operation: Operation::ROR, mode: Addressing::Accumulator, ..Self::default() },
      0x66 => Self { operation: Operation::ROR, mode: Addressing::Zero_Page, ..Self::default() },
      0x76 => Self { operation: Operation::ROR, mode: Addressing::Zero_Page_X, ..Self::default() },
      0x6E => Self { operation: Operation::ROR, mode: Addressing::Absolute, ..Self::default() },
      0x7E => Self { operation: Operation::ROR, mode: Addressing::Absolute_X, ..Self::default() },

      // RTI
      0x40 => Self { operation: Operation::RTI, mode: Addressing::Implicit, ..Self::default() },

      // RTS
      0x60 => Self { operation: Operation::RTS, mode: Addressing::Implicit, ..Self::default() },

      // SBC
      0xE9 => Self { operation: Operation::SBC, mode: Addressing::Immediate, ..Self::default() },
      0xE5 => Self { operation: Operation::SBC, mode: Addressing::Zero_Page, ..Self::default() },
      0xF5 => Self { operation: Operation::SBC, mode: Addressing::Zero_Page_X, ..Self::default() },
      0xED => Self { operation: Operation::SBC, mode: Addressing::Absolute, ..Self::default() },
      0xFD => Self { operation: Operation::SBC, mode: Addressing::Absolute_X, ..Self::default() },
      0xF9 => Self { operation: Operation::SBC, mode: Addressing::Absolute_Y, ..Self::default() },
      0xE1 => Self { operation: Operation::SBC, mode: Addressing::Indexed_Indirect, ..Self::default() },
      0xF1 => Self { operation: Operation::SBC, mode: Addressing::Indirect_Indexed, ..Self::default() },

      // SEC
      0x38 => Self { operation: Operation::SEC, mode: Addressing::Implicit, ..Self::default() },

      // SED
      0xF8 => Self { operation: Operation::SED, mode: Addressing::Implicit, ..Self::default() },

      // SEI
      0x78 => Self { operation: Operation::SEI, mode: Addressing::Implicit, ..Self::default() },

      // STA
      0x85 => Self { operation: Operation::STA, mode: Addressing::Zero_Page, ..Self::default() },
      0x95 => Self { operation: Operation::STA, mode: Addressing::Zero_Page_X, ..Self::default() },
      0x8D => Self { operation: Operation::STA, mode: Addressing::Absolute, ..Self::default() },
      0x9D => Self { operation: Operation::STA, mode: Addressing::Absolute_X, ..Self::default() },
      0x99 => Self { operation: Operation::STA, mode: Addressing::Absolute_Y, ..Self::default() },
      0x81 => Self { operation: Operation::STA, mode: Addressing::Indexed_Indirect, ..Self::default() },
      0x91 => Self { operation: Operation::STA, mode: Addressing::Indirect_Indexed, ..Self::default() },

      // STX
      0x86 => Self { operation: Operation::STX, mode: Addressing::Zero_Page, ..Self::default() },
      0x96 => Self { operation: Operation::STX, mode: Addressing::Zero_Page_Y, ..Self::default() },
      0x8E => Self { operation: Operation::STX, mode: Addressing::Absolute, ..Self::default() },

      // STY
      0x84 => Self { operation: Operation::STY, mode: Addressing::Zero_Page, ..Self::default() },
      0x94 => Self { operation: Operation::STY, mode: Addressing::Zero_Page_X, ..Self::default() },
      0x8C => Self { operation: Operation::STY, mode: Addressing::Absolute, ..Self::default() },

      // TAX
      0xAA => Self { operation: Operation::TAX, mode: Addressing::Implicit, ..Self::default() },

      // TAY
      0xA8 => Self { operation: Operation::TAY, mode: Addressing::Implicit, ..Self::default() },

      // TSX
      0xBA => Self { operation: Operation::TSX, mode: Addressing::Implicit, ..Self::default() },

      // TXA
      0x8A => Self { operation: Operation::TXA, mode: Addressing::Implicit, ..Self::default() },

      // TXS
      0x9A => Self { operation: Operation::TXS, mode: Addressing::Implicit, ..Self::default() },

      // TYA
      0x98 => Self { operation: Operation::TYA, mode: Addressing::Implicit, ..Self::default() },

      _ => panic!("Instruction Decode: Bytes 0x{} not a valid opcode", byte),
    }
  }
}
