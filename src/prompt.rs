use std::io::{self, BufRead, IsTerminal}; 
use std::env;                
use std::process;           

pub fn read_stdin() -> String {
    // Sammle Kommandozeilenargumente in einem Vector
    let argv: Vec<String> = env::args().collect();

    // Prüfe, ob genügend Argumente vorhanden sind
    if argv.len() < 2 {
        eprintln!("Not enough arguments supplied");
        process::exit(1);
    }

    // Erstelle den Prompt aus den Argumenten (ab Index 1)
    let mut prompt = String::new();
    for part in &argv[1..] {
        prompt.push_str(part);        // Füge aktuelles Argument hinzu
        prompt.push_str(" ");         // Füge Leerzeichen als Trennung hinzu
    }
    // Entferne das letzte überflüssige Leerzeichen
    prompt = prompt.trim().to_string();

    // Initialisiere den Buffer für die stdin-Eingabe
    let mut buffer: String = String::new();

    // Prüfe, ob die Eingabe von einem Terminal kommt oder piped ist
    if !io::stdin().is_terminal() {
        let stdin = io::stdin();
        // Lese alle Zeilen von stdin in den buffer
        for line in stdin.lock().lines() {
            match line {
                Ok(line) => {
                    buffer.push_str(&line);    // Füge die Zeile zum Buffer hinzu
                    buffer.push('\n');         // Behalte Zeilenumbrüche bei
                }
                Err(e) => {
                    eprintln!("Fehler beim Lesen von stdin: {}", e);
                    process::exit(1);
                }
            }
        }
        println!("stdin input -> {}", buffer);
    }

    // Wenn stdin leer ist, geben wir nur den Prompt zurück
    // Andernfalls könnten wir Prompt und Buffer kombinieren
    if !buffer.is_empty() {
        prompt.push_str(&buffer); // Kombiniere Prompt mit stdin-Eingabe
    }

    prompt // Rückgabe des finalen Strings
}