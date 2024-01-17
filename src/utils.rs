pub fn convert_addr(bytes: &[u8]) -> u16 {
    ((bytes[1] as u16) << 8) | (bytes[0] as u16)
}
