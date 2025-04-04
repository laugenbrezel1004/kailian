use clap::{Arg, Command};
use std::env;
use std::io::{self, BufRead, IsTerminal};
use crate::coffee;
use crate::ai;

pub fn read_stdin() -> Result<String, String> {
    let matches = Command::new("kailian")
        .version("0.1.0")
        .author("Laurenz Schmidt")
        .about("A simple, yet powerful CLI wrapper for ollama")
        // Option für den Prompt mit -a/--ask, erwartet einen Wert
        .arg(
            Arg::new("ask")
                .short('a')
                .long("ask")
                .help("Ask the mighty ollama a question")
                .num_args(0..)
            //.action(clap::ArgAction::SetTrue)
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
        // Wichtig: Mindestens eine Option erforderlich
        .arg_required_else_help(true)
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
        ai::list_local_models::list_models();
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
    if matches.contains_id("ask") {
        return build_prompt();
    }

    // Sollte nie erreicht werden wegen args_required_else_help
    unreachable!("No arguments provided, but Clap should have caught this");
}

fn build_prompt() -> Result<String, String> {
    let mut prompt = String::new();
    let argv: Vec<String> = env::args().collect();
   
    #[cfg(debug_assertions)]
    println!("argv -> {:?}", argv);

    // Überprüfen, ob mindestens 3 Argumente vorhanden sind
    if argv.len() < 3 {
        // Beispiel: kailian -a hi -> 3 args (Programmname + Flag + Wert)
        return Err("No prompt provided after -a/--ask\nTry --help for more info".to_string());
    }

    // Verarbeite alle Argumente ab Index 2 (ignoriere Programmname und Flag)
    let args_prompt = argv[2..].join(" ");
    prompt.push_str(&args_prompt);

    // STDIN verarbeiten
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
                    return Err(format!("Fehler beim Lesen von stdin: {}", e));
                }
            }
        }
    }

    // Kombiniere STDIN mit Argumenten, falls vorhanden
    if !stdin_buffer.is_empty() {
        prompt.push_str(&stdin_buffer);
    }

    // Entferne überflüssige Leerzeichen und überprüfe, ob der Prompt leer ist
    prompt = prompt.trim().to_string();
  

    Ok(prompt)
}
