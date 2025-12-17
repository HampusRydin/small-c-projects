#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MIN_LENGTH 4
#define MAX_LENGTH 128
#define DEFAULT_LENGTH 16

const char LOWERCASE[] = "abcdefghijklmnopqrstuvwxyz";
const char UPPERCASE[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
const char DIGITS[] = "0123456789";
const char SYMBOLS[] = "!@#$%^&*()_+-=[]{}|;:,.<>?";

typedef struct {
    int length;
    int use_lowercase;
    int use_uppercase;
    int use_digits;
    int use_symbols;
} PasswordConfig;

void init_random() {
    srand((unsigned int)time(NULL));
}

int random_int(int min, int max) {
    return min + rand() % (max - min + 1);
}

char random_char(const char *charset, int charset_len) {
    return charset[random_int(0, charset_len - 1)];
}

void build_charset(const PasswordConfig *config, char *charset, int *charset_len) {
    charset[0] = '\0';
    *charset_len = 0;

    if (config->use_lowercase) {
        strcat(charset, LOWERCASE);
        *charset_len += strlen(LOWERCASE);
    }
    if (config->use_uppercase) {
        strcat(charset, UPPERCASE);
        *charset_len += strlen(UPPERCASE);
    }
    if (config->use_digits) {
        strcat(charset, DIGITS);
        *charset_len += strlen(DIGITS);
    }
    if (config->use_symbols) {
        strcat(charset, SYMBOLS);
        *charset_len += strlen(SYMBOLS);
    }
}

void generate_password(const PasswordConfig *config, char *password) {
    char charset[256] = {0};
    int charset_len = 0;

    build_charset(config, charset, &charset_len);

    if (charset_len == 0) {
        strcpy(password, "ERROR: No character set selected");
        return;
    }

    for (int i = 0; i < config->length; i++) {
        password[i] = random_char(charset, charset_len);
    }
    password[config->length] = '\0';
}

int parse_length(const char *str) {
    int len = atoi(str);
    if (len < MIN_LENGTH || len > MAX_LENGTH) {
        return -1;
    }
    return len;
}

void print_usage(const char *progname) {
    printf("Usage:\n");
    printf("  %s [OPTIONS]\n\n", progname);
    printf("Options:\n");
    printf("  -l, --length N        Password length (default: %d, min: %d, max: %d)\n", DEFAULT_LENGTH, MIN_LENGTH, MAX_LENGTH);
    printf("  -u, --uppercase       Include uppercase letters (default: yes)\n");
    printf("  -L, --no-uppercase    Exclude uppercase letters\n");
    printf("  -d, --digits          Include digits (default: yes)\n");
    printf("  -D, --no-digits       Exclude digits\n");
    printf("  -s, --symbols         Include symbols (default: yes)\n");
    printf("  -S, --no-symbols      Exclude symbols\n");
    printf("  -a, --all             Include all character types (default)\n");
    printf("  -n, --numbers-only    Generate numeric password only\n");
    printf("  -h, --help            Show this help message\n\n");
    printf("Examples:\n");
    printf("  %s                    Generate a %d-character password with all character types\n", progname, DEFAULT_LENGTH);
    printf("  %s -l 20              Generate a 20-character password\n", progname);
    printf("  %s -l 12 -S          Generate a 12-character password without symbols\n", progname);
    printf("  %s -n -l 6            Generate a 6-digit numeric PIN\n", progname);
}

int parse_args(int argc, char *argv[], PasswordConfig *config) {
    // Set defaults
    config->length = DEFAULT_LENGTH;
    config->use_lowercase = 1;
    config->use_uppercase = 1;
    config->use_digits = 1;
    config->use_symbols = 1;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            return 0; // Signal to show help
        } else if (strcmp(argv[i], "-l") == 0 || strcmp(argv[i], "--length") == 0) {
            if (i + 1 >= argc) {
                printf("Error: -l/--length requires a value\n");
                return -1;
            }
            int len = parse_length(argv[++i]);
            if (len == -1) {
                printf("Error: Length must be between %d and %d\n", MIN_LENGTH, MAX_LENGTH);
                return -1;
            }
            config->length = len;
        } else if (strcmp(argv[i], "-u") == 0 || strcmp(argv[i], "--uppercase") == 0) {
            config->use_uppercase = 1;
        } else if (strcmp(argv[i], "-L") == 0 || strcmp(argv[i], "--no-uppercase") == 0) {
            config->use_uppercase = 0;
        } else if (strcmp(argv[i], "-d") == 0 || strcmp(argv[i], "--digits") == 0) {
            config->use_digits = 1;
        } else if (strcmp(argv[i], "-D") == 0 || strcmp(argv[i], "--no-digits") == 0) {
            config->use_digits = 0;
        } else if (strcmp(argv[i], "-s") == 0 || strcmp(argv[i], "--symbols") == 0) {
            config->use_symbols = 1;
        } else if (strcmp(argv[i], "-S") == 0 || strcmp(argv[i], "--no-symbols") == 0) {
            config->use_symbols = 0;
        } else if (strcmp(argv[i], "-a") == 0 || strcmp(argv[i], "--all") == 0) {
            config->use_lowercase = 1;
            config->use_uppercase = 1;
            config->use_digits = 1;
            config->use_symbols = 1;
        } else if (strcmp(argv[i], "-n") == 0 || strcmp(argv[i], "--numbers-only") == 0) {
            config->use_lowercase = 0;
            config->use_uppercase = 0;
            config->use_digits = 1;
            config->use_symbols = 0;
        } else {
            printf("Error: Unknown option '%s'\n", argv[i]);
            return -1;
        }
    }

    return 1; // Success
}

int main(int argc, char *argv[]) {
    PasswordConfig config;
    char password[MAX_LENGTH + 1];

    int result = parse_args(argc, argv, &config);
    if (result == 0) {
        print_usage(argv[0]);
        return 0;
    } else if (result == -1) {
        print_usage(argv[0]);
        return 1;
    }

    init_random();
    generate_password(&config, password);

    printf("%s\n", password);

    return 0;
}

