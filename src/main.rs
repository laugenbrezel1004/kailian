mod ai;
mod coffee;
mod envs;
mod prompt;
mod daemon;

use std::process;
use std

// TODO: Automatisches Starten des Daemons implementieren
// TODO: Chat-Modus implementierenn
// TODO: Konfig für kailian überarbeiten und nurnoch einen api wert / ip adresse haben (endpoints werden von ollama-rs definiert)
// TODO: /api/ps in ollama-rs vorhanden? -> Wenn nicht von Hand implementieren 
// TODO: --self-destroy -> internal shutdown(flag) 
// TODO: Markdown in CLI interpretieren 
// TODO: Logging für Ollamad
// TODO: Einlesen der confis aus kailian.conf fehlerbeständiger machen
// TODO: markdown parser
#[tokio::main]
async fn main() {
   
    
    // /etc/kailian/kailain.conf und umgebungsvaraiblen auslesen 
    let kailian_env = envs::EnvVariables::new();
    let kailian_env = match kailian_env {
        Ok(vars) => vars,
        Err(err) => {
            eprintln!("Error: {}", err);
            process::exit(1);
        }
    };
    
    #[cfg(debug_assertions)]
    println!("Vars ->\n {:?}\n", kailian_env);

    // prompt macht hier die ganze logik
    let result = prompt::read_stdin(&kailian_env).await;
    if let Err(e) = result {
        eprintln!("Error: {}", e);
        process::exit(1);
    }
}
