use ollama_rs::generation::completion::request::GenerationRequest;
use ollama_rs::Ollama;
use tokio::io::{self, AsyncWriteExt};
use tokio_stream::StreamExt;

pub async fn api_call() {

    let model = "deepseek-r1:14b".to_string();
    let prompt = "Why is the sky blue?".to_string();
    //let ollama = Ollama::new("http://10.0.0.4".to_string(), 11434);
    let ollama = Ollama::default();

    let mut stream = ollama.generate_stream(GenerationRequest::new(model, prompt)).await.unwrap();

    let mut stdout = io::stdout();
    while let Some(res) = stream.next().await {
        let responses = res.unwrap();
        for resp in responses {
            stdout.write_all(resp.response.as_bytes()).await.unwrap();
            stdout.flush().await.unwrap();
        }
    }
}