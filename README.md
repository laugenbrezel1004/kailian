<h1 align="center">
  <img src="https://img.shields.io/badge/🚧-STILL UNDER CONSTRUCTION-orange?style=for-the-badge" alt="Under Construction">
</h1>

<div align="center">
  <h1>Kailian - The Linux Admin's Quirky Sidekick! 🤖🎉</h1>
  <p>Your terminal's eccentric companion, rebuilt in Rust with AI superpowers</p>

[![Rust Version](https://img.shields.io/badge/rust-1.70+-blue.svg)](https://www.rust-lang.org/)
[![License](https://img.shields.io/badge/license-GPLv3-blue)](LICENSE)
[![Ollama Powered](https://img.shields.io/badge/powered_by-Ollama-FF6C37)](https://ollama.com)
</div>

**Kailian** is the terminal companion you didn’t know you needed. Rebuilt in Rust for Linux and powered by [Ollama](https://ollama.com/), it answers your questions, analyzes command output, and serves ASCII coffee art with a side of humor. Whether you’re debugging logs or pondering life’s mysteries, Kailian’s got your back for a seamless, fun experience.

## ✨ Features
- **Ask Anything**: Query the AI for Linux tips, code fixes, or existential musings.
- **Pipe It In**: Feed command output (e.g., `cat error.log`) for smart analysis.
- **ASCII Art**: Brew virtual coffee to lighten your admin grind.
- - **Model Management**: List, switch, and inspect AI models
- **Rust-Powered**: Fast, safe, and built for the long haul.
- **Customizable**: Tweak the AI’s vibe via a simple config file.


## 🚀 Quick Start

### Prerequisites
- Rust & Cargo: `curl --proto '=https' --tlsv1.2 -sSf https://sh.rustup.rs | sh`
- Ollama (optional for AI features): `curl -fsSL https://ollama.com/install.sh | sh`

### Installation
```bash
git clone https://github.com/laugenbrezel1004/kailian.git
cd kailian
cargo build --release
sudo mkdir -p /etc/kailian
sudo cp target/release/kailian /usr/local/bin/
sudo cp kailian.conf /etc/kailian/
```
This drops Kailian into `/usr/local/bin` and the config into `/etc/kailian/`. You’re official now.

## Basic Commands 🔧

| Command                                     | What It Does                                      |
|---------------------------------------------|---------------------------------------------------|
| `kailian -a "question"`                       | Ask the AI anything—profound or absurd (quote it to avoid shell globbing chaos) |
| `kailian --running-model`                   | Show the current AI model’s stats                 |
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
name = "deepseek-r1:671b"
endpoint = "http://localhost:11434/api/generate"
system = "You are a linux admin who answers correctly and without markdown"
```

**Tweaks:** Adjust the `system` prompt to make the AI snarky, verbose, or whatever vibe you’re feeling.

## Examples

**Ask a Deep Question:**

```bash
kailian -a "What is the meaning of life?"
# " is only needed to mask special characters for the shell
```

**Show Model Info:**

```bash
kailian --running-model
deepseek-r1:671b
```

**List Available Models:**

```bash
kailian --show-models
# ...
```

**Brew Coffee:**

```bash
kailian --coffee
           ~      
          ~    
              ~      
       _____________
      <_____________> ___
      |             |/ _ \
      |             | | |
      |             |_| |
   ___|             |\___/
  /    \___________/    \
  \_____________________/
       \___________/

```

**Summarize Directory Mess:**

```bash
tree / -L 1| kailian -a Summarize this directory mess
This directory structure appears to be a typical Linux filesystem layout with some symbolic links. Here's the summary:

Main directories:
- /boot - For boot loader files
- /dev - Device files
- /efi - EFI system partition
- /etc - System configuration files
- /home - User home directories
- /media - Mount point for removable media
- /mnt - Temporary mount points
- /opt - Optional software packages
- /proc - Process information pseudo-filesystem
- /root - Root user's home directory
- /run - Runtime variable data
- /sys - Kernel and driver information
- /tmp - Temporary files
- /usr - User programs and files (with symbolic links to bin, lib, and lib64)
- /var - Variable data

Symbolic links:
- bin -> usr/bin
- lib -> usr/lib
- lib64 -> usr/lib64
- sbin -> usr/bin

Single file:
- rsync.log

Total: 20 directories and 1 file

```

## Troubleshooting

* **Configfile AWOL?:** Verify `/etc/kailian/kailian.conf`. Regenerate with `sudo kailian --create-config` if needed.
* **“Ollama’s ghosts you?”:** Test it: `curl http://localhost:11434`. Wake it with `--start-ollama` if it’s dozing.

## 🌿 Environment Variable
* `KAILIAN_MODEL` Overwrite the model set in the kailian.conf (maybe with `export` to use across shell sessions).
```

Written by some local ai