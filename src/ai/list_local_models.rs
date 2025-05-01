use ollama_rs::Ollama;
use crate::envs::ConfigVariables;

pub async fn list_models(env_variables: &ConfigVariables) -> Result<(), String>{
    // Initialize the Ollama instance with the given environment variable and port
    let ollama = Ollama::new(env_variables.kailian_endpoint.to_string(), 11434);

    // Attempt to list local models asynchronously
    match ollama.list_local_models().await {
        Ok(models) => {
            println!("Available models: {:?}", models);
        }
        Err(e) => {
            return Err(e.to_string());
        }
    }

    Ok(())
}