use std::io::{stdout, Write};
use crate::envs::EnvVariables;
use curl::easy::Easy;

pub fn running_model(env_variables: &EnvVariables) -> Result<(), String> {
    let mut easy = Easy::new();
    let url = format!("{}/api/ps", env_variables.kailian_endpoint);

    #[cfg(debug_assertions)]
    println!("url -> {}", url);

    easy.url(&url).map_err(|e| e.to_string())?;
    easy.write_function(|data| {
        stdout().write_all(data).unwrap();
        Ok(data.len())
    }).map_err(|e|e.to_string())?;
    easy.perform().map_err(|e|e.to_string())?;
    println!(); // to get rid of the "%" at the end
    Ok(())
}