use std::io::{self, BufRead, IsTerminal};
use clap::{Arg, Command};
use std::env;
use std::process;
use crate::coffee;

pub fn read_stdin() -> String {
    let matches = Command::new("kailian")
        .version("0.1.0")
        .author("Laurenz Schmdit")
        .about("A simple, yet powerful CLI wrapper for ollama")
        .arg(
            Arg::new("create_config")
                .short('c')
                .long("create-config")
                .help("Create a new default config file")
                .action(clap::ArgAction::SetTrue), // Flag (true/false)
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
        .get_matches();

    // Argumente auswerten
    if matches.get_flag("create_config") {
        println!("Erstelle eine neue Standard-Konfigurationsdatei...");
    }
    if matches.get_flag("show_config") {
        println!("Zeige die Konfigurationsdatei...");
    }
    if matches.get_flag("coffee") {
        coffee::sip_coffee();
    }
    if matches.get_flag("list_models") {
        println!("Zeige alle verfügbaren AI-Modelle...");
    }
    if matches.get_flag("running_model") {
        println!("Zeige das aktuell laufende AI-Modell...");
    }
    if matches.get_flag("start_ollama") {
        println!("Starte eine neue lokale Ollama-Instanz...");
    }
    if matches.get_flag("kill_ollama") {
        println!("Beende eine laufende Ollama-Instanz...");
    }

    // Prüfen, ob überhaupt Argumente übergeben wurden
    let argv: Vec<String> = env::args().collect();
    if argv.len() < 2 && matches.subcommand().is_none() && !matches.args_present() {
        eprintln!("Not enough arguments supplied. Use --help for more information.");
        process::exit(1);
    }

    let mut prompt = String::new();
    for part in &argv[1..] {
        prompt.push_str(part);
        prompt.push_str(" ");
    }
    prompt = prompt.trim().to_string();

    let mut stdin_buffer = String::new();
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

    prompt
}