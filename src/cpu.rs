use bitfield_struct::bitfield;


use crate::instructions;


#[bitfield(u8)]
struct Flags {
    carry: bool,
    zero: bool,
    irq_disable: bool,
    decimal_mode: bool,
    break_flag: bool,

    #[bits(default = true)]
    __: bool,
    overflow: bool,
    signed: bool,
}


struct Registers {
    accumlator: u8,
    reg_x: u8,
    reg_y: u8,
    program_counter: u16,
    stack_pointer: u8,
    status_flags: Flags,
}

impl Default for Registers {
    fn default() -> Registers {
        Registers {
            accumlator: 0,
            reg_x: 0,
            reg_y: 0,
            program_counter: 0xFFFC,
            stack_pointer: 0xFD,
            status_flags: Flags::default(),
        }
    }
}

enum CpuState {

}

pub struct CPU {
    reg_file: Registers,
    memory: [u8; 0x10000],
}

impl CPU {
    pub fn new() -> Self {
        Self {
            reg_file: Registers::default(),
            memory: [0; 0x10000],
        }
    }

    pub fn init_memory(&mut self, program_rom: &[u8], program_size: usize) {
        self.memory[(0x10000 - program_size)..].copy_from_slice(program_rom);
        self.reset();
    }

    pub fn do_cycle() {

    }

    // TODO should I make this behave like
    // https://www.pagetable.com/?p=410
    fn reset(&mut self) {
        // For now simply setting program counter to value at reset vector
        self.reg_file.program_counter = self.memory[0xFFFC] as u16 | ((self.memory[0xFFFD] as u16) << 8);
        self.reg_file.stack_pointer = 0xFD;
    }
}
