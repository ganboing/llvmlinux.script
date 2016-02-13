#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "filter.h"

int check_is_bc(char* path){
    char buf[snprintf(NULL, 0,MYPREFIX "check-if-bc %s", path) + 1];
    sprintf(buf, MYPREFIX "/check-if-bc %s", path);
    if(system(buf)){
        fputs("[ignoring {", stderr);
        fputs(path, stderr);
        fputs("} because it's not a llvm ir]\n", stderr);
        return 0;
    }
    return 1;
}

int check_suffix(char *str, char *suffix) {
    int len = strlen(str);
    int len_suffix = strlen(suffix);
    if (len < len_suffix) {
        return len - len_suffix;
    }
    return memcmp(str + len - len_suffix, suffix, len_suffix);
}