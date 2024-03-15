#include <stdio.h>
#include <stdlib.h>

#define MAX_PROCESSES 1000
#define MAX_RESOURCES 5
#define MAX_MEMORY 1024

typedef struct {
    int id;
    int priority;
    int arrival_time;
    int execution_time;
    int memory_size;
    int printers;
    int scanners;
    int modems;
    int cd_drives;
} Process;

typedef struct Node {
    Process process;
    struct Node* next;
} Node;

typedef struct {
    Node* front;
    Node* rear;
} Queue;

typedef struct {
    int printers;
    int scanners;
    int modems;
    int cd_drives;
    int memory_used;
} Resources;

Resources resources = {2, 1, 1, 2, 0};

void read_dispatch_list(Queue *queue) {
    char filename[100];
    FILE *file;

    // Prompt the user for the file name
    printf("Enter the address file of the Dispatcher list: ");
    scanf("%s", filename);

    // Open the file for reading
    file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    // Read the file line by line and add processes to the queue
    while (!feof(file)) {
        Process process;
        int res = fscanf(file, "%d, %d, %d, %d, %d, %d, %d, %d, %d",
                  &process.arrival_time,
                  &process.priority,
                  &process.execution_time,
                  &process.memory_size,
                  &process.printers,
                  &process.scanners,
                  &process.modems,
                  &process.cd_drives,
                  &process.id);
        if (res == 9) {
            Node* new_node = (Node*)malloc(sizeof(Node));
            new_node->process = process;
            new_node->next = NULL;
            queue->rear->next = new_node;
            queue->rear = new_node;
        }
    }

    // Close the file
    fclose(file);
}

void execute_process(Process process) {
    // Simulate process execution
    printf("Executing process %d (Priority: %d, Arrival Time: %d, Execution Time: %d)\n",
           process.id, process.priority, process.arrival_time, process.execution_time);
}

void allocate_resources(Process process) {
    // Allocate resources to the process
    resources.printers -= process.printers;
    resources.scanners -= process.scanners;
    resources.modems -= process.modems;
    resources.cd_drives -= process.cd_drives;
    resources.memory_used += process.memory_size;
}

void release_resources(Process process) {
    // Release resources used by the process
    resources.printers += process.printers;
    resources.scanners += process.scanners;
    resources.modems += process.modems;
    resources.cd_drives += process.cd_drives;
    resources.memory_used -= process.memory_size;
}

void run_FCFS(Queue *queue) {
    printf("f");
    Node* current = queue->front->next; // Skip the dummy node
    while (current != NULL) {
        printf("ff");
        Process process = current->process;
        if (process.priority == 0) { // Real-Time process
            execute_process(process);
            release_resources(process);
        }
        current = current->next; // Move to the next process
    }
}

void run_User_Feedback(Queue *queue) {
    Node* current = queue->front->next; // Skip the dummy node
    while (current != NULL) {
        Process process = current->process;
        if (process.priority > 0 && process.priority < 4) { // User priority process
            if (process.memory_size <= MAX_MEMORY && 
                process.printers <= resources.printers &&
                process.scanners <= resources.scanners &&
                process.modems <= resources.modems &&
                process.cd_drives <= resources.cd_drives) {
                execute_process(process);
                allocate_resources(process);
            } else {
                printf("Insufficient resources to execute process %d\n", process.id);
            }
        }
        current = current->next; // Move to the next process
    }
}

void run_Mixed(Queue *queue) {
    Node* current = queue->front->next; // Skip the dummy node
    while (current != NULL) {
        Process process = current->process;
        if (process.memory_size <= MAX_MEMORY && 
            process.printers <= resources.printers &&
            process.scanners <= resources.scanners &&
            process.modems <= resources.modems &&
            process.cd_drives <= resources.cd_drives) {
            execute_process(process);
            allocate_resources(process);
        } else {
            printf("Insufficient resources to execute process %d\n", process.id);
        }
        current = current->next; // Move to the next process
    }
}

int main() {
    Queue queue;
    queue.front = (Node*)malloc(sizeof(Node));
    queue.front->next = NULL;
    queue.rear = queue.front;

    read_dispatch_list(&queue);
    
    // Run scheduling algorithms
    run_FCFS(&queue);
    run_User_Feedback(&queue);
    run_Mixed(&queue);

    // Free memory used by the queue
    Node* current = queue.front;
    while (current != NULL) {
        Node* temp = current;
        current = current->next;
        free(temp);
    }

    return 0;
}
