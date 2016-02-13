#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <alloca.h>
#include <assert.h>

#include "filter.h"

int main(int argc, char **argv) {
    int i, j, found = 0;
    for (i = 1; i != argc; ++i) {
        if (!strcmp(argv[i], "-o")) {
            assert(i + 1 != argc);
            if (!check_suffix(argv[i + 1], ".o")) {
                found = i + 1;
            }
            break;
        }
    }
    if (found) {
        pid_t pid = fork();
        if (!pid) {
            int new_argc = argc - found + 2;
            char *new_argv[new_argc + 1];
            new_argv[0] = "llvm-link";
            new_argv[1] = "-o";
            for (i = 2, j = found; j != argc; ++j) {
                if(!strcmp(argv[j], "--start-group")||
                        !strcmp(argv[j], "--end-group")){
                    continue;
                }
                new_argv[i] = malloc(strlen(argv[j]) + 3 + 1);
                //seems that alloca does not work. Will investigate later
                sprintf(new_argv[i], "%s.bc", argv[j]);
                //add some check here
                if(i > 2 && !check_is_bc(new_argv[i])){
                    continue; //the file NOT a LLVM IR
                }
                ++i; //accept the current bc
            }
            new_argv[i] = NULL;
            if(i > 3){
                return execvp(new_argv[0], new_argv);
            }
            return 0;
        }
        int status;
        pid_t pid2 = wait(&status);
        assert(pid == pid2);
        if(status)
            return status;
    }
    argv[0] = "ld-real";
    return execvp(argv[0], argv);
}