use std::{env, io};
use std::io::{BufRead, IsTerminal};

pub struct Prompt {
    data: String,
}

pub async fn new() -> Result<Prompt, String> {
    let mut prompt = Prompt {
        data: String::new(),
    };

    let argv: Vec<String> = env::args().collect();

    let args_prompt = argv[2..].join(" ");
    prompt.data.push_str(&args_prompt);

    let mut stdin_buffer = String::new();
    if !io::stdin().is_terminal() {
        let stdin = io::stdin();
        for line in stdin.lock().lines() {
            match line {
                Ok(line) => {
                    stdin_buffer.push_str(&line);
                    stdin_buffer.push('\n');
                }
                Err(e) => return Err(e.to_string()),
            }
        }
    }

    // Kombiniere STDIN mit Argumenten, falls vorhanden
    //nochmal genauer angucken
    if !stdin_buffer.is_empty() {
        prompt.data.push_str(&stdin_buffer);
    }

    prompt.data = prompt.data.trim().to_string();
    Ok(prompt)
}
