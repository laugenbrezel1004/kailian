 # Kailian

**Kailian** isn’t just a command-line tool—it’s your terminal’s quirky sidekick, built in C for Linux and powered by [Ollama](https://ollama.com/). Whether you’re interrogating a local AI, admiring ASCII coffee art, or piping in comma
nd output for sage advice, Kailian’s got your back. It leans on `libcurl` for HTTP wizardry, `cJSON` for JSON crunching, and optionally `ncurses` for a graphical interface, and `pthread` for multi-threading.

## Installation

Clone the repository:
```bash
git clone https://github.com/yourusername/kailian.git
cd kailian
```
### Dependencies (for Linux)

Install required libraries:
```bash
sudo apt-get install libcurl4-openssl-dev libncurses5-dev pthread-win32
```
### Compile and Install

Compile the source code:
```bash
make
```
(Optional) Install it:
```bash
sudo make install
```
Drops Kailian into `/usr/local/bin` and the config into `/etc/kailian`. You’re official now.

## Usage

Use the following commands to interact with Kailian:

- `kailian [command] [arguments]` : Ask it stuff with quotes: `kailian "Why’s my Wi-Fi flaky?"`
- Flags for flair: `--help`, `--coffee`, you name it.
- Pipe in chaos: `cat error.log | kailian "Fix this!"`

## Commands to Tickle Your Fancy

Command    | What It Does
------------|-----------------
`kailian "question"`  | Ask the AI anything—serious or silly
`--model`    | Flex the current model’s stats
`--show-models`   | Parade all available AI models
`--coffee`     | Brew a terminal coffee—decaf, sadly

## Configuration

Kailian's config file hangs out at:

- `/etc/kailian/kailian.conf` (release builds)
- `./kailian.conf` (debug builds)

Sample Config (With Attitude)
```bash
[general]
name = "deepseek-r1:14" ; The AI’s stage name—sounds edgy
endpoint = "http://localhost:11434/api/generate" ; Where answers are born
info_endpoint = "http://localhost:11434/api/tags" ; Model gossip hub
running_model_endpoint = "http://localhost:11434/api/ps" ; Who’s awake?
ollama_version_endpoint = "http://localhost:11434/api/version" ; Ollama’s flex
system="You are a linux admin who answers correctly and without markdown" ; AI’s vibe—gruff but helpful
```
Hack It: Change the system prompt to make the AI chatty, curt, or downright sarcastic.

## Examples

Ask Something Smart:
```bash
kailian "How do I kill a zombie process?"
Output: kill -9 <pid> (AI: "It’s dead now. Next!")
```
Check the Model:
```bash
kailian --model
See what’s powering the magic.
```
Model Lineup:
```bash
kailian --show-models
Meet the AI crew.
```
Coffee Time:
```bash
kailian --coffee
Sip virtually while you debug.
```
Pipe Dreams:
```bash
tree | kailian "Summarize this directory mess"
Kailian tidies up your thoughts.
```

## Troubleshooting

“Config’s gone!”: Check `/etc/kailian/kailian.conf` (release) or `./kailian.conf` (debug). Make one if it’s lost.
“Ollama’s napping!”: Ping it: `curl http://localhost:11434`. Wake it up if it’s snoozing.
Build Blues: Missing libs? Install libcurl or cJSON. Makefile moaning? Edit LDFLAGS.
“Access denied!”: Sudo up for release installs. You’re the boss—prove it.
