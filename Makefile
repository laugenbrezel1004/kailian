
# Compiler und Flags
CC = gcc
CFLAGS = -Wall -g
LDFLAGS = -lcurl -lcjson

# Pfade
PATH_HEADER = ./src/lib
PATH_SRC = ./src/
PATH_BIN = ./bin

# Ziel-Binärdatei
TARGET = $(PATH_BIN)/kailian

# Quell- und Objektdateien
SRC_FILES = main.c $(PATH_SRC)/api.c
OBJ_FILES = $(SRC_FILES:.c=.o)

# Standardziel
all: $(TARGET)

# Kompilieren der Haupt-Binärdatei
$(TARGET): $(OBJ_FILES)
	@mkdir -p $(PATH_BIN)
	$(CC) $(CFLAGS) -I$(PATH_HEADER) $(OBJ_FILES) -o $(TARGET) $(LDFLAGS)

# Kompilieren einzelner Objektdateien
%.o: %.c
	$(CC) $(CFLAGS) -I$(PATH_HEADER) -c $< -o $@

# Bereinigen des Projekts
clean:
	rm -rf $(OBJ_FILES) $(TARGET)

