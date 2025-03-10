# Kaillian - The Linux Admin's Quirky Sidekick! 🤖🎉

**Kailian** isn’t just a command-line tool—it’s your terminal’s quirky sidekick, built in C for Linux and powered by [Ollama](https://ollama.com/). Whether you’re interrogating a local AI, admiring ASCII coffee art, or piping in command output for sage advice, Kailian’s got your back. It leans on `libcurl` for HTTP wizardry, `cJSON` for JSON crunching and `llogger` for logging.


## Prerequisites
- GCC or another C compiler
- Libraries:
  - `cJSON` (e.g., `sudo pacman -S cjson` or `sudo apt-get install libcjson-dev`)
  - `libcurl` (e.g., `sudo pacman -S curl` or `sudo apt-get install libcurl4-openssl-dev`)


## Building 🛠️
Compile and install the source code:
```bash
git clone https://github.com/laugenbrezel1004/kaillian.git
cd kaillian
make 
sudo make install
```

Drops Kailian into `/usr/local/bin` and the config into `/etc/kailian`. You’re official now.


## Usage 🔧

Command    | What It Does
------------|-----------------
`kaillian "question"`  | Ask the AI anything—from the profound to the preposterous (Just put "" if you don't want to activate shellglobbing, but remember, it’s a party here!)
`--model`    | Flaunt the AI’s muscle (stats)
`--show-models`   | Put all the AI models on the catwalk, strutting their stuff
`--create-config`   | Create new config if old one is lost or corrupt
`--coffee`     | Boil some liquid happiness for your terminal—alas, it's decaf
`--help`     | If you’re lost and need directions to fun town
`--info`     | Get some basic info about me
`--show-environment`     | Peek into the kaillian.conf file
`--start-ollama`     | Start a local ollama instance
`--kill-ollama`     | Kill a local ollama instance
`cat error.log`\| kaillian "Fix this!"| :) 🤖
`man yes`\| kaillian Explain me this command| Yes! 🚀


## Configuration

Kailian's config file hangs out at:

- `/etc/kailian/kailian.conf` 

Sample Config 
```bash
name = "deepseek-r1:14" ; The AI’s stage name—sounds edgy
endpoint = "http://localhost:11434/api/generate" ; Where answers are born
info_endpoint = "http://localhost:11434/api/tags" ; Model gossip hub
running_model_endpoint = "http://localhost:11434/api/ps" ; Who’s awake?
ollama_version_endpoint = "http://localhost:11434/api/version" ; Ollama’s flex
system = "You are a linux admin who answers correctly and without markdown" ; AI’s vibe—gruff but helpful
```
Hack It: Change the system prompt to make the AI chatty, curt, or downright sarcastic.

## Examples

Ask Something Smart:
```bash
kailian "What is coffee?"
-> Coffee, my friend, is the elixir of life that allows us to function before we've fully processed the fact that it's still morning.
```

Check the current model in use:
```bash
kailian --model
-> Model Name: mistral:latest
```
See what’s powering the magic.

Model Lineup:
```bash
kailian --show-models
-> Model Name: qwen2.5:14b
Model Name: granite3.2:latest
Model Name: mistral:latest
Model Name: deepseek-r1:32b
Model Name: codellama:13b
Model Name: deepseek-r1:14b
Model Name: deepseek-r1:1.5b
Model Name: deepseek-r1:latest
```
Meet the AI crew.

Coffee Time:
```bash
kailian --coffee
          ~      
              ~    
            ~      
       _____________
      <_____________> ___
      |             |/ _ \
      |               | | |
      |               |_| |
   ___|             |\___/
  /    \___________/    \
  \_____________________/
       \___________/

```
Sip virtually while you debug.

Pipe Dreams:
```bash
tree | kailian "Summarize this directory mess"
->  Looks like you're brewing your own Linux-powered robot, Kailian! With this many lines of code, I hope it can make me a cup of coffee when I ask it nicely.
```
Kailian tidies up your thoughts.

## Troubleshooting

“Config’s gone!”: Check `/etc/kailian/kailian.conf`. Make one if it’s lost with "--create-config".

“Ollama’s napping!”: Ping it: `curl http://localhost:11434`. Wake it up if it’s snoozing.
