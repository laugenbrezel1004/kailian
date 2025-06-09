use std::io::{stdout, Write};
use crate::envs::core::ConfigVariables;
use curl::easy::Easy;

pub fn running_model(env_variables: &ConfigVariables) -> Result<(), String> {
    let mut easy = Easy::new();
    let url = format!("{}/api/ps", env_variables.kailian_endpoint);

    //make output cleaner
    easy.url(&url).map_err(|e| e.to_string())?;
    easy.write_function(|data| {
        stdout().write_all(data).unwrap();
        Ok(data.len())
    }).map_err(|e|e.to_string())?;
    easy.perform().map_err(|e|e.to_string())?;
    println!(); // to get rid of the "%" at the end
    Ok(())
}