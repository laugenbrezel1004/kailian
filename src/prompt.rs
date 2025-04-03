use std::io::{self, BufRead, IsTerminal};
use clap::{Arg, Command};
use std::env;
use std::process;
pub fn read_stdin() -> String {
    let argv: Vec<String> = env::args().collect();


    // TODO: Auf --help verweisen
    if argv.len() < 2 {
        eprintln!("Not enough arguments supplied");
        process::exit(1);
    }

    let matches = Command::new("kailian")
        .version("0.1.0")
        .author("Laurenz Schmdit")
        .about("A simple, yet powerfull CLI wrapper for ollama")
        .arg(
            Arg::new("create_config")
                .short('c')
                .long("create-config")
                .help("Create a new default config file"),
        )
        .arg(
            Arg::new("show_config")
                .short('s')
                .long("show-config")
                .help("Show the config file"),
        )
        .arg(
            Arg::new("coffee")
                .short('C')
                .long("coffee")
                .help("Let's sip some virtually coffee"),
        )
        .arg(
            Arg::new("list_models")
                .short('l')
                .long("list-models")
                .help("Show all available ai-models"),
        )
        .arg(
            Arg::new("running_model")
                .short('r')
                .long("running-model")
                .help("Show the running ai-model"),
        )
        .arg(
            Arg::new("start_ollama")
                .short('S')
                .long("start-ollama")
                .help("Start a new local ollama instance"),
        )
        .arg(
            Arg::new("kill_ollama")
                .short('k')
                .long("kill-ollama")
                .help("Kill a running ollama instance"),
        )
        .get_matches();

    if matches.get_flag("verbose") {
        println!("Verbose-Modus aktiviert.");
    }

    // if let Some(config_path) = matches.get_one::<String>("config") {
    //    println!("Konfigurationsdatei: {}", config_path);
    // }
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