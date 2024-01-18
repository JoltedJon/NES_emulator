pub fn convert_addr(bytes: &[u8]) -> u16 {
    ((bytes[1] as u16) << 8) | (bytes[0] as u16)
}

pub fn cross_boundary(addr: u16, offset: u8) -> bool {
    (addr % 256) != ((addr + offset as u16) % 256)
}
