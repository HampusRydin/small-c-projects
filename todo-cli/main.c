#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TASKS 1000
#define MAX_DESC 256
#define TASKS_FILE "tasks.txt"

// ANSI color codes
#define COLOR_RESET   "\033[0m"
#define COLOR_RED     "\033[31m"
#define COLOR_GREEN   "\033[32m"
#define COLOR_YELLOW  "\033[33m"
#define COLOR_BLUE     "\033[34m"
#define COLOR_CYAN    "\033[36m"
#define COLOR_BOLD    "\033[1m"
#define COLOR_DIM     "\033[2m"

typedef struct {
    int done;
    char desc[MAX_DESC];
} Task;

Task tasks[MAX_TASKS];
int task_count = 0;

void trim_newline(char *s) {
    size_t len = strlen(s);
    if (len > 0 && s[len - 1] == '\n') {
        s[len - 1] = '\0';
    }
}

void load_tasks() {
    FILE *f = fopen(TASKS_FILE, "r");
    if (!f) {
        return;
    }

    int done;
    char buffer[MAX_DESC];

    while (task_count < MAX_TASKS && fscanf(f, "%d ", &done) == 1) {
        if (!fgets(buffer, sizeof(buffer), f)) {
            break;
        }
        trim_newline(buffer);

        tasks[task_count].done = done;
        strncpy(tasks[task_count].desc, buffer, MAX_DESC - 1);
        tasks[task_count].desc[MAX_DESC - 1] = '\0';

        task_count++;
    }

    fclose(f);
}

void save_tasks() {
    FILE *f = fopen(TASKS_FILE, "w");
    if (!f) {
        perror("Failed to open tasks file for writing");
        exit(1);
    }

    for (int i = 0; i < task_count; i++) {
        fprintf(f, "%d %s\n", tasks[i].done, tasks[i].desc);
    }

    fclose(f);
}

void list_tasks() {
    if (task_count == 0) {
        printf("%sNo tasks yet.%s\n", COLOR_YELLOW, COLOR_RESET);
        return;
    }

    printf("\n%s%s--- Todo List ---%s\n", COLOR_BOLD, COLOR_CYAN, COLOR_RESET);
    for (int i = 0; i < task_count; i++) {
        if (tasks[i].done) {
            printf("%s%d.%s [%s✓%s] %s%s%s\n",
                   COLOR_BOLD, i + 1, COLOR_RESET,
                   COLOR_GREEN COLOR_BOLD, COLOR_RESET,
                   COLOR_DIM, tasks[i].desc, COLOR_RESET);
        } else {
            printf("%s%d.%s [%s %s] %s%s%s\n",
                   COLOR_BOLD, i + 1, COLOR_RESET,
                   COLOR_YELLOW, COLOR_RESET,
                   COLOR_BOLD, tasks[i].desc, COLOR_RESET);
        }
    }
    printf("\n");
}

void add_task(const char *desc) {
    if (task_count >= MAX_TASKS) {
        printf("%sError:%s Task list is full.\n", COLOR_RED COLOR_BOLD, COLOR_RESET);
        return;
    }

    tasks[task_count].done = 0;
    strncpy(tasks[task_count].desc, desc, MAX_DESC - 1);
    tasks[task_count].desc[MAX_DESC - 1] = '\0';

    task_count++;
    save_tasks();
    printf("%s✓ Added:%s %s%s%s\n", COLOR_GREEN COLOR_BOLD, COLOR_RESET, COLOR_YELLOW, desc, COLOR_RESET);
}

void mark_done(int index) {
    if (index < 1 || index > task_count) {
        printf("%sError:%s Invalid task number.\n", COLOR_RED COLOR_BOLD, COLOR_RESET);
        return;
    }

    tasks[index - 1].done = 1;
    save_tasks();
    printf("%s✓ Marked task %s%d%s as done.%s\n", COLOR_GREEN COLOR_BOLD, COLOR_YELLOW, index, COLOR_GREEN COLOR_BOLD, COLOR_RESET);
}

void print_usage(const char *progname) {
    printf("%sUsage:%s\n", COLOR_BOLD COLOR_CYAN, COLOR_RESET);
    printf("  %s%s list%s\n", COLOR_YELLOW, progname, COLOR_RESET);
    printf("  %s%s add \"task description\"%s\n", COLOR_YELLOW, progname, COLOR_RESET);
    printf("  %s%s done TASK_NUMBER%s\n", COLOR_YELLOW, progname, COLOR_RESET);
}

int main(int argc, char *argv[]) {
    load_tasks();

    if (argc < 2) {
        print_usage(argv[0]);
        return 1;
    }

    if (strcmp(argv[1], "list") == 0) {
        list_tasks();
    } else if (strcmp(argv[1], "add") == 0) {
        if (argc < 3) {
            printf("%sError:%s missing task description.\n", COLOR_RED COLOR_BOLD, COLOR_RESET);
            print_usage(argv[0]);
            return 1;
        }
        add_task(argv[2]);
    } else if (strcmp(argv[1], "done") == 0) {
        if (argc < 3) {
            printf("%sError:%s missing task number.\n", COLOR_RED COLOR_BOLD, COLOR_RESET);
            print_usage(argv[0]);
            return 1;
        }
        int index = atoi(argv[2]);
        mark_done(index);
    } else {
        printf("%sError:%s Unknown command: %s%s%s\n", COLOR_RED COLOR_BOLD, COLOR_RESET, COLOR_YELLOW, argv[1], COLOR_RESET);
        print_usage(argv[0]);
        return 1;
    }

    return 0;
}
