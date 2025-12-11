#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TASKS 1000
#define MAX_DESC 256
#define TASKS_FILE "tasks.txt"

typedef struct {
    int done;                 // 0 = not done, 1 = done
    char desc[MAX_DESC];      // task description
} Task;

Task tasks[MAX_TASKS];
int task_count = 0;

/* ---------- Utility: trim newline from fgets ---------- */
void trim_newline(char *s) {
    size_t len = strlen(s);
    if (len > 0 && s[len - 1] == '\n') {
        s[len - 1] = '\0';
    }
}

/* ---------- Load tasks from file ---------- */
void load_tasks() {
    FILE *f = fopen(TASKS_FILE, "r");
    if (!f) {
        // No file yet, that's fine
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

/* ---------- Save tasks to file ---------- */
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

/* ---------- List tasks ---------- */
void list_tasks() {
    if (task_count == 0) {
        printf("No tasks yet.\n");
        return;
    }

    for (int i = 0; i < task_count; i++) {
        printf("%d. [%c] %s\n",
               i + 1,
               tasks[i].done ? 'x' : ' ',
               tasks[i].desc);
    }
}

/* ---------- Add a task ---------- */
void add_task(const char *desc) {
    if (task_count >= MAX_TASKS) {
        printf("Task list is full.\n");
        return;
    }

    tasks[task_count].done = 0;
    strncpy(tasks[task_count].desc, desc, MAX_DESC - 1);
    tasks[task_count].desc[MAX_DESC - 1] = '\0';

    task_count++;
    save_tasks();
    printf("Added: %s\n", desc);
}

/* ---------- Mark a task as done ---------- */
void mark_done(int index) {
    if (index < 1 || index > task_count) {
        printf("Invalid task number.\n");
        return;
    }

    tasks[index - 1].done = 1;
    save_tasks();
    printf("Marked task %d as done.\n", index);
}

/* ---------- Show usage ---------- */
void print_usage(const char *progname) {
    printf("Usage:\n");
    printf("  %s list\n", progname);
    printf("  %s add \"task description\"\n", progname);
    printf("  %s done TASK_NUMBER\n", progname);
}

/* ---------- main ---------- */
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
            printf("Error: missing task description.\n");
            print_usage(argv[0]);
            return 1;
        }
        add_task(argv[2]);  // simple: only uses argv[2]; could join more args later
    } else if (strcmp(argv[1], "done") == 0) {
        if (argc < 3) {
            printf("Error: missing task number.\n");
            print_usage(argv[0]);
            return 1;
        }
        int index = atoi(argv[2]);
        mark_done(index);
    } else {
        printf("Unknown command: %s\n", argv[1]);
        print_usage(argv[0]);
        return 1;
    }

    return 0;
}
