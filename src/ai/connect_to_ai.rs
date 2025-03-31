use ollama_rs::generation::completion::request::GenerationRequest;
use ollama_rs::Ollama;
use tokio::io::{self, AsyncWriteExt};
use tokio::task;
use tokio_stream::StreamExt;
use tokio::time::{self, Duration};
use std::sync::Arc;
use tokio::sync::Mutex;
use crate::kailian_env::EnvVariables; // Nur diese Zeile ist nötig

pub async fn api_call(prompt: &String) {
    let env_variables = EnvVariables::read(); // Korrigierter Variablenname
    let model = env_variables.kailian_model; // Verwende den Wert aus EnvVariables
    let prompt = prompt.to_string();
    let ollama = Ollama::default();

    let spinner_running = Arc::new(Mutex::new(true));
    let spinner_running_clone = Arc::clone(&spinner_running);

    let spinner_handle = task::spawn(async move {
        let spinner_chars = ['⠋', '⠙', '⠹', '⠸', '⠼', '⠴', '⠦', '⠧', '⠇', '⠏'];
        let mut index = 0;
        let mut stdout = io::stdout();

        while *spinner_running_clone.lock().await {
            stdout
                .write_all(format!("\r{}", spinner_chars[index]).as_bytes())
                .await
                .unwrap();
            stdout.flush().await.unwrap();
            index = (index + 1) % spinner_chars.len();
            time::sleep(Duration::from_millis(100)).await;
        }

        stdout.write_all(b"\r\x1B[K").await.unwrap();
        stdout.flush().await.unwrap();
    });

    let mut stream = ollama
        .generate_stream(GenerationRequest::new(model, prompt))
        .await
        .unwrap();

    let mut stdout = io::stdout();
    while let Some(res) = stream.next().await {
        let responses = res.unwrap();
        for resp in responses {
            if *spinner_running.lock().await {
                *spinner_running.lock().await = false;
            }
            stdout.write_all(resp.response.as_bytes()).await.unwrap();
            stdout.flush().await.unwrap();
        }
    }

    *spinner_running.lock().await = false;
    spinner_handle.await.unwrap();
}