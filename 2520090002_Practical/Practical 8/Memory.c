#include <stdio.h>
#include <stdlib.h>

int main() {
    int i;

    // malloc()
    int *a = (int *)malloc(5 * sizeof(int));

    if (a == NULL) {
        printf("malloc failed\n");
        return 1;
    }

    printf("Memory allocated using malloc()\n");

    for (i = 0; i < 5; i++) {
        a[i] = (i + 1) * 10;
    }

    printf("Values using malloc: ");
    for (i = 0; i < 5; i++) {
        printf("%d ", a[i]);
    }
    printf("\n");

    // calloc()
    int *b = (int *)calloc(5, sizeof(int));

    if (b == NULL) {
        printf("calloc failed\n");
        free(a);
        return 1;
    }

    printf("Memory allocated using calloc()\n");

    printf("Initial calloc values: ");
    for (i = 0; i < 5; i++) {
        printf("%d ", b[i]);
    }
    printf("\n");

    // realloc()
    a = (int *)realloc(a, 10 * sizeof(int));

    if (a == NULL) {
        printf("realloc failed\n");
        free(b);
        return 1;
    }

    printf("Memory resized using realloc()\n");

    for (i = 5; i < 10; i++) {
        a[i] = (i + 1) * 10;
    }

    printf("Values after realloc: ");
    for (i = 0; i < 10; i++) {
        printf("%d ", a[i]);
    }
    printf("\n");

    // free()
    free(a);
    free(b);

    printf("Memory released using free()\n");

    return 0;
}
