#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define HASH_TABLE_SIZE 101
#define MAX_KEY_LEN 256
#define MAX_VAL_LEN 1024
#define STORAGE_FILE "kvstore.dat"

// ANSI color codes
#define COLOR_RESET   "\033[0m"
#define COLOR_RED     "\033[31m"
#define COLOR_GREEN   "\033[32m"
#define COLOR_YELLOW  "\033[33m"
#define COLOR_BLUE     "\033[34m"
#define COLOR_CYAN    "\033[36m"
#define COLOR_BOLD    "\033[1m"

// Key-Value pair structure
typedef struct KVPair {
    char key[MAX_KEY_LEN];
    char value[MAX_VAL_LEN];
    struct KVPair *next;  // For collision chaining
} KVPair;

// Hash table structure
typedef struct {
    KVPair *buckets[HASH_TABLE_SIZE];
    int count;
} HashTable;

HashTable *table = NULL;

// Hash function (djb2 algorithm)
uint32_t hash(const char *key) {
    uint32_t hash = 5381;
    int c;
    while ((c = *key++)) {
        hash = ((hash << 5) + hash) + c; // hash * 33 + c
    }
    return hash % HASH_TABLE_SIZE;
}

// Initialize hash table
HashTable* create_table() {
    HashTable *ht = (HashTable*)calloc(1, sizeof(HashTable));
    if (!ht) {
        printf("%sError:%s Memory allocation failed.\n", COLOR_RED COLOR_BOLD, COLOR_RESET);
        return NULL;
    }
    return ht;
}

// Create a new key-value pair
KVPair* create_pair(const char *key, const char *value) {
    KVPair *pair = (KVPair*)malloc(sizeof(KVPair));
    if (!pair) {
        return NULL;
    }
    
    strncpy(pair->key, key, MAX_KEY_LEN - 1);
    pair->key[MAX_KEY_LEN - 1] = '\0';
    strncpy(pair->value, value, MAX_VAL_LEN - 1);
    pair->value[MAX_VAL_LEN - 1] = '\0';
    pair->next = NULL;
    
    return pair;
}

// Insert or update a key-value pair
int set_value(const char *key, const char *value) {
    if (!table || !key || !value) {
        return 0;
    }
    
    if (strlen(key) == 0 || strlen(key) >= MAX_KEY_LEN) {
        printf("%sError:%s Key length must be between 1 and %d characters.\n", 
               COLOR_RED COLOR_BOLD, COLOR_RESET, MAX_KEY_LEN - 1);
        return 0;
    }
    
    if (strlen(value) >= MAX_VAL_LEN) {
        printf("%sError:%s Value length must be less than %d characters.\n", 
               COLOR_RED COLOR_BOLD, COLOR_RESET, MAX_VAL_LEN);
        return 0;
    }
    
    uint32_t index = hash(key);
    KVPair *current = table->buckets[index];
    
    // Check if key already exists
    while (current != NULL) {
        if (strcmp(current->key, key) == 0) {
            // Update existing value
            strncpy(current->value, value, MAX_VAL_LEN - 1);
            current->value[MAX_VAL_LEN - 1] = '\0';
            return 1;
        }
        current = current->next;
    }
    
    // Key doesn't exist, create new pair
    KVPair *new_pair = create_pair(key, value);
    if (!new_pair) {
        printf("%sError:%s Failed to create key-value pair.\n", COLOR_RED COLOR_BOLD, COLOR_RESET);
        return 0;
    }
    
    // Insert at head of chain
    new_pair->next = table->buckets[index];
    table->buckets[index] = new_pair;
    table->count++;
    
    return 1;
}

// Get value by key
const char* get_value(const char *key) {
    if (!table || !key) {
        return NULL;
    }
    
    uint32_t index = hash(key);
    KVPair *current = table->buckets[index];
    
    while (current != NULL) {
        if (strcmp(current->key, key) == 0) {
            return current->value;
        }
        current = current->next;
    }
    
    return NULL;
}

// Delete a key-value pair
int delete_value(const char *key) {
    if (!table || !key) {
        return 0;
    }
    
    uint32_t index = hash(key);
    KVPair *current = table->buckets[index];
    KVPair *prev = NULL;
    
    while (current != NULL) {
        if (strcmp(current->key, key) == 0) {
            if (prev == NULL) {
                // First in chain
                table->buckets[index] = current->next;
            } else {
                prev->next = current->next;
            }
            free(current);
            table->count--;
            return 1;
        }
        prev = current;
        current = current->next;
    }
    
    return 0;
}

// List all key-value pairs
void list_all() {
    if (!table) {
        return;
    }
    
    if (table->count == 0) {
        printf("%sNo entries in store.%s\n", COLOR_YELLOW, COLOR_RESET);
        return;
    }
    
    printf("\n%s%s--- Key-Value Store (%d entries) ---%s\n", COLOR_BOLD, COLOR_CYAN, table->count, COLOR_RESET);
    
    for (int i = 0; i < HASH_TABLE_SIZE; i++) {
        KVPair *current = table->buckets[i];
        while (current != NULL) {
            printf("%s%s%s: %s%s%s\n", 
                   COLOR_YELLOW COLOR_BOLD, current->key, COLOR_RESET,
                   COLOR_CYAN, current->value, COLOR_RESET);
            current = current->next;
        }
    }
    printf("\n");
}

// Clear all entries
void clear_all() {
    if (!table) {
        return;
    }
    
    for (int i = 0; i < HASH_TABLE_SIZE; i++) {
        KVPair *current = table->buckets[i];
        while (current != NULL) {
            KVPair *next = current->next;
            free(current);
            current = next;
        }
        table->buckets[i] = NULL;
    }
    table->count = 0;
}

// Save hash table to disk
int save_to_disk() {
    FILE *f = fopen(STORAGE_FILE, "wb");
    if (!f) {
        printf("%sError:%s Failed to open storage file for writing.\n", COLOR_RED COLOR_BOLD, COLOR_RESET);
        return 0;
    }
    
    // Write count
    fwrite(&table->count, sizeof(int), 1, f);
    
    // Write all key-value pairs
    for (int i = 0; i < HASH_TABLE_SIZE; i++) {
        KVPair *current = table->buckets[i];
        while (current != NULL) {
            // Write key length, key, value length, value
            uint16_t key_len = strlen(current->key);
            uint16_t val_len = strlen(current->value);
            
            fwrite(&key_len, sizeof(uint16_t), 1, f);
            fwrite(current->key, sizeof(char), key_len, f);
            fwrite(&val_len, sizeof(uint16_t), 1, f);
            fwrite(current->value, sizeof(char), val_len, f);
            
            current = current->next;
        }
    }
    
    fclose(f);
    return 1;
}

// Load hash table from disk
int load_from_disk() {
    FILE *f = fopen(STORAGE_FILE, "rb");
    if (!f) {
        // File doesn't exist yet, that's okay
        return 0;
    }
    
    int count;
    if (fread(&count, sizeof(int), 1, f) != 1) {
        fclose(f);
        return 0;
    }
    
    for (int i = 0; i < count; i++) {
        uint16_t key_len, val_len;
        
        if (fread(&key_len, sizeof(uint16_t), 1, f) != 1) break;
        if (key_len >= MAX_KEY_LEN) {
            fclose(f);
            return 0;
        }
        
        char key[MAX_KEY_LEN];
        if (fread(key, sizeof(char), key_len, f) != key_len) break;
        key[key_len] = '\0';
        
        if (fread(&val_len, sizeof(uint16_t), 1, f) != 1) break;
        if (val_len >= MAX_VAL_LEN) {
            fclose(f);
            return 0;
        }
        
        char value[MAX_VAL_LEN];
        if (fread(value, sizeof(char), val_len, f) != val_len) break;
        value[val_len] = '\0';
        
        set_value(key, value);
    }
    
    fclose(f);
    return 1;
}

// Free all memory
void free_table() {
    if (!table) {
        return;
    }
    
    clear_all();
    free(table);
    table = NULL;
}

// Print usage information
void print_usage(const char *progname) {
    printf("%sUsage:%s\n", COLOR_BOLD COLOR_CYAN, COLOR_RESET);
    printf("  %s%s set <key> <value>%s    - Set or update a key-value pair\n", COLOR_YELLOW, progname, COLOR_RESET);
    printf("  %s%s get <key>%s            - Get value for a key\n", COLOR_YELLOW, progname, COLOR_RESET);
    printf("  %s%s delete <key>%s         - Delete a key-value pair\n", COLOR_YELLOW, progname, COLOR_RESET);
    printf("  %s%s list%s                 - List all key-value pairs\n", COLOR_YELLOW, progname, COLOR_RESET);
    printf("  %s%s clear%s                - Clear all entries\n", COLOR_YELLOW, progname, COLOR_RESET);
    printf("  %s%s count%s                - Show number of entries\n", COLOR_YELLOW, progname, COLOR_RESET);
    printf("  %s%s help%s                 - Show this help message\n", COLOR_YELLOW, progname, COLOR_RESET);
    printf("\n%sExamples:%s\n", COLOR_BOLD, COLOR_RESET);
    printf("  %s%s set name \"John Doe\"%s\n", COLOR_YELLOW, progname, COLOR_RESET);
    printf("  %s%s get name%s\n", COLOR_YELLOW, progname, COLOR_RESET);
    printf("  %s%s set age 30%s\n", COLOR_YELLOW, progname, COLOR_RESET);
    printf("  %s%s list%s\n", COLOR_YELLOW, progname, COLOR_RESET);
}

int main(int argc, char *argv[]) {
    // Initialize table
    table = create_table();
    if (!table) {
        return 1;
    }
    
    // Load existing data
    load_from_disk();
    
    if (argc < 2) {
        print_usage(argv[0]);
        free_table();
        return 1;
    }
    
    int result = 0;
    
    if (strcmp(argv[1], "set") == 0) {
        if (argc < 4) {
            printf("%sError:%s 'set' requires key and value arguments.\n", COLOR_RED COLOR_BOLD, COLOR_RESET);
            print_usage(argv[0]);
        } else {
            if (set_value(argv[2], argv[3])) {
                printf("%s✓ Set%s '%s%s%s' = '%s%s%s'\n", 
                       COLOR_GREEN COLOR_BOLD, COLOR_RESET,
                       COLOR_YELLOW, argv[2], COLOR_RESET,
                       COLOR_CYAN, argv[3], COLOR_RESET);
                save_to_disk();
            } else {
                result = 1;
            }
        }
    } else if (strcmp(argv[1], "get") == 0) {
        if (argc < 3) {
            printf("%sError:%s 'get' requires a key argument.\n", COLOR_RED COLOR_BOLD, COLOR_RESET);
            print_usage(argv[0]);
        } else {
            const char *value = get_value(argv[2]);
            if (value) {
                printf("%s%s%s\n", COLOR_CYAN, value, COLOR_RESET);
            } else {
                printf("%sKey '%s%s%s' not found.%s\n", COLOR_YELLOW, COLOR_BOLD, argv[2], COLOR_YELLOW, COLOR_RESET);
                result = 1;
            }
        }
    } else if (strcmp(argv[1], "delete") == 0) {
        if (argc < 3) {
            printf("%sError:%s 'delete' requires a key argument.\n", COLOR_RED COLOR_BOLD, COLOR_RESET);
            print_usage(argv[0]);
        } else {
            if (delete_value(argv[2])) {
                printf("%s✓ Deleted%s key '%s%s%s'\n", 
                       COLOR_GREEN COLOR_BOLD, COLOR_RESET,
                       COLOR_YELLOW, argv[2], COLOR_RESET);
                save_to_disk();
            } else {
                printf("%sKey '%s%s%s' not found.%s\n", COLOR_YELLOW, COLOR_BOLD, argv[2], COLOR_YELLOW, COLOR_RESET);
                result = 1;
            }
        }
    } else if (strcmp(argv[1], "list") == 0) {
        list_all();
    } else if (strcmp(argv[1], "clear") == 0) {
        clear_all();
        save_to_disk();
        printf("%s✓ Cleared all entries.%s\n", COLOR_GREEN COLOR_BOLD, COLOR_RESET);
    } else if (strcmp(argv[1], "count") == 0) {
        printf("%s%d%s entries in store.\n", COLOR_CYAN COLOR_BOLD, table->count, COLOR_RESET);
    } else if (strcmp(argv[1], "help") == 0 || strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0) {
        print_usage(argv[0]);
    } else {
        printf("%sError:%s Unknown command: %s%s%s\n", COLOR_RED COLOR_BOLD, COLOR_RESET, COLOR_YELLOW, argv[1], COLOR_RESET);
        print_usage(argv[0]);
        result = 1;
    }
    
    free_table();
    return result;
}

