mod ai;
mod prompt;
mod kailian_env;

use ai::connect_to_ai;

#[tokio::main]
async fn main() {
    loop {
        
    
    let prompt: String = prompt::read_stdin();
   // println!("Prompt => {}", prompt);
    //connect_to_ai::api_completion_generation(&prompt).await;
    connect_to_ai::api_chat_mode(&prompt).await;
    }
}
