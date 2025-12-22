#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <time.h>

#define PORT 8080
#define BUFFER_SIZE 8192
#define MAX_PATH_LEN 512

// ANSI color codes
#define COLOR_RESET   "\033[0m"
#define COLOR_RED     "\033[31m"
#define COLOR_GREEN   "\033[32m"
#define COLOR_YELLOW  "\033[33m"
#define COLOR_BLUE     "\033[34m"
#define COLOR_CYAN    "\033[36m"
#define COLOR_BOLD    "\033[1m"

// HTTP Request structure
typedef struct {
    char method[16];
    char path[MAX_PATH_LEN];
    char version[16];
    char headers[BUFFER_SIZE];
} HttpRequest;

// Parse HTTP request line
int parse_request(const char *buffer, HttpRequest *req) {
    char method[16], path[MAX_PATH_LEN], version[16];
    
    if (sscanf(buffer, "%15s %511s %15s", method, path, version) != 3) {
        return 0;
    }
    
    strncpy(req->method, method, sizeof(req->method) - 1);
    req->method[sizeof(req->method) - 1] = '\0';
    
    strncpy(req->path, path, sizeof(req->path) - 1);
    req->path[sizeof(req->path) - 1] = '\0';
    
    strncpy(req->version, version, sizeof(req->version) - 1);
    req->version[sizeof(req->version) - 1] = '\0';
    
    return 1;
}

// Get current time in HTTP format
void get_http_time(char *time_str, size_t len) {
    time_t now = time(NULL);
    struct tm *tm_info = gmtime(&now);
    strftime(time_str, len, "%a, %d %b %Y %H:%M:%S GMT", tm_info);
}

// Get MIME type based on file extension
const char* get_mime_type(const char *path) {
    const char *ext = strrchr(path, '.');
    if (!ext) return "text/plain";
    
    ext++; // Skip the dot
    
    if (strcmp(ext, "html") == 0 || strcmp(ext, "htm") == 0) return "text/html";
    if (strcmp(ext, "css") == 0) return "text/css";
    if (strcmp(ext, "js") == 0) return "application/javascript";
    if (strcmp(ext, "json") == 0) return "application/json";
    if (strcmp(ext, "png") == 0) return "image/png";
    if (strcmp(ext, "jpg") == 0 || strcmp(ext, "jpeg") == 0) return "image/jpeg";
    if (strcmp(ext, "gif") == 0) return "image/gif";
    if (strcmp(ext, "pdf") == 0) return "application/pdf";
    
    return "text/plain";
}

// Send HTTP response
void send_response(int client_fd, int status_code, const char *status_text, 
                   const char *content_type, const char *body, size_t body_len) {
    char time_str[64];
    get_http_time(time_str, sizeof(time_str));
    
    char response[BUFFER_SIZE];
    int len = snprintf(response, sizeof(response),
        "HTTP/1.1 %d %s\r\n"
        "Server: Simple-HTTP-Server/1.0\r\n"
        "Date: %s\r\n"
        "Content-Type: %s\r\n"
        "Content-Length: %zu\r\n"
        "Connection: close\r\n"
        "\r\n",
        status_code, status_text, time_str, content_type, body_len);
    
    send(client_fd, response, len, 0);
    if (body && body_len > 0) {
        send(client_fd, body, body_len, 0);
    }
}

// Send error response
void send_error(int client_fd, int status_code, const char *message) {
    char body[512];
    int len = snprintf(body, sizeof(body),
        "<!DOCTYPE html>\n"
        "<html><head><title>%d %s</title></head>\n"
        "<body><h1>%d %s</h1><p>%s</p></body></html>\n",
        status_code, message, status_code, message, message);
    
    send_response(client_fd, status_code, message, "text/html", body, len);
}

// Read file content
int read_file(const char *path, char **content, size_t *size) {
    FILE *f = fopen(path, "rb");
    if (!f) {
        return 0;
    }
    
    // Get file size
    fseek(f, 0, SEEK_END);
    long file_size = ftell(f);
    fseek(f, 0, SEEK_SET);
    
    if (file_size < 0 || file_size > 10 * 1024 * 1024) { // Max 10MB
        fclose(f);
        return 0;
    }
    
    *size = (size_t)file_size;
    *content = (char*)malloc(*size + 1);
    if (!*content) {
        fclose(f);
        return 0;
    }
    
    size_t read = fread(*content, 1, *size, f);
    fclose(f);
    
    if (read != *size) {
        free(*content);
        return 0;
    }
    
    (*content)[*size] = '\0';
    return 1;
}

// Check if file exists and is readable
int file_exists(const char *path) {
    struct stat st;
    return (stat(path, &st) == 0 && S_ISREG(st.st_mode));
}

// Handle HTTP request
void handle_request(int client_fd, const char *request_buffer) {
    HttpRequest req = {0};
    
    if (!parse_request(request_buffer, &req)) {
        send_error(client_fd, 400, "Bad Request");
        return;
    }
    
    // Log request
    printf("%s[%s]%s %s%s%s %s%s%s\n", 
           COLOR_CYAN, req.method, COLOR_RESET,
           COLOR_YELLOW, req.path, COLOR_RESET,
           COLOR_BLUE, req.version, COLOR_RESET);
    
    // Only support GET method for now
    if (strcmp(req.method, "GET") != 0) {
        send_error(client_fd, 501, "Not Implemented");
        return;
    }
    
    // Handle root path
    if (strcmp(req.path, "/") == 0) {
        const char *html = 
            "<!DOCTYPE html>\n"
            "<html><head><title>Simple HTTP Server</title></head>\n"
            "<body><h1>Welcome to Simple HTTP Server</h1>\n"
            "<p>Server is running successfully!</p>\n"
            "<p>Try accessing a file like <a href=\"/index.html\">index.html</a></p>\n"
            "</body></html>\n";
        send_response(client_fd, 200, "OK", "text/html", html, strlen(html));
        return;
    }
    
    // Remove leading slash and check for directory traversal
    char file_path[MAX_PATH_LEN];
    if (req.path[0] == '/') {
        strncpy(file_path, req.path + 1, sizeof(file_path) - 1);
    } else {
        strncpy(file_path, req.path, sizeof(file_path) - 1);
    }
    file_path[sizeof(file_path) - 1] = '\0';
    
    // Security: prevent directory traversal
    if (strstr(file_path, "..") != NULL) {
        send_error(client_fd, 403, "Forbidden");
        return;
    }
    
    // Try to serve file
    if (file_exists(file_path)) {
        char *content = NULL;
        size_t content_size = 0;
        
        if (read_file(file_path, &content, &content_size)) {
            const char *mime_type = get_mime_type(file_path);
            send_response(client_fd, 200, "OK", mime_type, content, content_size);
            free(content);
        } else {
            send_error(client_fd, 500, "Internal Server Error");
        }
    } else {
        send_error(client_fd, 404, "Not Found");
    }
}

// Handle client connection
void handle_client(int client_fd, struct sockaddr_in *client_addr __attribute__((unused))) {
    char buffer[BUFFER_SIZE];
    
    // Read request
    ssize_t bytes_read = recv(client_fd, buffer, sizeof(buffer) - 1, 0);
    if (bytes_read <= 0) {
        close(client_fd);
        return;
    }
    
    buffer[bytes_read] = '\0';
    
    // Handle request
    handle_request(client_fd, buffer);
    
    close(client_fd);
}

// Print server info
void print_server_info(int port) {
    printf("\n%s%s========================================%s\n", COLOR_BOLD, COLOR_GREEN, COLOR_RESET);
    printf("%s%s  Simple HTTP Server Running%s\n", COLOR_BOLD, COLOR_GREEN, COLOR_RESET);
    printf("%s%s========================================%s\n\n", COLOR_BOLD, COLOR_GREEN, COLOR_RESET);
    printf("Server listening on %shttp://localhost:%d%s\n", COLOR_CYAN, port, COLOR_RESET);
    printf("Press %sCtrl+C%s to stop the server\n\n", COLOR_YELLOW, COLOR_RESET);
}

int main(int argc, char *argv[]) {
    int port = PORT;
    
    // Parse port from command line if provided
    if (argc > 1) {
        port = atoi(argv[1]);
        if (port <= 0 || port > 65535) {
            printf("%sError:%s Invalid port number. Using default port %d.\n", 
                   COLOR_RED COLOR_BOLD, COLOR_RESET, PORT);
            port = PORT;
        }
    }
    
    // Create socket
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        printf("%sError:%s Failed to create socket.\n", COLOR_RED COLOR_BOLD, COLOR_RESET);
        return 1;
    }
    
    // Set socket options (reuse address)
    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        printf("%sError:%s Failed to set socket options.\n", COLOR_RED COLOR_BOLD, COLOR_RESET);
        close(server_fd);
        return 1;
    }
    
    // Bind socket
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);
    
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        printf("%sError:%s Failed to bind to port %d. Port may be in use.\n", 
               COLOR_RED COLOR_BOLD, COLOR_RESET, port);
        close(server_fd);
        return 1;
    }
    
    // Listen for connections
    if (listen(server_fd, 10) < 0) {
        printf("%sError:%s Failed to listen on socket.\n", COLOR_RED COLOR_BOLD, COLOR_RESET);
        close(server_fd);
        return 1;
    }
    
    print_server_info(port);
    
    // Accept connections
    while (1) {
        struct sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);
        
        int client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_len);
        if (client_fd < 0) {
            printf("%sError:%s Failed to accept connection.\n", COLOR_RED COLOR_BOLD, COLOR_RESET);
            continue;
        }
        
        handle_client(client_fd, &client_addr);
    }
    
    close(server_fd);
    return 0;
}

