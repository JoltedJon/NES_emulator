use crate::{cpu::{CPU, Flags}, utils, LOG_INFO};


#[derive(Copy, Clone)]
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

#[derive(Debug, Copy, Clone, PartialEq, Eq)]
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

#[derive(Copy, Clone)]
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
      // Immediate
      0x09 => Self { operation: Operation::ORA, mode: Addressing::Immediate, execute_cycles: 2 },
      0x29 => Self { operation: Operation::AND, mode: Addressing::Immediate, execute_cycles: 2 },
      0x49 => Self { operation: Operation::EOR, mode: Addressing::Immediate, execute_cycles: 2 },
      0x69 => Self { operation: Operation::ADC, mode: Addressing::Immediate, execute_cycles: 2 },
      0xA0 => Self { operation: Operation::LDY, mode: Addressing::Immediate, execute_cycles: 2 },
      0xA2 => Self { operation: Operation::LDX, mode: Addressing::Immediate, execute_cycles: 2 },
      0xA9 => Self { operation: Operation::LDA, mode: Addressing::Immediate, execute_cycles: 2 },
      0xC0 => Self { operation: Operation::CPY, mode: Addressing::Immediate, execute_cycles: 2 },
      0xC9 => Self { operation: Operation::CMP, mode: Addressing::Immediate, execute_cycles: 2 },
      0xE0 => Self { operation: Operation::CPX, mode: Addressing::Immediate, execute_cycles: 2 },
      0xE9 => Self { operation: Operation::SBC, mode: Addressing::Immediate, execute_cycles: 2 },



      // Zero Page
      0x05 => Self { operation: Operation::ORA, mode: Addressing::ZeroPage, execute_cycles: 3 },
      0x24 => Self { operation: Operation::BIT, mode: Addressing::ZeroPage, execute_cycles: 3 },
      0x25 => Self { operation: Operation::AND, mode: Addressing::ZeroPage, execute_cycles: 3 },
      0x45 => Self { operation: Operation::EOR, mode: Addressing::ZeroPage, execute_cycles: 3 },
      0x65 => Self { operation: Operation::ADC, mode: Addressing::ZeroPage, execute_cycles: 3 },
      0x84 => Self { operation: Operation::STY, mode: Addressing::ZeroPage, execute_cycles: 3 },
      0x85 => Self { operation: Operation::STA, mode: Addressing::ZeroPage, execute_cycles: 3 },
      0x86 => Self { operation: Operation::STX, mode: Addressing::ZeroPage, execute_cycles: 3 },
      0xA4 => Self { operation: Operation::LDY, mode: Addressing::ZeroPage, execute_cycles: 3 },
      0xA5 => Self { operation: Operation::LDA, mode: Addressing::ZeroPage, execute_cycles: 3 },
      0xA6 => Self { operation: Operation::LDX, mode: Addressing::ZeroPage, execute_cycles: 3 },
      0xC4 => Self { operation: Operation::CPY, mode: Addressing::ZeroPage, execute_cycles: 3 },
      0xC5 => Self { operation: Operation::CMP, mode: Addressing::ZeroPage, execute_cycles: 3 },
      0xE4 => Self { operation: Operation::CPX, mode: Addressing::ZeroPage, execute_cycles: 3 },
      0xE5 => Self { operation: Operation::SBC, mode: Addressing::ZeroPage, execute_cycles: 3 },

      0x06 => Self { operation: Operation::ASL, mode: Addressing::ZeroPage, execute_cycles: 5 },
      0x26 => Self { operation: Operation::ROL, mode: Addressing::ZeroPage, execute_cycles: 5 },
      0x46 => Self { operation: Operation::LSR, mode: Addressing::ZeroPage, execute_cycles: 5 },
      0x66 => Self { operation: Operation::ROR, mode: Addressing::ZeroPage, execute_cycles: 5 },
      0xC6 => Self { operation: Operation::DEC, mode: Addressing::ZeroPage, execute_cycles: 5 },
      0xE6 => Self { operation: Operation::INC, mode: Addressing::ZeroPage, execute_cycles: 5 },



      // Zero Page, X
      0x15 => Self { operation: Operation::ORA, mode: Addressing::ZeroPageX, execute_cycles: 4 },
      0x35 => Self { operation: Operation::AND, mode: Addressing::ZeroPageX, execute_cycles: 4 },
      0x55 => Self { operation: Operation::EOR, mode: Addressing::ZeroPageX, execute_cycles: 4 },
      0x75 => Self { operation: Operation::ADC, mode: Addressing::ZeroPageX, execute_cycles: 4 },
      0x94 => Self { operation: Operation::STY, mode: Addressing::ZeroPageX, execute_cycles: 4 },
      0x95 => Self { operation: Operation::STA, mode: Addressing::ZeroPageX, execute_cycles: 4 },
      0xB4 => Self { operation: Operation::LDY, mode: Addressing::ZeroPageX, execute_cycles: 4 },
      0xB5 => Self { operation: Operation::LDA, mode: Addressing::ZeroPageX, execute_cycles: 4 },
      0xD5 => Self { operation: Operation::CMP, mode: Addressing::ZeroPageX, execute_cycles: 4 },
      0xF5 => Self { operation: Operation::SBC, mode: Addressing::ZeroPageX, execute_cycles: 4 },

      0x16 => Self { operation: Operation::ASL, mode: Addressing::ZeroPageX, execute_cycles: 6 },
      0x36 => Self { operation: Operation::ROL, mode: Addressing::ZeroPageX, execute_cycles: 6 },
      0x56 => Self { operation: Operation::LSR, mode: Addressing::ZeroPageX, execute_cycles: 6 },
      0x76 => Self { operation: Operation::ROR, mode: Addressing::ZeroPageX, execute_cycles: 6 },
      0xD6 => Self { operation: Operation::DEC, mode: Addressing::ZeroPageX, execute_cycles: 6 },
      0xF6 => Self { operation: Operation::INC, mode: Addressing::ZeroPageX, execute_cycles: 6 },



      // Zero Page, Y
      0xB6 => Self { operation: Operation::LDX, mode: Addressing::ZeroPageY, execute_cycles: 4 },
      0x96 => Self { operation: Operation::STX, mode: Addressing::ZeroPageY, execute_cycles: 4 },



      // Absolute
      0x4C => Self { operation: Operation::JMP, mode: Addressing::Absolute, execute_cycles: 3 },

      0x6D => Self { operation: Operation::ADC, mode: Addressing::Absolute, execute_cycles: 4 },
      0x2D => Self { operation: Operation::AND, mode: Addressing::Absolute, execute_cycles: 4 },
      0x2C => Self { operation: Operation::BIT, mode: Addressing::Absolute, execute_cycles: 4 },
      0xCD => Self { operation: Operation::CMP, mode: Addressing::Absolute, execute_cycles: 4 },
      0xEC => Self { operation: Operation::CPX, mode: Addressing::Absolute, execute_cycles: 4 },
      0xCC => Self { operation: Operation::CPY, mode: Addressing::Absolute, execute_cycles: 4 },
      0xAD => Self { operation: Operation::LDA, mode: Addressing::Absolute, execute_cycles: 4 },
      0xAE => Self { operation: Operation::LDX, mode: Addressing::Absolute, execute_cycles: 4 },
      0xAC => Self { operation: Operation::LDY, mode: Addressing::Absolute, execute_cycles: 4 },
      0x0D => Self { operation: Operation::ORA, mode: Addressing::Absolute, execute_cycles: 4 },
      0xED => Self { operation: Operation::SBC, mode: Addressing::Absolute, execute_cycles: 4 },
      0x8D => Self { operation: Operation::STA, mode: Addressing::Absolute, execute_cycles: 4 },
      0x8E => Self { operation: Operation::STX, mode: Addressing::Absolute, execute_cycles: 4 },
      0x8C => Self { operation: Operation::STY, mode: Addressing::Absolute, execute_cycles: 4 },
      0x4D => Self { operation: Operation::EOR, mode: Addressing::Absolute, execute_cycles: 4 },

      0x0E => Self { operation: Operation::ASL, mode: Addressing::Absolute, execute_cycles: 6 },
      0x20 => Self { operation: Operation::JSR, mode: Addressing::Absolute, execute_cycles: 6 },
      0x2E => Self { operation: Operation::ROL, mode: Addressing::Absolute, execute_cycles: 6 },
      0x4E => Self { operation: Operation::LSR, mode: Addressing::Absolute, execute_cycles: 6 },
      0x6E => Self { operation: Operation::ROR, mode: Addressing::Absolute, execute_cycles: 6 },
      0xCE => Self { operation: Operation::DEC, mode: Addressing::Absolute, execute_cycles: 6 },
      0xEE => Self { operation: Operation::INC, mode: Addressing::Absolute, execute_cycles: 6 },



      // Absolute, X
      0x7D => Self { operation: Operation::ADC, mode: Addressing::AbsoluteX, execute_cycles: 4 },
      0x3D => Self { operation: Operation::AND, mode: Addressing::AbsoluteX, execute_cycles: 4 },
      0xFD => Self { operation: Operation::SBC, mode: Addressing::AbsoluteX, execute_cycles: 4 },
      0xDD => Self { operation: Operation::CMP, mode: Addressing::AbsoluteX, execute_cycles: 4 },
      0x1D => Self { operation: Operation::ORA, mode: Addressing::AbsoluteX, execute_cycles: 4 },
      0x5D => Self { operation: Operation::EOR, mode: Addressing::AbsoluteX, execute_cycles: 4 },
      0xBD => Self { operation: Operation::LDA, mode: Addressing::AbsoluteX, execute_cycles: 4 },
      0xBC => Self { operation: Operation::LDY, mode: Addressing::AbsoluteX, execute_cycles: 4 },

      0x9D => Self { operation: Operation::STA, mode: Addressing::AbsoluteX, execute_cycles: 5 },

      0x1E => Self { operation: Operation::ASL, mode: Addressing::AbsoluteX, execute_cycles: 7 },
      0x3E => Self { operation: Operation::ROL, mode: Addressing::AbsoluteX, execute_cycles: 7 },
      0x5E => Self { operation: Operation::LSR, mode: Addressing::AbsoluteX, execute_cycles: 7 },
      0x7E => Self { operation: Operation::ROR, mode: Addressing::AbsoluteX, execute_cycles: 7 },
      0xDE => Self { operation: Operation::DEC, mode: Addressing::AbsoluteX, execute_cycles: 7 },
      0xFE => Self { operation: Operation::INC, mode: Addressing::AbsoluteX, execute_cycles: 7 },



      // Absolute, Y
      0x79 => Self { operation: Operation::ADC, mode: Addressing::AbsoluteY, execute_cycles: 4 },
      0x39 => Self { operation: Operation::AND, mode: Addressing::AbsoluteY, execute_cycles: 4 },
      0xD9 => Self { operation: Operation::CMP, mode: Addressing::AbsoluteY, execute_cycles: 4 },
      0x59 => Self { operation: Operation::EOR, mode: Addressing::AbsoluteY, execute_cycles: 4 },
      0xB9 => Self { operation: Operation::LDA, mode: Addressing::AbsoluteY, execute_cycles: 4 },
      0xBE => Self { operation: Operation::LDX, mode: Addressing::AbsoluteY, execute_cycles: 4 },
      0x19 => Self { operation: Operation::ORA, mode: Addressing::AbsoluteY, execute_cycles: 4 },
      0xF9 => Self { operation: Operation::SBC, mode: Addressing::AbsoluteY, execute_cycles: 4 },

      0x99 => Self { operation: Operation::STA, mode: Addressing::AbsoluteY, execute_cycles: 5 },



      // Indexed Indirect
      0x61 => Self { operation: Operation::ADC, mode: Addressing::IndexedIndirect, execute_cycles: 6 },
      0x21 => Self { operation: Operation::AND, mode: Addressing::IndexedIndirect, execute_cycles: 6 },
      0xC1 => Self { operation: Operation::CMP, mode: Addressing::IndexedIndirect, execute_cycles: 6 },
      0x41 => Self { operation: Operation::EOR, mode: Addressing::IndexedIndirect, execute_cycles: 6 },
      0xA1 => Self { operation: Operation::LDA, mode: Addressing::IndexedIndirect, execute_cycles: 6 },
      0x01 => Self { operation: Operation::ORA, mode: Addressing::IndexedIndirect, execute_cycles: 6 },
      0xE1 => Self { operation: Operation::SBC, mode: Addressing::IndexedIndirect, execute_cycles: 6 },
      0x81 => Self { operation: Operation::STA, mode: Addressing::IndexedIndirect, execute_cycles: 6 },



      // Indirect Indexed
      0x71 => Self { operation: Operation::ADC, mode: Addressing::IndirectIndexed, execute_cycles: 5 },
      0x31 => Self { operation: Operation::AND, mode: Addressing::IndirectIndexed, execute_cycles: 5 },
      0xD1 => Self { operation: Operation::CMP, mode: Addressing::IndirectIndexed, execute_cycles: 5 },
      0x51 => Self { operation: Operation::EOR, mode: Addressing::IndirectIndexed, execute_cycles: 5 },
      0xB1 => Self { operation: Operation::LDA, mode: Addressing::IndirectIndexed, execute_cycles: 5 },
      0x11 => Self { operation: Operation::ORA, mode: Addressing::IndirectIndexed, execute_cycles: 5 },
      0xF1 => Self { operation: Operation::SBC, mode: Addressing::IndirectIndexed, execute_cycles: 5 },
      0x91 => Self { operation: Operation::STA, mode: Addressing::IndirectIndexed, execute_cycles: 6 },



      // Accumulator
      0x0A => Self { operation: Operation::ASL, mode: Addressing::Accumulator, execute_cycles: 2 },
      0x4A => Self { operation: Operation::LSR, mode: Addressing::Accumulator, execute_cycles: 2 },
      0x2A => Self { operation: Operation::ROL, mode: Addressing::Accumulator, execute_cycles: 2 },
      0x6A => Self { operation: Operation::ROR, mode: Addressing::Accumulator, execute_cycles: 2 },



      // Relative
      0x90 => Self { operation: Operation::BCC, mode: Addressing::Relative, execute_cycles: 2 },
      0xB0 => Self { operation: Operation::BCS, mode: Addressing::Relative, execute_cycles: 2 },
      0xF0 => Self { operation: Operation::BEQ, mode: Addressing::Relative, execute_cycles: 2 },
      0x30 => Self { operation: Operation::BMI, mode: Addressing::Relative, execute_cycles: 2 },
      0xD0 => Self { operation: Operation::BNE, mode: Addressing::Relative, execute_cycles: 2 },
      0x10 => Self { operation: Operation::BPL, mode: Addressing::Relative, execute_cycles: 2 },
      0x50 => Self { operation: Operation::BVC, mode: Addressing::Relative, execute_cycles: 2 },
      0x70 => Self { operation: Operation::BVS, mode: Addressing::Relative, execute_cycles: 2 },



      // Implicit
      0x18 => Self { operation: Operation::CLC, mode: Addressing::Implicit, execute_cycles: 2 },
      0x38 => Self { operation: Operation::SEC, mode: Addressing::Implicit, execute_cycles: 2 },
      0x58 => Self { operation: Operation::CLI, mode: Addressing::Implicit, execute_cycles: 2 },
      0x78 => Self { operation: Operation::SEI, mode: Addressing::Implicit, execute_cycles: 2 },
      0x88 => Self { operation: Operation::DEY, mode: Addressing::Implicit, execute_cycles: 2 },
      0x8A => Self { operation: Operation::TXA, mode: Addressing::Implicit, execute_cycles: 2 },
      0x98 => Self { operation: Operation::TYA, mode: Addressing::Implicit, execute_cycles: 2 },
      0x9A => Self { operation: Operation::TXS, mode: Addressing::Implicit, execute_cycles: 2 },
      0xA8 => Self { operation: Operation::TAY, mode: Addressing::Implicit, execute_cycles: 2 },
      0xAA => Self { operation: Operation::TAX, mode: Addressing::Implicit, execute_cycles: 2 },
      0xB8 => Self { operation: Operation::CLV, mode: Addressing::Implicit, execute_cycles: 2 },
      0xBA => Self { operation: Operation::TSX, mode: Addressing::Implicit, execute_cycles: 2 },
      0xC8 => Self { operation: Operation::INY, mode: Addressing::Implicit, execute_cycles: 2 },
      0xCA => Self { operation: Operation::DEX, mode: Addressing::Implicit, execute_cycles: 2 },
      0xD8 => Self { operation: Operation::CLD, mode: Addressing::Implicit, execute_cycles: 2 },
      0xE8 => Self { operation: Operation::INX, mode: Addressing::Implicit, execute_cycles: 2 },
      0xEA => Self { operation: Operation::NOP, mode: Addressing::Implicit, execute_cycles: 2 },
      0xF8 => Self { operation: Operation::SED, mode: Addressing::Implicit, execute_cycles: 2 },

      0x08 => Self { operation: Operation::PHP, mode: Addressing::Implicit, execute_cycles: 3 },
      0x48 => Self { operation: Operation::PHA, mode: Addressing::Implicit, execute_cycles: 3 },

      0x28 => Self { operation: Operation::PLP, mode: Addressing::Implicit, execute_cycles: 4 },
      0x68 => Self { operation: Operation::PLA, mode: Addressing::Implicit, execute_cycles: 4 },

      0x40 => Self { operation: Operation::RTI, mode: Addressing::Implicit, execute_cycles: 6 },
      0x60 => Self { operation: Operation::RTS, mode: Addressing::Implicit, execute_cycles: 6 },

      0x00 => Self { operation: Operation::BRK, mode: Addressing::Implicit, execute_cycles: 7 },


      // Indirect
      0x6C => Self { operation: Operation::JMP, mode: Addressing::Indirect, execute_cycles: 5 },

      unknown => {
        LOG_INFO!("decoded unknown byte {}", unknown);
        Self { operation: Operation::NOP, mode: Addressing::Implicit, execute_cycles: 2 }
      },
    }
  }

  fn absolute(&mut self, cpu: &mut CPU) {
    let addr = cpu.read_memory_addr(cpu.program_counter);
    let mut byte = cpu.read_memory(addr);

    cpu.program_counter += 2;

    match self.operation {
      Operation::ADC => adc(cpu, addr),
      Operation::AND => and(cpu, addr),
      Operation::BIT => bit(cpu, addr),
      Operation::CMP => cmp(cpu, addr),
      Operation::CPX => cpx(cpu, addr),
      Operation::CPY => cpy(cpu, addr),
      Operation::DEC => dec(cpu, addr),
      Operation::EOR => eor(cpu, addr),
      Operation::INC => inc(cpu, addr),
      Operation::JMP => jmp(cpu, addr),
      Operation::JSR => jsr(cpu, addr),
      Operation::LDA => lda(cpu, addr),
      Operation::LDX => ldx(cpu, addr),
      Operation::LDY => ldy(cpu, addr),
      Operation::ORA => ora(cpu, addr),
      Operation::SBC => sbc(cpu, addr),
      Operation::STA => sta(cpu, addr),
      Operation::STX => stx(cpu, addr),
      Operation::STY => sty(cpu, addr),

      // Weridos
      Operation::ASL => {
        asl(cpu, &mut byte);
        cpu.write_memory(addr, byte);
      },
      Operation::LSR => {
        lsr(cpu, &mut byte);
        cpu.write_memory(addr, byte);
      },
      Operation::ROL => {
        rol(cpu, &mut byte);
        cpu.write_memory(addr, byte);
      },
      Operation::ROR => {
        ror(cpu, &mut byte);
        cpu.write_memory(addr, byte);
      },

      _ => panic!("Invalid absolute"),
    }
  }

  fn absolute_x(&mut self, cpu: &mut CPU) -> u8 {
    let addr = cpu.read_memory_addr(cpu.program_counter);
    let cross = utils::cross_boundary(addr, cpu.reg_x);

    let addr = addr.wrapping_add(cpu.reg_x as u16);
    let mut byte = cpu.read_memory(addr);

    cpu.program_counter += 2;

    match self.operation {
      Operation::ADC => adc(cpu, addr),
      Operation::AND => and(cpu, addr),
      Operation::SBC => sbc(cpu, addr),
      Operation::CMP => cmp(cpu, addr),
      Operation::ORA => ora(cpu, addr),
      Operation::EOR => eor(cpu, addr),
      Operation::LDA => lda(cpu, addr),
      Operation::LDY => ldy(cpu, addr),
      Operation::STA => sta(cpu, addr),
      Operation::DEC => dec(cpu, addr),
      Operation::INC => inc(cpu, addr),

      Operation::ASL => {
        asl(cpu, &mut byte);
        cpu.write_memory(addr, byte);
      },
      Operation::ROL => {
        rol(cpu, &mut byte);
        cpu.write_memory(addr, byte);
      },
      Operation::ROR => {
        ror(cpu, &mut byte);
        cpu.write_memory(addr, byte);
      },
      Operation::LSR => {
        lsr(cpu, &mut byte);
        cpu.write_memory(addr, byte);
      },

      _ => panic!("Invalid absolute, X"),
    }

    cross as u8
  }

  fn absolute_y(&mut self, cpu: &mut CPU) -> u8 {
    let addr = cpu.read_memory_addr(cpu.program_counter);
    let cross = utils::cross_boundary(addr, cpu.reg_y);

    let addr = addr.wrapping_add(cpu.reg_y as u16);

    cpu.program_counter += 2;

    match self.operation {
      Operation::ADC => adc(cpu, addr),
      Operation::AND => and(cpu, addr),
      Operation::CMP => cmp(cpu, addr),
      Operation::EOR => eor(cpu, addr),
      Operation::LDA => lda(cpu, addr),
      Operation::LDX => ldx(cpu, addr),
      Operation::ORA => ora(cpu, addr),
      Operation::SBC => sbc(cpu, addr),
      Operation::STA => sta(cpu, addr),

      _ => panic!("Invalid absolute, Y"),
    }

    cross as u8
  }

  fn accumulator(&mut self, cpu: &mut CPU) {
    let mut byte = cpu.accumulator;

    match self.operation {
      Operation::ASL => asl(cpu, &mut byte),
      Operation::LSR => lsr(cpu, &mut byte),
      Operation::ROL => rol(cpu, &mut byte),
      Operation::ROR => ror(cpu, &mut byte),

      _ => panic!("Invalid accumulator"),
    }

    cpu.accumulator = byte;
  }

  fn immediate(&mut self, cpu: &mut CPU) {
    match self.operation {
        Operation::ADC => adc(cpu, cpu.program_counter),
        Operation::AND => and(cpu, cpu.program_counter),
        Operation::CMP => cmp(cpu, cpu.program_counter),
        Operation::CPX => cpx(cpu, cpu.program_counter),
        Operation::CPY => cpx(cpu, cpu.program_counter),
        Operation::EOR => eor(cpu, cpu.program_counter),
        Operation::LDA => lda(cpu, cpu.program_counter),
        Operation::LDX => ldx(cpu, cpu.program_counter),
        Operation::LDY => ldy(cpu, cpu.program_counter),
        Operation::ORA => ora(cpu, cpu.program_counter),
        Operation::SBC => sbc(cpu, cpu.program_counter),

        _ => panic!("Invalid immediate"),
    }

    cpu.program_counter += 1;
  }

  fn implicit(&mut self, cpu: &mut CPU) {
    match self.operation {
      Operation::BRK => brk(cpu),
      Operation::CLC => clc(cpu),
      Operation::CLD => cld(cpu),
      Operation::CLI => cli(cpu),
      Operation::CLV => clv(cpu),
      Operation::DEX => dex(cpu),
      Operation::DEY => dey(cpu),
      Operation::INX => inx(cpu),
      Operation::INY => iny(cpu),
      Operation::NOP => nop(),
      Operation::PHA => pha(cpu),
      Operation::PHP => php(cpu),
      Operation::PLA => pla(cpu),
      Operation::PLP => plp(cpu),
      Operation::RTI => rti(cpu),
      Operation::RTS => rts(cpu),
      Operation::SEC => sec(cpu),
      Operation::SED => sed(cpu),
      Operation::SEI => sei(cpu),
      Operation::TAX => tax(cpu),
      Operation::TAY => tay(cpu),
      Operation::TSX => tsx(cpu),
      Operation::TXA => txa(cpu),
      Operation::TXS => txs(cpu),
      Operation::TYA => tya(cpu),

      _ => panic!("Invalid implicit"),
    }

  }

  fn indirect(&mut self, cpu: &mut CPU) {
    let addr = cpu.read_memory_addr(cpu.program_counter);
    let addr = cpu.read_memory_addr(addr);

    cpu.program_counter += 2;

    match self.operation {
      Operation::JMP => jmp(cpu, addr),

      _ => panic!("Invalid indirect"),
    }
  }

  fn indexed_indirect(&mut self, cpu: &mut CPU) {
    // At PC is zero page address, read it, then add x to it
    // read the address stored at (memory[pc] + x)
    let addr = cpu.read_memory(cpu.program_counter).wrapping_add(cpu.reg_x);
    let addr = cpu.read_memory_addr(addr as u16);

    cpu.program_counter += 1;
    match self.operation {
      Operation::ADC => adc(cpu, addr),
      Operation::AND => and(cpu, addr),
      Operation::CMP => cmp(cpu, addr),
      Operation::EOR => eor(cpu, addr),
      Operation::LDA => lda(cpu, addr),
      Operation::ORA => ora(cpu, addr),
      Operation::SBC => sbc(cpu, addr),
      Operation::STA => sta(cpu, addr),

      _ => panic!("Invalid indexed indirect"),
    }
  }

  fn indirect_indexed(&mut self, cpu: &mut CPU) -> u8 {
    // Current PC byte is an address in zero page,
    // At that address is another address, read it
    // Add reg y to that address
    // addr_1 = memory[pc] (zero page address 0x00-0xFF)
    // addr_final = memory[addr_1] + register Y
    let addr = cpu.read_memory(cpu.program_counter) as u16;
    let addr = cpu.read_memory_addr(addr);

    let cross = utils::cross_boundary(addr, cpu.reg_y);
    let addr = addr.wrapping_add(cpu.reg_y as u16);

    cpu.program_counter += 1;
    match self.operation {
      Operation::ADC => adc(cpu, addr),
      Operation::AND => and(cpu, addr),
      Operation::CMP => cmp(cpu, addr),
      Operation::EOR => eor(cpu, addr),
      Operation::LDA => lda(cpu, addr),
      Operation::ORA => ora(cpu, addr),
      Operation::SBC => sbc(cpu, addr),
      Operation::STA => sta(cpu, addr),

      _ => panic!("Invalid indexed indirect"),
    }

    cross as u8
  }

  fn relative(&mut self, cpu: &mut CPU) -> u8 {
    let offset = cpu.read_memory(cpu.program_counter);


    cpu.program_counter += 1;

    let original_pc = cpu.program_counter;
    let delay;

    match self.operation {
      Operation::BCC => delay = bcc(cpu, offset) as u8,
      Operation::BCS => delay = bcs(cpu, offset) as u8,
      Operation::BEQ => delay = beq(cpu, offset) as u8,
      Operation::BMI => delay = bmi(cpu, offset) as u8,
      Operation::BNE => delay = bne(cpu, offset) as u8,
      Operation::BPL => delay = bpl(cpu, offset) as u8,
      Operation::BVC => delay = bvc(cpu, offset) as u8,
      Operation::BVS => delay = bvs(cpu, offset) as u8,

      _ => panic!("Invalid relative"),
    }

    delay + (if (original_pc % 256) != (cpu.program_counter % 256){ 2 } else { 0 })
  }

  fn zero_page(&mut self, cpu: &mut CPU) {
    let addr = cpu.read_memory(cpu.program_counter) as u16;

    let mut byte = cpu.read_memory(addr);

    cpu.program_counter += 1;

    match self.operation {
      Operation::ADC => adc(cpu, addr),
      Operation::AND => and(cpu, addr),
      Operation::BIT => bit(cpu, addr),
      Operation::CMP => cmp(cpu, addr),
      Operation::CPX => cpx(cpu, addr),
      Operation::CPY => cpy(cpu, addr),
      Operation::DEC => dec(cpu, addr),
      Operation::EOR => eor(cpu, addr),
      Operation::INC => inc(cpu, addr),
      Operation::LDA => lda(cpu, addr),
      Operation::LDX => ldx(cpu, addr),
      Operation::LDY => ldy(cpu, addr),
      Operation::ORA => ora(cpu, addr),
      Operation::SBC => sbc(cpu, addr),
      Operation::STA => sta(cpu, addr),
      Operation::STX => stx(cpu, addr),
      Operation::STY => sty(cpu, addr),

      Operation::ASL => {
        asl(cpu, &mut byte);
        cpu.write_memory(addr, byte);
      },
      Operation::LSR => {
        lsr(cpu, &mut byte);
        cpu.write_memory(addr, byte);
      },
      Operation::ROL => {
        rol(cpu, &mut byte);
        cpu.write_memory(addr, byte);
      },
      Operation::ROR => {
        ror(cpu, &mut byte);
        cpu.write_memory(addr, byte);
      },

      _ => panic!("Invalid zero page"),
    }
  }

  fn zero_page_x(&mut self, cpu: &mut CPU) {
    let addr = cpu.read_memory(cpu.program_counter).wrapping_add(cpu.reg_x) as u16;

    let mut byte = cpu.read_memory(addr);

    cpu.program_counter += 1;

    match self.operation {
      Operation::ADC => adc(cpu, addr),
      Operation::AND => and(cpu, addr),
      Operation::CMP => cmp(cpu, addr),
      Operation::DEC => dec(cpu, addr),
      Operation::EOR => eor(cpu, addr),
      Operation::INC => inc(cpu, addr),
      Operation::LDA => lda(cpu, addr),
      Operation::LDY => ldy(cpu, addr),
      Operation::ORA => ora(cpu, addr),
      Operation::SBC => sbc(cpu, addr),
      Operation::STA => sta(cpu, addr),
      Operation::STY => sty(cpu, addr),

      Operation::ASL => {
        asl(cpu, &mut byte);
        cpu.write_memory(addr, byte);
      },
      Operation::LSR => {
        lsr(cpu, &mut byte);
        cpu.write_memory(addr, byte);
      },
      Operation::ROL => {
        rol(cpu, &mut byte);
        cpu.write_memory(addr, byte);
      },
      Operation::ROR => {
        ror(cpu, &mut byte);
        cpu.write_memory(addr, byte);
      },

      _ => panic!("Invalid zero page, X"),
    }
  }

  fn zero_page_y(&mut self, cpu: &mut CPU) {
    let addr = cpu.read_memory(cpu.program_counter).wrapping_add(cpu.reg_y) as u16;

    cpu.program_counter += 1;

    match self.operation {
      Operation::LDX => ldx(cpu, addr),
      Operation::STX => stx(cpu, addr),

      _ => panic!("Invalid zero page, Y"),
    }
  }

  pub fn execute(&mut self, cpu: &mut CPU) -> u8 {
    cpu.program_counter += 1;

    let mut delay: u8 = 0;

    match self.mode {
        Addressing::Absolute        => self.absolute(cpu),
        Addressing::AbsoluteX       => delay = self.absolute_x(cpu),
        Addressing::AbsoluteY       => delay = self.absolute_y(cpu),
        Addressing::Accumulator     => self.accumulator(cpu),
        Addressing::Immediate       => self.immediate(cpu),
        Addressing::Implicit        => self.implicit(cpu),
        Addressing::Indirect        => self.indirect(cpu),
        Addressing::IndexedIndirect => self.indexed_indirect(cpu),
        Addressing::IndirectIndexed => delay = self.indirect_indexed(cpu),
        Addressing::Relative        => delay = self.relative(cpu) as u8,
        Addressing::ZeroPage        => self.zero_page(cpu),
        Addressing::ZeroPageX       => self.zero_page_x(cpu),
        Addressing::ZeroPageY       => self.zero_page_y(cpu),
    }
    delay
  }
}

// Execution of instructions
fn adc(cpu: &mut CPU, addr: u16) {
    let opp: u8 = cpu.read_memory(addr);
    let acc: u8 = cpu.accumulator;

    // This result shouldn't have any wrapping issues as the highest value this can attain is
    // 0x101 (255 + 255 + 1)
    let result: u16 = (acc as u16) + (opp as u16) + (cpu.status_flags.carry() as u16);

    cpu.status_flags.set_carry(result > 0xFF);
    cpu.status_flags.set_overflow(((acc ^ result as u8) & (opp ^ result as u8)) & 0x80 != 0);

    cpu.accumulator = result as u8;
    cpu.set_signed(cpu.accumulator);
    cpu.set_zero(cpu.accumulator);
}

fn and(cpu: &mut CPU, addr: u16) {
    let value: u8 = cpu.read_memory(addr);
    cpu.accumulator &= value;
    cpu.set_zero(cpu.accumulator);
    cpu.set_signed(cpu.accumulator);
}

// This is supposed to operate on memory OR accumulator
fn asl(cpu: &mut CPU, byte: &mut u8) {
    cpu.status_flags.set_carry(*byte & 0x80 == 1);

    *byte <<= 1;

    // if byte is the accumulator this should work hopefully
    cpu.set_zero(cpu.accumulator);
    cpu.set_signed(*byte);
}

fn bcc(cpu: &mut CPU, offset: u8) -> bool {
    cpu.branch(!cpu.status_flags.carry(), offset)
}

fn bcs(cpu: &mut CPU, offset: u8) -> bool {
    cpu.branch(cpu.status_flags.carry(), offset)
}

fn beq(cpu: &mut CPU, offset: u8) -> bool {
    cpu.branch(cpu.status_flags.zero(), offset)
}

fn bit(cpu: &mut CPU, addr: u16) {
    let mem_byte = cpu.read_memory(addr);
    cpu.set_signed(mem_byte & 0x80);
    cpu.status_flags.set_overflow(mem_byte & 0x40 == 1);
    cpu.set_zero(cpu.accumulator & mem_byte);
}

fn bmi(cpu: &mut CPU, offset: u8) -> bool {
    cpu.branch(cpu.status_flags.signed(), offset)
}

fn bne(cpu: &mut CPU, offset: u8) -> bool {
    cpu.branch(!cpu.status_flags.zero(), offset)
}

fn bpl(cpu: &mut CPU, offset: u8) -> bool {
    cpu.branch(!cpu.status_flags.signed(), offset)
}

fn brk(cpu: &mut CPU) {
    cpu.push_addr(cpu.program_counter);
    cpu.push(cpu.status_flags.into());
    cpu.program_counter = utils::convert_addr(&[cpu.read_memory(0xFFFE),cpu.read_memory(0xFFFF)]);
    cpu.status_flags.set_break_flag(true);
}

fn bvc(cpu: &mut CPU, offset: u8) -> bool {
    cpu.branch(!cpu.status_flags.overflow(), offset)
}

fn bvs(cpu: &mut CPU, offset: u8) -> bool {
    cpu.branch(cpu.status_flags.overflow(), offset)
}

fn clc(cpu: &mut CPU) {
    cpu.status_flags.set_carry(false);
}

fn cld(cpu: &mut CPU) {
    cpu.status_flags.set_decimal_mode(false);
}

fn cli(cpu: &mut CPU) {
    cpu.status_flags.set_irq_disable(false);
}

fn clv(cpu: &mut CPU) {
    cpu.status_flags.set_overflow(false);
}

fn cmp(cpu: &mut CPU, addr: u16) {
    cpu.compare(cpu.accumulator, addr);
}

fn cpx(cpu: &mut CPU, addr: u16) {
    cpu.compare(cpu.reg_x, addr);
}

fn cpy(cpu: &mut CPU, addr: u16) {
    cpu.compare(cpu.reg_y, addr);
}

fn dec(cpu: &mut CPU, addr: u16) {
    let result: u8 = cpu.read_memory(addr).wrapping_sub(1);
    cpu.set_signed(result);
    cpu.set_zero(result);
    cpu.write_memory(addr, result);
}

fn dex(cpu: &mut CPU) {
    cpu.reg_x = cpu.reg_x.wrapping_sub(1);
    cpu.set_zero(cpu.reg_x);
    cpu.set_signed(cpu.reg_x);
}

fn dey(cpu: &mut CPU) {
    cpu.reg_y = cpu.reg_y.wrapping_sub(1);
    cpu.set_zero(cpu.reg_y);
    cpu.set_signed(cpu.reg_y);
}

fn eor(cpu: &mut CPU, addr: u16) {
    cpu.accumulator = cpu.accumulator ^ cpu.read_memory(addr);
    cpu.set_zero(cpu.accumulator);
    cpu.set_signed(cpu.accumulator);
}

fn inc(cpu: &mut CPU, addr: u16) {
    let result: u8 = cpu.read_memory(addr).wrapping_add(1);
    cpu.set_signed(result);
    cpu.set_zero(result);
    cpu.write_memory(addr, result);
}

fn inx(cpu: &mut CPU) {
    cpu.reg_x = cpu.reg_x.wrapping_add(1);
    cpu.set_zero(cpu.reg_x);
    cpu.set_signed(cpu.reg_x);
}

fn iny(cpu: &mut CPU) {
    cpu.reg_y = cpu.reg_y.wrapping_add(1);
    cpu.set_zero(cpu.reg_y);
    cpu.set_signed(cpu.reg_y);
}

fn jmp(cpu: &mut CPU, addr: u16) {
    cpu.program_counter = addr;
}

fn jsr(cpu: &mut CPU, addr: u16) {
    // At this point PC should point towards the instruction after JSR
    cpu.push_addr(cpu.program_counter);
    cpu.program_counter = addr;
}

fn lda(cpu: &mut CPU, addr: u16) {
    cpu.accumulator = cpu.read_memory(addr);
    cpu.set_signed(cpu.accumulator);
    cpu.set_zero(cpu.accumulator);
}

fn ldx(cpu: &mut CPU, addr: u16) {
    cpu.reg_x = cpu.read_memory(addr);
    cpu.set_signed(cpu.reg_x);
    cpu.set_zero(cpu.reg_x);
}

fn ldy(cpu: &mut CPU, addr: u16) {
    cpu.reg_y = cpu.read_memory(addr);
    cpu.set_signed(cpu.reg_y);
    cpu.set_zero(cpu.reg_y);
}

fn lsr(cpu: &mut CPU, byte: &mut u8) {
    cpu.status_flags.set_carry(*byte & 0x01 == 1);

    *byte >>= 1;

    // if byte is the accumulator this should work hopefully
    cpu.set_zero(cpu.accumulator);
    cpu.set_signed(*byte);
}

fn nop() {
    return;
}

fn ora(cpu: &mut CPU, addr: u16) {
    cpu.accumulator |= cpu.read_memory(addr);
    cpu.set_zero(cpu.accumulator);
    cpu.set_signed(cpu.accumulator);
}

fn pha(cpu: &mut CPU) {
    cpu.push(cpu.accumulator);
}

fn php(cpu: &mut CPU) {
    cpu.push(cpu.status_flags.into());
}

fn pla(cpu: &mut CPU) {
    let acc: u8 = cpu.pop();
    cpu.set_zero(acc);
    cpu.set_signed(acc);
    cpu.accumulator = acc;
}

fn plp(cpu: &mut CPU) {
    cpu.status_flags = Flags::from(cpu.pop());
}

fn rol(cpu: &mut CPU, byte: &mut u8) {
    let msb: bool = (*byte & 0x80) == 1;

    println!("Before: {}", byte);
    *byte <<= 1;
    println!("After: {}", byte);
    *byte |= cpu.status_flags.carry() as u8;

    cpu.status_flags.set_carry(msb);

    cpu.set_zero(cpu.accumulator);
    cpu.set_signed(*byte);
}

fn ror(cpu: &mut CPU, byte: &mut u8) {
    let lsb: bool = (*byte & 0x01) == 1;

    *byte >>= 1;
    *byte |= (cpu.status_flags.carry() as u8) << 7;

    cpu.status_flags.set_carry(lsb);

    cpu.set_zero(cpu.accumulator);
    cpu.set_signed(*byte);
}

fn rti(cpu: &mut CPU) {
    cpu.status_flags = Flags::from(cpu.pop());
    cpu.program_counter = cpu.pop_addr();
}

fn rts(cpu: &mut CPU) {
    cpu.program_counter = cpu.pop_addr();
}

fn sbc(cpu: &mut CPU, addr: u16) {
    let acc: u8 = cpu.accumulator;
    let opp: u8 = cpu.read_memory(addr);

    let result: u16 = (acc as u16).wrapping_sub(opp as u16).wrapping_sub(1 - cpu.status_flags.carry() as u16);

    cpu.status_flags.set_carry(result > 0xFF);
    cpu.status_flags.set_overflow(((acc ^ result as u8) & (opp ^ result as u8)) & 0x80 != 0);

    cpu.accumulator = result as u8;
    cpu.set_signed(cpu.accumulator);
    cpu.set_zero(cpu.accumulator);
}

fn sec(cpu: &mut CPU) {
    cpu.status_flags.set_carry(true);
}

fn sed(cpu: &mut CPU) {
    cpu.status_flags.set_decimal_mode(true);
}

fn sei(cpu: &mut CPU) {
    cpu.status_flags.set_irq_disable(true);
}

fn sta(cpu: &mut CPU, addr: u16) {
    cpu.write_memory(addr, cpu.accumulator);
}

fn stx(cpu: &mut CPU, addr: u16) {
    cpu.write_memory(addr, cpu.reg_x);
}

fn sty(cpu: &mut CPU, addr: u16) {
    cpu.write_memory(addr, cpu.reg_y);
}

fn tax(cpu: &mut CPU) {
    cpu.reg_x = cpu.accumulator;
    cpu.set_signed(cpu.reg_x);
    cpu.set_zero(cpu.reg_x);
}

fn tay(cpu: &mut CPU) {
    cpu.reg_y = cpu.accumulator;
    cpu.set_signed(cpu.reg_y);
    cpu.set_zero(cpu.reg_y);
}

fn tsx(cpu: &mut CPU) {
    cpu.reg_x = cpu.stack_pointer;
    cpu.set_signed(cpu.reg_x);
    cpu.set_zero(cpu.reg_x);
}

fn txa(cpu: &mut CPU) {
    cpu.accumulator = cpu.reg_x;
    cpu.set_signed(cpu.accumulator);
    cpu.set_zero(cpu.accumulator);
}

fn txs(cpu: &mut CPU) {
    cpu.stack_pointer = cpu.reg_x;
    cpu.set_signed(cpu.stack_pointer);
    cpu.set_zero(cpu.stack_pointer);
}

fn tya(cpu: &mut CPU) {
    cpu.accumulator = cpu.reg_y;
    cpu.set_signed(cpu.accumulator);
    cpu.set_zero(cpu.accumulator);
}
