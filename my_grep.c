#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc < 2) printf("my_grep: searchterm [file ...]\n"), exit(1);
    char *buffer = NULL, n = '\0';
    int i = 0;
    size_t buffer_size = 0;
    ssize_t line_size;

    for (int z = (argc == 2) ? 1 : 2; z <= argc - 1; z++) {
        FILE *ftemp = (argc != 2) ? fopen(argv[z], "r") : stdin;
        if (argc != 2 && ftemp == NULL) printf("my_grep: cannot open file\n"), exit(1);

        line_size = getline(&buffer, &buffer_size, ftemp);

        while (line_size >= 0) {
            (((line_size < 0) && i != 0) || (strstr(buffer, argv[1]))) ? ((line_size < 0) && i != 0) ? printf("\n"), i++ : printf("%s", buffer), i++ : n;
            line_size = getline(&buffer, &buffer_size, ftemp);
        }

        i = 1;
        fclose(ftemp);
    }
}