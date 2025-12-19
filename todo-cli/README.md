# Todo CLI

A simple command-line todo list manager written in C.

## Concepts practiced
- structs
- dynamic memory
- file I/O
- command-line arguments

## Building

### Using Make (Recommended)
```bash
make
```

### Using GCC directly
```bash
gcc -Wall -Wextra main.c -o todo
```

### Other Make targets
```bash
make clean    # Remove compiled binaries
make rebuild  # Clean and rebuild
make help     # Show available targets
```
