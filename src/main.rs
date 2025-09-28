mod ai;
mod daemon;
mod envs;
mod prompt;
mod utils;

use std::process;

// TODO: Automatisches Starten des Daemons implementieren
// TODO: Konfig für kailian überarbeiten und nurnoch einen api wert / ip adresse haben (endpoints werden von ollama-rs definiert)
// TODO: /api/ps in ollama-rs vorhanden? -> Wenn nicht von Hand implementieren
// TODO: --self-destroy -> internal shutdown(flag)
// TODO: Markdown in CLI interpretieren
// TODO: Einlesen der confis aus kailian.conf fehlerbeständiger machen
// TODO: Config per user in homedir which overwrites the global configuration
// TODO: markdown parser
// In Progress
// TODO: variable for how to long to keep model aliave
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
    kailian_parameters.fill_config_variables()?;

    // prompt makes all the logic of building the new prompt and sends it to the ai
    prompt::stdin::read_stdin_flags(&kailian_parameters).await?;

    Ok(())
}
