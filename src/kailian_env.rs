use std::env;

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
    pub fn read() -> EnvVariables {
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
        println!("variable {kailain_envs:?}" );
        return kailain_envs;
    }
}