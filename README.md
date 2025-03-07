 # Kailian

**Kailian** isn’t just a command-line tool—it’s your terminal’s quirky sidekick, built in C for Linux and powered by [Ollama](https://ollama.com/). Whether you’re interrogating a local AI, admiring ASCII coffee art, or piping in command output for sage advice, Kailian’s got your back. It leans on `libcurl` for HTTP wizardry, `cJSON` for JSON crunching and `llogger` for logging.

## Installation

### Dependencies (for Linux)

## Prerequisites
- CMake 3.10 or higher
- GCC or another C compiler
- Libraries:
  - `cJSON` (e.g., `sudo pacman -S cjson` or `sudo apt-get install libcjson-dev`)
  - `libcurl` (e.g., `sudo pacman -S curl` or `sudo apt-get install libcurl4-openssl-dev`)


## Building

Compile and install the source code:
```bash
git clone https://github.com/laugenbrezel1004/kailian.git
cd kailian
mkdir build && cd build
make .. -DCMAKE_BUILD_TYPE=Release
make
```

Drops Kailian into `/usr/local/bin` and the config into `/etc/kailian`. You’re official now.



Installs the bin and config file in the local dir.

## Usage

Command    | What It Does
------------|-----------------
`kailian "question"`  | Ask the AI anything—from the profound to the preposterous (Just put "" if you don't want to activate shellglobbing, but remember, it's a party here!)
`--model`    | Flaunt the AI's muscle (stats)
`--show-models`   | Put all the AI models on the catwalk, strutting their stuff
`--coffee`     | Boil some liquid happiness for your terminal—alas, it's decaf
`--help`     | If you're lost and need directions to fun town
`--info`     | Get some basic info about me
`--show-environment`     | Peek into the kailian.conf file
`cat error.log`\| kailian "Fix this!"| :) 
`man yes`\| kailian Explain me this command| Yes!

## Configuration

Kailian's config file hangs out at:

- `/etc/kailian/kailian.conf` (release builds)

Sample Config (With Attitude)
```bash
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
kailian "How do I kill systemd?"
```
Check the Model:
```bash
kailian --model
```
See what’s powering the magic.

Model Lineup:
```bash
kailian --show-models
```
Meet the AI crew.

Coffee Time:
```bash
kailian --coffee
```
Sip virtually while you debug.

Pipe Dreams:
```bash
tree | kailian "Summarize this directory mess"
```
Kailian tidies up your thoughts.

## Troubleshooting

“Config’s gone!”: Check `/etc/kailian/kailian.conf`. Make one if it’s lost.

“Ollama’s napping!”: Ping it: `curl http://localhost:11434`. Wake it up if it’s snoozing.

Build Blues: Missing libs? Install libcurl or cJSON. Makefile moaning? Edit LDFLAGS.

“Access denied!”: Sudo up for release installs. You’re the boss—prove it.
