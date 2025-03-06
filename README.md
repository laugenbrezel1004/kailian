# What is "kailian"?

"kailian" is a command-line interface (CLI) written in C that provides an API for interacting with ollama via the cli. Ollama is a popular open-source tool for running large language models (LLMs) locally on your machine. 




 # Kailian Project Makefile

This project uses a Makefile to build and manage its source files. Here's how to use 
the provided Makefile to compile and run this project:

## Compiler and Flags
- The default compiler is `gcc`.
- The C flags used are `-Wall -Wextra -pedantic -std=c11` for better code quality.
- Additional libraries `-lcurl`, `-lcjson`, and `-llogger` are linked.
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
- The compiled binary is placed in the `bin` directory for debug builds or system directories for release builds (`/bin` and `/etc/kailian`).
- Configuration files are stored in the `etc/kailian` directory during a release build, and in the project root for debug builds.

## Target
The final compiled binary is named `kailian`.

## Build Commands
By default, running `make` will compile the project for a debug build and create the binary file `bin/kailian`. If you want to create the release build, use `make release`. To install the compiled binary in the appropriate system directory (for release builds only), run `make install`.


If you wish to clean the build files or rebuild the project from scratch, use `make clean` or `make rebuild`, respectively. To specifically build for debug or release mode without building anything else, use `make debug` or `make release`, respectively.

## Notes
- The Makefile uses wildcards and pattern rules to automatically detect source files 
during the build process.
- The project will fail if an unrecognized BUILD_MODE is specified (e.g., using `make
 BUILD_MODE=unknown`). Use either `debug` or `release`.

