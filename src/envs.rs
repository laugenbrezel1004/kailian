#![allow(unused_imports)]
use std::{env, fs};

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
        let content = fs::read_to_string(KAILIAN_CONF_PATH)
            .map_err(|_| format!("Error: Could not open config file '{}'", KAILIAN_CONF_PATH))?;

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


        // TODO: ENVS einlesen
        match env::var("KAILIAN_MODEL") {
           Ok(value) => variables.kailian_model = value,
            Err(_) => (),
        }
        Ok(variables)
    }
}
