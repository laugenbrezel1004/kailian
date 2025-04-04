<h1 style="text-align: center; background-color: #f0f0f0; padding: 10px; border: 1px solid #ccc;">🚧 STILL UNDER CONSTRUCTION 🚧</h1>
# Kailian - The Linux Admin's Quirky Sidekick! 🤖🎉

**Kailian** is your terminal’s eccentric companion, now rebuilt in Rust for Linux and powered by [Ollama](https://ollama.com/). Ask it anything, admire ASCII coffee art, or pipe in command output for witty insights. With `curl` for HTTP magic, `ollama-rs` for AI streaming, and `clap` for slick CLI parsing, Kailian is here to make your admin life both efficient and entertaining.

## Prerequisites
- Rust (latest stable recommended, install via `curl --proto '=https' --tlsv1.2 -sSf https://sh.rustup.rs | sh`)
- Cargo (comes with Rust)
- Optional: `ollama` installed locally for AI functionality (`curl -fsSL https://ollama.com/install.sh | sh`)

## Building 🛠️
```bash
git clone https://github.com/laugenbrezel1004/kailian.git
cd kailian
cargo build --release
sudo cp target/release/kailian /usr/local/bin/
sudo mkdir -p /etc/kailian && sudo cp kailian.conf /etc/kailian/
```
This drops Kailian into `/usr/local/bin` and the config into `/etc/kailian/`. You’re official now.

## Usage 🔧

| Command                                     | What It Does                                      |
|---------------------------------------------|---------------------------------------------------|
| `kailian -a question`                       | Ask the AI anything—profound or absurd (quote it to avoid shell globbing chaos) |
| `kailian --model`                           | Show the current AI model’s stats                 |
| `kailian --show-models`                     | List all available AI models in style           |
| `kailian --create-config`                   | Generate a fresh config if yours is missing or broken |
| `kailian --coffee`                          | Brew some ASCII coffee—sadly, still decaf        |
| `kailian --help`                            | Get directions to the fun zone                   |
| `kailian --show-environment`                | Peek at the `kailian.conf` file                 |
| `kailian --start-ollama`                    | Fire up a local Ollama instance                  |
| `kailian --kill-ollama`                     | Shut down a local Ollama instance                 |
| `cat error.log \| kailian "Fix this!"`      | Pipe in errors for AI analysis | 
| `man yes \| kailian "Explain this command"` | Get AI explanations of commands |

## Configuration

Kailian’s config lives at:

`/etc/kailian/kailian.conf`

**Sample Config:**

```ini
[config]
name = "deepseek-r1:14" ; The AI’s codename—sounds cool, right?
endpoint = "http://localhost:11434/api/generate" ; Where the magic happens
info_endpoint = "http://localhost:11434/api/tags" ; Model info hub
running_model_endpoint = "http://localhost:11434/api/ps" ; Who’s running?
ollama_version_endpoint = "http://localhost:11434/api/version" ; Ollama’s brag
system = "You are a linux admin who answers correctly and without markdown" ; AI’s personality—straight to the point
```

**Tweaks:** Adjust the `system` prompt to make the AI snarky, verbose, or whatever vibe you’re feeling.

## Examples

**Ask a Deep Question:**

```bash
kailian "What is the meaning of life?"
```

**Show Model Info:**

```bash
kailian --model
```

**List Available Models:**

```bash
kailian --show-models
```

**Brew Coffee:**

```bash
kailian --coffee
```

**Summarize Directory Mess:**

```bash
tree | kailian "Summarize this directory mess"
```

## Troubleshooting

* **“Config’s AWOL!”:** Verify `/etc/kailian/kailian.conf`. Regenerate with `--create-config` if needed.
* **“Ollama’s asleep!”:** Test it: `curl http://localhost:11434`. Wake it with `--start-ollama` if it’s dozing.

## Environment Variables

* `KAILIAN_CONFIG`: Override the default config path (e.g., `KAILIAN_CONFIG=/custom/path/kailian.conf kailian "hi"`).
```

Written by some local ai