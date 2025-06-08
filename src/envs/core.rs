pub const KAILIAN_CONF_PATH: &str = "/etc/kailian/kailian.conf";

#[derive(Debug)]
pub struct ConfigVariables {
    pub kailian_model: String,
    pub kailian_endpoint: String,
    pub kailian_system: String,
    pub kailian_alive: String,
}
pub fn new() -> ConfigVariables {
    // create Default-Struct
    ConfigVariables {
        kailian_model: String::new(),
        kailian_endpoint: String::new(),
        kailian_system: String::new(),
        kailian_alive: String::new(),
    }
}