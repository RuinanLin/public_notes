# Flags
CFLAGS = -Wall -g

# Source files and target executables
SRCS := $(wildcard *.c)
TARGETS := $(patsubst %.c, build/%, $(SRCS))

# Default rule
all: $(TARGETS)

# Rule to build each executable in build/
build/%: %.c | build
	$(CC) $(CFLAGS) -o $@ $<

# Ensure build directory exists
build:
	mkdir -p build

# Clean up
clean:
	rm -rf build

.PHONY: all clean
