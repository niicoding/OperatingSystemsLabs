#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc ==1) printf("my_wc: [option ...] [file...]\n"), exit(1);
    int wc = 0, lc = 0, cc = 0, line_bool = 0, word_bool = 0, char_bool = 0, arg_val, i = -0, fc = 0;

    for (int f = 1; f < argc; (!(argv[f][0] == '-' && argv[f][2] == '\0' && (argv[f][1] == 'l' || 'w' || 'c'))) ? fc++, f++ : f++);

    for (int z = argc - fc; z <= argc - 1; z++) {
        FILE *ftemp = fopen(argv[z], "r");

        for (int i = 1; i <= (argc - fc); i++) {
            arg_val = (argv[i][0] == '-' && argv[i][2] == '\0') ? (argv[i][1] == 'l') ? 1 : (argv[i][1] == 'w') ? 2 : (argv[i][1] == 'c') ? 3 : -1 : (argc - fc == 1) ? 0 : -1;

            switch (arg_val) {
                case 1:
                    line_bool = (line_bool == 0) ? 1 : -1;
                    break;
                case 2:
                    word_bool = (word_bool == 0) ? 1 : -1;
                    break;
                case 3:
                    char_bool = (char_bool == 0) ? 1 : -1;
                    break;
                case 0:
                    line_bool = 1, word_bool = 1, char_bool = 1;
                    break;
            }
        }

        (word_bool == -1 || char_bool == -1 || line_bool == -1) ? printf("my_wc: [option ...] [file...]\n"), exit(1) : (ftemp == NULL) ? printf("my_wc: cannot open file\n"), exit(1) : i;

        char *buffer = NULL;
        size_t buffer_size = 0;
        ssize_t line_size;

        line_size = getline(&buffer, &buffer_size, ftemp);
        lc = 0;
        while (line_size >= 0) {
            for (int b = 0; b <= line_size; b++) {
                if (buffer[b] == ' ') wc++;
            }

            if (buffer[0] != ' ' && buffer[line_size - 1] != ' ') wc++;

            cc = cc + line_size;
            line_size = getline(&buffer, &buffer_size, ftemp);
            lc++;
        }

        if (line_bool == 1 || arg_val == 0) printf("%d ", lc);
        if (word_bool == 1 || arg_val == 0) printf("%d ", wc);
        if (char_bool == 1 || arg_val == 0) printf("%d ", cc);
        (z == argc - 1) ? printf("%s", argv[z]) : printf("%s", argv[z]), printf("\n");

        wc = 0, lc = 0, cc = 0, line_bool = 0, word_bool = 0, char_bool = 0, arg_val = -0;
        fclose(ftemp);
    }
}