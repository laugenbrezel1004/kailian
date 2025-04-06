
use ollama_rs::Ollama;
use crate::envs::EnvVariables;

pub async fn list_models() {
        let ollama = Ollama::default();
    //let ollama = Ollama::new(kailian_variables.kailian_generate.to_string(), 11434);
    let res = ollama.list_local_models().await.unwrap();
    //let test = ollama.
    // TODO: JSON parsen
    #[cfg(debug_assertions)]
    println!("res -> {:?}", res);
}