use std::io::{self, BufRead, IsTerminal};
use std::env;
use std::process;
pub fn read_stdin() -> String {
    let argv: Vec<String> = env::args().collect();

    if argv.len() < 2 {
        eprintln!("Not enough arguments supplied");
        process::exit(1);
    }
//auf flags achten!!!
    let mut prompt = String::new();
    for part in &argv[1..] {
        prompt.push_str(part);
        prompt.push_str(" ");
    }
    prompt = prompt.to_string();

    let mut stdin_buffer: String = String::new();

    if !io::stdin().is_terminal() {
        let stdin = io::stdin();
        for line in stdin.lock().lines() {
            match line {
                Ok(line) => {
                    stdin_buffer.push_str(&line);
                    stdin_buffer.push('\n');
                }
                Err(e) => {
                    eprintln!("Fehler beim Lesen von stdin: {}", e);
                    process::exit(1);
                }
            }
        }
    }

    if !stdin_buffer.is_empty() {
        prompt.push_str(&stdin_buffer);
//        println!("stdin input -> {}", prompt);
    }

    prompt
}