use ollama_rs::Ollama;
use crate::envs::core::ConfigVariables;

pub async fn list_models(env_variables: &ConfigVariables) -> Result<(), String> {
    let ollama = Ollama::new(
        env_variables.kailian_endpoint.clone(),
        11434,
    );

    // List local models with streamlined error handling
    let models = ollama
        .list_local_models()
        .await
        .map_err(|e| format!("Failed to fetch local models: {}", e))?;

    println!("Available models: {:?}", models);
    Ok(())
}