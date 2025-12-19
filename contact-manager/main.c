#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME 100
#define MAX_EMAIL 100
#define MAX_PHONE 20
#define CONTACTS_FILE "contacts.txt"

// ANSI color codes
#define COLOR_RESET   "\033[0m"
#define COLOR_RED     "\033[31m"
#define COLOR_GREEN   "\033[32m"
#define COLOR_YELLOW  "\033[33m"
#define COLOR_BLUE     "\033[34m"
#define COLOR_MAGENTA "\033[35m"
#define COLOR_CYAN    "\033[36m"
#define COLOR_WHITE   "\033[37m"
#define COLOR_BOLD    "\033[1m"

typedef struct Contact {
    char name[MAX_NAME];
    char email[MAX_EMAIL];
    char phone[MAX_PHONE];
    struct Contact *next;
} Contact;

Contact *head = NULL;
int contact_count = 0;

void trim_newline(char *s) {
    size_t len = strlen(s);
    if (len > 0 && s[len - 1] == '\n') {
        s[len - 1] = '\0';
    }
}

Contact* create_contact(const char *name, const char *email, const char *phone) {
    Contact *new_contact = (Contact*)malloc(sizeof(Contact));
    if (!new_contact) {
        printf("%sError:%s Memory allocation failed.\n", COLOR_RED COLOR_BOLD, COLOR_RESET);
        return NULL;
    }

    strncpy(new_contact->name, name, MAX_NAME - 1);
    new_contact->name[MAX_NAME - 1] = '\0';
    strncpy(new_contact->email, email, MAX_EMAIL - 1);
    new_contact->email[MAX_EMAIL - 1] = '\0';
    strncpy(new_contact->phone, phone, MAX_PHONE - 1);
    new_contact->phone[MAX_PHONE - 1] = '\0';
    new_contact->next = NULL;

    return new_contact;
}

void add_contact(const char *name, const char *email, const char *phone) {
    Contact *new_contact = create_contact(name, email, phone);
    if (!new_contact) {
        return;
    }

    if (head == NULL) {
        head = new_contact;
    } else {
        Contact *current = head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = new_contact;
    }
    contact_count++;
    printf("%s✓ Contact added successfully.%s\n", COLOR_GREEN COLOR_BOLD, COLOR_RESET);
}

void load_contacts() {
    FILE *f = fopen(CONTACTS_FILE, "r");
    if (!f) {
        return;
    }

    char name[MAX_NAME];
    char email[MAX_EMAIL];
    char phone[MAX_PHONE];

    while (fgets(name, sizeof(name), f)) {
        trim_newline(name);
        if (!fgets(email, sizeof(email), f)) break;
        trim_newline(email);
        if (!fgets(phone, sizeof(phone), f)) break;
        trim_newline(phone);

        add_contact(name, email, phone);
    }

    fclose(f);
}

void save_contacts() {
    FILE *f = fopen(CONTACTS_FILE, "w");
    if (!f) {
        printf("%sError:%s ", COLOR_RED COLOR_BOLD, COLOR_RESET);
        perror("Failed to open contacts file for writing");
        return;
    }

    Contact *current = head;
    while (current != NULL) {
        fprintf(f, "%s\n%s\n%s\n", current->name, current->email, current->phone);
        current = current->next;
    }

    fclose(f);
}

void list_contacts() {
    if (head == NULL) {
        printf("%sNo contacts found.%s\n", COLOR_YELLOW, COLOR_RESET);
        return;
    }

    printf("\n%s%s--- Contact List ---%s\n", COLOR_BOLD, COLOR_CYAN, COLOR_RESET);
    Contact *current = head;
    int index = 1;
    while (current != NULL) {
        printf("%s%d.%s %s%s%s\n", COLOR_BOLD, index, COLOR_RESET, COLOR_YELLOW COLOR_BOLD, current->name, COLOR_RESET);
        printf("   %sEmail:%s %s\n", COLOR_CYAN, COLOR_RESET, current->email);
        printf("   %sPhone:%s %s\n\n", COLOR_CYAN, COLOR_RESET, current->phone);
        current = current->next;
        index++;
    }
}

Contact* find_contact_by_index(int index) {
    if (index < 1 || index > contact_count) {
        return NULL;
    }

    Contact *current = head;
    for (int i = 1; i < index; i++) {
        current = current->next;
    }
    return current;
}

void search_contacts(const char *query) {
    if (head == NULL) {
        printf("%sNo contacts found.%s\n", COLOR_YELLOW, COLOR_RESET);
        return;
    }

    printf("\n%s%s--- Search Results ---%s\n", COLOR_BOLD, COLOR_CYAN, COLOR_RESET);
    Contact *current = head;
    int index = 1;
    int found = 0;

    while (current != NULL) {
        if (strstr(current->name, query) != NULL ||
            strstr(current->email, query) != NULL ||
            strstr(current->phone, query) != NULL) {
            printf("%s%d.%s %s%s%s\n", COLOR_BOLD, index, COLOR_RESET, COLOR_YELLOW COLOR_BOLD, current->name, COLOR_RESET);
            printf("   %sEmail:%s %s\n", COLOR_CYAN, COLOR_RESET, current->email);
            printf("   %sPhone:%s %s\n\n", COLOR_CYAN, COLOR_RESET, current->phone);
            found = 1;
        }
        current = current->next;
        index++;
    }

    if (!found) {
        printf("%sNo contacts found matching '%s'.%s\n", COLOR_YELLOW, query, COLOR_RESET);
    }
}

void update_contact(int index, const char *name, const char *email, const char *phone) {
    Contact *contact = find_contact_by_index(index);
    if (!contact) {
        printf("%sError:%s Invalid contact number.\n", COLOR_RED COLOR_BOLD, COLOR_RESET);
        return;
    }

    if (name && strlen(name) > 0) {
        strncpy(contact->name, name, MAX_NAME - 1);
        contact->name[MAX_NAME - 1] = '\0';
    }
    if (email && strlen(email) > 0) {
        strncpy(contact->email, email, MAX_EMAIL - 1);
        contact->email[MAX_EMAIL - 1] = '\0';
    }
    if (phone && strlen(phone) > 0) {
        strncpy(contact->phone, phone, MAX_PHONE - 1);
        contact->phone[MAX_PHONE - 1] = '\0';
    }

    printf("%s✓ Contact updated successfully.%s\n", COLOR_GREEN COLOR_BOLD, COLOR_RESET);
}

void delete_contact(int index) {
    if (index < 1 || index > contact_count) {
        printf("%sError:%s Invalid contact number.\n", COLOR_RED COLOR_BOLD, COLOR_RESET);
        return;
    }

    Contact *to_delete;
    if (index == 1) {
        to_delete = head;
        head = head->next;
    } else {
        Contact *current = head;
        for (int i = 1; i < index - 1; i++) {
            current = current->next;
        }
        to_delete = current->next;
        current->next = to_delete->next;
    }

    printf("%s✓ Deleted:%s %s%s%s\n", COLOR_GREEN COLOR_BOLD, COLOR_RESET, COLOR_YELLOW, to_delete->name, COLOR_RESET);
    free(to_delete);
    contact_count--;
}

void free_contacts() {
    Contact *current = head;
    while (current != NULL) {
        Contact *next = current->next;
        free(current);
        current = next;
    }
    head = NULL;
    contact_count = 0;
}

void print_usage(const char *progname) {
    printf("%sUsage:%s\n", COLOR_BOLD COLOR_CYAN, COLOR_RESET);
    printf("  %s%s list%s                           - List all contacts\n", COLOR_YELLOW, progname, COLOR_RESET);
    printf("  %s%s add \"name\" \"email\" \"phone\"%s   - Add a new contact\n", COLOR_YELLOW, progname, COLOR_RESET);
    printf("  %s%s search \"query\"%s                 - Search contacts by name, email, or phone\n", COLOR_YELLOW, progname, COLOR_RESET);
    printf("  %s%s update INDEX \"name\" \"email\" \"phone\"%s - Update a contact (use \"\" to skip a field)\n", COLOR_YELLOW, progname, COLOR_RESET);
    printf("  %s%s delete INDEX%s                   - Delete a contact\n", COLOR_YELLOW, progname, COLOR_RESET);
}

int main(int argc, char *argv[]) {
    load_contacts();

    if (argc < 2) {
        print_usage(argv[0]);
        free_contacts();
        return 1;
    }

    if (strcmp(argv[1], "list") == 0) {
        list_contacts();
    } else if (strcmp(argv[1], "add") == 0) {
        if (argc < 5) {
            printf("%sError:%s missing arguments. Need name, email, and phone.\n", COLOR_RED COLOR_BOLD, COLOR_RESET);
            print_usage(argv[0]);
        } else {
            add_contact(argv[2], argv[3], argv[4]);
            save_contacts();
        }
    } else if (strcmp(argv[1], "search") == 0) {
        if (argc < 3) {
            printf("%sError:%s missing search query.\n", COLOR_RED COLOR_BOLD, COLOR_RESET);
            print_usage(argv[0]);
        } else {
            search_contacts(argv[2]);
        }
    } else if (strcmp(argv[1], "update") == 0) {
        if (argc < 6) {
            printf("%sError:%s missing arguments. Need index, name, email, and phone.\n", COLOR_RED COLOR_BOLD, COLOR_RESET);
            print_usage(argv[0]);
        } else {
            int index = atoi(argv[2]);
            update_contact(index, argv[3], argv[4], argv[5]);
            save_contacts();
        }
    } else if (strcmp(argv[1], "delete") == 0) {
        if (argc < 3) {
            printf("%sError:%s missing contact number.\n", COLOR_RED COLOR_BOLD, COLOR_RESET);
            print_usage(argv[0]);
        } else {
            int index = atoi(argv[2]);
            delete_contact(index);
            save_contacts();
        }
    } else {
        printf("%sError:%s Unknown command: %s%s%s\n", COLOR_RED COLOR_BOLD, COLOR_RESET, COLOR_YELLOW, argv[1], COLOR_RESET);
        print_usage(argv[0]);
        free_contacts();
        return 1;
    }

    free_contacts();
    return 0;
}

