# Quellverzeichnisse
SRC_DIRS = src src/arguments src/core

# Quelldateien
SRCS = $(wildcard src/*.c) \
       $(wildcard src/arguments/*.c) \
       $(wildcard src/core/*.c)

# Objektdateien
OBJS = $(patsubst src/%.c,build/%.o,$(SRCS))

# Verzeichnisregeln
$(shell mkdir -p build/core build/arguments)

# Kompilierungsregeln
build/core/%.o: src/core/%.c
	$(CC) $(CFLAGS) -c $< -o $@

build/arguments/%.o: src/arguments/%.c
	$(CC) $(CFLAGS) -c $< -o $@

build/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Hauptziel
build/Kailian: $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)