#![allow(dead_code, unused_imports)]


use bitfield_struct::bitfield;
use log::error;
use crate::{instructions::{Instruction, Operation}, utils};

pub struct Memory {
  pub pattern_table: [u8; 0x2000],
  pub nametable: [u8; 0x1000],
  pub palette_ram: [u8; 0x0020],
}

impl Memory {
  pub fn new() -> Self {
    Self {
      pattern_table: [0; 0x2000],
      nametable: [0; 0x1000],
      palette_ram: [0; 0x0020],
    }
  }

  pub fn read(&self, addr: u16) -> u8 {
    match addr {
      0..=0x1FFF => {
        self.pattern_table[addr as usize]
      }
      0x2000..=0x3EFF => {
        let addr = (addr - 0x2000) % (0x1000);
        self.nametable[addr as usize]
      }
      0x3F00..=0x3FFF => {
        let addr = (addr - 0x3F00) % 0x20;
        self.palette_ram[addr as usize]
      }
      _ => {
        error!("PPU Read Invalid Address {}", addr);
        0
      }
    }
  }

  pub fn write(&mut self, addr: u16, byte: u8) {
    match addr {
      0..=0x1FFF => {
        self.pattern_table[addr as usize] = byte;
      }
      0x2000..=0x3EFF => {
        let addr = (addr - 0x2000) % (0x1000);
        self.nametable[addr as usize] = byte;
      }
      0x3F00..=0x3FFF => {
        let addr = (addr - 0x3F00) % 0x20;
        self.palette_ram[addr as usize] = byte;
      }
      _ => {
        error!("PPU Write Invalid Address {} -> {}", addr, byte);
      }
    }
  }

  pub fn load_rom(&mut self, character_rom: &[u8], character_size: usize) {
    if character_size > 0x2000 {
      todo!("Bigger character sizes not yet supported");
    }
    self.pattern_table[..(character_size)].copy_from_slice(character_rom);

    utils::print_hexdump(&self.pattern_table);
  }
}

pub struct PPU {
  pub memory: Memory,
}

impl PPU {
  pub fn new() -> Self {
    Self {
      memory: Memory::new(),
    }
  }

  pub fn init_memory(&mut self, character_rom: &[u8], character_size: usize) {
    self.memory.load_rom(character_rom, character_size);
  }
}
