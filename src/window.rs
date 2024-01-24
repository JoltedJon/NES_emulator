
// Height and Width constants of NES window in pixels
const NES_HEIGHT: u8 = 224;
const NES_WIDTH: u8 = 256;

pub struct NESWindow {
  context: Sdl,
  video_subsystem: VideoSubsystem,
  window: Window,
  canvas: Canvas<Window>
}

impl NESWindow {
  pub fn new() -> Self {

  }
}
