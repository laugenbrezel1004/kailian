use crate::envs::core::ConfigVariables;
use ollama_rs::Ollama;



pub async fn list_models(env_variables: &ConfigVariables) -> Result<(), String> {
    let ollama = Ollama::new(env_variables.kailian_endpoint.clone(), 11434);
    let models = ollama
        .list_local_models()
        .await
        .map_err(|e| format!("Failed to fetch local models: {}", e))?;

    for model in models {
        println!("Model: {}", model.name)
    } 

    Ok(())
}