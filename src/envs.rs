use std::{env, fs};
use std::fmt;
use std::fmt::Formatter;

pub const KAILIAN_CONF_PATH: &str = "/etc/kailian/kailian.conf";

#[derive(Debug)]
pub struct EnvVariables {
    pub kailian_model: String,
    pub kailian_generate: String,
    pub kailian_info: String,
    pub kailian_running_model: String,
    pub kailian_ollama_version: String,
    pub kailian_chat: String,
    pub kailian_show: String,
    pub kailian_system: String,
}

impl fmt::Display for EnvVariables {
    fn fmt(&self, f: &mut Formatter<'_>) -> fmt::Result {
        write!(f, "Model: {}\n", self.kailian_model)
    }
}
impl EnvVariables {
    pub fn new() -> Result<EnvVariables, String> {

        // Default-Struct erzeugen
        let mut variables = EnvVariables {
            kailian_model: String::new(),
            kailian_generate: String::new(),
            kailian_info: String::new(),
            kailian_running_model: String::new(),
            kailian_ollama_version: String::new(),
            kailian_chat: String::new(),
            kailian_show: String::new(),
            kailian_system: String::new(),
        };

        // Config-Datei einlesen
        let content = fs::read_to_string(KAILIAN_CONF_PATH).map_err(|err| err.to_string())?;


        // Inhalt verarbeiten 
        // Mögliche Fehlerquellen elimieren
        for line in content.lines() {
            if let Some((key, value)) = line.split_once('=') {
                match key.trim() {
                    "name" => variables.kailian_model = value.trim().to_string(),
                    "endpoint_generate" => variables.kailian_generate = value.trim().to_string(),
                    "endpoint_info" => variables.kailian_info = value.trim().to_string(),
                    "endpoint_running_model" => variables.kailian_running_model = value.trim().to_string(),
                    "endpoint_ollama_version" => variables.kailian_ollama_version = value.trim().to_string(),
                    "endpoint_chat" => variables.kailian_chat = value.trim().to_string(),
                    "endpoint_show" => variables.kailian_show = value.trim().to_string(),
                    "system" => variables.kailian_system = value.trim().to_string(),
                    _ => {} // Unbekannte Schlüssel ignorieren
                    // TODO: Fehler!!!! wenn kein schlüssel
                }
            }
        }

        if let Ok(value) = env::var("KAILIAN_MODEL") {
            variables.kailian_model = value;
        }
        Ok(variables)
    }
}
pub fn create_config() -> Result<(), String> {
    std::fs::write(KAILIAN_CONF_PATH, "name = gemma3:27b
endpoint_generate = http://localhost:11434
endpoint_info = http://localhost:11434/api/tags
endpoint_running_model = http://localhost:11434/api/ps
endpoint_ollama_version = http://localhost:11434/api/version
endpoint_chat = http://localhost:11434/api/chat
endpoint_show = http://localhost:11434/api/show
system = \"You are a highly efficient systems AI built to assist with programming, log analysis, and Linux commands. Provide short, precise answers. Excel at writing and debugging code, analyzing system logs for errors or patterns, and delivering accurate Linux command solutions. Use your expertise to optimize systems and troubleshoot issues quickly.\"
").map_err(|e| e.to_string())?;
    Ok(())
}
