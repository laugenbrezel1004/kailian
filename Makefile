# Compiler and flags
CC = gcc
CFLAGS = -std=c11 -Wall -Iinclude
LDFLAGS = -lcjson -lcurl  # Link cJSON and libcurl

# Directories
SRCDIR = src
ARGDIR = src/arguments
BINDIR = build
CONFDIR = /etc/kailian
INSTALLDIR = /usr/bin

# Source files
SOURCES = $(wildcard $(SRCDIR)/*.c) $(wildcard $(ARGDIR)/*.c)
OBJECTS = $(patsubst $(SRCDIR)/%.c,$(BINDIR)/%.o,$(SOURCES:$(ARGDIR)/%.c=$(BINDIR)/%.o))

# Target executable
TARGET = $(BINDIR)/Kailian

# Config file
CONFIG = kailian.conf

# Default target
all: $(TARGET)

# Link object files into executable
$(TARGET): $(OBJECTS)
	@mkdir -p $(BINDIR)
	$(CC) $(OBJECTS) -o $(TARGET) $(LDFLAGS)

# Compile source files to object files from src/
$(BINDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(BINDIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Compile source files to object files from src/arguments/
$(BINDIR)/%.o: $(ARGDIR)/%.c
	@mkdir -p $(BINDIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Install target
install: $(TARGET)
	@install -d $(INSTALLDIR)
	@install -m 755 $(TARGET) $(INSTALLDIR)/Kailian
	@install -d $(CONFDIR)
	@install -m 644 $(CONFIG) $(CONFDIR)/kailian.conf
	@echo "Installed to $(INSTALLDIR)/Kailian and $(CONFDIR)/kailian.conf"

# Clean target
clean:
	rm -rf $(BINDIR)

# Phony targets
.PHONY: all install clean
