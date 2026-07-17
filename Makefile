CC = gcc
CFLAGS = -std=c89 -pedantic -Wall -Wextra -O2 -Iinclude
LDFLAGS = -lm
TARGET = causal_game
OBJ = src/main.o src/graph.o src/io.o src/game.o src/dynamics.o

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET) $(LDFLAGS)

src/%.o: src/%.c include/model.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)

run: $(TARGET)
	./$(TARGET) data/graph.txt data/config.txt

.PHONY: all clean run
