# HTTP Server

A simple HTTP/1.1 server implementation in C using POSIX sockets. Covers:
- Socket programming and network communication
- HTTP protocol parsing and response generation
- File serving capabilities
- MIME type detection
- Error handling and proper HTTP status codes

## Features

- **HTTP/1.1 Protocol** - Proper request parsing and response generation
- **Static File Serving** - Serve HTML, CSS, JavaScript, images, and other files
- **MIME Type Detection** - Automatic content-type headers based on file extensions
- **Error Handling** - Proper HTTP status codes (200, 404, 500, etc.)
- **Security** - Basic directory traversal protection
- **Color Logging** - Colored console output for requests

## Building

### Using Make
```bash
make
```

### Using GCC directly
```bash
gcc -Wall -Wextra -std=c11 main.c -o http-server
```

### Other Make targets
```bash
make clean    # Remove compiled binaries
make rebuild  # Clean and rebuild
make help     # Show available targets
```

## Usage

### Start the Server

```bash
# Default port (8080)
./http-server

# Custom port
./http-server 3000
```

### Access the Server

Once running, open your browser and visit:
- `http://localhost:8080` - Default page
- `http://localhost:8080/index.html` - Serve index.html file
- `http://localhost:8080/yourfile.html` - Serve any file in the server directory

### Stop the Server

Press `Ctrl+C` to stop the server.

## Examples

```bash
# Start server on default port
./http-server

# Start server on port 3000
./http-server 3000

# In another terminal or browser:
curl http://localhost:8080/
curl http://localhost:8080/index.html
```

## Supported File Types

The server automatically detects MIME types for:
- HTML: `.html`, `.htm`
- CSS: `.css`
- JavaScript: `.js`
- JSON: `.json`
- Images: `.png`, `.jpg`, `.jpeg`, `.gif`
- PDF: `.pdf`
- Plain text: default

## Technical Details

### Socket Programming
- Uses POSIX sockets (`sys/socket.h`)
- IPv4 addressing (`AF_INET`)
- TCP protocol (`SOCK_STREAM`)
- Non-blocking ready for future enhancements

### HTTP Implementation
- Parses HTTP request line (method, path, version)
- Generates proper HTTP/1.1 responses
- Includes required headers (Server, Date, Content-Type, Content-Length)
- Handles common HTTP methods (GET implemented)

### Security Features
- Directory traversal protection (blocks `..` in paths)
- File size limits (max 10MB)
- File existence checks before serving

## Learning Concepts

- **Socket Programming**: Creating, binding, listening, and accepting connections
- **Network Protocols**: Understanding HTTP request/response format
- **System Calls**: Using POSIX socket APIs (`socket()`, `bind()`, `listen()`, `accept()`, `send()`, `recv()`)
- **File I/O**: Reading and serving files over the network
- **Protocol Parsing**: Parsing HTTP requests and generating responses
- **Error Handling**: Proper HTTP status codes and error messages

## Limitations

- Single-threaded (handles one request at a time)
- Only GET method implemented
- No HTTPS support
- No directory listing
- Basic security (suitable for local development only)

## Platform Support

- macOS (tested)
- Linux (should work)
- Windows (requires WSL or MinGW with socket support)

## Example Output

```
========================================
  Simple HTTP Server Running
========================================

Server listening on http://localhost:8080
Press Ctrl+C to stop the server

[GET] / HTTP/1.1
[GET] /index.html HTTP/1.1
```

## Future Enhancements

- Multi-threading for concurrent requests
- POST/PUT/DELETE method support
- Directory listing
- CGI support
- HTTPS/TLS support
- Configuration file support
- Logging to file

