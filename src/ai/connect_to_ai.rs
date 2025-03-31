use ollama_rs::generation::completion::request::GenerationRequest;
use ollama_rs::Ollama;
use tokio::io::{self, AsyncWriteExt};
use tokio::task;
use tokio_stream::StreamExt;
use tokio::time::{self, Duration};
use std::sync::Arc;
use tokio::sync::Mutex;

pub async fn api_call(prompt: &String) {
    let model = "deepseek-r1:14b".to_string();
    let prompt = prompt.to_string();
    let ollama = Ollama::default();

    // Erstelle einen Flag, um den Spinner zu steuern
    let spinner_running = Arc::new(Mutex::new(true));
    let spinner_running_clone = Arc::clone(&spinner_running);

    // Starte den Spinner in einem separaten Task
    let spinner_handle = task::spawn(async move {
        let spinner_chars = ['⠋', '⠙', '⠹', '⠸', '⠼', '⠴', '⠦', '⠧', '⠇', '⠏'];
        let mut index = 0;
        let mut stdout = io::stdout();

        while *spinner_running_clone.lock().await {
            stdout
                .write_all(format!("\r{} Verbinden...", spinner_chars[index]).as_bytes())
                .await
                .unwrap();
            stdout.flush().await.unwrap();
            index = (index + 1) % spinner_chars.len();
            time::sleep(Duration::from_millis(100)).await;
        }

        // Lösche den Spinner
        stdout.write_all(b"\r\x1B[K").await.unwrap();
        stdout.flush().await.unwrap();
    });

    // API-Stream-Anfrage
    let mut stream = ollama
        .generate_stream(GenerationRequest::new(model, prompt))
        .await
        .unwrap();

    let mut stdout = io::stdout();
    while let Some(res) = stream.next().await {
        let responses = res.unwrap();
        for resp in responses {
            // Stoppe den Spinner beim ersten Response
            if *spinner_running.lock().await {
                *spinner_running.lock().await = false;
            }
            stdout.write_all(resp.response.as_bytes()).await.unwrap();
            stdout.flush().await.unwrap();
        }
    }

    // Stelle sicher, dass der Spinner gestoppt wird
    *spinner_running.lock().await = false;
    spinner_handle.await.unwrap();
}