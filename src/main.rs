mod ai;
mod prompt;

use ai::connect_to_ai;

#[tokio::main]
async fn main() {
    
    let prompt: String = prompt::read_stdin();
    println!("Prompt => {}", prompt);
    connect_to_ai::api_call().await;
}
