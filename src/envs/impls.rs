use crate::envs::core::{ConfigVariables, KAILIAN_CONF_PATH};
use std::env;
use std::fmt;
use std::fmt::Formatter;
use std::fs;

impl fmt::Display for ConfigVariables {
    fn fmt(&self, f: &mut Formatter<'_>) -> fmt::Result {
        write!(
            f,
            "Model: {}\nEndpoint: {}\nSystem prompt: {}",
            self.kailian_model, self.kailian_endpoint, self.kailian_system
        )
    }
}

impl ConfigVariables {
    pub fn fill_config_variables(&mut self) -> Result<(), String> {
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
                    "name" => self.kailian_model = value.to_string(),
                    "endpoint" => self.kailian_endpoint = value.to_string(),
                    "system" => self.kailian_system = value.to_string(),
                    "alive" => self.kailian_alive = value.to_string(),
                    _ => eprintln!("kailian: Unknown key {}", key), // Logging statt Ignorieren
                }
            } else {
                return Err(format!("Invalid line: {}", line));
            }
        }

        // Rufe die Methode check_env_vars auf
        self.check_env_vars();
        Ok(())
    }

    fn check_env_vars(&mut self) {
        let env_vars = ["KAILIAN_MODEL", "KAILIAN_ENDPOINT", "KAILIAN_ALIVE"];
        for var in env_vars {
            if let Ok(value) = env::var(var) {
                match var {
                    "KAILIAN_MODEL" => self.kailian_model = value,
                    "KAILIAN_ENDPOINT" => self.kailian_endpoint = value,
                    "KAILIAN_ALIVE" => self.kailian_alive = value,
                    _ => {}, // Unerreichbar, aber für Vollständigkeit
                }
            }
        }
    }
}