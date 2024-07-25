CC = gcc
TARGET = reed

# Compilation flags
FLAGS = -O2 -flto #-fsanitize=undefined #-pg

# Define the source files, the object files and dependencies
SRC = $(wildcard src/*.c src/*/*.c)
OBJ = $(patsubst %.c, build/%.o, $(SRC))
DEPS = $(patsubst %.c, build/%.d, $(SRC))

# Default rule to build the program
all: $(TARGET)

# Rule to link the object files and create the executable
$(TARGET): $(OBJ)
	$(CC) $(FLAGS) -o $(TARGET) $(OBJ)

# Rule to compile the source files into object files
build/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(FLAGS) -MMD -MP -c $< -o $@

-include $(DEPS)

# Rule to clean up files
clean:
	rm -f $(TARGET) $(OBJ)
	rm -rf build
	rm -f errorRec.bin fixed.bin corrupted.bin original.bin

# PHONY targets to avoid conflicts with files named 'all' or 'clean'
.PHONY: all clean
