# Compiler und Flags
CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c11
LDFLAGS = -lcurl -lcjson -llogger

# Build-Modus (Debug oder Release)
BUILD_MODE ?= debug  # Standard auf Debug statt Release

# Verzeichnisse
SRC_DIR = src
INCLUDE_DIRS = include $(SRC_DIR)/arguments
BUILD_BASE = build
BIN_DIR = bin
CONF_SRC = kailian.conf

# Build-spezifische Einstellungen
ifeq ($(BUILD_MODE),debug)
    CFLAGS += -g
    BUILD_DIR = $(BUILD_BASE)/debug
    INSTALL_DIR = $(BIN_DIR)
    CONF_DIR = etc/kailian
else ifeq ($(BUILD_MODE),release)
    CFLAGS += -O2 -DNDEBUG  # -DNDEBUG deaktiviert asserts
    BUILD_DIR = $(BUILD_BASE)/release
    INSTALL_DIR = /usr/local/bin
    CONF_DIR = /etc/kailian
else
    $(error Unknown BUILD_MODE: $(BUILD_MODE). Use 'debug' or 'release')
endif

# Ziel-Dateien
TARGET = $(INSTALL_DIR)/kailian
CONF_TARGET = $(CONF_DIR)/$(CONF_SRC)

# Quell- und Objektdateien
SRC_FILES := $(shell find $(SRC_DIR) -type f -name '*.c')
OBJ_FILES := $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRC_FILES))
INCLUDE_FLAGS := $(addprefix -I,$(INCLUDE_DIRS))

# Standardziel
all: $(TARGET)

# Installation (inkl. Konfigurationsdatei)
install: $(TARGET) $(CONF_TARGET)
	@echo "Installation complete: $(TARGET) and $(CONF_TARGET)"

# Linken
$(TARGET): $(OBJ_FILES) | $(INSTALL_DIR)
	@echo "Linking: $@"
	$(CC) $(OBJ_FILES) -o $@ $(LDFLAGS)

# Konfigurationsdatei kopieren
$(CONF_TARGET): $(CONF_SRC) | $(CONF_DIR)
	@echo "Installing config: $@"
	cp -f $< $@

# Objektdateien kompilieren
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	@echo "Compiling: $< -> $@"
	$(CC) $(CFLAGS) $(INCLUDE_FLAGS) -c $< -o $@

# Verzeichnisse erstellen
$(BUILD_DIR) $(INSTALL_DIR) $(CONF_DIR):
	@mkdir -p $@

# Bereinigen
clean:
	@echo "Cleaning build files..."
	rm -rf $(BUILD_BASE) $(BIN_DIR)

# Neu bauen
rebuild: clean all

# Debug-Build
debug:
	$(MAKE) BUILD_MODE=debug

# Release-Build mit Installation
release:
	$(MAKE) BUILD_MODE=release clean install

# Deinstallation
uninstall:
	@echo "Uninstalling..."
	rm -f $(TARGET) $(CONF_TARGET)
	@if [ "$(BUILD_MODE)" = "release" ]; then rmdir --ignore-fail-on-non-empty $(CONF_DIR) 2>/dev/null || true; fi

# Phony-Ziele
.PHONY: all clean rebuild debug release install uninstall

# Abhängigkeiten automatisch generieren
DEP_FILES := $(OBJ_FILES:.o=.d)
CFLAGS += -MMD -MP
-include $(DEP_FILES)
