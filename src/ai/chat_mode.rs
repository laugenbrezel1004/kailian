use std::fs;
use nix::unistd;
use crate::envs;
use ollama_rs::{
    generation::chat::{request::ChatMessageRequest, ChatMessage, MessageRole, ChatMessageResponseStream},
};
use tokio::io::{stdout};
use serde::{Deserialize, Serialize};

use ollama_rs::Ollama;
use tokio::io::{self, AsyncWriteExt};
use tokio::task;
use tokio_stream::StreamExt;
use tokio::time::{self, sleep, Duration};
use std::sync::Arc;
use tokio::sync::Mutex;
// Custom struct for serializing/deserializing chat messages to/from the file
#[derive(Serialize, Deserialize, Clone)]
struct SerializableChatMessage {
    role: String,
    content: String,
    images: Option<Vec<String>>,
    tool_calls: Vec<String>,
}

fn to_serializable(msg: &ChatMessage) -> SerializableChatMessage {
    SerializableChatMessage {
        role: match msg.role {
            MessageRole::User => "user".to_string(),
            MessageRole::Assistant => "assistant".to_string(),
            MessageRole::System => "system".to_string(),
            _ => {
                "error".to_string()
            }
        },
        content: msg.content.clone(),
        images: None,
        tool_calls: vec![],
    }
}

fn from_serializable(msg: SerializableChatMessage) -> ChatMessage {
    let role = match msg.role.as_str() {
        "user" => MessageRole::User,
        "assistant" => MessageRole::Assistant,
        "system" => MessageRole::System,
        _ => MessageRole::User,
    };
    ChatMessage {
        role,
        content: msg.content,
        images: None,
        tool_calls: Vec::new(),
    }
}

pub async fn chat(prompt: &String, kailian_variables: &envs::EnvVariables) -> Result<(), String> {
    let prompt = prompt.to_string();
    let uid = unistd::getuid();
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
    println!("uid -> {}", uid);
    let history_file = format!("/run/user/{}/kailian-session-context", uid);

    let mut stdout = stdout();

    #[cfg(debug_assertions)]
    println!("History file -> {}", history_file);

    // Initialize history by reading from the file
    let history: Arc<Mutex<Vec<ChatMessage>>> = Arc::new(Mutex::new(
        read_history_from_file(&history_file)
            .map(|serializable_msgs| serializable_msgs.into_iter().map(from_serializable).collect())
            .unwrap_or_else(|e| {
                println!("Failed to read history from file (using empty history): {}", e);
                vec![]
            })
    ));

    // Append the user's prompt to the history
    {
        let mut history_lock = history.lock().map_err(|e| format!("Mutex poisoned: {}", e))?;
        let user_message = ChatMessage::user(prompt.clone());
        history_lock.push(user_message.clone());
        // Save the updated history to the file
        save_history_to_file(&history_file, &history_lock)?;
    }

    let mut stream: ChatMessageResponseStream = ollama
        .send_chat_messages_with_history_stream(
            history.clone(),
            ChatMessageRequest::new(
                kailian_variables.kailian_model.to_string(),
                vec![ChatMessage::user(prompt.to_string())],
            ),
        )
        .await
        .map_err(|e| e.to_string())?;

    let mut response = String::new();
    while let Some(Ok(res)) = stream.next().await {
        stdout.write_all(res.message.content.as_bytes()).await.map_err(|e| e.to_string())?;
        stdout.flush().await.map_err(|e| e.to_string())?;
        response += res.message.content.as_str();
    }

    // Append the assistant's response to the history
    {
        let mut history_lock = history.lock().map_err(|e| format!("Mutex poisoned: {}", e))?;
        let assistant_message = ChatMessage {
            role: MessageRole::Assistant,
            content: response.clone(),
            images: None, // Default for images
            tool_calls: Vec::new(), // Default for tool_calls
        };
        history_lock.push(assistant_message);
        // Save the updated history to the file
        save_history_to_file(&history_file, &history_lock)?;
    }

    #[cfg(debug_assertions)]
    println!("current history -> {:?}", history.lock().unwrap());

    Ok(())
}

// Read history from the file
fn read_history_from_file(file_path: &str) -> Result<Vec<SerializableChatMessage>, String> {
    let content = fs::read_to_string(file_path).map_err(|e| format!("Failed to read file: {}", e))?;
    if content.trim().is_empty() || content == "empty" {
        return Ok(vec![]);
    }
    let history: Vec<SerializableChatMessage> = serde_json::from_str(&content)
        .map_err(|e| format!("Failed to deserialize history: {}", e))?;
    Ok(history)
}

// Save history to the file by overwriting (since we store the full history)
fn save_history_to_file(file_path: &str, history: &[ChatMessage]) -> Result<(), String> {
    let serializable_history: Vec<SerializableChatMessage> = history.iter().map(to_serializable).collect();
    let serialized = serde_json::to_string(&serializable_history)
        .map_err(|e| format!("Failed to serialize history: {}", e))?;
    fs::write(file_path, serialized).map_err(|e| format!("Failed to write to file: {}", e))?;
    Ok(())
}
pub fn delete_old_context() -> Result<(), String> {
    let uid = unistd::getuid();
    #[cfg(debug_assertions)]
    println!("uid -> {}", uid);
    let history_file = format!("/run/user/{}/kailian-session-context", uid);
    fs::write(history_file, "".to_string()).map_err(|e| e.to_string())?;
    Ok(())
}