mod ai;
mod coffee;
mod envs;
mod prompt;
use envs::impls;
mod daemon;

use std::process;

// TODO: Automatisches Starten des Daemons implementieren
// TODO: Konfig für kailian überarbeiten und nurnoch einen api wert / ip adresse haben (endpoints werden von ollama-rs definiert)
// TODO: /api/ps in ollama-rs vorhanden? -> Wenn nicht von Hand implementieren
// TODO: --self-destroy -> internal shutdown(flag)
// TODO: Markdown in CLI interpretieren
// TODO: Logging für Ollamad
// TODO: Einlesen der confis aus kailian.conf fehlerbeständiger machen
// TODO: markdown parser
// TODO: List local mocdel besser gestalten

// In Progress
// TODO: variable for how to long to keep model aliave
// TODO: Code refactoren
#[tokio::main]
async fn main() {
    if let Err(e) = run().await {
        eprintln!("kailian: {}", e);
        process::exit(1);
    }
}

async fn run() -> Result<(), String> {
    let mut kailian_parameters = envs::core::new();

    // read config file and env vars
    if let Err(e) = kailian_parameters.fill_config_variables() {
        return Err(e);
    }

    
    
    // prompt makes all the logic of building the new prompt and sends it to the ai
    let prompt = prompt::core::new();
    if let Err(e) = prompt::read_stdin(&kailian_parameters).await {
        return Err(e);
    };
    Ok(())
}
