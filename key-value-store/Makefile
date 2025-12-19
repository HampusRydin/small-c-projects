# Makefile for Key-Value Store

# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c11
TARGET = kvstore
SOURCE = main.c

# Default target
all: $(TARGET)

# Build the executable
$(TARGET): $(SOURCE)
	$(CC) $(CFLAGS) $(SOURCE) -o $(TARGET)
	@echo "✓ Built $(TARGET) successfully"

# Clean build artifacts
clean:
	rm -f $(TARGET) $(TARGET).exe kvstore.dat
	@echo "✓ Cleaned build artifacts"

# Rebuild from scratch
rebuild: clean all

# Install (copy to a directory in PATH, optional)
install: $(TARGET)
	@echo "To install, copy $(TARGET) to a directory in your PATH"
	@echo "Example: cp $(TARGET) /usr/local/bin/"

# Help target
help:
	@echo "Available targets:"
	@echo "  make          - Build the project (default)"
	@echo "  make clean    - Remove compiled binaries and data file"
	@echo "  make rebuild  - Clean and rebuild"
	@echo "  make help     - Show this help message"

.PHONY: all clean rebuild install help

