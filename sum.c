#include <stdio.h>
#include <stdlib.h>
int main(int argc, char *argv[]) {
    if (argc != 2) exit(1);
    FILE *fp = fopen(argv[1], "r");
    for (int total = 0, i = 0, numbers[10]; fscanf(fp, "%d,", &numbers[i]) > 0; total += numbers[i++])
        if (i == 9) printf("%d", total + numbers[9]);
}