#![allow(non_camel_case_types)]

use log::error;

use crate::{utils, Header, Memory_Request};

pub struct CPU_Memory {
  pub internal_ram: [u8; 0x0800],

  // pub ppu_registers: [u8; 0x0008],

  pub audio_and_io: [u8; 0x0018],
  pub io_functionality: [u8; 0x0008],

  pub cartidge_space: [u8; 0xBFE0],
}

impl CPU_Memory {
  pub fn new(program_rom: &[u8], program_size: usize) -> Self {
    let mut temp: Self = Self {
      internal_ram: [0; 0x0800],
      audio_and_io: [0; 0x0018],
      io_functionality: [0; 0x0008],
      cartidge_space: [0; 0xBFE0]
    };

    temp.cartidge_space[(0xBFE0 - program_size)..].copy_from_slice(program_rom);

    temp
  }

  pub fn write(&mut self, addr: u16, byte: u8) {
    // TODO need to have a way to commit battery backed addresses to memory
    match addr {
      0..=0x1FFF => {
        let addr = addr % 0x800;
        self.internal_ram[addr as usize] = byte;
      },
      0x2000..=0x3FFF => {
        // let addr = (addr - 0x2000) % 0x8;
        // self.ppu_ram[addr as usize] = byte;
        error!("CPU WRITE - ({:0x}) should have gone to PPU", addr);
      },
      0x4000..=0x4017 => {
        let addr = (addr - 0x4000) % 0x18;
        self.audio_and_io[addr as usize] = byte;
      },
      0x4018..=0x401F => {
        let addr = (addr - 0x4018) % 0x8;
        self.io_functionality[addr as usize] = byte;
      }
      0x4020..=0xFFFF => {
        let addr = addr - 0x4020;
        self.cartidge_space[addr as usize] = byte;
      }
    }
  }

  pub fn read(&self, addr: u16) -> u8 {
    match addr {
      0..=0x1FFF => {
        let addr = addr % 0x800;
        // println!("\tInternal Ram: {:0x} -> {:0x}", addr, self.internal_ram[addr as usize]);
        self.internal_ram[addr as usize]
      },
      0x2000..=0x3FFF => {
        // let addr = (addr - 0x2000) % 0x8;
        // println!("\tPPU Registers: {:0x} -> {:0x}", addr, self.ppu_ram[addr as usize]);
        // self.ppu_ram[addr as usize]
        error!("CPU READ - ({:0x}) should have gone to PPU", addr);
        0
      },
      0x4000..=0x4017 => {
        let addr = (addr - 0x4000) % 0x18;
        // println!("\tAPU and I/O registers: {:0x} -> {:0x}", addr, self.audio_and_io[addr as usize]);
        self.audio_and_io[addr as usize]
      },
      0x4018..=0x401F => {
        let addr = (addr - 0x4018) % 0x8;
        // println!("\tAPU and I/O Functionality: {:0x} -> {:0x}", addr, self.io_functionality[addr as usize]);
        self.io_functionality[addr as usize]
      }
      0x4020..=0xFFFF => {
        let addr = addr - 0x4020;
        // println!("\tCartidge Space: {:0x} -> {:0x}", addr, self.cartidge_space[addr as usize]);
        self.cartidge_space[addr as usize]
      }
    }
  }
}

pub struct PPU_Memory {
  pub pattern_table: [u8; 0x2000],
  pub nametable: [u8; 0x1000],
  pub palette_ram: [u8; 0x0020],
}

impl PPU_Memory {
  pub fn new(character_rom: &[u8], character_size: usize) -> Self {
    if character_size > 0x2000 {
      todo!("Bigger character sizes not yet supported");
    }

    let mut temp: Self = Self {
      pattern_table: [0; 0x2000],
      nametable: [0; 0x1000],
      palette_ram: [0; 0x0020],
    };
    temp.pattern_table[..(character_size)].copy_from_slice(character_rom);

    temp
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
}

pub struct Memory {
  cpu: CPU_Memory,
  ppu: PPU_Memory,
}

pub struct Memory_Response {
  cpu_response: Option<u8>,
  ppu_response: Option<u8>
}

impl Memory {
  // Takes in ROM without the header
  pub fn new(head: Header, bytes: &[u8]) -> Self {
    Self {
      cpu: CPU_Memory::new(&bytes[..(head.program_size as usize)], head.program_size as usize),
      ppu: PPU_Memory::new(&bytes[(head.program_size as usize)..((head.program_size + head.character_size) as usize)], head.character_size as usize)
    }
  }

  pub fn request(&mut self, cpu_req: Memory_Request, ppu_req: Memory_Request) -> Memory_Response {
    let mut response: Memory_Response = Memory_Response { cpu_response: None, ppu_response: None };

    match cpu_req {
      Memory_Request::Read(addr) => response.cpu_response = Some(self.cpu.read(addr)),
      Memory_Request::Write(addr, byte) => self.cpu.write(addr, byte)
    }

    match ppu_req {
      Memory_Request::Read(addr) => response.ppu_response = Some(self.ppu.read(addr)),
      Memory_Request::Write(addr, byte) => self.ppu.write(addr, byte),
    }

    response
  }
}
