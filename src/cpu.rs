use bitfield_struct::bitfield;

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

// struct Memory {

// }
