#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

typedef struct {
    Process processes[MAX_PROCESSES];
    int front;
    int rear;
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
    while (fscanf(file, "%d, %d, %d, %d, %d, %d, %d, %d, %d",
                  &queue->processes[queue->rear].arrival_time,
                  &queue->processes[queue->rear].priority,
                  &queue->processes[queue->rear].execution_time,
                  &queue->processes[queue->rear].memory_size,
                  &queue->processes[queue->rear].printers,
                  &queue->processes[queue->rear].scanners,
                  &queue->processes[queue->rear].modems,
                  &queue->processes[queue->rear].cd_drives,
                  &queue->processes[queue->rear].id) == 9) {
        queue->rear++;
        if (queue->rear >= MAX_PROCESSES) {
            printf("Max processes reached.\n");
            break;
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
    // Run processes in FCFS order
    for (int i = queue->front; i < queue->rear; i++) {
        Process process = queue->processes[i];
        if (process.priority == 0) { // Real-Time process
            execute_process(process);
            release_resources(process);
        }
    }
}

void run_User_Feedback(Queue *queue) {
    // Implement User Feedback scheduler

}

void run_Mixed(Queue *queue) {
    // Implement Mixed scheduler
    
}

int main() {
    Queue queue;
    queue.front = 0;
    queue.rear = 0;

    read_dispatch_list(&queue);

    // Run scheduling algorithms
    run_FCFS(&queue);
    run_User_Feedback(&queue);
    run_Mixed(&queue);

    return 0;
}
