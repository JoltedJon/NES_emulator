#![allow(dead_code)]

use std::env;
use std::fs;
use std::process::exit;

pub mod cpu;
pub mod instructions;
pub mod utils;
pub mod ppu;

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
struct Header {
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

    // TODO could be used later on to detect file encoding
    // if let Ok(s) = std::str::from_utf8(&bytes[0..3]) {
    //     println!("{}", s);
    // }

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

fn main() {
    let args: Vec<String> = env::args().collect();// println!("Contents of {:?}:\n\n", Path::new(path).file_name().unwrap());

    let mut pc: Option<u16> =None;
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
        LOG_ERROR!("Rom with mapper ", head.mapper_number, " not supported yet");
        exit(1);
    }

    println!("{:?}", head);

    let mut nes_cpu: cpu::CPU = cpu::CPU::new();

    nes_cpu.init_memory(&content[0x10..(0x10 + head.program_size as usize)], head.program_size as usize);

    loop {
        nes_cpu.do_cycle();
    }


}
