# What is "kailian"?

"kailian" is a command-line interface (CLI) written in C that provides an API for interacting with ollama via the cli. Ollama is a popular open-source tool for running large language models (LLMs) locally on your machine. 



# Kailian Project

This repository contains the source code for the Kailian application. It is written in C and requires some prerequisites to build. The Makefile provided automates the build process, including compilation, installation
, and cleaning.

## Table of Contents
1. [Prerequisites](#prerequisites)
2. [Installation](#installation)
3. [Building the Project](#building-the-project)
4. [Using the Application](#using-the-application)
5. [Troubleshooting](#troubleshooting)

---

## Prerequisites

Before building the project, ensure you have the following installed:

- **C Compiler (GCC)**
- **Make工具**
- **cURL Development Libraries**: `libcurl-dev` or similar package for your OS
- **JSON Development Libraries**: `libcjson-dev` or similar package for your OS
- **Logger Library**: Ensure you have the logger library installed

On Ubuntu/Debian, install these dependencies using:

```bash
sudo apt-get update && sudo apt-get install gcc make libcurl-dev libcjson-dev
```

---

## Installation

1. Clone this repository:
   ```bash
   git clone https://github.com/yourusername/kailian.git
   cd kailian
   ```

2. Build the project by running:
   ```bash
   make all
   ```

3. Install the built files (for release mode):
   ```bash
   make install
   ```

---

## Building the Project

The Makefile provides several targets to manage the build process:

### Targets

- `make all`: Compiles the project in the specified build mode (debug by default).
- `make debug`: Builds the project in debug mode with debugging symbols.
- `make release`: Builds the project in release mode and installs it.
- `make install`: Installs the built files in the target directory.
- `make clean`: Removes all generated files, including object files and build directories.
- `make rebuild`: Cleans and rebuilds the project.

### Build Modes

The project supports two build modes:

1. **Debug Mode**:
   - Enables debugging symbols.
   - Places the output in `build/debug`.
   - Uses a development-friendly configuration directory (`etc/kailian`).

2. **Release Mode**:
   - Optimizes the code for performance.
   - Places the output in `/bin`.
   - Uses a system-wide configuration directory (`/etc/kailian`).

To switch between modes, use:

```bash
make BUILD_MODE=debug  # Forces debug mode
make BUILD_MODE=release # Forces release mode
```

---

## Using the Application

### Overview

Kailian is a command-line application that provides functionality to interact with various services. It reads configuration from `kailian.conf` by default.

### Configuration File

The application expects a configuration file named `kailian.conf` in the directory:

- Debug Mode: `etc/kailian/`
- Release Mode: `/etc/kailian/`

Example configuration file:

```bash
# Sample kailian.conf
OPTION1=value1
OPTION2=value2
```

### Usage

After building and installing, you can run the application as follows:

#### In Debug Mode:
```bash
./bin/kailian --config etc/kailian/kailian.conf
```

#### In Release Mode:
```bash
kailian --config /etc/kailian/kailian.conf
```

### Command-Line Arguments

The application supports basic command-line arguments, which are documented in the source code. Use `-h` or `--help` to view available options:

```bash
kailian -h
```

---

## Troubleshooting

### Common Issues

1. **Missing Dependencies**:
   Ensure all required libraries (cURL, cJSON, logger) are installed.

2. **Permission Denied During Installation**:
   Run the Makefile with superuser privileges:
   ```bash
   sudo make install
   ```

3. **Build Errors**:
   Check the error messages for missing files or compilation issues.
   If you encounter library-related errors, verify the installation of dependencies.

### Reporting Issues

If you encounter any bugs or issues, please open an issue on the [GitHub Issue Tracker](https://github.com/yourusername/kailian/issues).

---

## Acknowledgments

- Thanks to the maintainers of cURL and cJSON for their excellent libraries.
- Special thanks to the contributors who helped improve this project.

--- 

