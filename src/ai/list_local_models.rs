
use ollama_rs::Ollama;
use crate::envs::EnvVariables;

pub async fn list_models(kailian_variables: &EnvVariables) {
   
    let ollama = Ollama::new(kailian_variables.kailian_generate.to_string(), 11434);
    let res = ollama.list_local_models().await.unwrap();
    println!("res -> {:?}", res);
}