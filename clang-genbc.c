#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <fcntl.h>

int main(int argc, char **argv) {
    int i, found = 0;
    for (i = 1; i != argc; ++i) {
        if (!strcmp(argv[i], "-o") && strcmp(argv[i+1], "-")  && strcmp(argv[i+1], "/dev/null")) {
            assert(!found);
            found = i + 1;
        }
    }
    assert(found != argc);
    if (found) {
        pid_t pid = fork();
        if (!pid) {
            //make sure child does not eat or puke
            dup2(open("/dev/null", O_RDONLY), STDIN_FILENO);
            dup2(open("/dev/null", O_WRONLY), STDOUT_FILENO);
            dup2(open("/dev/null", O_WRONLY), STDERR_FILENO);
            //child
            char *new_argv[argc + 2];
            memcpy(new_argv + 2, argv + 1, sizeof(char *) * argc);
            new_argv[0] = "clang-real";
            new_argv[1] = "-emit-llvm";
            //<path>.bc
            char obj_path[strlen(argv[found]) + 3 + 1];
            new_argv[found + 1] = strcat(strcpy(obj_path, argv[found]), ".bc");
            return execvp(new_argv[0], new_argv);
        }
        int status;
        pid_t pid2 = wait(&status);
        assert(pid == pid2);
        (void)status; //we don't care about exit code for now
    }
    argv[0] = "clang-real";
    return execvp(argv[0], argv);
}
