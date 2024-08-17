# Makefile

# Compiler
CC = gcc

# Compiler flags (using sdl2-config for SDL2 flags)
CFLAGS = $(shell sdl2-config --cflags)
LDFLAGS = $(shell sdl2-config --libs)

# Target executable
TARGET = sdl2_example

# Source files
SRCS = main.c

# Object files
OBJS = $(SRCS:.c=.o)

# Default rule to build the target
all: $(TARGET)

# Rule to link the executable
$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

# Rule to compile .c files to .o files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Rule to clean up the object files only
clean:
	rm -f $(OBJS)

# Rule to print a message about available commands
help:
	@echo "Usage: make [target]"
	@echo "Targets:"
	@echo "  all     - Build the executable"
	@echo "  clean   - Remove build artifacts"
