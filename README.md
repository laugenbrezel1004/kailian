# What is "kailian"?

"kailian" is a command-line interface (CLI) written in C that provides an API for interacting with ollama via the cli. Ollama is a popular open-source tool for running large language models (LLMs) locally on your machine. 

# some dependencies
cJSON
c-logger
cmark





 # Kailian Project Makefile

This project uses a Makefile to build and manage its source files. Here's how to use 
the provided Makefile to compile and run this project:

## Compiler and Flags
- The default compiler is `gcc`.
- The C flags used are `-Wall -Wextra -pedantic -std=c11` for better code quality.
- Additional libraries `-lcurl^H`, `-lcjson`, and `-llogger` are linked.
- To build in debug mode, use `-g` flag for debugging information. In release mode, u
se the `-O2` flag for optimized performance.

## Build Modes (Debug or Release)
You can choose to build either in **debug** or **release** mode by specifying the app
ropriate BUILD_MODE variable before running make commands:

```sh
$ make BUILD_MODE=debug        # Build in debug mode
$ make BUILD_MODE=release     # Build in release mode (optimized)
```

## Directories and Files
- Source files are located in the `src` directory.
- Header files are found in the `include` directory.
- The compiled binary is placed in the `bin` directory for debug builds or system dir
ectories for release builds (either `/bin` or `/etc/kailian`).
- Configuration files are stored in the `etc/kailian` directory during a release buil
d, and in the project root for debug builds.

## Target
The final compiled binary is named `kailian`.

## Build Commands
By default, running `make` will compile the project for a
