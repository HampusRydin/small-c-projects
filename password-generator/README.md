# Password Generator

A command-line password generator written in C. Covers:
- Random number generation
- String manipulation and character set building
- Command-line argument parsing
- Configuration structures
- Input validation

## Features

- **Customizable length** - Generate passwords from 4 to 128 characters
- **Character set control** - Include/exclude uppercase, lowercase, digits, and symbols
- **Multiple modes** - Generate full passwords or numeric PINs
- **Secure random generation** - Uses time-based seeding for randomness

## Building

### Using Make
```bash
make
```

### Using GCC directly
```bash
gcc -Wall -Wextra main.c -o password
```

### Other Make targets
```bash
make clean    # Remove compiled binaries
make rebuild  # Clean and rebuild
make help     # Show available targets
```

## Usage

```bash
# Generate a default 16-character password with all character types
./password

# Generate a 20-character password
./password -l 20

# Generate a password without symbols
./password -l 12 -S

# Generate a 6-digit numeric PIN
./password -n -l 6

# Generate a password with only letters (no digits or symbols)
./password -D -S

# Show help
./password --help
```

## Options

- `-l, --length N` - Set password length (default: 16, min: 4, max: 128)
- `-u, --uppercase` - Include uppercase letters (default: yes)
- `-L, --no-uppercase` - Exclude uppercase letters
- `-d, --digits` - Include digits (default: yes)
- `-D, --no-digits` - Exclude digits
- `-s, --symbols` - Include symbols (default: yes)
- `-S, --no-symbols` - Exclude symbols
- `-a, --all` - Include all character types (default)
- `-n, --numbers-only` - Generate numeric password only
- `-h, --help` - Show help message

## Learning Concepts

- **Random Number Generation**: Using `rand()` and `srand()` with time-based seeding
- **String Operations**: Building character sets dynamically with `strcat()`
- **Command-line Parsing**: Processing flags and options from `argv`
- **Structures**: Using `struct` to organize configuration data
- **Input Validation**: Checking length constraints and option combinations

## Character Sets

- **Lowercase**: a-z (26 characters)
- **Uppercase**: A-Z (26 characters)
- **Digits**: 0-9 (10 characters)
- **Symbols**: !@#$%^&*()_+-=[]{}|;:,.<>? (20 characters)

