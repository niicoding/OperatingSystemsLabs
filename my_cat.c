#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    if (argc < 2) exit(0);
    int q = 0;
    FILE *ftemp;
    char *buffer = NULL;
    size_t buffer_size = 0;
    ssize_t line_size;

    for (int i = 1; i < argc; i++) {
        ftemp = fopen(argv[i], "r");

        if (ftemp == NULL) {
            printf("my_cat: cannot open file\n");
            exit(1);
        }

        line_size = getline(&buffer, &buffer_size, ftemp);

        while (line_size >= 0) {
            (((line_size < 0) && i != 0)) ? printf("\n"), printf("%s", buffer), q++ : printf("%s", buffer), q++;
            line_size = getline(&buffer, &buffer_size, ftemp);
        }
        q = 0;
    }

    free(buffer);
    buffer = NULL;
    fclose(ftemp);
}
