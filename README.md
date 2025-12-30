# Small C Projects

This repository contains a collection of small C projects created to
learn and practice low-level programming concepts.

Each directory is a standalone project focusing on specific topics such as:
- memory management
- pointers
- data structures
- file I/O
- CLI tools

The projects are small and focused.

## Projects

### [Contact Manager](contact-manager/)
A command-line contact management system with CRUD operations, search functionality, and persistent storage using linked lists.

**Concepts:** Linked lists, dynamic memory, file I/O, string manipulation

### [Password Generator](password-generator/)
Generate secure passwords with customizable length and character sets (uppercase, lowercase, digits, symbols).

**Concepts:** Random number generation, string manipulation, command-line parsing, structures

### [Todo CLI](todo-cli/)
A simple command-line todo list manager to track tasks with completion status.

**Concepts:** Arrays, file I/O, command-line arguments, structs

### [Key-Value Store](key-value-store/)
A persistent key-value store implementation using hash tables with collision handling and binary file storage.

**Concepts:** Hash tables, hash functions, collision resolution (chaining), binary file I/O, data structures

### [HTTP Server](http-server/)
A simple HTTP/1.1 server implementation using POSIX sockets with static file serving capabilities.

**Concepts:** Socket programming, network protocols, HTTP parsing, system calls, file I/O over network

## Building

Each project can be built independently using Make or GCC directly:

### Using Make
```bash
# Contact Manager
cd contact-manager && make

# Password Generator
cd password-generator && make

# Todo CLI
cd todo-cli && make

# Key-Value Store
cd key-value-store && make

# HTTP Server
cd http-server && make
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

# Key-Value Store
cd key-value-store
gcc -Wall -Wextra -std=c11 main.c -o kvstore

# HTTP Server
cd http-server
gcc -Wall -Wextra -std=c11 main.c -o http-server
```

### Make targets available in all projects
- `make` or `make all` - Build the project
- `make clean` - Remove compiled binaries
- `make rebuild` - Clean and rebuild
- `make help` - Show available targets

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Contributing

These are learning projects. Feel free to fork, modify, and experiment!
