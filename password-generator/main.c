#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MIN_LENGTH 4
#define MAX_LENGTH 128
#define DEFAULT_LENGTH 16

// ANSI color codes
#define COLOR_RESET   "\033[0m"
#define COLOR_RED     "\033[31m"
#define COLOR_GREEN   "\033[32m"
#define COLOR_YELLOW  "\033[33m"
#define COLOR_BLUE     "\033[34m"
#define COLOR_MAGENTA "\033[35m"
#define COLOR_CYAN    "\033[36m"
#define COLOR_BOLD    "\033[1m"
#define COLOR_BRIGHT_GREEN "\033[92m"
#define COLOR_BRIGHT_YELLOW "\033[93m"

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
        strcpy(password, "");
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
    printf("%sUsage:%s\n", COLOR_BOLD COLOR_CYAN, COLOR_RESET);
    printf("  %s%s [OPTIONS]%s\n\n", COLOR_YELLOW, progname, COLOR_RESET);
    printf("%sOptions:%s\n", COLOR_BOLD, COLOR_RESET);
    printf("  %s-l, --length N%s        Password length (default: %s%d%s, min: %s%d%s, max: %s%d%s)\n", COLOR_CYAN, COLOR_RESET, COLOR_YELLOW, DEFAULT_LENGTH, COLOR_RESET, COLOR_YELLOW, MIN_LENGTH, COLOR_RESET, COLOR_YELLOW, MAX_LENGTH, COLOR_RESET);
    printf("  %s-u, --uppercase%s       Include uppercase letters (default: yes)\n", COLOR_CYAN, COLOR_RESET);
    printf("  %s-L, --no-uppercase%s    Exclude uppercase letters\n", COLOR_CYAN, COLOR_RESET);
    printf("  %s-d, --digits%s          Include digits (default: yes)\n", COLOR_CYAN, COLOR_RESET);
    printf("  %s-D, --no-digits%s       Exclude digits\n", COLOR_CYAN, COLOR_RESET);
    printf("  %s-s, --symbols%s         Include symbols (default: yes)\n", COLOR_CYAN, COLOR_RESET);
    printf("  %s-S, --no-symbols%s      Exclude symbols\n", COLOR_CYAN, COLOR_RESET);
    printf("  %s-a, --all%s             Include all character types (default)\n", COLOR_CYAN, COLOR_RESET);
    printf("  %s-n, --numbers-only%s    Generate numeric password only\n", COLOR_CYAN, COLOR_RESET);
    printf("  %s-h, --help%s            Show this help message\n\n", COLOR_CYAN, COLOR_RESET);
    printf("%sExamples:%s\n", COLOR_BOLD, COLOR_RESET);
    printf("  %s%s%s                    Generate a %s%d%s-character password with all character types\n", COLOR_YELLOW, progname, COLOR_RESET, COLOR_YELLOW, DEFAULT_LENGTH, COLOR_RESET);
    printf("  %s%s -l 20%s              Generate a 20-character password\n", COLOR_YELLOW, progname, COLOR_RESET);
    printf("  %s%s -l 12 -S%s          Generate a 12-character password without symbols\n", COLOR_YELLOW, progname, COLOR_RESET);
    printf("  %s%s -n -l 6%s            Generate a 6-digit numeric PIN\n", COLOR_YELLOW, progname, COLOR_RESET);
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
                printf("%sError:%s -l/--length requires a value\n", COLOR_RED COLOR_BOLD, COLOR_RESET);
                return -1;
            }
            int len = parse_length(argv[++i]);
            if (len == -1) {
                printf("%sError:%s Length must be between %s%d%s and %s%d%s\n", COLOR_RED COLOR_BOLD, COLOR_RESET, COLOR_YELLOW, MIN_LENGTH, COLOR_RESET, COLOR_YELLOW, MAX_LENGTH, COLOR_RESET);
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
            printf("%sError:%s Unknown option '%s%s%s'\n", COLOR_RED COLOR_BOLD, COLOR_RESET, COLOR_YELLOW, argv[i], COLOR_RESET);
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

    if (strlen(password) == 0) {
        printf("%sError:%s No character set selected. Please enable at least one character type.\n", COLOR_RED COLOR_BOLD, COLOR_RESET);
        return 1;
    }

    // Color the password with alternating colors for better visibility
    printf("%s%s%s%s\n", COLOR_BOLD COLOR_BRIGHT_GREEN, password, COLOR_RESET, COLOR_RESET);

    return 0;
}

