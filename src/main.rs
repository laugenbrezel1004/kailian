mod ai;
mod prompt;
pub mod envs;

use std::process::exit;
use ai::connect_to_ai;
// TODO: help seite schreiben

#[tokio::main]
async fn main() {
    // Lesen des Prompts aus der Standardeingabe
    let prompt: String = prompt::read_stdin();

    // Initialisierung der Umgebungsvariablen
    // kann von jeder funktion global benutzt werden 
    // TODO: Port extra angeben, für Ollama::new() oder port aus vorhanden Wert auslesen
    let kailian_env = envs::EnvVariables::new();

    // Überprüfung des Ergebnisses von EnvVariables::new()
    let env_vars = match kailian_env {
        Ok(vars) => vars,
        Err(err) => {
            eprintln!("{}", err);
            exit(1); // Beendet das Programm im Fehlerfall
        }
    };
    #[cfg(debug_assertions)]
    println!("Vars -> {:?}", env_vars);

    // TODO: Automatisches Starten des Daemons implementieren
    #[cfg(debug_assertions)]
    println!("Kein Autodaemon vorhanden -> Bitte Ollama manuell starten.");

    // Aufruf der API zur Generierung
    // TODO: Implement Result<(), String> for better Errorhandeling
     connect_to_ai::api_completion_generation(&prompt, &env_vars).await;
     

    // TODO: Chat-Modus implementieren
    // connect_to_ai::api_chat_mode(&prompt).await;
}
