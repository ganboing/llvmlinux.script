#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <alloca.h>
#include <assert.h>

#include "filter.h"

int main(int argc, char** argv){
    int i;
    assert(argc > 2);
    if(!strcmp(argv[1], "rcsD")){
        pid_t pid = fork();
        if(!pid){
            for(i = 2; i!=argc; ++i){
                char* new_arg = malloc(strlen(argv[i]) + 3 + 1);
                sprintf(new_arg, "%s.bc", argv[i]);
                if(i > 2 && !check_is_bc(new_arg)){
                    argv[i] = MYPREFIX "/empty.bc";
                }
                else{
                    argv[i] = new_arg;
                }
            }
            if(argc > 3){
                argv[0] = "llvm-link";
                argv[1] = "-o";
            }
            else{
                argv[0] = "cp";
                argv[1] = MYPREFIX "/empty.bc";
            }
            return execvp(argv[0], argv);
        }
        int status;
        pid_t pid2 = wait(&status);
        assert(pid == pid2);
        if(status) {
            return status;
        }
    }
    argv[0] = "ar-real";
    return execvp(argv[0], argv);
}