use crate::ai;
use crate::daemon;
use crate::envs::config;
use crate::prompt::stdin;
use clap::ArgMatches;

pub async fn evaluate_flags(
    matches: ArgMatches,
    kailian_parameter: &crate::envs::core::ConfigVariables,
) -> Result<(), String> {
    // Argumente auswerten

    if matches.get_flag("create_config") {
        return config::create_config();
    }
    if matches.get_flag("show_config") {
        println!("{}", kailian_parameter);
        return Ok(());
    }
    if matches.get_flag("coffee") {
        //wird von user mit ^c beendet
        crate::utils::coffee::sip_coffee();
    }
    if matches.get_flag("list_models") {
        return ai::list_local_models::list_models(kailian_parameter).await;
    }
    if matches.get_flag("running_model") {
        return ai::get_running_model::running_model(kailian_parameter);
    }
    if matches.get_flag("start_ollama") {
        return daemon::daemonize_ollama();
    }
    if matches.get_flag("kill_ollama") {
        return daemon::kill_ollama_daemon();
    }
    if matches.get_flag("new_chat") {
        return ai::chat_mode::delete_old_context();
    }
    if matches.contains_id("ask") {
        return match stdin::get_prompt().await {
            Ok(prompt) => {
                ai::connect_to_ai::api_completion_generation(&prompt, kailian_parameter).await
            }
            Err(e) => Err(e),
        };
    }
    if matches.contains_id("chat") {
        return match stdin::get_prompt().await {
            Ok(prompt) => ai::chat_mode::chat(&prompt, kailian_parameter).await,
            Err(e) => Err(e),
        };
    }
    // Sollte nie erreicht werden wegen args_required_else_help
    unreachable!("No arguments provided, but Clap should have caught this");
}
