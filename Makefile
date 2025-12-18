CC = cc
CFLAGS = -I src/include -Wall -Wextra
LDFLAGS = -L libs -lm

SRC = $(wildcard src/*.c)
OBJ = $(SRC:.c=.o)

# final executable
build/homed: build $(OBJ)
		$(CC) $(OBJ) $(LDFLAGS) -o $@

# ensure build directory exists
build:
		mkdir -p build

# compile .c → .o
src/%.o: src/%.c
		$(CC) $(CFLAGS) -c $< -o $@

clean:	