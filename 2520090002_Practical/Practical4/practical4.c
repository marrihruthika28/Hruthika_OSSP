#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
    int i;
    pid_t pid;
    int status;

    printf("Parent process started. PID = %d\n", getpid());

    for (i = 1; i <= 3; i++)
    {
        pid = fork();

        if (pid < 0)
        {
            perror("fork failed");
            exit(1);
        }

        if (pid == 0)
        {
            printf("Child %d started. PID = %d\n", i, getpid());
            sleep(i);
            printf("Child %d completed. PID = %d\n", i, getpid());
            exit(i);
        }
    }

    printf("\nParent is waiting for children using wait()...\n");

    for (i = 0; i < 3; i++)
    {
        pid = wait(&status);

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
