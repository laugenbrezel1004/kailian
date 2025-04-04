use clap::{Arg, Command};
use std::env;
use std::io::{self, BufRead, IsTerminal};
use std::process;
use crate::coffee;

pub fn read_stdin() -> Option<String> {
    let matches = Command::new("kailian")
        .version("0.1.0")
        .author("Laurenz Schmidt")
        .about("A simple, yet powerful CLI wrapper for ollama")
        // Option für den Prompt mit -a/--ask
        .arg(
            Arg::new("ask")
                .short('a')
                .long("ask")
                .help("Ask the mighty ollama a question")
                .action(clap::ArgAction::SetTrue),
        )
        // Weitere Optionen
        .arg(
            Arg::new("create_config")
                .short('c')
                .long("create-config")
                .help("Create a new default config file")
                .action(clap::ArgAction::SetTrue),
        )
        .arg(
            Arg::new("show_config")
                .short('s')
                .long("show-config")
                .help("Show the config file")
                .action(clap::ArgAction::SetTrue),
        )
        .arg(
            Arg::new("coffee")
                .short('C')
                .long("coffee")
                .help("Let's sip some virtual coffee")
                .action(clap::ArgAction::SetTrue),
        )
        .arg(
            Arg::new("list_models")
                .short('l')
                .long("list-models")
                .help("Show all available ai-models")
                .action(clap::ArgAction::SetTrue),
        )
        .arg(
            Arg::new("running_model")
                .short('r')
                .long("running-model")
                .help("Show the running ai-model")
                .action(clap::ArgAction::SetTrue),
        )
        .arg(
            Arg::new("start_ollama")
                .short('S')
                .long("start-ollama")
                .help("Start a new local ollama instance")
                .action(clap::ArgAction::SetTrue),
        )
        .arg(
            Arg::new("kill_ollama")
                .short('k')
                .long("kill-ollama")
                .help("Kill a running ollama instance")
                .action(clap::ArgAction::SetTrue),
        )
        .get_matches_from(env::args().collect::<Vec<String>>());

    // Argumente auswerten
    if matches.get_flag("create_config") {
        todo!("Create Configuration");
    }
    if matches.get_flag("show_config") {
        todo!("Show Configuration");
    }
    if matches.get_flag("coffee") {
        coffee::sip_coffee();
    }
    if matches.get_flag("list_models") {
        todo!("list available models");
    }
    if matches.get_flag("running_model") {
        todo!("Show running model");
    }
    if matches.get_flag("start_ollama") {
        todo!("Start new ollama instance as a daemon");
    }
    if matches.get_flag("kill_ollama") {
        todo!("Kill ollama instance");
    }
    if matches.get_flag("ask") {
        return build_prompt();
    }
    None
    // TODO: Rückgabe
}
  

fn build_prompt() -> Option<String> {

    
    let argv: Vec<String> = env::args().collect();
    if argv.len() < 3 {
        // kailian -a hi -> 3 arguments at least
       eprintln!("Too fex arguemnts\n Try --help for more info");
        process::exit(1);
    }
    
    let mut stdin_buffer = String::new();
    let mut prompt = String::new();
    // wenn input von z. B. Pipe
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
    }
    Some(prompt)
}
