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

void man() {
    // todo - check if works properly
    fprintf(stdout, "The program is a solution to the museum problem. The available commands:\n\n"
                    "Starting the museum:\n"
                    "   ./museum crt no Na Nb      creates museum maximizing no. of visitors at the same time\n"
                    "   ./museum crt time Na Nb    creates museum minimizing time to leave hall B\n"
                    "   ./museum dst               destroys the museum\n\n"
                    "Adding visitors:              (X = 1, 2, 4 or 8)\n"
                    "   ./museum A X               adds X visitors that will not enter B\n"
                    "   ./museum B X               adds X visitors that will enter B\n");
}

void museumDst() {
    // todo
}

void createNo(const int Na, const int Nb) {
    if (Nb < Na) {
        // todo
    } else
        fprintf(stdout, "Na must be greater than Nb\n");
}

void createTime(const int Na, const int Nb) {
    if (Nb < Na) {
        // todo
    } else
        fprintf(stdout, "Na must be greater than Nb\n");
}

void museumAddA(const int x) {
    switch (x) {
        case 1:
            // todo
            break;
        case 2:
            // todo
            break;
        case 4:
            // todo
            break;
        case 8:
            // todo
            break;
        default:
            fprintf(stdout, "Wrong number of visitors\n\n");
    }
}

void museumAddB(const int x) {
    switch (x) {
        case 1:
            // todo
            break;
        case 2:
            // todo
            break;
        case 4:
            // todo
            break;
        case 8:
            // todo
            break;
        default:
            fprintf(stdout, "Wrong number of visitors\n\n");
    }
}