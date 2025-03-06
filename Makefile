# Compiler und Flags
CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c11
LDFLAGS = -lcurl -lcjson -llogger -lcmark

# Build-Modus (Debug oder Release)
BUILD_MODE ?= debug

ifeq ($(BUILD_MODE), debug)
    CFLAGS += -g
    BUILD_DIR = build/debug
    INSTALL_DIR = bin
    CONF_DIR = etc/kailian
else ifeq ($(BUILD_MODE), release)
    CFLAGS += -O2
    BUILD_DIR = build/release
    INSTALL_DIR = /bin
    CONF_DIR = /etc/kailian
else
    $(error Unbekannter BUILD_MODE: $(BUILD_MODE). Verwenden Sie 'debug' oder 'release'.)
endif

# Pfade
SRC_DIR = src
INCLUDE_DIR = include
BIN_DIR = bin

# Ziel-Binärdatei
TARGET = $(INSTALL_DIR)/kailian
CONF_FILE = kailian.conf
CONF_TARGET = $(CONF_DIR)/$(CONF_FILE)

# Automatische Erkennung der Quell- und Header-Dateien
SRC_FILES := $(wildcard $(SRC_DIR)/*.c) $(wildcard $(SRC_DIR)/arguments/*.c)
HEADER_FILES := $(wildcard $(INCLUDE_DIR)/*.h)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRC_FILES))
OBJ_FILES += $(patsubst $(SRC_DIR)/arguments/%.c,$(BUILD_DIR)/arguments/%.o,$(wildcard $(SRC_DIR)/arguments/*.c))

# Standardziel
all: $(TARGET)

# Installation im Release-Modus (inkl. Konfigurationsdatei)
install: $(TARGET) $(CONF_TARGET)

# Kompilieren der Haupt-Binärdatei
$(TARGET): $(OBJ_FILES)
	echo "Linking: $@"
	mkdir -p $(INSTALL_DIR)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

# Kopieren der Konfigurationsdatei
$(CONF_TARGET): $(CONF_FILE)
	echo "Installing config: $@"
	mkdir -p $(CONF_DIR)
	cp $(CONF_FILE) $(CONF_TARGET)

# Kompilieren einzelner Objektdateien
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	echo "Compiling: $< -> $@"
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) -c $< -o $@

$(BUILD_DIR)/arguments/%.o: $(SRC_DIR)/arguments/%.c | $(BUILD_DIR)/arguments
	echo "Compiling: $< -> $@"
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) -c $< -o $@

# Verzeichnisse erstellen (falls nicht vorhanden)
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/arguments:
	mkdir -p $(BUILD_DIR)/arguments

# Bereinigen des Projekts
clean:
	echo "Cleaning build files..."
	rm -rf build $(BIN_DIR)
ifeq ($(BUILD_MODE), release)
	echo "Removing installed files..."
	rm -f $(TARGET)
	rm -f $(CONF_TARGET)
endif

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

