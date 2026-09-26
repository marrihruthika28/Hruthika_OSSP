#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    int *data = (int *)malloc(5 * sizeof(int));

    if (data == NULL) {
        printf("Memory allocation failed\n");
        return 1;
    }

    for (int i = 0; i < 5; i++) {
        data[i] = (i + 1) * 10;
    }

    printf("Before fork:\n");
    printf("Parent PID: %d\n", getpid());

    printf("Data address: %p\n", (void *)data);

    printf("Data: ");
    for (int i = 0; i < 5; i++) {
        printf("%d ", data[i]);
    }
    printf("\n\n");

    pid_t pid = fork();

    if (pid < 0) {
        perror("fork failed");
        free(data);
        return 1;
    }

    if (pid == 0) {
        // Child process
        printf("Child process:\n");
        printf("Child PID: %d\n", getpid());
        printf("Data address before modification: %p\n", (void *)data);

        printf("Child data before modification: ");
        for (int i = 0; i < 5; i++) {
            printf("%d ", data[i]);
        }
        printf("\n");

        // Modify data in child
        data[0] = 999;

        printf("Child data after modification: ");
        for (int i = 0; i < 5; i++) {
            printf("%d ", data[i]);
        }
        printf("\n");

        printf("Data address after modification: %p\n\n",
               (void *)data);

        free(data);
        exit(0);
    }
    else {
        // Parent process
        wait(NULL);

        printf("Parent process after child modification:\n");

        printf("Parent data: ");
        for (int i = 0; i < 5; i++) {
            printf("%d ", data[i]);
        }
        printf("\n");

        printf("Parent data address: %p\n", (void *)data);

        free(data);
    }

    return 0;
}
