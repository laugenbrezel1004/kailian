use ollama_rs::generation::completion::request::GenerationRequest;
use ollama_rs::Ollama;
use tokio::io::{self, AsyncWriteExt};
use tokio::task;
use tokio_stream::StreamExt;
use tokio::time::{self, sleep, Duration};
use std::sync::Arc;
use tokio::sync::Mutex;
use crate::envs::EnvVariables;

pub async fn api_completion_generation(prompt: &String, kailian_variables: &EnvVariables) -> Result<(), String> {
    let prompt = prompt.to_string();
    let ollama = Ollama::new(kailian_variables.kailian_endpoint.to_string(), 11434);

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

    #[cfg(debug_assertions)]
    println!("Sending request to Ollama...");
    
    let mut stream = match ollama.generate_stream(GenerationRequest::new(kailian_variables.kailian_model.to_string(), prompt.clone())).await {
        Ok(stream) => stream,
        Err(e) => {
            *spinner_running.lock().await = false;
            spinner_handle.await.unwrap();
            return Err(e.to_string());
        }
    };



    let mut stdout = io::stdout();
    let mut is_first_message = true;

    while let Some(res) = stream.next().await {
        let responses = res.map_err(|e| e.to_string())?;
        for resp in responses {
            if is_first_message {
                let mut spinner = spinner_running.lock().await;
                if *spinner {
                    *spinner = false;
                    drop(spinner);
                    sleep(Duration::from_millis(100)).await;
                }
                is_first_message = false;
            }
            stdout.write_all(resp.response.as_bytes()).await.map_err(|e|e.to_string())?;
            stdout.flush().await.map_err(|e|e.to_string())?;
        }
    }

    *spinner_running.lock().await = false;
    if let Err(e) = spinner_handle.await {
        return Err(e.to_string());
    }
    //to get ride fo the "%" at the end of the answer
    println!();
    Ok(())
}


