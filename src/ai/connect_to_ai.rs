#![allow(unused_imports)]

use std::process::exit;
use ollama_rs::generation::completion::request::GenerationRequest;
use ollama_rs::Ollama;
use tokio::io::{self, AsyncWriteExt};
use tokio::task;
use tokio_stream::StreamExt;
use tokio::time::{self, Duration};
use std::sync::Arc;
use tokio::sync::Mutex;
use ollama_rs::generation::chat::{ChatMessage, request::ChatMessageRequest};
use ollama_rs::history::ChatHistory;
use crate::envs::EnvVariables;

pub async fn api_completion_generation(prompt: &String, kailian_variables: &EnvVariables) {
    let prompt = prompt.to_string();
  
    let ollama = Ollama::new(kailian_variables.kailian_generate.to_string(), 11434);

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

    println!("Sending request to Ollama...");
    let mut stream = match ollama
        .generate_stream(GenerationRequest::new(kailian_variables.kailian_model.to_string(), prompt.clone()))
        .await {
        Ok(stream) => {
            stream
        }
        Err(e) => {
            println!("Error connecting to Ollama: {}", e);
            *spinner_running.lock().await = false;
            spinner_handle.await.unwrap();
            return;
        }
    };

    let mut stdout = io::stdout();
    while let Some(res) = stream.next().await {
        match res {
            Ok(responses) => {
                for resp in responses {
                    if *spinner_running.lock().await {
                        *spinner_running.lock().await = false;
                    }
                    stdout.write_all(resp.response.as_bytes()).await.unwrap();
                    stdout.flush().await.unwrap();
                }
            }
            Err(e) => {
                println!("Stream error: {}", e);
                break;
            }
        }
    }

    *spinner_running.lock().await = false;
    spinner_handle.await.unwrap();
    println!("Done!");
}

//TODO: To be done chat-mode