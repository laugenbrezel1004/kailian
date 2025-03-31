use std::env;

pub struct EnvVariables {
    pub kailian_model: String,
}

impl EnvVariables {
    pub fn read() -> EnvVariables {
        let kailian_model = env::var("KAILIAN_MODEL").unwrap_or_else(|_| "default_model".to_string());
        println!("variable {}", kailian_model);
        EnvVariables {
            kailian_model,
        }
    }
}