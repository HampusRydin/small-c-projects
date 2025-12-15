# Contact Manager

A command-line contact management system written in C. This project demonstrates:
- Linked list data structures
- File I/O operations
- CRUD (Create, Read, Update, Delete) operations
- String manipulation and searching
- Dynamic memory management

## Features

- **Add contacts** with name, email, and phone number
- **List all contacts** in a formatted view
- **Search contacts** by name, email, or phone number
- **Update contacts** by index (can update individual fields)
- **Delete contacts** by index
- **Persistent storage** - contacts are saved to `contacts.txt` automatically

## Building

```bash
gcc -o contact main.c
```

## Usage

```bash
# List all contacts
./contact list

# Add a new contact
./contact add "John Doe" "john@example.com" "555-1234"

# Search for contacts
./contact search "John"

# Update a contact (use "" to skip a field you don't want to change)
./contact update 1 "Jane Doe" "jane@example.com" "555-5678"

# Delete a contact
./contact delete 1
```

## File Format

Contacts are stored in `contacts.txt` with each contact taking three lines:
- Line 1: Name
- Line 2: Email
- Line 3: Phone

The file is automatically created when you add your first contact.

## Learning Concepts

- **Linked Lists**: Dynamic data structure for storing contacts
- **Memory Management**: Proper allocation and deallocation with `malloc()` and `free()`
- **File I/O**: Reading from and writing to files for persistence
- **String Operations**: Using `strncpy()`, `strstr()` for safe string handling
- **Command-line Parsing**: Processing command-line arguments

