# Key-Value Store

A persistent key-value store implementation in C using hash tables. This project demonstrates:
- Hash table data structure with collision handling
- Hash function implementation (djb2 algorithm)
- Persistent storage with binary file I/O
- Linked list chaining for collision resolution
- Memory management and data structures

## Features

- **Set/Get/Delete operations** - Store, retrieve, and remove key-value pairs
- **Persistent storage** - Data is automatically saved to disk (`kvstore.dat`)
- **Hash table implementation** - Fast O(1) average case lookups
- **Collision handling** - Uses chaining (linked lists) to handle hash collisions
- **List all entries** - View all stored key-value pairs
- **Clear store** - Remove all entries at once
- **Entry count** - See how many entries are stored

## Building

### Using Make (Recommended)
```bash
make
```

### Using GCC directly
```bash
gcc -Wall -Wextra -std=c11 main.c -o kvstore
```

### Other Make targets
```bash
make clean    # Remove compiled binaries and data file
make rebuild  # Clean and rebuild
make help     # Show available targets
```

## Usage

```bash
# Set a key-value pair
./kvstore set name "John Doe"
./kvstore set age 30
./kvstore set city "New York"

# Get a value
./kvstore get name

# List all entries
./kvstore list

# Delete a key
./kvstore delete age

# Clear all entries
./kvstore clear

# Count entries
./kvstore count

# Show help
./kvstore help
```

## Examples

```bash
# Store user information
./kvstore set username "johndoe"
./kvstore set email "john@example.com"
./kvstore set role "developer"

# Retrieve information
./kvstore get username
./kvstore get email

# List everything
./kvstore list

# Update a value (just set it again)
./kvstore set role "senior developer"

# Remove an entry
./kvstore delete email
```

## Technical Details

### Hash Table
- **Size**: 101 buckets (prime number for better distribution)
- **Hash Function**: djb2 algorithm (fast and well-distributed)
- **Collision Resolution**: Separate chaining with linked lists

### Storage Format
- Binary file format for efficiency
- Stores key length, key, value length, value for each entry
- Automatically loads on startup and saves on modifications

### Limitations
- Maximum key length: 255 characters
- Maximum value length: 1023 characters
- Fixed hash table size (101 buckets)

## Learning Concepts

- **Hash Tables**: Understanding hash functions and collision resolution
- **Data Structures**: Implementing hash tables with chaining
- **Binary File I/O**: Reading and writing structured binary data
- **Memory Management**: Proper allocation and deallocation
- **Algorithm Design**: Hash function selection and collision handling
- **Linked Lists**: Used for collision chaining

## Performance

- **Average case**: O(1) for get, set, delete operations
- **Worst case**: O(n) if all keys hash to the same bucket (unlikely with good hash function)
- **Storage**: Efficient binary format, loads/saves only on modifications

