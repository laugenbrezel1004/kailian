use std::env;
use std::io::IsTerminal;
use std::{io, process::exit};

pub fn read_stdin() -> String {
    let argv: Vec<String> = env::args().collect();
    if argv.len() < 3 {
        eprintln!("Not enough arguments supplied");
        exit(1);
    }
    //validierung der flags
    //stdin ggf einlesen.

    let mut prompt = String::new();
    for part in &argv[1..] {
        prompt.push_str(part);
        prompt.push_str(" ");
    }

    //if input goes over pipe and not terminal append the
    //piped input to the prompt
    let mut buffer: String = String::new();
    if !io::stdin().is_terminal() {
        io::stdin().read_line(&mut buffer).unwrap();
        prompt.push_str(&buffer);
    }

    prompt
}
