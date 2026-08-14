#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
    int i;
    pid_t pid;
    pid_t child[3];
    int status;

    printf("Parent process started. PID = %d\n", getpid());

    for (i = 0; i < 3; i++)
    {
        pid = fork();

        if (pid < 0)
        {
            perror("fork failed");
            exit(1);
        }

        if (pid == 0)
        {
            printf("Child %d started. PID = %d\n", i + 1, getpid());
            sleep(3 - i);
            printf("Child %d completed. PID = %d\n", i + 1, getpid());
            exit(i + 1);
        }
        else
        {
            child[i] = pid;
        }
    }

    printf("\nParent is waiting for specific children using waitpid()...\n");

    for (i = 0; i < 3; i++)
    {
        pid = waitpid(child[i], &status, 0);

        if (pid > 0)
        {
            if (WIFEXITED(status))
            {
                printf("Parent collected child PID %d with exit status %d\n",
                       pid, WEXITSTATUS(status));
            }
        }
    }

    printf("All child processes completed.\n");
    printf("Parent process exiting. PID = %d\n", getpid());

    return 0;
}
