use json;
use ollama_rs::Ollama;
use crate::envs::EnvVariables;

pub async fn list_models(env_variables: &EnvVariables) {
    // evtl. noch schöner darstellen
    let ollama = Ollama::new(env_variables.kailian_generate.to_string(), 11434);
    let answer = ollama.list_local_models().await.unwrap();
    for i in &answer {
        print!("{:?}\n", i); 
    }
}