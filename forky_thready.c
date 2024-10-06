#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

void process_behavior(int id, int num_procs)
{
    printf("Process %d (PID: %d) beginning\n", id, getpid());

    srand(time(NULL) ^ (getpid() << 16));
    int sleep_time = rand() % 8 + 1;
    sleep(sleep_time);

    if (id < num_procs)
    {
        printf("Process %d (PID: %d) creating Process %d\n", id, getpid(), id + 1);
    }

    printf("Process %d (PID: %d) exiting\n", id, getpid());
}

void pattern_1(int num_procs)
{
    for (int i = 1; i <= num_procs; i++)
    {
        pid_t pid = fork();
        if (pid == 0)
        {
            // Child process
            process_behavior(i, num_procs);
            exit(0);
        }
    }

    for (int i = 0; i < num_procs; i++)
    {
        wait(NULL);
    }
}

void pattern_2(int id, int num_procs)
{
    if (id > num_procs)
        return;

    pid_t pid = fork();
    if (pid == 0)
    {
        process_behavior(id, num_procs);
        pattern_2(id + 1, num_procs); // Recursive call to create the next process
        exit(0);
    }
    else
    {
        wait(NULL);
    }
}

void pattern_3(int id)
{
    if (id > 7)
        return;

    pid_t pid1 = fork();
    if (pid1 == 0 && id * 2 <= 7)
    {
        // First child of current process
        process_behavior(id * 2, 7);
        pattern_3(id * 2); // Recursive call for child 1
        exit(0);
    }

    pid_t pid2 = fork();
    if (pid2 == 0 && id * 2 + 1 <= 7)
    {
        // Second child of current process
        process_behavior(id * 2 + 1, 7);
        pattern_3(id * 2 + 1); // Recursive call for child 2
        exit(0);
    }

    // Wait for both children
    wait(NULL);
    wait(NULL);
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        fprintf(stderr, "Usage: %s <number of processes> <pattern number>\n", argv[0]);
        return 1;
    }

    int num_procs = atoi(argv[1]);
    int pattern = atoi(argv[2]);

    if (num_procs < 1 || num_procs > 256)
    {
        fprintf(stderr, "Number of processes should be between 1 and 256.\n");
        return 1;
    }

    printf("Starting pattern %d with %d processes...\n", pattern, num_procs);

    switch (pattern)
    {
    case 1:
        pattern_1(num_procs);
        break;
    case 2:
        pattern_2(1, num_procs);
        break;
    case 3:
        pattern_3(1);
        break;
    default:
        fprintf(stderr, "Invalid pattern number. Please choose 1, 2, or 3.\n");
        return 1;
    }

    printf("All processes have completed.\n");
    return 0;
}
