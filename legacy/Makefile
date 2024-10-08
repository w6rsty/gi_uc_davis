CC = clang
CFLAGS = -std=c11 -O3 -g -Wall -Wextra -Wpedantic -Wstrict-aliasing
CFLAGS += -Wno-pointer-arith -Wno-newline-eof -Wno-unused-parameter -Wno-gnu-statement-expression
CFLAGS += -Wno-gnu-compound-literal-initializer -Wno-gnu-zero-variadic-macro-arguments
LDFLAGS = -lm

SRC = $(wildcard src/*.c)
OBJ = $(SRC:.c=.o)
BIN = bin

.PHONY: all clean

all: dirs gi

dirs:
    mkdir -p ./bin

run: all
	$(BIN)/gi

gi: $(OBJ)
	$(CC) -o $(BIN)/gi $^ $(LDFLAGS)

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	rm -rf $(BIN) $(OBJ)