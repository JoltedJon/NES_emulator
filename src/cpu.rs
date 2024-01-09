#![allow(dead_code)]


use bitfield_struct::bitfield;
use crate::{instructions::{Instruction, Addressing, Operation}, utils};



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
    accumulator: u8,
    reg_x: u8,
    reg_y: u8,
    program_counter: u16,
    stack_pointer: u8,
    status_flags: Flags,
}

impl Default for Registers {
    fn default() -> Registers {
        Registers {
            accumulator: 0,
            reg_x: 0,
            reg_y: 0,
            program_counter: 0xFFFC,
            stack_pointer: 0xFD,  // TODO this might want to be set to 0?
            status_flags: Flags::default(),
        }
    }
}

enum CpuStates {
    Fetch,
    Decode(u8), // How many bytes left to decode
    Execute,
    Delay(u8), // Incase certain instructions need a delay after execution
}

enum Data {
    Byte(u8),
    Addr(u16),
}

pub struct CPU {
    reg_file: Registers,
    memory: [u8; 0x10000],

    state: CpuStates,

    inst: Option<Instruction>,
    data: Option<Data>,

    cycle: u64,
}

impl CPU {
    pub fn new() -> Self {
        Self {
            reg_file: Registers::default(),
            memory: [0; 0x10000],
            state: CpuStates::Fetch,
            inst: None,
            data: None,
            cycle: 0,
        }
    }

    pub fn init_memory(&mut self, program_rom: &[u8], program_size: usize) {
        self.memory[(0x10000 - program_size)..].copy_from_slice(program_rom);
        self.reset();
    }

    // TODO
    // Might need to take in something like memory responses for banking
    // Also might need to send out interrupt stuff
    pub fn do_cycle(&mut self) {
        match self.state {
            CpuStates::Fetch => {
                self.fetch_state();
            }
            CpuStates::Decode(x) => todo!(),
            CpuStates::Execute => todo!(),
            CpuStates::Delay(x) => todo!(),
        }

        self.cycle += 1;
    }

    // TODO should I make this behave like
    // https://www.pagetable.com/?p=410
    fn reset(&mut self) {
        self.reg_file = Registers::default();
        // For now simply setting program counter to value at reset vector
        self.reg_file.program_counter = utils::convert_addr(&self.memory[0xFFFC..0xFFFD]);

        // Supposedly interrupts are default to being disabled
        self.reg_file.status_flags.set_irq_disable(true);
    }

    // Fetch Next opcode and figure out what the next state should be
    fn fetch_state(&mut self) {
        self.inst = Some(Instruction::decode_inst(self.fetch()));
        self.reg_file.program_counter += 1;

        match self.inst.as_ref().unwrap().mode {
            Addressing::Implicit |
            Addressing::Accumulator => {
                self.data = None;
                self.state = CpuStates::Execute;
            },
            Addressing::Immediate       |
            Addressing::ZeroPage        |
            Addressing::ZeroPageX       |
            Addressing::ZeroPageY       |
            Addressing::Relative        |
            Addressing::IndexedIndirect |
            Addressing::IndirectIndexed => {
                self.data = Some(Data::Byte(0));
                self.state = CpuStates::Decode(1);
            },
            Addressing::Absolute  |
            Addressing::AbsoluteX |
            Addressing::AbsoluteY |
            Addressing::Indirect  => {
                self.data = Some(Data::Addr(0));
                self.state = CpuStates::Decode(2);
            }
        }

    }

    fn decode_state(&mut self, mut num_bytes: u8) {


        match self.data.as_mut().unwrap() {
            Data::Byte(_) => self.data = Some(Data::Byte(self.fetch())),
            Data::Addr(x) => {
                match num_bytes {
                    1 => self.data = Some(Data::Addr(*x | ((self.fetch() as u16) << 8))),
                    2 => self.data = Some(Data::Addr(self.fetch() as u16)),
                    _ => panic!("Decode State: Impossible Match"),
                }
            },
        }

        num_bytes -= 1;
        if num_bytes > 0 {
            self.state = CpuStates::Decode(num_bytes);
            return;
        }

        let mut inst = self.inst.as_mut().unwrap();


        match inst.mode {
            Addressing::Implicit => todo!(),
            Addressing::Accumulator => todo!(),
            Addressing::Immediate => {
                self.execute_immediate(&inst);

            },
            Addressing::ZeroPage => todo!(),
            Addressing::ZeroPageX => todo!(),
            Addressing::ZeroPageY => todo!(),
            Addressing::Relative => todo!(),
            Addressing::Absolute => todo!(),
            Addressing::AbsoluteX => todo!(),
            Addressing::AbsoluteY => todo!(),
            Addressing::Indirect => todo!(),
            Addressing::IndexedIndirect => todo!(),
            Addressing::IndirectIndexed => todo!(),
        }


        self.reg_file.program_counter += 1;
    }

    // Immediate Instructions run in same cycle as getting all data
    fn execute_immediate(&mut self, inst: &Instruction) {
        if let Data::Byte(value) = self.data.as_ref().unwrap() {
            match inst.operation {
                Operation::ADC => self.adc(*value),
                Operation::AND => self.and(*value),
                _ => panic!("Decode State: Impossible Immediate"),
            }
        }
    }
}

// Helper functions for instructions executing and such
impl CPU {
    // Fetches current PC
    fn fetch(&mut self) -> u8 {
        self.memory[self.reg_file.program_counter as usize]
    }

    fn push(&mut self, byte: u8) {
        self.memory[0x0100 | (self.reg_file.stack_pointer as usize)] = byte;
        self.reg_file.stack_pointer -= 1;
    }

    fn pop(&mut self) -> u8 {
        self.reg_file.stack_pointer = self.reg_file.stack_pointer.wrapping_add(1);
        self.memory[0x0100 | (self.reg_file.stack_pointer as usize)]
    }

    fn pop_addr(&mut self) -> u16 {
        utils::convert_addr(&[self.pop(), self.pop()])
    }

    fn push_addr(&mut self, addr: u16) {
        self.push((addr >> 8) as u8);
        self.push((addr & 0xFF) as u8);
    }

    fn set_zero(&mut self, byte: u8) {
        self.reg_file.status_flags.set_zero(byte == 0);
    }

    fn set_signed(&mut self, byte: u8) {
        self.reg_file.status_flags.set_signed((byte as i8) < 0);
    }

    // General branching
    // Branches need to wait extra cycle if successful branch
    fn branch(&mut self, cond: bool, offset: u8) -> bool{
        if cond {
            self.reg_file.program_counter = self.reg_file.program_counter.wrapping_add(utils::offset_to_addr(offset));
        }
        cond
    }

    // General Comparing
    fn compare(&mut self, byte: u8, addr: u16) {
        let result: u8 = byte - self.memory[addr as usize];
        self.set_signed(result);
        self.set_zero(result);
        self.reg_file.status_flags.set_carry(byte >= self.memory[addr as usize]);
    }

}


// Execution of instructionsbyte
impl CPU {
    fn adc(&mut self, opp: u8) {
        let acc: u8 = self.reg_file.accumulator;

        // This result shouldn't have any wrapping issues as the highest value this can attain is
        // 0x101 (255 + 255 + 1)
        let result: u16 = (acc as u16) + (opp as u16) + (self.reg_file.status_flags.carry() as u16);

        self.reg_file.status_flags.set_carry(result > 0xFF);
        self.reg_file.status_flags.set_overflow(((acc ^ result as u8) & (opp ^ result as u8)) & 0x80 != 0);

        self.reg_file.accumulator = result as u8;
        self.set_signed(self.reg_file.accumulator);
        self.set_zero(self.reg_file.accumulator);
    }

    fn and(&mut self, value: u8) {
        self.reg_file.accumulator &= value;
        self.set_zero(self.reg_file.accumulator);
        self.set_signed(self.reg_file.accumulator);
    }

    // This is supposed to operate on memory OR accumulator
    fn asl(&mut self, byte: &mut u8) {
        self.reg_file.status_flags.set_carry(*byte & 0x80 == 1);

        *byte <<= 1;

        // if byte is the accumulator this should work hopefully
        self.set_zero(self.reg_file.accumulator);
        self.set_signed(*byte);
    }

    fn bcc(&mut self, offset: u8) -> bool {
        self.branch(!self.reg_file.status_flags.carry(), offset)
    }

    fn bcs(&mut self, offset: u8) -> bool {
        self.branch(self.reg_file.status_flags.carry(), offset)
    }

    fn beq(&mut self, offset: u8) -> bool {
        self.branch(self.reg_file.status_flags.zero(), offset)
    }

    fn bit(&mut self, addr: u16) {
        self.set_signed(self.memory[addr as usize] & 0x80);
        self.reg_file.status_flags.set_overflow(self.memory[addr as usize] & 0x40 == 1);
        self.set_zero(self.reg_file.accumulator & self.memory[addr as usize]);
    }

    fn bmi(&mut self, offset: u8) -> bool {
        self.branch(self.reg_file.status_flags.signed(), offset)
    }

    fn bne(&mut self, offset: u8) -> bool {
        self.branch(!self.reg_file.status_flags.zero(), offset)
    }

    fn bpl(&mut self, offset: u8) -> bool {
        self.branch(!self.reg_file.status_flags.signed(), offset)
    }

    fn brk(&mut self) {
        self.push_addr(self.reg_file.program_counter);
        self.push(self.reg_file.status_flags.into());
        self.reg_file.program_counter = utils::convert_addr(&self.memory[0xFFFE..0xFFFF]);
        self.reg_file.status_flags.set_break_flag(true);
    }

    fn bvc(&mut self, offset: u8) -> bool {
        self.branch(!self.reg_file.status_flags.overflow(), offset)
    }

    fn bvs(&mut self, offset: u8) -> bool {
        self.branch(self.reg_file.status_flags.overflow(), offset)
    }

    fn clc(&mut self) {
        self.reg_file.status_flags.set_carry(false);
    }

    fn cld(&mut self) {
        self.reg_file.status_flags.set_decimal_mode(false);
    }

    fn cli(&mut self) {
        self.reg_file.status_flags.set_irq_disable(false);
    }

    fn clv(&mut self) {
        self.reg_file.status_flags.set_overflow(false);
    }

    fn cmp(&mut self, addr: u16) {
        self.compare(self.reg_file.accumulator, addr);
    }

    fn cpx(&mut self, addr: u16) {
        self.compare(self.reg_file.reg_x, addr);
    }

    fn cpy(&mut self, addr: u16) {
        self.compare(self.reg_file.reg_y, addr);
    }

    fn dec(&mut self, addr: u16) {
        let result: u8 = self.memory[addr as usize] - 1;
        self.set_signed(result);
        self.set_zero(result);
        self.memory[addr as usize] = result;
    }

    fn dex(&mut self) {
        self.reg_file.reg_x -= 1;
        self.set_zero(self.reg_file.reg_x);
        self.set_signed(self.reg_file.reg_x);
    }

    fn dey(&mut self) {
        self.reg_file.reg_y -= 1;
        self.set_zero(self.reg_file.reg_y);
        self.set_signed(self.reg_file.reg_y);
    }

    fn eor(&mut self, addr: u16) {
        self.reg_file.accumulator = self.reg_file.accumulator ^ self.memory[addr as usize];
        self.set_zero(self.reg_file.accumulator);
        self.set_signed(self.reg_file.accumulator);
    }

    fn inc(&mut self, addr: u16) {
        let result: u8 = self.memory[addr as usize].wrapping_add(1);
        self.set_signed(result);
        self.set_zero(result);
        self.memory[addr as usize] = result;
    }

    fn inx(&mut self) {
        self.reg_file.reg_x = self.reg_file.reg_x.wrapping_add(1);
        self.set_zero(self.reg_file.reg_x);
        self.set_signed(self.reg_file.reg_x);
    }

    fn iny(&mut self) {
        self.reg_file.reg_y = self.reg_file.reg_y.wrapping_add(1);
        self.set_zero(self.reg_file.reg_y);
        self.set_signed(self.reg_file.reg_y);
    }

    fn jmp(&mut self, addr: u16) {
        self.reg_file.program_counter = addr;
    }

    fn jsr(&mut self, addr: u16) {
        // At this point PC should point towards the instruction after JSR
        self.push_addr(self.reg_file.program_counter);
        self.reg_file.program_counter = addr;
    }

    fn lda(&mut self, addr: u16) {
        self.reg_file.accumulator = self.memory[addr as usize];
        self.set_signed(self.reg_file.accumulator);
        self.set_zero(self.reg_file.accumulator);
    }

    fn ldx(&mut self, addr: u16) {
        self.reg_file.reg_x = self.memory[addr as usize];
        self.set_signed(self.reg_file.reg_x);
        self.set_zero(self.reg_file.reg_x);
    }

    fn ldy(&mut self, addr: u16) {
        self.reg_file.reg_y = self.memory[addr as usize];
        self.set_signed(self.reg_file.reg_y);
        self.set_zero(self.reg_file.reg_y);
    }

    fn lsr(&mut self, byte: &mut u8) {
        self.reg_file.status_flags.set_carry(*byte & 0x01 == 1);

        *byte >>= 1;

        // if byte is the accumulator this should work hopefully
        self.set_zero(self.reg_file.accumulator);
        self.set_signed(*byte);
    }

    fn nop() {
        return;
    }

    fn ora(&mut self, addr: u16) {
        self.reg_file.accumulator |= self.memory[addr as usize];
        self.set_zero(self.reg_file.accumulator);
        self.set_signed(self.reg_file.accumulator);
    }

    fn pha(&mut self) {
        self.push(self.reg_file.accumulator);
    }

    fn php(&mut self) {
        self.push(self.reg_file.status_flags.into());
    }

    fn pla(&mut self) {
        let acc: u8 = self.pop();
        self.set_zero(acc);
        self.set_signed(acc);
        self.reg_file.accumulator = acc;
    }

    fn plp(&mut self) {
        self.reg_file.status_flags = Flags(self.pop());
    }

    fn rol(&mut self, byte: &mut u8) {
        let msb: bool = (*byte & 0x80) == 1;

        *byte <<= 1;
        *byte &= self.reg_file.status_flags.carry() as u8;

        self.reg_file.status_flags.set_carry(msb);

        // TODO if byte is the accumulator this should work hopefully
        self.set_zero(self.reg_file.accumulator);
        self.set_signed(*byte);
    }

    fn ror(&mut self, byte: &mut u8) {
        let lsb: bool = (*byte & 0x01) == 1;

        *byte >>= 1;
        *byte &= (self.reg_file.status_flags.carry() as u8) << 7;

        self.reg_file.status_flags.set_carry(lsb);

        // TODO if byte is the accumulator this should work hopefully
        self.set_zero(self.reg_file.accumulator);
        self.set_signed(*byte);
    }

    fn rti(&mut self) {
        self.reg_file.status_flags = Flags(self.pop());
        self.reg_file.program_counter = self.pop_addr();
    }

    fn rts(&mut self) {
        self.reg_file.program_counter = self.pop_addr();
    }

    fn sbc(&mut self, addr: u16) {
        let acc: u8 = self.reg_file.accumulator;
        let opp: u8 = self.memory[addr as usize];

        let result: u16 = (acc as u16).wrapping_sub(opp as u16).wrapping_sub(1 - self.reg_file.status_flags.carry() as u16);

        self.reg_file.status_flags.set_carry(result > 0xFF);
        self.reg_file.status_flags.set_overflow(((acc ^ result as u8) & (opp ^ result as u8)) & 0x80 != 0);

        self.reg_file.accumulator = result as u8;
        self.set_signed(self.reg_file.accumulator);
        self.set_zero(self.reg_file.accumulator);
    }

    fn sec(&mut self) {
        self.reg_file.status_flags.set_carry(true);
    }

    fn sed(&mut self) {
        self.reg_file.status_flags.set_decimal_mode(true);
    }

    fn sei(&mut self) {
        self.reg_file.status_flags.set_irq_disable(true);
    }

    fn sta(&mut self, addr: u16) {
        self.memory[addr as usize] = self.reg_file.accumulator;
    }

    fn stx(&mut self, addr: u16) {
        self.memory[addr as usize] = self.reg_file.reg_x;
    }

    fn sty(&mut self, addr: u16) {
        self.memory[addr as usize] = self.reg_file.reg_y;
    }

    fn tax(&mut self) {
        self.reg_file.reg_x = self.reg_file.accumulator;
        self.set_signed(self.reg_file.reg_x);
        self.set_zero(self.reg_file.reg_x);
    }

    fn tay(&mut self) {
        self.reg_file.reg_y = self.reg_file.accumulator;
        self.set_signed(self.reg_file.reg_y);
        self.set_zero(self.reg_file.reg_y);
    }

    fn tsx(&mut self) {
        self.reg_file.reg_x = self.reg_file.stack_pointer;
        self.set_signed(self.reg_file.reg_x);
        self.set_zero(self.reg_file.reg_x);
    }

    fn txa(&mut self) {
        self.reg_file.accumulator = self.reg_file.reg_x;
        self.set_signed(self.reg_file.accumulator);
        self.set_zero(self.reg_file.accumulator);
    }

    fn txs(&mut self) {
        self.reg_file.stack_pointer = self.reg_file.reg_x;
        self.set_signed(self.reg_file.stack_pointer);
        self.set_zero(self.reg_file.stack_pointer);
    }

    fn tya(&mut self) {
        self.reg_file.accumulator = self.reg_file.reg_y;
        self.set_signed(self.reg_file.accumulator);
        self.set_zero(self.reg_file.accumulator);
    }
}
