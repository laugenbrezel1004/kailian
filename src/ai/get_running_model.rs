use crate::envs::core::ConfigVariables;
use curl::easy::Easy;
use serde_json::Value;
use std::io::{Write, stdout};

pub fn running_model(env_variables: &ConfigVariables) -> Result<(), String> {
    let mut easy = Easy::new();
    let url = format!("{}/api/ps", env_variables.kailian_endpoint);

    //make output cleaner
    easy.url(&url).map_err(|e| e.to_string())?;
    easy.write_function(|data| {
        let value: Value = serde_json::from_slice(data).unwrap();
        let model_value = &value["models"][0]["model"];
        println!("Model: {}", model_value);
        //stdout().write_all(data).unwrap();
        Ok(data.len())
    })
    .map_err(|e| e.to_string())?;
    easy.perform().map_err(|e| e.to_string())?;
    println!(); // to get rid of the "%" at the end
    Ok(())
}
