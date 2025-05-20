CC = gcc
CFLAGS = -Wall -Wextra -Iinclude
SRC = src/main.c src/student.c src/extra.c
OBJ = build/main.o build/student.o build/extra.o
BIN = bin/sms

all: $(BIN)

$(BIN): $(OBJ)
	@mkdir -p bin
	$(CC) $(OBJ) -o $(BIN)

build/%.o: src/%.c
	@mkdir -p build
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf build bin

.PHONY: all clean

