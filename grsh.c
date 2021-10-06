#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>


void error() {
    char error_message[30] = "An error has occurred\n";
    write(STDERR_FILENO, error_message, strlen(error_message));
}

void cd(char* cd) {
    if (chdir(cd) == -1) error();
}

void interactive(){
    int empty_path = 0;
    int bin_missing = 0;

    while(1) {
        char *buffer = NULL, *command[1000], *new_arr[1000];
        int c, amp_count = 0;
        char delimitspaces[] = " \t\r\n\v\f\n";
        char delimitamps[] =  "&";

        printf("grsh> ");
        c = getchar();
        (c != EOF) ? ungetc(c, stdin) : exit(0);

        size_t buffer_size = 0;
        getline(&buffer, &buffer_size, stdin);

        int cc = 0;
        for (int a = 0; a < 1; a++) {
            char* delimitedstr = (char *) malloc(sizeof(char) * 2000);

            for (int d = 0; d < strlen(buffer); d++) {
                if (buffer[d] != '>') {
                    delimitedstr[cc] = buffer[d];
                    cc++;
                } else {
                    delimitedstr[cc] = ' ';
                    delimitedstr[cc+1] = '>';
                    delimitedstr[cc+2] = ' ';
                    cc = cc + 3;
                }
            }
            buffer = delimitedstr;
        }

        char *tokenamps = strtok(buffer, delimitamps);

        int t = 0;
        while (tokenamps != NULL) {
            command[t] = tokenamps;
            amp_count++;
            tokenamps = strtok(NULL, delimitamps);
            //printf("%s\n", command[t]);
            t++;
        }

        int r = 0;
        for (int a = 0; a < t; a++, r++) {
            char *token = strtok(command[a], delimitspaces);

            while (token != NULL) {
                new_arr[r] = token;
                token = strtok(NULL, delimitspaces);
                r++;
            }
            if (a != t - 1) new_arr[r] = "&";
        }

        getenv("PATH"),



        new_arr[r - 1] = ((char *) '\n' == new_arr[r - 1]) ? '\0' : new_arr[r - 1];

        int x = 0;
        for (int z = 0; z < amp_count; z++) {

            char *parr_arr[1000];
            int pai = 0;

            for (; x < r - 1; x++, pai++) {
                if (strstr(new_arr[x], "&")) {
                    x++;
                    break;
                }

                parr_arr[pai] = new_arr[x];
            }
            parr_arr[pai] = '\0';
            parr_arr[pai + 1] = (char*) NULL;

            char *av = (char *) malloc(sizeof(char) * 2000);
            av[0] = '\0';

            char *bin_file = (char *) malloc(sizeof(char) * 2000);
            strncat(bin_file, "/bin/", 6);
            strncat(bin_file, parr_arr[0], strlen(parr_arr[0]) + 1);
            char *usr_bin_file = (char *) malloc(sizeof(char) * 2000);
            strncat(usr_bin_file, "/usr/bin/", 10);
            strncat(usr_bin_file, parr_arr[0], strlen(parr_arr[0]) + 1);

            int ac = ((access(bin_file, F_OK) == -1) && (access(usr_bin_file, F_OK) == -1)) ? -1 : 0;
            av = ((access(bin_file, F_OK) == -1)) ? usr_bin_file : bin_file;

            if (strstr(parr_arr[0], "exit") || strstr(parr_arr[0], "cd") || strstr(parr_arr[0], "path")) {
                if (strstr(parr_arr[0], "exit")) (pai > 1) ? error() : exit(0);
                if (strstr(parr_arr[0], "cd") && pai == 2) cd(parr_arr[1]);


                if (strstr(parr_arr[0], "path")) {
                    char *path_args[1000];
                    for (int u = 0; u < pai - 1; u++)
                        path_args[u] = parr_arr[u + 1];

                    char *path = (char *) malloc(sizeof(char) * 2000);
                    path[0] = '\0';
                    if (pai > 1) {
                        bin_missing = 1;
                        for (int j = 0; j < pai - 1; j++) {
                            size_t n = strlen(path_args[j]);
                            if (strstr(path_args[j], "/bin") || strstr(path_args[j], "/usr/bin")) bin_missing = 0;
                            if (j != 0) strncat(path, ":", 2);
                            strncat(path, path_args[j], n + 1);
                        }
                    }

                    if (pai == 1) setenv("PATH", "", 1), empty_path = 1;
                    if (pai != 1) setenv("PATH", path, 1), empty_path = 0;
                }

            } else {
                int redir_error = 0;
                int file_r_error = 0;
                int redir_pres  = 0;
                int redir_index = -1;
                int file_index = 0;
                char *arr_redir[1000];

                for (int k = 0; k < pai; k++) {
                    if (parr_arr[k][0] == '>') redir_pres = 1, redir_index = k;
                    if (strstr(parr_arr[k], ">") && redir_pres == 1 && redir_index != k) redir_error = 1;
                    if (redir_pres == 1 && k > redir_index && k != redir_index + 1) file_r_error = 1;

                    for (int p = 0; p < strlen(new_arr[k]); p++) {
                        if (parr_arr[k][p] == '>' && p != 0) redir_error = 1;
                    }

                    if (parr_arr[k][0] != '>') arr_redir[file_index++] = parr_arr[k];
                }
                file_index--;

                if (parr_arr[0][0] == '>') redir_error = 1;

                if (ac != -1 && empty_path == 0 && bin_missing == 0) {
                    pid_t child_pid;
                    int stat_loc;

                    child_pid = fork();
                    if (child_pid == 0) {
                        if (redir_pres == 1) {
                            if (redir_error == 0 && file_r_error == 0) {
                                freopen(arr_redir[file_index], "w", stdout);
                                freopen(arr_redir[file_index], "w", stderr);
                                arr_redir[file_index] = '\0';
                                arr_redir[file_index + 1] = (char *) NULL;
                                execvp(av, arr_redir);
                                exit(1);
                            } else error();
                        } else if (redir_pres == 0) execvp(av, parr_arr); exit(1);
                    } else waitpid(child_pid, &stat_loc, WUNTRACED);
                } else error();
            }
        }
        free(buffer);
        buffer = NULL;
    }
}

void batch(char* argument){
    FILE *fp = fopen(argument, "r");
    
    if(fp == NULL){
        error();
        exit(1);
    }

    int empty_path = 0;
    int bin_missing = 0;

    while(!feof(fp)){
        char *buffer = NULL, *command[1000], *new_arr[1000];
        int amp_count = 0;
        char delimitspaces[] = " \t\r\n\v\f\n";
        char delimitamps[] =  "&";

        size_t buffer_size = 0;
        getline(&buffer, &buffer_size, fp);
        buffer[strlen(buffer) -1] = '\0';

        int cc = 0;
        for (int a = 0; a < 1; a++) {
            char* delimitedstr = (char *) malloc(sizeof(char) * 2000);

            for (int d = 0; d < strlen(buffer); d++) {
                if (buffer[d] != '>') {
                    delimitedstr[cc] = buffer[d];
                    cc++;
                } else {
                    delimitedstr[cc] = ' ';
                    delimitedstr[cc+1] = '>';
                    delimitedstr[cc+2] = ' ';
                    cc = cc + 3;
                }
            }
            buffer = delimitedstr;
        }

        char *tokenamps = strtok(buffer, delimitamps);

        int t = 0;
        while (tokenamps != NULL) {
            command[t] = tokenamps;
            amp_count++;
            tokenamps = strtok(NULL, delimitamps);
            t++;
        }

        int r = 0;
        for (int a = 0; a < t; a++, r++) {
            char *token = strtok(command[a], delimitspaces);

            while (token != NULL) {
                new_arr[r] = token;
                token = strtok(NULL, delimitspaces);
                r++;
            }
            if (a != t - 1) new_arr[r] = "&";
        }

        getenv("PATH");

        new_arr[r - 1] = ((char *) '\n' == new_arr[r - 1]) ? '\0' : new_arr[r - 1];

        int x = 0;
        for (int z = 0; z < amp_count; z++) {

            char *parr_arr[1000];
            int pai = 0;

            for (; x < r - 1; x++, pai++) {
                if (strstr(new_arr[x], "&")) {
                    x++;
                    break;
                }

                parr_arr[pai] = new_arr[x];
            }
            parr_arr[pai] = '\0';
            parr_arr[pai + 1] = (char*) NULL;

            char *av = (char *) malloc(sizeof(char) * 2000);
            av[0] = '\0';

            char *bin_file = (char *) malloc(sizeof(char) * 2000);
            strncat(bin_file, "/bin/", 6);
            strncat(bin_file, parr_arr[0], strlen(parr_arr[0]) + 1);
            char *usr_bin_file = (char *) malloc(sizeof(char) * 2000);
            strncat(usr_bin_file, "/usr/bin/", 10);
            strncat(usr_bin_file, parr_arr[0], strlen(parr_arr[0]) + 1);

            int ac = ((access(bin_file, F_OK) == -1) && (access(usr_bin_file, F_OK) == -1)) ? -1 : 0;
            av = ((access(bin_file, F_OK) == -1)) ? usr_bin_file : bin_file;

            if (strstr(parr_arr[0], "exit") || strstr(parr_arr[0], "cd") || strstr(parr_arr[0], "path")) {
                if (strstr(parr_arr[0], "exit")) (pai > 1) ? error() : exit(0);
                if (strstr(parr_arr[0], "cd") && pai == 2) cd(parr_arr[1]);


                if (strstr(parr_arr[0], "path")) {
                    char *path_args[1000];
                    for (int u = 0; u < pai - 1; u++)
                        path_args[u] = parr_arr[u + 1];

                    char *path = (char *) malloc(sizeof(char) * 2000);
                    path[0] = '\0';
                    if (pai > 1) {
                        bin_missing = 1;
                        for (int j = 0; j < pai - 1; j++) {
                            size_t n = strlen(path_args[j]);
                            if (strstr(path_args[j], "/bin") || strstr(path_args[j], "/usr/bin")) bin_missing = 0;
                            if (j != 0) strncat(path, ":", 2);
                            strncat(path, path_args[j], n + 1);
                        }
                    }

                    if (pai == 1) setenv("PATH", "", 1), empty_path = 1;
                    if (pai != 1) setenv("PATH", path, 1), empty_path = 0;
                }

            } else {
                int redir_error = 0;
                int file_r_error = 0;
                int redir_pres  = 0;
                int redir_index = -1;
                int file_index = 0;
                char *arr_redir[1000];

                for (int k = 0; k < pai; k++) {
                    if (parr_arr[k][0] == '>') redir_pres = 1, redir_index = k;
                    if (strstr(parr_arr[k], ">") && redir_pres == 1 && redir_index != k) redir_error = 1;
                    if (redir_pres == 1 && k > redir_index && k != redir_index + 1) file_r_error = 1;

                    for (int p = 0; p < strlen(new_arr[k]); p++) {
                        if (parr_arr[k][p] == '>' && p != 0) redir_error = 1;
                    }

                    if (parr_arr[k][0] != '>') arr_redir[file_index++] = parr_arr[k];
                }
                file_index--;

                if (parr_arr[0][0] == '>') redir_error = 1;

                //printf("redir_error: %i file_r_error: %i redir_pres: %i redir_index: %i file_index: %i\n", redir_error, file_r_error, redir_pres, redir_index, file_index);

                if (ac != -1 && empty_path == 0 && bin_missing == 0) {
                    pid_t child_pid;
                    int stat_loc;

                    child_pid = fork();
                    if (child_pid == 0) {
                        //printf("arr[i]: %s, arr[i - 1]: %s\n", arr[i], arr[i - 1]);
                        //printf("arr_redir[file_index + 3]: %s, arr_redir[file_index + 2]: %s, arr_redir[file_index + 1]: %s, arr_redir[file_index]: %s, arr_redir[file_index - 1]: %s, arr_redir[file_index - 2': %s\n\n", arr_redir[file_index + 3], arr_redir[file_index + 2], arr_redir[file_index + 1], arr_redir[file_index], arr_redir[file_index - 1], arr_redir[file_index - 2]);
                        if (redir_pres == 1) {
                            if (redir_error == 0 && file_r_error == 0) {
                                freopen(arr_redir[file_index], "w", stdout);
                                freopen(arr_redir[file_index], "w", stderr);
                                arr_redir[file_index] = '\0';
                                arr_redir[file_index + 1] = (char *) NULL;
                                execvp(av, arr_redir);
                                exit(1);
                            } else error();
                        } else if (redir_pres == 0) execvp(av, parr_arr); exit(1);
                    } else waitpid(child_pid, &stat_loc, WUNTRACED);
                } else error();
            }
        }
        free(buffer);
        buffer = NULL;

    }
    exit(0); //exit once end of file is reached
}

int main(int argc, char *argv[]){
    if (argc > 2) exit(1);

    if (argc == 1) {
        interactive();
    }
    if (argc == 2) {
        batch(argv[1]);
    }
    else {
        error();
    }

    return 0;
}



