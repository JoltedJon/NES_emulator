#![allow(dead_code, unused_imports)]


use bitfield_struct::bitfield;
use log::error;
use crate::{instructions::{Instruction, Operation}, utils};


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
