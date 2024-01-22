#![allow(dead_code, unused_imports)]


use std::process::exit;

use bitfield_struct::bitfield;
use crate::{instructions::{Instruction, Operation}, utils};



#[bitfield(u8)]
pub struct Flags {
    pub carry: bool,
    pub zero: bool,
    pub irq_disable: bool,
    pub decimal_mode: bool,
    pub break_flag: bool,

    #[bits(default = true)]
    pub __: bool,
    pub overflow: bool,
    pub signed: bool,
}



#[derive(Debug)]
enum CpuStates {
    Fetch,
    Execute(u8),
    Delay(u8),
}

enum Data {
    Byte(u8),
    Addr(u16),
}

pub struct Memory {
    pub internal_ram: [u8; 0x0800],

    pub ppu_ram: [u8; 0x0008],

    pub audio_and_io: [u8; 0x0018],
    pub io_functionality: [u8; 0x0008],

    pub cartidge_space: [u8; 0xBFE0],
}

impl Memory {
    pub fn new() -> Self {
        Self {
            internal_ram: [0; 0x0800],
            ppu_ram: [0; 0x0008],
            audio_and_io: [0; 0x0018],
            io_functionality: [0; 0x0008],
            cartidge_space: [0; 0xBFE0]
        }
    }

    pub fn read(&self, addr: u16) -> u8 {
        // LOG_INFO!("Memory read - Virtual Address: ", addr);
        match addr {
            0..=0x1FFF => {
                let addr = addr % 0x800;
                // println!("\tInternal Ram: {:0x} -> {:0x}", addr, self.internal_ram[addr as usize]);
                self.internal_ram[addr as usize]
            },
            0x2000..=0x3FFF => {
                let addr = (addr - 0x2000) % 0x8;
                // println!("\tPPU Registers: {:0x} -> {:0x}", addr, self.ppu_ram[addr as usize]);
                self.ppu_ram[addr as usize]
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

    pub fn write(&mut self, addr: u16, byte: u8) {
        // TODO need to have a way to commit battery backed addresses to memory
        match addr {
            0..=0x1FFF => {
                let addr = addr % 0x800;
                self.internal_ram[addr as usize] = byte;
            },
            0x2000..=0x3FFF => {
                let addr = (addr - 0x2000) % 0x8;
                self.ppu_ram[addr as usize] = byte;
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

    pub fn load_rom(&mut self, program_rom: &[u8], program_size: usize) {
        self.cartidge_space[(0xBFE0 - program_size)..].copy_from_slice(program_rom);
    }
}

pub struct CPU {
    // Registers
    pub accumulator: u8,
    pub reg_x: u8,
    pub reg_y: u8,
    pub program_counter: u16,
    pub stack_pointer: u8,
    pub status_flags: Flags,


    pub memory: Memory,

    state: CpuStates,

    inst: Option<Instruction>,
    data: Option<Data>,

    cycle: u64,
}

impl CPU {
    pub fn new() -> Self {
        Self {
            accumulator: 0,
            reg_x: 0,
            reg_y: 0,
            program_counter: 0xFFFC,
            stack_pointer: 0xFD,
            status_flags: Flags::default(),
            memory: Memory::new(),
            state: CpuStates::Fetch,
            inst: None,
            data: None,
            cycle: 0,
        }
    }

    pub fn init_memory(&mut self, program_rom: &[u8], program_size: usize) {
        // self.memory[(0x10000 - program_size)..].copy_from_slice(program_rom);
        self.memory.load_rom(program_rom, program_size);
        self.reset();
    }

    pub fn write_memory(&mut self, addr: u16, value: u8) {
        // self.memory[addr as usize] = value;
        self.memory.write(addr, value);
    }

    pub fn read_memory(&mut self, addr: u16) -> u8 {
        // self.memory[addr as usize]
        self.memory.read(addr)
    }

    // return
    pub fn read_memory_addr(&mut self, addr: u16) -> u16 {
        utils::convert_addr(&[self.read_memory(addr), self.read_memory(addr + 1)])
    }

    // TODO
    // Might need to take in something like memory responses for banking
    // Also might need to send out interrupt stuff
    pub fn do_cycle(&mut self) {
        self.print_state();
        match self.state {
            CpuStates::Fetch => {
                self.inst = Some(Instruction::decode_inst(self.fetch()));

                self.state = CpuStates::Execute(self.inst.as_mut().unwrap().execute_cycles - 1)
            },
            CpuStates::Execute(x) => {
                if x > 0 {
                    self.state = CpuStates::Execute(x - 1);
                } else {
                    // Execute will take care of things like changing PC
                    let mut inst = self.inst.unwrap();

                    let delay = inst.execute(self);

                    if inst.operation == Operation::BRK {
                        println!("Break");
                        let mut input = String::new();
                        std::io::stdin().read_line(&mut input).expect("cannot read user input");
                    }

                    if delay > 0 {
                        self.state = CpuStates::Delay(delay);
                    } else {
                        self.state = CpuStates::Fetch;
                    }
                }
            },
            CpuStates::Delay(delay) => self.state = if delay == 1 { CpuStates::Fetch } else { CpuStates::Delay(delay - 1) },
        }

        self.cycle += 1;
    }

    fn print_state(&self) {
        use std::io::Write;

        match self.state {
            CpuStates::Fetch | CpuStates::Delay(_) => {
                return;
            },
            CpuStates::Execute(x) => {
                if x > 0 {return}

                println!("CPU State Before Cycle({}): {:?}", self.cycle, self.state);
                println!("Execute: {:?}", self.inst.as_ref().unwrap().operation);
                println!("\tPC:              {:x}", self.program_counter);
                println!("\tAccumulator:     {}",   self.accumulator);
                println!("\tReg X:           {}",   self.reg_x);
                println!("\tReg Y:           {}",   self.reg_y);
                println!("\tStack Pointer:   {}",   self.stack_pointer);

                println!("\tFlags: ");
                println!("\t\tcarry:         {}",   self.status_flags.carry());
                println!("\t\tzero:          {}",   self.status_flags.zero());
                println!("\t\tIRQ Disabled:  {}",   self.status_flags.irq_disable());
                println!("\t\tDecimal Mode:  {}",   self.status_flags.decimal_mode());
                println!("\t\tBreak:         {}",   self.status_flags.break_flag());
                println!("\t\tOverflow:      {}",   self.status_flags.overflow());
                println!("\t\tSigned:        {}",   self.status_flags.signed());
            },
        }
        println!("\n");

        std::io::stdout().flush().unwrap();
    }

    // TODO should I make this behave like
    // https://www.pagetable.com/?p=410
    fn reset(&mut self) {
        // For now simply setting program counter to value at reset vector
        self.program_counter = utils::convert_addr(&[self.memory.read(0xFFFC), self.memory.read(0xFFFD)]);

        // Supposedly interrupts are default to being disabled
        self.status_flags.set_irq_disable(true);
    }


}

// Helper functions for instructions executing and such
impl CPU {
    // Fetches current PC
    pub fn fetch(&mut self) -> u8 {
        self.read_memory(self.program_counter)
    }

    pub fn push(&mut self, byte: u8) {
        self.write_memory(0x0100 | (self.stack_pointer as u16), byte);
        self.stack_pointer = self.stack_pointer.wrapping_sub(1);
    }

    pub fn pop(&mut self) -> u8 {
        self.stack_pointer = self.stack_pointer.wrapping_add(1);
        self.read_memory(0x0100 | (self.stack_pointer as u16))
    }

    pub fn pop_addr(&mut self) -> u16 {
        utils::convert_addr(&[self.pop(), self.pop()])
    }

    pub fn push_addr(&mut self, addr: u16) {
        self.push((addr >> 8) as u8);
        self.push((addr & 0xFF) as u8);
    }

    pub fn set_zero(&mut self, byte: u8) {
        self.status_flags.set_zero(byte == 0);
    }

    pub fn set_signed(&mut self, byte: u8) {
        self.status_flags.set_signed((byte as i8) < 0);
    }

    // General branching
    // Branches need to wait extra cycle if successful branch
    pub fn branch(&mut self, cond: bool, offset: u8) -> bool{
        if cond {
            self.program_counter = self.program_counter.wrapping_add_signed((offset as i8).into());
        }
        cond
    }

    // General Comparing
    pub fn compare(&mut self, byte: u8, addr: u16) {
        let mem_byte = self.read_memory(addr);
        let result: u8 = byte.wrapping_sub(mem_byte);
        self.set_signed(result);
        self.set_zero(result);
        self.status_flags.set_carry(byte >= mem_byte);
    }

}
