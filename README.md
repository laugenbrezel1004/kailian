# What is "kailian"?

"kailian" is a command-line interface (CLI) written in C that provides an API for interacting with ollama via the cli. Ollama is a popular open-source tool for running large language models (LLMs) locally on your machine. 

# Kailian Project

A C-based project that utilizes cURL and cJSON libraries for HTTP requests and JSON handling.

## Features

- **Modular Build System**: Supports both Debug and Release builds with automatic configuration based on `BUILD_MODE`.
- **Dependency Management**: Handles external libraries (cURL, cJSON) through linker flags.
- **Configuration Handling**: Includes a configuration file (`kailian.conf`) that can be installed to system or user directories based on bui
ld mode.
- **Clean Build Process**: Uses Make targets for building, installing, cleaning, and uninstalling.

## Prerequisites

### Dependencies
- GCC compiler
- cURL library
- cJSON library
- liblogger (for logging functionality)

### Installation Steps

1. Clone the repository:
```bash
git clone https://github.com/yourusername/kailian.git
cd kailian
```

2. Install dependencies if needed:
```bash
sudo apt-get install gcc curl libcurl-dev libcjson-dev
```

3. Build and install in Debug mode:
```bash
make debug
```

Or build and install in Release mode (recommended for production):
```bash
make release
```

## Usage

### Building the Project

- To build without installing:
```bash
make all
```

- To rebuild from scratch:
```bash
make clean && make all
```

### Installing the Project

- Install to default directories (/usr/local/bin, /etc/kailian) in Release mode:
```bash
make install
```

- Install in Debug mode (installs to local bin directory):
```bash
make debug install
```

### Running the Project

Run the compiled binary from its installation location. For example, in Debug mode:
```bash
./bin/kailian
```

In Release mode:
```bash
/usr/local/bin/kailian
```

## Configuration File

The project includes a configuration file (`kailian.conf`) that needs to be installed alongside the binary.

- In Debug mode:
```bash
cp kailian.conf bin/etc/kailian/
```

- In Release mode:
```bash
sudo cp kailian.conf /etc/kailian/
```

## Build Targets

Here are some of the key Make targets you might find useful:

| Target      | Description |
|-------------|-------------|
| `all`       | Compiles the project without installing. |
| `install`   | Installs the compiled binary and configuration file to target directories based on BUILD_MODE. |
| `clean`     | Removes all build artifacts. |
| `rebuild`   | Cleans and rebuilds everything from scratch. |
| `debug`     | Switches to Debug mode and runs `make all`. |
| `release`   | Switches to Release mode, cleans, builds, and installs the project. |
| `uninstall` | Uninstalls the previously installed binary and configuration file. |

## Development

### Contributing

- Fork the repository.
- Create a feature branch.
- Commit your changes with clear commit messages.
- Push to the branch and create a Pull Request.

### Testing

The project should include unit tests for better reliability. You can run tests using:
```bash
make test
```

