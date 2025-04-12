use std::fs;
use std::fs::File;
use nix::unistd;
use ollama_rs::generation::completion::request::GenerationRequest;
use tokio::io::{self};
use tokio::task;
use tokio::time::{self, sleep, Duration};
use crate::envs;
use ollama_rs::{
    generation::chat::{request::ChatMessageRequest, ChatMessage, ChatMessageResponseStream},
    Ollama,
};
use std::sync::{Arc, Mutex};
use tokio::io::{stdout, AsyncWriteExt};
use tokio_stream::StreamExt;
pub async fn chat(prompt: &String, kailian_variables: &envs::EnvVariables) -> Result<(), String> {
    let prompt = prompt.to_string();
    let ollama = Ollama::new(kailian_variables.kailian_endpoint.to_string(), 11434);
    let uid = unistd::getuid();
    println!("uid -> {}", uid);
    let mut stdout = stdout();
    let history_file = format!("/run/user/{}/kailian-session-context", uid);
    println!("tmpfile -> {}", history_file);
    fs::write(&history_file, "empty".to_string()).map_err(|e| e.to_string())?;
    let history = Arc::new(Mutex::new(vec![]));


    let mut stream: ChatMessageResponseStream = ollama
        .send_chat_messages_with_history_stream(
            history.clone(),
            ChatMessageRequest::new(
                kailian_variables.kailian_model.to_string(),
                vec![ChatMessage::user(prompt.to_string())],
            ),
        )
        .await.map_err(|e| e.to_string())?;
    fs::write(&history_file, s);

    let mut response = String::new();
    while let Some(Ok(res)) = stream.next().await {
        stdout.write_all(res.message.content.as_bytes()).await.map_err(|e| e.to_string())?;
        stdout.flush().await.map_err(|e| e.to_string())?;
        response += res.message.content.as_str();
    }
    println!("current histroy -> {:?}", history);

    dbg!(&history.lock().unwrap());

    Ok(())
}

