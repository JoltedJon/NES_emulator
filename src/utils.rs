pub fn convert_addr(bytes: &[u8]) -> u16 {
    ((bytes[1] as u16) << 8) | (bytes[0] as u16)
}

pub fn cross_boundary(addr: u16, offset: u8) -> bool {
    (addr % 256) != ((addr + offset as u16) % 256)
}

#[macro_use]
mod log {
    #[macro_export]
    macro_rules! LOG_INFO {
        ($($arg:tt)*) => {
            println!("\033[96m[INFO]\033[0m {}", $($arg)*);
        };
    }

    #[macro_export]
    macro_rules! LOG_WARN {
        ($($arg:tt)*) => {
            println!("\033[93m[WARNING]\033[0m {}", $($arg)*);
        };
    }

    #[macro_export]
    macro_rules! LOG_ERROR {
        ($($arg:tt)*) => {
            println!("\033[91m[ERROR]\033[0m {}", $($arg)*);
        };
    }
}
