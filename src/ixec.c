#include <stdio.h>

#include "mem.h"
#include "exec.h"

int main(int argc, char** argv) {
    if (argc > 1) {
        char* last = argv[argc-1];
        Code code = codeLoads(last);
        codeExec(&code);
    }


    return 0;
}
