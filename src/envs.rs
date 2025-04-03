use std::{env, fs};
pub const KAILIAN_CONF_PATH: &str = "/etc/kailian/kailian.conf";


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
    pub fn new() -> EnvVariables {
        let content = fs::read_to_string(KAILIAN_CONF_PATH).expect("unable to read config file");

        for line in content.lines() {
            if line.trim().is_empty() || line.trim().starts_with('#') {
                continue;
            }

            if let Some((key, value)) = line.split_once('=') {
                let key = key.trim();
                let value = value.trim().trim_matches('"');

                match key {
                    key if key == "content" => self.kailian_model = value.to_string(),
                    key if key == "endpoint_generate" => self.kailian_generate = value.to_string(),
                    key if key == "endpoint_info" => self.kailian_info = value.to_string(),
                    key if key == "endpoint_running_model" => self.kailian_running_model = value.to_string(),
                    key if key == "endpoint_ollama_version" => self.kailian_ollama_version = value.to_string(),
                    key if key == "endpoint_chat" => self.kailian_chat = value.to_string(),
                    key if key == "endpoint_show" => self.kailian_show = value.to_string(),
                    key if key == "system" => self.kailian_system = value.to_string(),
                    _ => eprintln!("Unbekannter Konfigurationskey: {}", key),
                }
            }
        }
        
    }
    pub fn read_env(&mut self) {
        let kailain_envs = EnvVariables {
            kailian_model: env::var("KAILIAN_MODEL").unwrap_or_else(|_| "default_model".to_string()),
            kailian_generate: env::var("KAILIAN_GENERATE").unwrap_or_else(|_| "default_model".to_string()),
            kailian_info: env::var("KAILIAN_INFO").unwrap_or_else(|_| "default_model".to_string()),
            kailian_running_model:
            env::var("KAILIAN_RUNNING_MODEL").unwrap_or_else(|_| "default_model".to_string()),
            kailian_ollama_version:
            env::var("KAILIAN_OLLAMA_VERSION").unwrap_or_else(|_| "default_model".to_string()),
            kailian_chat:
            env::var("KAILIAN_CHAT").unwrap_or_else(|_| "default_model".to_string()),
            kailian_show:
            env::var("KAILIAN_SHOW").unwrap_or_else(|_| "default_model".to_string()),
            kailian_system:
            env::var("KAILIAN_SYSTEM").unwrap_or_else(|_| "default_model".to_string()),
        };
    }
}