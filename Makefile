# flags
CC = cc
CPPFLAGS = -Isrc/include
CFLAGS ?= -Wall -Wextra -MMD -MP
CFLAGS += -DSYSCONFDIR=\"/etc\"
LDFLAGS = -Llibs
LDLIBS = -lm

# source files
SRC := $(wildcard src/*.c)

# object files (mirrored under build/)
OBJ := $(SRC:src/%.c=build/%.o)

# avoid weird edge cases, if a file named clean ever appears.
.PHONY: clean build

# final executable
build/homed: $(OBJ) | build
	$(CC) $(OBJ) $(LDFLAGS) $(LDLIBS) -o $@

# ensure build directory exists
build:
	mkdir -p build

# compile .c → .o
build/%.o: src/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

# clean build artifacts
clean:
	rm -rf build

# include auto-generated header dependencies
-include $(OBJ:.o=.d)
