# Small C Projects

This repository contains a collection of small C projects created to
learn and practice low-level programming concepts.

Each directory is a standalone project focusing on specific topics such as:
- memory management
- pointers
- data structures
- file I/O
- CLI tools

The projects are intentionally small and focused.

## Projects

### 📇 [Contact Manager](contact-manager/)
A command-line contact management system with CRUD operations, search functionality, and persistent storage using linked lists.

**Concepts:** Linked lists, dynamic memory, file I/O, string manipulation

### 🔐 [Password Generator](password-generator/)
Generate secure passwords with customizable length and character sets (uppercase, lowercase, digits, symbols).

**Concepts:** Random number generation, string manipulation, command-line parsing, structures

### ✅ [Todo CLI](todo-cli/)
A simple command-line todo list manager to track tasks with completion status.

**Concepts:** Arrays, file I/O, command-line arguments, structs

## Features

All projects include:
- ✨ **Colorful output** - ANSI color codes for better readability
- 🔧 **Cross-platform** - Works on macOS, Linux, and Windows
- 📝 **Well-documented** - Each project has its own README with examples
- 🎯 **Focused scope** - Small, manageable projects perfect for learning

## Building

Each project can be built independently using Make (recommended) or GCC directly:

### Using Make (Recommended)
```bash
# Contact Manager
cd contact-manager && make

# Password Generator
cd password-generator && make

# Todo CLI
cd todo-cli && make
```

### Using GCC directly
```bash
# Contact Manager
cd contact-manager
gcc -Wall -Wextra main.c -o contact

# Password Generator
cd password-generator
gcc -Wall -Wextra main.c -o password

# Todo CLI
cd todo-cli
gcc -Wall -Wextra main.c -o todo
```

### Make targets available in all projects
- `make` or `make all` - Build the project
- `make clean` - Remove compiled binaries
- `make rebuild` - Clean and rebuild
- `make help` - Show available targets

## Contributing

These are learning projects. Feel free to fork, modify, and experiment!
