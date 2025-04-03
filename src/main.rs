mod ai;
mod prompt;
pub mod envs;

use ai::connect_to_ai;

#[tokio::main]
async fn main() {
    let prompt: String = prompt::read_stdin();
    let mut kailian_env = envs::EnvVariables::new();
    //read first the config and then the envs, because the envs should overwrite the config file
    kailian_env.read_env();
    //no autodaemon so far -> start ollama by hand
    connect_to_ai::api_completion_generation(&prompt).await;
    //connect_to_ai::api_chat_mode(&prompt).await;
}
