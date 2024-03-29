/// @file observe.c
/// @brief `observe` program.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sys/unistd.h"
#include "sys/types.h"
#include "sys/wait.h"
#include "system/syscall_types.h"
#include <time.h>
#include <signal.h>

int main(int argc, char **argv) {

    // Check command-line arguments.
    if (argc < 2) {
        printf("%s: missing operand.\n", argv[0]);
        printf("Try '%s --help' for more information.\n\n", argv[0]);
        return 1;
    }

    // Check if `--help` is provided.
    for (int i = 1; i < argc; ++i) {
        if ((strcmp(argv[i], "--help") == 0) || (strcmp(argv[i], "-h") == 0)) {
            printf("Observe executes a command repeatedly, displaying the output.\n");
            printf("Usage:\n");
            printf("    observe <command>\n");
            return 0;
        }
    }

    // Main loop to execute the command every 2 seconds
    while (1) {

        pid_t cpid = fork();

        if (cpid == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        }

        if (cpid == 0) {
            // Makes the new process a group leader
            //pid_t pid = getpid();
            //setpgid(cpid, pid);

            if (execvp(argv[1], &argv[1]) == -1) {
                printf("\nUnknown command: %s\n", argv[1]);
                exit(EXIT_FAILURE);
            }
        }
        
        sleep(2);
    }

    return 0;
}