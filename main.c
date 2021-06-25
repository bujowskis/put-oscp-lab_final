/*
 * Driver program for the final project - the museum problem
 *
 * The program is a hub to the task solution. Depending on the invocation,
 * it serves the purpose of all creating the museum and adding the visitors.
 *
 * There are two distinct algorithms:
 * 1. Maximizing no. of visitors in the museum at the same time
 * 2. Minimizing time of leaving hall B
 *
 * use "./museum" or "./museum man" to show the available commands
 */

#include "museum.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>

int main(int argc, char *argv[]) {
    if (argc == 2) {
        if (strcmp(argv[1], "man") == 0) {
            man();
            exit(0);
        }
        if (strcmp(argv[1], "dst") == 0) {
            // Destroy the museum
            museumDst();
            exit(47);
        }
    }
    if (argc == 3) {
        if (strcmp(argv[1], "A") == 0) {
            museumAddA(atoi(argv[2]));
            exit(0);
        }
        if (strcmp(argv[1], "B") == 0) {
            museumAddB(atoi(argv[2]));
            exit(0);
        }
    }
    if (argc == 5) {
        if (strcmp(argv[1], "crt") == 0) {
            if (strcmp(argv[2], "no") == 0) {
                // Create museum maximizing visitors
                createNo(atoi(argv[3]), atoi(argv[4]));
                exit(0);
            }
            if (strcmp(argv[2], "time") == 0) {
                // Create museum minimizing time of leaving B
                createTime(atoi(argv[3]), atoi(argv[4]));
                exit(0);
            }
        }
    }

    fprintf(stdout, "Wrong arguments\n\n");
    man();
    return 1;
}