#![allow(dead_code, unused_imports, non_camel_case_types)]

use std::env;
use std::fs;
use std::process::exit;
use std::rc::Rc;
use std::time::Duration;

use log;
use log::error;
use sdl2::event::Event;
use sdl2::keyboard::Keycode;
use sdl2::pixels::Color;

use crate::memory::Memory;

// pub mod cpu;
// pub mod instructions;
pub mod utils;
// pub mod ppu;
pub mod memory;

fn usage() {
  println!("Usage: ./emulator <path to file> <optional program counter>");
  exit(1);
}



#[derive(Debug)]
enum Mirroring {
  Horizontal,
  Vertical,
}

#[derive(Debug)]
pub struct Header {
  program_size: u32,
  character_size: u32,

  // Derived from Flags 6 and 7 bytes
  mapper_number: u8,

  // Flags 6
  mirroring: Mirroring,
  persistant_memory: bool, // Is there persistant memory
  trainer: bool,           // Is there the 512 byte trainer
  ignore_mirroring: bool,  // if set provide four-screen vram

  // Flags 7
  vs_unisystem: bool,      // TODO probably not going to implement these two
  playchoice:bool,         // 8 KB of hint screen data stored after CHR data

  // Flags 8
  program_ram_size: u8,
}



fn parse_header(bytes: &[u8]) -> Header {

  std::env::set_var("RUST_BACKTRACE", "1");

  Header {
    program_size: u32::from(bytes[4]) * 0x4000,
    character_size: u32::from(bytes[5]) * 0x2000,
    mapper_number: ((bytes[7] & 0xF0) | (bytes[6] >> 4)),

    mirroring: (match bytes[6] & 0x01 {
      0 => Mirroring::Horizontal,
      _ => Mirroring::Vertical
    }),

    persistant_memory: (bytes[6] & 0x02 != 0),
    trainer: (bytes[6] & 0x04 != 0),
    ignore_mirroring: (bytes[6] & 0x08 != 0),

    vs_unisystem: (bytes[7] & 0x01 != 0),
    playchoice: (bytes[7] & 0x02 != 0),

    program_ram_size: bytes[8],
  }
}

// CPU/PPU can only make 2 types of requests, and it will always request each cycle
pub enum Memory_Request {
  Read(u16),
  Write(u16, u8),
}

fn main() {
  let args: Vec<String> = env::args().collect();

  let mut pc: Option<u16> = None;
  if args.len() == 3 {
    pc = Some(args[2].parse::<u16>().unwrap());
  }

  if args.len() > 2 || args.len() > 3 {
    usage();
  }

  let path = &args[1];

  let content: Vec<u8> = fs::read(path).expect(&format!("Failed to read {}", path));
  let head = parse_header(&content);

  if head.mapper_number != 0 {
    error!("Rom with mapper {} not supported yet" , head.mapper_number);
    exit(1);
  }

  if head.playchoice {
    error!("Playchoice roms are not supported yet");
    exit(1);
  }

  println!("{:?}", head);

  // TODO
  // let ppu_rc: Rc<PPU_Registers> = Rc::new(PPU::new());

  let mut memory: Memory = Memory::new(head, &content[0x10..]);

  // let mut nes_cpu: cpu::CPU = cpu::CPU::new();
  // nes_cpu.init_memory(&content[0x10..(0x10 + head.program_size as usize)], head.program_size as usize);

  // let mut nes_ppu: ppu::PPU = ppu::PPU::new();
  // if head.character_size > 0 {
    //     nes_ppu.init_memory(&content[(0x10 + head.program_size as usize)..], head.character_size as usize);
    // }

    // loop {
      //     nes_cpu.do_cycle();
      // }

}
