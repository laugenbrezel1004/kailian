use std::{env, fs};
use std::env::var;
use std::fmt;
use std::fmt::Formatter;

pub const KAILIAN_CONF_PATH: &str = "/etc/kailian/kailian.conf";

#[derive(Debug)]
pub struct ConfigVariables {
    pub kailian_model: String,
    pub kailian_endpoint: String, 
    pub kailian_system: String,
}

impl fmt::Display for ConfigVariables {
    fn fmt(&self, f: &mut Formatter<'_>) -> fmt::Result {
        write!(f, "Model: {0}\nEndpoint: {1}\nSystem prompt: {2}", self.kailian_model, self.kailian_endpoint, self.kailian_system)
    }
}
impl ConfigVariables {
    pub fn new() -> Result<ConfigVariables, String> {

        // Default-Struct erzeugen
        let mut variables = ConfigVariables {
            kailian_model: String::new(),
            kailian_endpoint: String::new(),
            kailian_system: String::new(),
        };

        // Config-Datei einlesen
        let content = fs::read_to_string(KAILIAN_CONF_PATH).map_err(|err| err.to_string())?;


        for line in content.lines() {
            let line = line.trim();
            if line.is_empty() || line.starts_with('#') {
                continue; // Ignoriere leere Zeilen und Kommentare
            }
            if let Some((key, value)) = line.split_once('=') {
                let key = key.trim();
                let value = value.trim();
                match key {
                    "name" => variables.kailian_model = value.to_string(),
                    "endpoint" => variables.kailian_endpoint = value.to_string(),
                    "system" => variables.kailian_system = value.to_string(),
                    _ => eprintln!("Unkown key {}", key), // Logging statt Ignorieren
                }
            } else {
                return Err("Invalid line {}".to_string());
            }
        }

        // Check for Env-Variables to overwrite kailian.conf parameters
        if let Ok(value) = env::var("KAILIAN_MODEL") {
            variables.kailian_model = value;
        }
        if let Ok(value) = env::var("KAILIAN_ENDPOINT") {
            variables.kailian_endpoint = value;
        }
        Ok(variables)
    }
}
pub fn create_config() -> Result<(), String> {
    std::fs::write(KAILIAN_CONF_PATH, "name = gemma3:27b
endpoint = http://localhost:11434
system = \"You are a highly efficient systems AI built to assist with programming, log analysis, and Linux commands. Provide short, precise answers. Excel at writing and debugging code, analyzing system logs for errors or patterns, and delivering accurate Linux command solutions. Use your expertise to optimize systems and troubleshoot issues quickly.\"
").map_err(|e| e.to_string())?;
    Ok(())
}
