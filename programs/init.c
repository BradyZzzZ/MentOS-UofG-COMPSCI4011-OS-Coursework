/// @file init.c
/// @brief `init` program.
/// @copyright (c) 2014-2024 This file is distributed under the MIT License.
/// See LICENSE.md for details.

#include <stdio.h>
#include <stdlib.h>
#include <sys/unistd.h>
#include <sys/wait.h>
#include <time.h>

extern char **environ;

int main(int argc, char *argv[], char *envp[])
{
    char *_argv[] = { "login", NULL };
    int status;
    // @jsinger set up environ
    int i;
    environ = (char **)malloc(5*sizeof(char*));
    for (i=0; i<5; i++) {
      environ[i] = NULL;
    }
    if (fork() == 0) {
        printf("forked login process...\n");
        execve("/bin/login", _argv, envp);
	//execve("/bin/login", _argv, environ);
        printf("This is bad, I should not be here! EXEC NOT WORKING\n");
    }
    while (1) {
        wait(&status);
    }
    return 0;
}
