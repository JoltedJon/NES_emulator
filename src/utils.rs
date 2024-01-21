pub fn convert_addr(bytes: &[u8]) -> u16 {
    ((bytes[1] as u16) << 8) | (bytes[0] as u16)
}

pub fn cross_boundary(addr: u16, offset: u8) -> bool {
    (addr % 256) != ((addr + offset as u16) % 256)
}

pub fn print_hexdump(bytes: &[u8]) {
    for (i, chunk) in bytes.chunks(16).enumerate() {
        print!("{:04X}  ", i * 16);
        for b in chunk {
            print!("{:02X} ", b);
        }
        println!();
    }
}

#[macro_use]
mod log {
    #[macro_export]
    macro_rules! LOG_INFO {
        ($($args:expr),*) => {{
            print!("\x1b[96m[INFO]\x1b[0m ");
            $(
                print!("{}", $args);
            )*
            println!("");
        }};
    }

    #[macro_export]
    macro_rules! LOG_WARN {
        ($($args:expr),*) => {{
            print!("\x1b[93m[WARNING]\x1b[0m ");
            $(
                print!("{}", $args);
            )*
            println!("");
        }};
    }

    #[macro_export]
    macro_rules! LOG_ERROR {
        ($($args:expr),*) => {{
            print!("\x1b[91m[ERROR]\x1b[0m ");
            $(
                print!("{}", $args);
            )*
            println!("");
        }};
    }
}
