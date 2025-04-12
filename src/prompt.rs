use crate::{ai, envs};
use crate::envs::EnvVariables;
use crate::coffee;
use clap::{Arg, Command};
use std::env;
use std::io::{self, BufRead, IsTerminal};
use crate::daemon::{daemonize_ollama, kill_ollama_daemon};

pub async fn read_stdin(env_vars: &EnvVariables) -> Result<(), String> {
    let matches = Command::new("kailian")
        .version("1.1.0")
        .author("Laurenz Schmidt")
        .about("A simple, yet powerful CLI wrapper for ollama")
        // Option für den Prompt mit -a/--ask, erwartet einen Wert
        .after_help(
            "ENVIRONMENT VARIABLES:\n\
                     KAILIAN_MODEL    Overwrites the model set in the config file"
        )
        .arg(
            Arg::new("ask")
                .short('a')
                .long("ask")
                .value_name("YourQuestion")
                .help("Ask ollama a question (quote if using wildcards)")
                .num_args(1..),
        )
        .arg(
            Arg::new("create_config")
                .short('c')
                .long("create-config")
                .help("Create a new default config file (overwrites existing")
                .action(clap::ArgAction::SetTrue),
        )
        .arg(
            Arg::new("show_config")
                .short('s')
                .long("show-config")
                .help("Show the current config file")
                .action(clap::ArgAction::SetTrue),
        )
        .arg(
            Arg::new("coffee")
                .short('f')
                .long("coffee")
                .help("Let's sip some virtual coffee")
                .action(clap::ArgAction::SetTrue),
        )
        .arg(
            Arg::new("list_models")
                .short('l')
                .long("list-models")
                .help("Show all available AI models")
                .action(clap::ArgAction::SetTrue),
        )
        .arg(
            Arg::new("running_model")
                .short('r')
                .long("running-model")
                .help("Show the currently running AI model (or none if no model is loaded")
                .action(clap::ArgAction::SetTrue),
        )
        .arg(
            Arg::new("start_ollama")
                .short('t')
                .long("start-ollama")
                .help("Start a new local ollama instance")
                .action(clap::ArgAction::SetTrue),
        )
        .arg(
            Arg::new("kill_ollama")
                .short('k')
                .long("kill-ollama")
                .help("Kill the local running ollama instance")
                .action(clap::ArgAction::SetTrue),
        )
        // Wichtig: Mindestens eine Option erforderlich
        .arg_required_else_help(true)
        .get_matches_from(env::args().collect::<Vec<String>>());

    // Argumente auswerten
    if matches.get_flag("create_config") {
        return envs::create_config();
    }
    if matches.get_flag("show_config") {
        println!("{}", &env_vars);
        return Ok(());
    }
    if matches.get_flag("coffee") {
        //wird von user mit ^c beendet
        coffee::sip_coffee();
        unreachable!("Something went wrong");
    }
    if matches.get_flag("list_models") {
        return ai::list_local_models::list_models(&env_vars).await;
    }
    if matches.get_flag("running_model") {
        return ai::get_running_model::running_model(&env_vars);
    }
    if matches.get_flag("start_ollama") {
        return daemonize_ollama();
    }
    if matches.get_flag("kill_ollama") {
        return kill_ollama_daemon();
    }
    if matches.contains_id("ask") {
        return build_prompt(&env_vars).await;
    }

    // Sollte nie erreicht werden wegen args_required_else_help
    unreachable!("No arguments provided, but Clap should have caught this");
}


//später Result wieder implementieren 
async fn build_prompt(env_variables: &EnvVariables) -> Result<(), String> {
    let mut prompt = String::new();
    let argv: Vec<String> = env::args().collect();

    #[cfg(debug_assertions)]
    println!("argv -> {:?}", argv);

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
                    return Err(e.to_string())
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

    return ai::connect_to_ai::api_completion_generation(&prompt, &env_variables).await;
}
