/**
 * @file procsim.c
 * @brief Program entry point.  Runs the process scheduler simulation
 *
 * Course: CSC3210
 * Section: 003
 * Assignment: Process Simulator
 * Name: Hudson Arney
 */

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_PROCESSES 100
#define MAX_IO_DEVICES 10

typedef struct
{
    int pid;
    int priority;
    int io_device;
    int start_ready_wait_time;
    int ready_wait_time;
    int start_io_wait_time;
    int io_wait_time;
    bool io_pending;
} Process;

Process processes[MAX_PROCESSES];        
Process all_processes[MAX_PROCESSES];    
int num_processes = 0;                  
int front = 0;                           // Front of the queue
int rear = -1;                           // Rear of the queue
int count = 0;                           // Number of elements in the queue
bool preemptive_scheduler = false;       // Flag for preemptive scheduling
int current_time = 0;                    
int idle_time = 0;                       
int start_idle_time = 0;                 
int io_completion_times[MAX_IO_DEVICES]; // Array to store completion times for I/O devices
int process_completion_count = 0;        
Process running_process;              
Process io_processes[MAX_IO_DEVICES];    // Array to store processes waiting for I/O
int io_process_count = 0;                // Number of processes waiting for I/O

void enqueue(Process process);
Process dequeue();
bool isQueueEmpty();
void handleProcessStart(int priority);
void handleIORequest(int io_device);
void handleIOEnd(int io_device);
void handleProcessEnd();
void printStatistics();

int main(int argc, char *argv[])
{
    char *samples_dir = "samples/";
    char *filename = "sample2_input.txt";
    char *full_path = malloc(strlen(samples_dir) + strlen(filename) + 1);
    strcpy(full_path, samples_dir);
    strcat(full_path, filename);

    FILE *input = fopen(full_path, "r"); 
    free(full_path);                    

    if (input == NULL)
    {
        printf("Error opening file.\n");
        perror("fopen");
        return 1;
    }

    int preemptive_scheduler_int;
    fscanf(input, "%d", &preemptive_scheduler_int);
    preemptive_scheduler = (preemptive_scheduler_int != 0);

    // Read each event
    int event_time;
    int operation_code;
    int priority;
    int io_device;

    printf("Simulation started: Preemption: %s\n", preemptive_scheduler ? "True" : "False");
    printf("\n");
    while (fscanf(input, "%d %d", &event_time, &operation_code) == 2)
    {
        current_time = event_time;

        // set first time to idle time
        if (idle_time == 0)
        {
            idle_time = current_time;
        }

        switch (operation_code)
        {
        case 1:
            // Process start
            fscanf(input, "%d", &priority);
            handleProcessStart(priority);
            break;
        case 2:
            // I/O request
            fscanf(input, "%d", &io_device);
            handleIORequest(io_device);
            break;
        case 3:
            // I/O request complete
            fscanf(input, "%d", &io_device);
            handleIOEnd(io_device);
            break;
        case 4:
            handleProcessEnd();
            break;
        }
        if (running_process.pid == 0)
        {
            start_idle_time = current_time;
        }
        if (running_process.pid != 0 && start_idle_time != 0)
        {
            idle_time += current_time - start_idle_time;
            start_idle_time = 0;
        }
    }

    fclose(input);

    printStatistics();

    return 0;
}

void enqueue(Process process)
{
    if (!isQueueEmpty())
    {
        int i = 0;
        for (i = 0; i < count; i++)
        {
            if (process.priority < processes[i].priority)
            {
                break;
            }
        }
        for (int j = count; j > i; j--)
        {
            processes[j] = processes[j - 1];
        }
        processes[i] = process;
        count++;
    }
    else
    {
        processes[0] = process;
        count++;
    }
}

Process dequeue()
{
    if (!isQueueEmpty())
    {
        // loop through processes array to find the process with the highest priority
        Process highest_priority_process = processes[0];
        int highest_priority_process_index = 0;
        for (int i = 1; i < count; i++)
        {
            if (processes[i].priority > highest_priority_process.priority)
            {
                highest_priority_process = processes[i];
                highest_priority_process_index = i;
            }
        }
        // shift elements to the left
        for (int i = highest_priority_process_index; i < count - 1; i++)
        {
            processes[i] = processes[i + 1];
        }
        count--;
        return highest_priority_process;
    }
}

bool isQueueEmpty()
{
    // loop through processes array to check if there are no processes
    bool empty = true;
    for (int i = 0; i < MAX_PROCESSES; i++)
    {
        if (processes[i].pid != 0)
        {
            empty = false;
            break;
        }
    }
    return empty;
}

void handleProcessStart(int priority)
{
    num_processes++;

    // Initialize new process
    Process new_process;
    new_process.pid = num_processes;
    new_process.priority = priority;
    new_process.io_device = -1;
    new_process.ready_wait_time = 0;
    new_process.start_ready_wait_time = current_time;
    new_process.io_wait_time = 0;
    new_process.io_pending = false;

    // Print process start message
    printf("%d: Staring process with PID: %d PRIORITY: %d\n", current_time, num_processes, priority);

    // Schedule new process if no process is currently running
    if (count == 0)
    {
        printf("%d: Process scheduled to run with PID: %d PRIORITY: %d\n", current_time, num_processes, priority);
        running_process = new_process;
        new_process.ready_wait_time += current_time - new_process.start_ready_wait_time;

    }
    else if (preemptive_scheduler)
    {
        // Preemptive scheduling
        if (new_process.priority > running_process.priority)
        {
            // Preempt the currently running process
            running_process.start_ready_wait_time = current_time;
            enqueue(running_process);
            running_process = new_process;
            new_process.ready_wait_time += current_time - new_process.start_ready_wait_time;
            printf("%d: Process scheduled to run with PID: %d PRIORITY: %d\n", current_time, new_process.pid, new_process.priority);
        }
        else
        {
            // Enqueue the new process
            enqueue(new_process);
        }
    }
    else
    {
        // Enqueue the new process
        enqueue(new_process);
    }
    count++;
}

void handleIORequest(int io_device)
{
    // Update process information
    running_process.io_device = io_device;
    running_process.io_pending = true;

    printf("%d: Process with PID: %d waiting for I/O device %d\n", current_time, running_process.pid, io_device);
    running_process.start_io_wait_time = current_time;

    // add process to io_processes array
    io_processes[io_device - 1] = running_process;
    running_process = dequeue();
    running_process.ready_wait_time += current_time - running_process.start_ready_wait_time;
    printf("%d: Starting process with PID: %d PRIORITY: %d\n", current_time, running_process.pid, running_process.priority);
}

void handleIOEnd(int io_device)
{
    // Update process information
    io_processes[io_device - 1].io_pending = false;
    io_processes[io_device - 1].io_wait_time += current_time - io_completion_times[io_device - 1];

    printf("%d: I/O completed for I/O device %d\n", current_time, io_device);

    io_processes[io_device - 1].io_wait_time = current_time - io_processes[io_device - 1].start_io_wait_time;

    if (running_process.pid == 0)
    {
        running_process = io_processes[io_device - 1];
        
    }
    else
    {
        enqueue(io_processes[io_device - 1]);
    }
    // set a dummy process in io_processes array
    io_processes[io_device - 1] = (Process){.pid = 0};

    // Schedule process to run after I/O completion
    if (!isQueueEmpty())
    {
        printf("%d: Process scheduled to run with PID: %d PRIORITY: %d\n", current_time, running_process.pid, running_process.priority);
    }
}

void handleProcessEnd()
{
    // Update process information
    process_completion_count++;

    printf("%d: Ending process with PID: %d\n", current_time, running_process.pid);

    all_processes[running_process.pid - 1] = running_process;

    count--;

    // Dequeue the completed process
    running_process = dequeue();
    running_process.ready_wait_time += current_time - running_process.start_ready_wait_time;

    if (running_process.pid != 0)
    {
        printf("%d: Process scheduled to run with PID: %d PRIORITY: %d\n", current_time, running_process.pid, running_process.priority);
    }
}

void printStatistics()
{
    printf("\nSimulation ended at time: %d\n", current_time);
    printf("System idle time: %d\n", idle_time);
    printf("\nProcess Information:\n");
    for (int i = 0; i < num_processes; i++)
    {
        printf("PID: %d, PRIORITY: %d, READY WAIT TIME: %d, I/O WAIT TIME: %d\n", all_processes[i].pid, all_processes[i].priority,
               all_processes[i].ready_wait_time, all_processes[i].io_wait_time);
    }
    printf("\n");
}