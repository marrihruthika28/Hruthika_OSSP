#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
    pid_t pid;

    pid = fork();

    if (pid < 0)
    {
        perror("fork failed");
        exit(1);
    }

    if (pid == 0)
    {
        printf("Child process started. PID = %d\n", getpid());
        printf("Child process is exiting...\n");
        exit(0);
    }
    else
    {
        printf("Parent process started. PID = %d\n", getpid());
        printf("Child PID = %d\n", pid);

        printf("Parent will NOT call wait() immediately.\n");
        printf("The child will become a zombie process.\n");

        sleep(20);

        printf("Parent is now calling wait()...\n");
        wait(NULL);

        printf("Zombie process has been removed.\n");
    }

    return 0;
}
