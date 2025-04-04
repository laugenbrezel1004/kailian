mod ai;
mod prompt;
mod envs;
mod coffee;

use std::process;
use ai::connect_to_ai;

// Generelle TODO:'S
// TODO: Automatisches Starten des Daemons implementiere
// TODO: Chat-Modus implementierenn
#[tokio::main]
async fn main() {

    // Initialisierung der Umgebungsvariablen
    // TODO: Port extra angeben, für Ollama::new() oder port aus vorhanden Wert auslesen
    // Überprüfung des Ergebnisses von EnvVariables::new()
    let kailian_env = envs::EnvVariables::new();
    let env_vars = match kailian_env {
        Ok(vars) => vars,
        Err(err) => {
            eprintln!("Error: {}", err);
            process::exit(1);
        }
    };
    #[cfg(debug_assertions)]
    println!("Vars -> {:?}", env_vars);


    // Validierung des Prompts und der Flags
    // und aufrufen der jeweiligen funktionnen
    let prompt_result = prompt::read_stdin();
    let prompt = match prompt_result {
        Ok(value) => value,
        Err(error) => {
            eprintln!("Error: {}", error);
            process::exit(1); // Beende das Programm im Fehlerfall
        }
    };

    #[cfg(debug_assertions)]
    print!("Prompt -> {}\n", prompt);




    #[cfg(debug_assertions)]
    println!("Kein Autodaemon vorhanden -> Bitte Ollama manuell starten.");

    // Aufruf der API zur Generierung
    // TODO: Implement Result<(), String> for better Errorhandeling
    connect_to_ai::api_completion_generation(&prompt, &env_vars).await;



    // connect_to_ai::api_chat_mode(&prompt).await;
}
