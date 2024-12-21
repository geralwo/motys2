# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -g -std=c17 -pedantic -Iinclude

# Source and output
SRC = src/motysc.c src/motys_stdlib.c src/lexer.c src/vm.c
OUTPUT = build/motys2

# Build target
all: $(OUTPUT)

# Ensure the build directory exists
build:
	mkdir -p build

# Linking and compilation
$(OUTPUT): build $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(OUTPUT)

# Run target
run: $(OUTPUT)
	./$(OUTPUT) tests/test.motys

# Clean target
clean:
	rm build/*
