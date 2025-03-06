
# Kailian

**Kailian** is a command-line tool written in C for Linux systems, designed to interact with local AI models powered by Ollama  (https://ollama.com/). It enables users to ask questions to a locally running AI model, retrieve model information, display a coffee animation, and more—all from the terminal. Built with portability and flexibility in mind, Kailian uses libraries like `libcurl`, `cJSON`, and `liblogger` to handle HTTP requests, JSON parsing, and logging.

## Table of Contents
- [Project Overview](#project-overview)
- [Features](#features)
- [Prerequisites](#prerequisites)
- [Installation](#installation)
- [Usage](#usage)
- [Configuration](#configuration)
- [Examples](#examples)
- [Troubleshooting](#troubleshooting)
- [Contributing](#contributing)
- [License](#license)

## Project Overview

Kailian is a lightweight CLI utility that connects to a local Ollama instance to:

- Query an AI model with natural language questions.
- Display information about the running model or available models.
- Provide a fun coffee animation in the terminal.
- Handle piped input for contextual queries (e.g., analyzing command output).

The project uses a Makefile for building in debug or release modes and is designed for Linux environments, adhering to the C11 standard.

## Features
- **AI Queries**: Ask questions to a local Ollama AI model and get responses.
- **Model Management**: Retrieve details about the current model or list available models.
- **Coffee Animation**: Enjoy a terminal-based coffee cup animation.
- **Piped Input**: Process input from other commands (e.g., `tree | kailian "Summarize this"`).
- **Configurable**: Customize via a configuration file.
- **Debug/Release Builds**: Build with debugging symbols or optimized for production.

## Prerequisites

To build and run Kailian, you need:

- **Operating System**: Linux (e.g., Ubuntu 22.04)
- **Compiler**: GCC (version 7.5 or later)
- **Libraries**:
  - `libcurl` (for HTTP requests to Ollama)
  - `cJSON` (for JSON parsing)
  - `liblogger` (for logging, optional depending on setup)
- **Ollama**: Installed and running locally on `http://localhost:11434`
- **Tools**: `make`, `git`

### Installing Dependencies
On Ubuntu/Debian:

```bash
sudo apt update
sudo apt install gcc make git libcurl4-openssl-dev libcjson-dev 
```
**Note:** `liblogger` might not be available in default repositories. You may need to find and install it separately.

## Installation

1. **Clone the repository:**

 ```bash
 git clone https://github.com/laugenbrezel1004/kailian.git 
 cd kailian 
 ```

2. **Build Kailian:**

 ```bash
 make
 ```

3. **(Optional) Install Kailian (requires sudo):**

 ```bash
 sudo make install 
 ```



## Usage

```
kailian [command] [arguments]
```

**Commands:**

- `kailian "question"`: Ask a question to the AI model.
- `kailian --model`: Display information about the running AI model.
- `kailian --show-models`: List all available AI models in Ollama.
- `kailian --coffee`: Show an animated coffee cup.

**Example:**

```bash
kailian "What is the capital of France?"
```

## Configuration

Kailian uses a configuration file located at `/etc/kailian/kailian.conf` (for release builds) or in the current directory (for debug builds).


The configuration file should contain the following settings:

```ini
[general]
name = "deepseek-r1:14" ; The AI model to use 
endpoint = "http://localhost:11434/api/generate" ; API endpoint for generating responses
info_endpoint = "http://localhost:11434/api/tags" ; Endpoint for model information
running_model_endpoint = "http://localhost:11434/api/ps" ; Endpoint for the running model

ollama_version_endpoint = "http://localhost:11434/api/version" ; Endpoint for Ollama version info

system="You are a linux admin who answers correctly and without markdown" ; System prompt to guide AI responses
```



## Examples

- **Ask a Question:**
 ```bash
 kailian "How do I list all running processes on Linux?"
 ```

- **Show Running Model:**
 ```bash
 kailian --model 
 ```

- **List Available Models:**

 ```bash
 kailian --show-models 
 ```

- **Coffee Animation:**
 ```bash
 kailian --coffee
 ```

- **Piped Input:**

 ```bash
 tree | kailian "Summarize this directory structure"
 ```



## Troubleshooting

- **"No such file or directory: /etc/kailian/kailian.conf"**: Ensure the configuration file exists and is correctly formatted. For debug buil
ds, place it in the current directory.
- **Connection Errors:** Verify that Ollama is running on `http://localhost:11434` (use `curl http://localhost:11434`).

- **Build Failures**: Confirm all dependencies are installed (`libcurl`, `cJSON`, etc.). Check `LDFLAGS` in the Makefile if linking fails.
- **Permission Denied**: Use `sudo` for release builds to install to `/usr/local/bin` and `/etc/kailian`.

## Examples

- **Ask a Question:**
 ```bash
 kailian "How do I list all running processes on Linux?"
 ```

- **Show Running Model:**
 ```bash
 kailian --model 
 ```

- **List Available Models:**

 ```bash
 kailian --show-models 
 ```

- **Coffee Animation:**
 ```bash
 kailian --coffee
 ```

- **Piped Input:**

 ```bash
 tree | kailian "Summarize this directory structure"
 ```



## Troubleshooting

- **"No such file or directory: /etc/kailian/kailian.conf"**: Ensure the configuration file exists and is correctly formatted. For debug buil
ds, place it in the current directory.
- **Connection Errors:** Verify that Ollama is running on `http://localhost:11434` (use `curl http://localhost:11434`).

- **Build Failures**: Confirm all dependencies are installed (`libcurl`, `cJSON`, etc.). Check `LDFLAGS` in the Makefile if linking fails.
- **Permission Denied**: Use `sudo` for release builds to install to `/usr/local/bin` and `/etc/kailian`.
