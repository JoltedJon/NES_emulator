use std::env;
use std::fs;

fn usage() {
    panic!("Usage: <program> <path to file>");
}

fn print_hexdump(bytes: &[u8]) {
    for (i, chunk) in bytes.chunks(16).enumerate() {
        print!("{:04X}  ", i * 16);
        for b in chunk {
            print!("{:02X} ", b);
        }
        println!();
    }
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
    vs_unisystem: bool,      // TODO what is this
    playchoice:bool,         // 8 KB of hint screen data stored after CHR data

    // Flags 8
    program_ram_size: u8,
}



fn parse_header(bytes: &[u8]) -> Header {

    // TODO could be used later on to detect file encoding
    // if let Ok(s) = std::str::from_utf8(&bytes[0..3]) {
    //     println!("{}", s);
    // }

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

    // print_hexdump(&content);

    if args.len() != 2 {
        usage();
    }

    let path = &args[1];

    let content: Vec<u8> = fs::read(path).expect(&format!("Failed to read {}", path));

    let head = parse_header(&content);

    println!("{:?}", head);

    let mut memory: [u8; 0x10000] = [0; 0x10000];

    memory[0x8000..].copy_from_slice(&content[..(head.program_size as usize)]);

    println!("{:02X?}", &memory[0xFFFC..0xFFFD]);
}
