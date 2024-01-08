
pub enum Addressing {
  Implicit,
  Accumulator,
  Immediate, // Immediate and ZeroPage execute as soon as they're done decoding, same cycle
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

#[derive(Debug)]
pub enum Operation {
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
  pub operation: Operation,
  pub mode: Addressing,

  pub execute_cycles: u8,
}

/*
  Each Instruction:
    Fetches
    <Get data>
    <Execute Cycles Delay>
    Executes
    <Delay depending on Instruction outcome>

  At minimum 2 cycles
*/

impl Instruction {
  pub fn decode_inst(byte: u8) -> Self {
    match byte {
      // ADC
      0x69 => Self { operation: Operation::ADC, mode: Addressing::Immediate,       execute_cycles: 0 },
      0x65 => Self { operation: Operation::ADC, mode: Addressing::ZeroPage,        execute_cycles: 0 },
      0x75 => Self { operation: Operation::ADC, mode: Addressing::ZeroPageX,       execute_cycles: 1 },
      0x6D => Self { operation: Operation::ADC, mode: Addressing::Absolute,        execute_cycles: 1 },
      0x7D => Self { operation: Operation::ADC, mode: Addressing::AbsoluteX,       execute_cycles: 1 },
      0x79 => Self { operation: Operation::ADC, mode: Addressing::AbsoluteY,       execute_cycles: 1 },
      0x61 => Self { operation: Operation::ADC, mode: Addressing::IndexedIndirect, execute_cycles: 3 },
      0x71 => Self { operation: Operation::ADC, mode: Addressing::IndirectIndexed, execute_cycles: 2 },

      // AND
      0x29 => Self { operation: Operation::AND, mode: Addressing::Immediate,       execute_cycles: 0 },
      0x25 => Self { operation: Operation::AND, mode: Addressing::ZeroPage,        execute_cycles: 1 },
      0x35 => Self { operation: Operation::AND, mode: Addressing::ZeroPageX,       execute_cycles: 2 },
      0x2D => Self { operation: Operation::AND, mode: Addressing::Absolute,        execute_cycles: 2 },
      0x3D => Self { operation: Operation::AND, mode: Addressing::AbsoluteX,       execute_cycles: 2 },
      0x39 => Self { operation: Operation::AND, mode: Addressing::AbsoluteY,       execute_cycles: 2 },
      0x21 => Self { operation: Operation::AND, mode: Addressing::IndexedIndirect, execute_cycles: 4 },
      0x31 => Self { operation: Operation::AND, mode: Addressing::IndirectIndexed, execute_cycles: 3 },

      // ASL
      0x0A => Self { operation: Operation::ASL, mode: Addressing::Accumulator, execute_cycles: 0 },
      0x06 => Self { operation: Operation::ASL, mode: Addressing::ZeroPage,    execute_cycles: 3 },
      0x16 => Self { operation: Operation::ASL, mode: Addressing::ZeroPageX,   execute_cycles: 4 },
      0x0E => Self { operation: Operation::ASL, mode: Addressing::Absolute,    execute_cycles: 4 },
      0x1E => Self { operation: Operation::ASL, mode: Addressing::AbsoluteX,   execute_cycles: 5 },

      // BCC
      0x90 => Self { operation: Operation::BCC, mode: Addressing::Relative, execute_cycles: 0 },

      // BCS
      0xB0 => Self { operation: Operation::BCS, mode: Addressing::Relative, execute_cycles: 0 },

      // BEQ
      0xF0 => Self { operation: Operation::BEQ, mode: Addressing::Relative, execute_cycles: 0 },

      // BIT
      0x24 => Self { operation: Operation::BIT, mode: Addressing::ZeroPage, execute_cycles: 1 },
      0x2C => Self { operation: Operation::BIT, mode: Addressing::Absolute, execute_cycles: 2 },

      // BMI
      0x30 => Self { operation: Operation::BMI, mode: Addressing::Relative, execute_cycles: 0 },

      // BNE
      0xD0 => Self { operation: Operation::BNE, mode: Addressing::Relative, execute_cycles: 0 },

      // BPL
      0x10 => Self { operation: Operation::BPL, mode: Addressing::Relative, execute_cycles: 0 },

      // BRK
      0x00 => Self { operation: Operation::BRK, mode: Addressing::Implicit, execute_cycles: 5 },

      // BVC
      0x50 => Self { operation: Operation::BVC, mode: Addressing::Relative, execute_cycles: 0 },

      // BVS
      0x70 => Self { operation: Operation::BVS, mode: Addressing::Relative, execute_cycles: 0 },

      // CLC
      0x18 => Self { operation: Operation::CLC, mode: Addressing::Implicit, execute_cycles: 0 },

      // CLD
      0xD8 => Self { operation: Operation::CLD, mode: Addressing::Implicit, execute_cycles: 0 },

      // CLI
      0x58 => Self { operation: Operation::CLI, mode: Addressing::Implicit, execute_cycles: 0 },

      // CLV
      0xB8 => Self { operation: Operation::CLV, mode: Addressing::Implicit, execute_cycles: 0 },

      // CMP
      0xC9 => Self { operation: Operation::CMP, mode: Addressing::Immediate,       execute_cycles: 0 },
      0xC5 => Self { operation: Operation::CMP, mode: Addressing::ZeroPage,        execute_cycles: 1 },
      0xD5 => Self { operation: Operation::CMP, mode: Addressing::ZeroPageX,       execute_cycles: 2 },
      0xCD => Self { operation: Operation::CMP, mode: Addressing::Absolute,        execute_cycles: 2 },
      0xDD => Self { operation: Operation::CMP, mode: Addressing::AbsoluteX,       execute_cycles: 2 },
      0xD9 => Self { operation: Operation::CMP, mode: Addressing::AbsoluteY,       execute_cycles: 2 },
      0xC1 => Self { operation: Operation::CMP, mode: Addressing::IndexedIndirect, execute_cycles: 4 },
      0xD1 => Self { operation: Operation::CMP, mode: Addressing::IndirectIndexed, execute_cycles: 3 },

      // CPX
      0xE0 => Self { operation: Operation::CPX, mode: Addressing::Immediate, execute_cycles: 0 },
      0xE4 => Self { operation: Operation::CPX, mode: Addressing::ZeroPage,  execute_cycles: 1 },
      0xEC => Self { operation: Operation::CPX, mode: Addressing::Absolute,  execute_cycles: 2 },

      // CPY
      0xC0 => Self { operation: Operation::CPY, mode: Addressing::Immediate, execute_cycles: 0 },
      0xC4 => Self { operation: Operation::CPY, mode: Addressing::ZeroPage,  execute_cycles: 1 },
      0xCC => Self { operation: Operation::CPY, mode: Addressing::Absolute,  execute_cycles: 2 },

      // DEC
      0xC6 => Self { operation: Operation::DEC, mode: Addressing::ZeroPage,  execute_cycles: 3 },
      0xD6 => Self { operation: Operation::DEC, mode: Addressing::ZeroPageX, execute_cycles: 4 },
      0xCE => Self { operation: Operation::DEC, mode: Addressing::Absolute,  execute_cycles: 4 },
      0xDE => Self { operation: Operation::DEC, mode: Addressing::AbsoluteX, execute_cycles: 5 },

      // DEX
      0xCA => Self { operation: Operation::DEX, mode: Addressing::Implicit, execute_cycles: 0 },

      // DEY
      0x88 => Self { operation: Operation::DEY, mode: Addressing::Implicit, execute_cycles: 0 },

      // EOR
      0x49 => Self { operation: Operation::EOR, mode: Addressing::Immediate,       execute_cycles: 0 },
      0x45 => Self { operation: Operation::EOR, mode: Addressing::ZeroPage,        execute_cycles: 1 },
      0x55 => Self { operation: Operation::EOR, mode: Addressing::ZeroPageX,       execute_cycles: 2 },
      0x4D => Self { operation: Operation::EOR, mode: Addressing::Absolute,        execute_cycles: 2 },
      0x5D => Self { operation: Operation::EOR, mode: Addressing::AbsoluteX,       execute_cycles: 2 },
      0x59 => Self { operation: Operation::EOR, mode: Addressing::AbsoluteY,       execute_cycles: 2 },
      0x41 => Self { operation: Operation::EOR, mode: Addressing::IndexedIndirect, execute_cycles: 4 },
      0x51 => Self { operation: Operation::EOR, mode: Addressing::IndirectIndexed, execute_cycles: 3 },

      // INC
      0xE6 => Self { operation: Operation::INC, mode: Addressing::ZeroPage,  execute_cycles: 3 },
      0xF6 => Self { operation: Operation::INC, mode: Addressing::ZeroPageX, execute_cycles: 4 },
      0xEE => Self { operation: Operation::INC, mode: Addressing::Absolute,  execute_cycles: 4 },
      0xFE => Self { operation: Operation::INC, mode: Addressing::AbsoluteX, execute_cycles: 5 },

      // INX
      0xE8 => Self { operation: Operation::INX, mode: Addressing::Implicit, execute_cycles: 0 },

      // INY
      0xC8 => Self { operation: Operation::INY, mode: Addressing::Implicit, execute_cycles: 0 },

      // JMP
      0x4C => Self { operation: Operation::JMP, mode: Addressing::Absolute, execute_cycles: 1 },
      0x6C => Self { operation: Operation::JMP, mode: Addressing::Indirect, execute_cycles: 3 },

      // JSR
      0x20 => Self { operation: Operation::JSR, mode: Addressing::Absolute, execute_cycles: 4 },

      // LDA
      0xA9 => Self { operation: Operation::LDA, mode: Addressing::Immediate,       execute_cycles: 0 },
      0xA5 => Self { operation: Operation::LDA, mode: Addressing::ZeroPage,        execute_cycles:         },
      0xB5 => Self { operation: Operation::LDA, mode: Addressing::ZeroPageX,       execute_cycles:        },
      0xAD => Self { operation: Operation::LDA, mode: Addressing::Absolute,        execute_cycles:         },
      0xBD => Self { operation: Operation::LDA, mode: Addressing::AbsoluteX,       execute_cycles:        },
      0xB9 => Self { operation: Operation::LDA, mode: Addressing::AbsoluteY,       execute_cycles:        },
      0xA1 => Self { operation: Operation::LDA, mode: Addressing::IndexedIndirect, execute_cycles:        },
      0xB1 => Self { operation: Operation::LDA, mode: Addressing::IndirectIndexed, execute_cycles:        },

      // LDX
      0xA2 => Self { operation: Operation::LDX, mode: Addressing::Immediate, execute_cycles: 0 },
      0xA6 => Self { operation: Operation::LDX, mode: Addressing::ZeroPage,  execute_cycles:         },
      0xB6 => Self { operation: Operation::LDX, mode: Addressing::ZeroPageX, execute_cycles:        },
      0xAE => Self { operation: Operation::LDX, mode: Addressing::Absolute,  execute_cycles:         },
      0xBE => Self { operation: Operation::LDX, mode: Addressing::AbsoluteX, execute_cycles:        },

      // LDY
      0xA0 => Self { operation: Operation::LDY, mode: Addressing::Immediate, execute_cycles: 0 },
      0xA4 => Self { operation: Operation::LDY, mode: Addressing::ZeroPage,  execute_cycles:         },
      0xB4 => Self { operation: Operation::LDY, mode: Addressing::ZeroPageX, execute_cycles:        },
      0xAC => Self { operation: Operation::LDY, mode: Addressing::Absolute,  execute_cycles:         },
      0xBC => Self { operation: Operation::LDY, mode: Addressing::AbsoluteX, execute_cycles:        },

      // LSR
      0x4A => Self { operation: Operation::LSR, mode: Addressing::Accumulator, execute_cycles:        },
      0x46 => Self { operation: Operation::LSR, mode: Addressing::ZeroPage,    execute_cycles:         },
      0x56 => Self { operation: Operation::LSR, mode: Addressing::ZeroPageX,   execute_cycles:        },
      0x4E => Self { operation: Operation::LSR, mode: Addressing::Absolute,    execute_cycles:         },
      0x5E => Self { operation: Operation::LSR, mode: Addressing::AbsoluteX,   execute_cycles:        },

      // NOP
      0xEA => Self { operation: Operation::NOP, mode: Addressing::Implicit, execute_cycles:         },

      // ORA
      0x09 => Self { operation: Operation::ORA, mode: Addressing::Immediate,       execute_cycles: 0 },
      0x05 => Self { operation: Operation::ORA, mode: Addressing::ZeroPage,        execute_cycles:         },
      0x15 => Self { operation: Operation::ORA, mode: Addressing::ZeroPageX,       execute_cycles:        },
      0x0D => Self { operation: Operation::ORA, mode: Addressing::Absolute,        execute_cycles:         },
      0x1D => Self { operation: Operation::ORA, mode: Addressing::AbsoluteX,       execute_cycles:        },
      0x19 => Self { operation: Operation::ORA, mode: Addressing::AbsoluteY,       execute_cycles:        },
      0x01 => Self { operation: Operation::ORA, mode: Addressing::IndexedIndirect, execute_cycles:        },
      0x11 => Self { operation: Operation::ORA, mode: Addressing::IndirectIndexed, execute_cycles:        },

      // PHA
      0x48 => Self { operation: Operation::PHA, mode: Addressing::Implicit, execute_cycles:         },

      // PHP
      0x08 => Self { operation: Operation::PHP, mode: Addressing::Implicit, execute_cycles:         },

      // PLA
      0x68 => Self { operation: Operation::PLA, mode: Addressing::Implicit, execute_cycles:         },

      // PLP
      0x28 => Self { operation: Operation::PLP, mode: Addressing::Implicit, execute_cycles:         },

      // ROL
      0x2A => Self { operation: Operation::ROL, mode: Addressing::Accumulator, execute_cycles:        },
      0x26 => Self { operation: Operation::ROL, mode: Addressing::ZeroPage,    execute_cycles:         },
      0x36 => Self { operation: Operation::ROL, mode: Addressing::ZeroPageX,   execute_cycles:        },
      0x2E => Self { operation: Operation::ROL, mode: Addressing::Absolute,    execute_cycles:         },
      0x3E => Self { operation: Operation::ROL, mode: Addressing::AbsoluteX,   execute_cycles:        },

      // ROR
      0x6A => Self { operation: Operation::ROR, mode: Addressing::Accumulator, execute_cycles:        },
      0x66 => Self { operation: Operation::ROR, mode: Addressing::ZeroPage,    execute_cycles:         },
      0x76 => Self { operation: Operation::ROR, mode: Addressing::ZeroPageX,   execute_cycles:        },
      0x6E => Self { operation: Operation::ROR, mode: Addressing::Absolute,    execute_cycles:         },
      0x7E => Self { operation: Operation::ROR, mode: Addressing::AbsoluteX,   execute_cycles:        },

      // RTI
      0x40 => Self { operation: Operation::RTI, mode: Addressing::Implicit, execute_cycles:         },

      // RTS
      0x60 => Self { operation: Operation::RTS, mode: Addressing::Implicit, execute_cycles:         },

      // SBC
      0xE9 => Self { operation: Operation::SBC, mode: Addressing::Immediate,       execute_cycles: 0 },
      0xE5 => Self { operation: Operation::SBC, mode: Addressing::ZeroPage,        execute_cycles:         },
      0xF5 => Self { operation: Operation::SBC, mode: Addressing::ZeroPageX,       execute_cycles:        },
      0xED => Self { operation: Operation::SBC, mode: Addressing::Absolute,        execute_cycles:         },
      0xFD => Self { operation: Operation::SBC, mode: Addressing::AbsoluteX,       execute_cycles:        },
      0xF9 => Self { operation: Operation::SBC, mode: Addressing::AbsoluteY,       execute_cycles:        },
      0xE1 => Self { operation: Operation::SBC, mode: Addressing::IndexedIndirect, execute_cycles:        },
      0xF1 => Self { operation: Operation::SBC, mode: Addressing::IndirectIndexed, execute_cycles:        },

      // SEC
      0x38 => Self { operation: Operation::SEC, mode: Addressing::Implicit, execute_cycles:         },

      // SED
      0xF8 => Self { operation: Operation::SED, mode: Addressing::Implicit, execute_cycles:         },

      // SEI
      0x78 => Self { operation: Operation::SEI, mode: Addressing::Implicit, execute_cycles:         },

      // STA
      0x85 => Self { operation: Operation::STA, mode: Addressing::ZeroPage,        execute_cycles:         },
      0x95 => Self { operation: Operation::STA, mode: Addressing::ZeroPageX,       execute_cycles:        },
      0x8D => Self { operation: Operation::STA, mode: Addressing::Absolute,        execute_cycles:         },
      0x9D => Self { operation: Operation::STA, mode: Addressing::AbsoluteX,       execute_cycles:        },
      0x99 => Self { operation: Operation::STA, mode: Addressing::AbsoluteY,       execute_cycles:        },
      0x81 => Self { operation: Operation::STA, mode: Addressing::IndexedIndirect, execute_cycles:        },
      0x91 => Self { operation: Operation::STA, mode: Addressing::IndirectIndexed, execute_cycles:        },

      // STX
      0x86 => Self { operation: Operation::STX, mode: Addressing::ZeroPage,  execute_cycles:         },
      0x96 => Self { operation: Operation::STX, mode: Addressing::ZeroPageY, execute_cycles:        },
      0x8E => Self { operation: Operation::STX, mode: Addressing::Absolute,  execute_cycles:         },

      // STY
      0x84 => Self { operation: Operation::STY, mode: Addressing::ZeroPage,  execute_cycles:         },
      0x94 => Self { operation: Operation::STY, mode: Addressing::ZeroPageX, execute_cycles:        },
      0x8C => Self { operation: Operation::STY, mode: Addressing::Absolute,  execute_cycles:         },

      // TAX
      0xAA => Self { operation: Operation::TAX, mode: Addressing::Implicit, execute_cycles:         },

      // TAY
      0xA8 => Self { operation: Operation::TAY, mode: Addressing::Implicit, execute_cycles:         },

      // TSX
      0xBA => Self { operation: Operation::TSX, mode: Addressing::Implicit, execute_cycles:         },

      // TXA
      0x8A => Self { operation: Operation::TXA, mode: Addressing::Implicit, execute_cycles:         },

      // TXS
      0x9A => Self { operation: Operation::TXS, mode: Addressing::Implicit, execute_cycles:         },

      // TYA
      0x98 => Self { operation: Operation::TYA, mode: Addressing::Implicit, execute_cycles:         },

      _ => panic!("Instruction Decode: Bytes 0x{} not a valid opcode", byte),
    }
  }
}
