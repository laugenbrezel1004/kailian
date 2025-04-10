use std::path::Path;
use std::process::{self, Command};

use nix::fcntl::{open, OFlag};
use nix::sys::stat::Mode;
use nix::unistd::{chdir, close, dup2, fork, setsid, ForkResult, };

pub fn daemonize_ollama() -> Result<(), String> {

    match unsafe { fork() } {
        Ok(ForkResult::Parent { .. }) => {
            process::exit(0);
        }
        Ok(ForkResult::Child) => {}
        Err(e) => {
            return Err(e.to_string());
        }
    }

    if let Err(e) = setsid() {
        return Err(e.to_string());
    }

    match unsafe { fork() } {
        Ok(ForkResult::Parent { .. }) => {
            process::exit(0);
        }
        Ok(ForkResult::Child) => {}
        Err(e) => {
            return Err(e.to_string());
        }
    }

    if let Err(e) = chdir(Path::new("/")) {
        return Err(e.to_string());
    }

    let dev_null = open(Path::new("/dev/null"), OFlag::O_RDWR, Mode::empty())
        .map_err(|e| format!("open /dev/null fehlgeschlagen: {}", e))?;
    dup2(dev_null, libc::STDIN_FILENO).map_err(|e| format!("dup2 stdin fehlgeschlagen: {}", e))?;
    dup2(dev_null, libc::STDOUT_FILENO).map_err(|e| format!("dup2 stdout fehlgeschlagen: {}", e))?;
    dup2(dev_null, libc::STDERR_FILENO).map_err(|e| format!("dup2 stderr fehlgeschlagen: {}", e))?;
    if dev_null > 2 {
        close(dev_null).map_err(|e| format!("close dev_null fehlgeschlagen: {}", e))?;
    }

    Command::new("ollama")
        .arg("serve")
        .spawn()
        .map_err(|e| format!("Ollama starten fehlgeschlagen: {}", e))?;



    Ok(())
}

pub fn kill_ollama_daemon() -> Result<(), String> {
    Command::new("killall").arg("ollama").spawn().map_err(|e| e.to_string())?;
    Ok(())
}