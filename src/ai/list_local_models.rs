use crate::envs::core::ConfigVariables;
use ollama_rs::Ollama;

pub async fn list_models(env_variables: &ConfigVariables) -> Result<(), String> {

        // List local models with streamlined error handling
        // impl Display Trait for future
        println!("Available models: {:?}",
    Ollama::new(env_variables.kailian_endpoint.clone(), 11434)
        .list_local_models()
        .await
        .map_err(|e| format!("Failed to fetch local models: {}", e))?
);
    Ok(())
}
