#include <stdio.h>
#include <stdlib.h>
int compare (const void* first, const void* second) {
    return (*((int*) first) > *((int*) second)) - (*((int*) first) < *((int*) second)); // Cast int* types to void* arguments, then dereference. Returns -1/0/1.
}

int main(int argc, char *argv[]) {
    if (argc != 2) exit(1);
    FILE *ftemp = fopen(argv[1], "r"), *f = fopen(argv[1], "r");
    int temp, i = 0;

    for (; fscanf(ftemp, "%d,", &temp) > 0; i++);
    int *numbers = (int*)malloc(sizeof(int) * i);
    for (i = 0; fscanf(f, "%d,", &numbers[i]) > 0; i++);

    qsort (numbers, i, sizeof(int), compare);

    for (int p = 0; p < i; p++)
        (p < (i - 1)) ? printf("%d ", numbers[p]) : printf("%d", numbers[p]);

    free(numbers);
}
