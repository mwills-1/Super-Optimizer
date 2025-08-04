CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -O2 -g
LDFLAGS = -ldl -lffi

# Try to find libffi installation
LIBFFI_PATH := $(shell brew --prefix libffi 2>/dev/null || echo "")
ifneq ($(LIBFFI_PATH),)
    CFLAGS += -I$(LIBFFI_PATH)/include
    LDFLAGS += -L$(LIBFFI_PATH)/lib
endif

# Source files
SOURCES = main.c dynamic_assembly.c execute.c load.c
OBJECTS = $(SOURCES:.c=.o)
TARGET = assembly_runner

# Default target
all: $(TARGET)

# Build the executable
$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# Compile source files
%.o: %.c %.h
	$(CC) $(CFLAGS) -c -o $@ $<

# Clean build artifacts
clean:
	rm -f $(OBJECTS) $(TARGET)

# Run with test arguments
test: $(TARGET)
	./$(TARGET) math.s add_numbers int 5 3

# Run with custom arguments
# Usage: make run ASSEMBLY=math.s FUNC=_add_numbers TYPE=int ARGS="5 3"
run: $(TARGET)
	@if [ -z "$(ASSEMBLY)" ] || [ -z "$(FUNC)" ] || [ -z "$(TYPE)" ] || [ -z "$(ARGS)" ]; then \
		echo "Usage: make run ASSEMBLY=<file.s> FUNC=<function_name> TYPE=<result_type> ARGS=\"<arg1> <arg2> ...\""; \
		echo "Example: make run ASSEMBLY=math.s FUNC=_add_numbers TYPE=int ARGS=\"5 3\""; \
		exit 1; \
	fi
	./$(TARGET) $(ASSEMBLY) $(FUNC) $(TYPE) $(ARGS)

# Interactive mode
interactive: $(TARGET)
	@echo "Assembly Function Runner"
	@echo "======================="
	@echo "Enter assembly file (e.g., math.s):"
	@read -p "> " assembly_file; \
	echo "Enter function name (e.g., _add_numbers):"
	@read -p "> " func_name; \
	echo "Enter result type (int, float, double, etc.):"
	@read -p "> " result_type; \
	echo "Enter arguments (space-separated):"
	@read -p "> " args; \
	./$(TARGET) $$assembly_file $$func_name $$result_type $$args

# Help
help:
	@echo "Available targets:"
	@echo "  all         - Build the assembly runner"
	@echo "  test        - Run with test arguments (math.s _add_numbers int 5 3)"
	@echo "  run         - Run with custom arguments"
	@echo "  interactive - Interactive mode"
	@echo "  clean       - Remove build artifacts"
	@echo "  help        - Show this help message"
	@echo ""
	@echo "Run examples:"
	@echo "  make run ASSEMBLY=math.s FUNC=_add_numbers TYPE=int ARGS=\"5 3\""
	@echo "  make run ASSEMBLY=multiply.s FUNC=_multiply TYPE=int ARGS=\"4 5\""
	@echo "  make interactive"
	@echo ""
	@echo "Direct usage:"
	@echo "  ./assembly_runner math.s _add_numbers int 5 3"
	@echo "  ./assembly_runner multiply.s _multiply int 4 5"

# Install dependencies (for macOS)
install-deps:
	brew install libffi

.PHONY: all test run interactive clean help install-deps