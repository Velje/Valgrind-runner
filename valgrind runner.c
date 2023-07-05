#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

const unsigned MAX_CHARACTERS = 30;
const unsigned MAX_ARGUEMENTS = 9;

#define check_error(cond,msg)\
    do {\
        if (cond) {\
            puts(msg);\
            exit(EXIT_FAILURE);\
        }\
    } while (0)

void free_memory(char** pointer, const unsigned end) {
    for (unsigned i = 5; i < end; i++) {
        free(*(pointer + i));
    }
    free(pointer);
}

int main(unsigned argc, char** argv) {

    check_error(argc < 2, "./valgrind a.out [arg arg]");
    char** args = malloc(sizeof(char*) * MAX_ARGUEMENTS);
    check_error(!args, "Allocation failed! (args)");

    unsigned argc2 = 0;
    args[argc2++] = "valgrind";
    args[argc2++] = "--leak-check=full";
    args[argc2++] = "--show-leak-kinds=all";
    args[argc2++] = "--track-origins=yes";
    args[argc2++] = "--verbose";
    argc -= 1;
    if (argc + argc2 + 1 > MAX_ARGUEMENTS) {
        free(args);
        check_error(true, "Too many arguements! (./valgrind a.out [arg arg] (max 2 arg))");
    }
    for (unsigned i = argc2; i < argc2 + argc; i++) {
        if (!(*(args + i) = malloc(MAX_CHARACTERS))) {
            free_memory(args, i);
            check_error(true, "Allocation failed! (*args)");
        }
    }
    for (unsigned i = 0; i < argc; i++) {
        if (i == 0) {
            args[argc2][0] = '.'; args[argc2][1] = '/'; args[argc2][2] = '\0';
        }
        else {
            args[argc2][0] = '\0';
        }
        args[argc2] = strcat(args[argc2], argv[i + 1]);
        argc2 += 1;
    }
    args[argc2] = NULL;

    if (execvp(args[0], args)) {
        free_memory(args, argc2);
        check_error(true, "execvp function failed!");
    }

}
