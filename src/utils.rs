pub fn convert_addr(bytes: &[u8]) -> u16 {
    ((bytes[1] as u16) << 8) | (bytes[0] as u16)
}

// Converts a u8 relative offset to a sign extended u16
pub fn offset_to_addr(byte: u8) -> u16 {
    ((byte as i16) | ((byte as i16 & 0x80) << 8)) as u16
}
