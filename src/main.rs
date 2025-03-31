mod ai;
mod prompt;
mod kailian_env;

use ai::connect_to_ai;

#[tokio::main]
async fn main() {
    
    let prompt: String = prompt::read_stdin();
   // println!("Prompt => {}", prompt);
    connect_to_ai::api_call(&prompt).await;
}
