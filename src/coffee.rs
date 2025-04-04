use std::io::{self, Write};
use std::thread::sleep;
use std::time::Duration;

// Frames für die Animation (Kaffeebecher mit beweglichem Dampf)
const COFFEE_FRAMES: [&str; 4] = [
    r#"
          ~      
              ~    
            ~      
       _____________
      <_____________> ___
      |             |/ _ \
      |             | | |
      |             |_| |
   ___|             |\___/
  /    \___________/    \
  \_____________________/
       \___________/
    "#,
    r#"
            ~      
          ~    
              ~      
       _____________
      <_____________> ___
      |             |/ _ \
      |             | | |
      |             |_| |
   ___|             |\___/
  /    \___________/    \
  \_____________________/
       \___________/
    "#,
    r#"
              ~      
            ~    
          ~      
       _____________
      <_____________> ___
      |             |/ _ \
      |             | | |
      |             |_| |
   ___|             |\___/
  /    \___________/    \
  \_____________________/
       \___________/
    "#,
    r#"
          ~      
              ~    
            ~      
       _____________
      <_____________> ___
      |             |/ _ \
      |             | | |
      |             |_| |
   ___|             |\___/
  /    \___________/    \
  \_____________________/
       \___________/
    "#,
];

pub fn sip_coffee() {
    sleep(Duration::from_secs(1)); // Kurze Pause vor dem Start

    // Endlosschleife für die Animation
    loop {
        for frame in COFFEE_FRAMES.iter() {
            // Bildschirm löschen und Cursor zurücksetzen
            print!("\x1B[2J\x1B[H");
            io::stdout().flush().unwrap();

            // Frame anzeigen
            println!("{}", frame);

            // Warten für die Animation
            sleep(Duration::from_millis(400)); // 400ms pro Frame
        }
    }
}