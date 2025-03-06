# Compiler und Flags
CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c11
LDFLAGS = -lcurl -lcjson -llogger

# Build-Modus (Debug oder Release)
BUILD_MODE ?= release

ifeq ($(BUILD_MODE), debug)
    CFLAGS += -g
    BUILD_DIR = build/debug
    INSTALL_DIR = bin
    CONF_DIR = etc/kailian
else ifeq ($(BUILD_MODE), release)
    CFLAGS += -O2
    BUILD_DIR = build/release
    INSTALL_DIR = /usr/local/bin
    CONF_DIR = /etc/kailian
endif

# Pfade
SRC_DIR = src
INCLUDE_DIRS = include/ $(SRC_DIR)/arguments/
BIN_DIR = bin

# Ziel-Binärdatei
TARGET = $(INSTALL_DIR)/kailian
CONF_FILE = kailian.conf
CONF_TARGET = $(CONF_DIR)/$(CONF_FILE)

# Automatische Erkennung der Quell- und Header-Dateien
SRC_FILES := $(shell find $(SRC_DIR) -type f -name "*.c")
OBJ_FILES := $(patsubst %.c, $(BUILD_DIR)/%.o, $(SRC_FILES))

# Verbesserte Include-Verwaltung
INCLUDE_FLAGS = $(foreach dir,$(INCLUDE_DIRS),-I$(dir))

# Standardziel
all: $(TARGET)

# Installation im Release-Modus (inkl. Konfigurationsdatei)
install: $(TARGET) $(CONF_TARGET)

# Kompilieren und Linken der Haupt-Binärdatei
$(TARGET): $(OBJ_FILES)
        echo "Linking: $@"
        $(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# Kopieren der Konfigurationsdatei
$(CONF_TARGET): $(CONF_FILE)
        echo "Installing config: $@"
        mkdir -p $(dir $(CONF_TARGET))
        cp -f $(CONF_FILE) $(CONF_TARGET)

# Kompilieren einzelner Objektdateien
$(BUILD_DIR)/%.o: %.c | $(BUILD_DIR)
        echo "Compiling: $< -> $@"
        $(CC) $(CFLAGS) $(INCLUDE_FLAGS) -c $< -o $@

# Verzeichnisse erstellen (falls nicht vorhanden)
$(BUILD_DIR):
        mkdir -p $(BUILD_DIR)

# Bereinigen des Projekts
clean:
        echo "Cleaning build files..."
        rm -rf build bin

# Bereinigen und neu bauen
rebuild: clean all

# Hilfsziel für Debug-Builds
debug:
        $(MAKE) BUILD_MODE=debug

# Hilfsziel für Release-Builds
release:
        $(MAKE) BUILD_MODE=release clean install

# Phony-Ziele
.PHONY: all clean rebuild debug release install

# Uninstall Zielführung
uninstall: $(TARGET) $(CONF_TARGET)
        echo "Uninstalling..."
        rm -f $(TARGET)
        rm -f $(CONF_TARGET)

