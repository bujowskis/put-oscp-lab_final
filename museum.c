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
#include <fcntl.h>

void man() {
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
    sem_unlink("/hallA");
    sem_unlink("/hallB");
    sem_unlink("/workA");
}

void createNo(const int Na, const int Nb) {
    if (Na <= 1 || Nb < 1) {
        fprintf(stdout, "Nb must be greater than 0, and Na must be greater than Nb\n");
        return;
    }
    if (Nb < Na) {
        sem_t *workA = sem_open("/workA", O_CREAT, 0600, Na - 1); // -1 to prevent deadlock
        if (workA == SEM_FAILED) {
            fprintf(stdout, "sem_open /workA\n");
            exit(10);
        }
        sem_t *hallA = sem_open("/hallA", O_CREAT, 0600, Na);
        if (hallA == SEM_FAILED) {
            fprintf(stdout, "sem_open /workA\n");
            exit(10);
        }
        sem_t *hallB = sem_open("/hallB", O_CREAT, 0600, Nb);
        if (hallB == SEM_FAILED) {
            fprintf(stdout, "sem_open /workA\n");
            exit(10);
        }
        fprintf(stdout, "Museum opened\n");
        int valA, valB, valW;
        size_t seconds = 0;
        while (1) {
            sleep(2);
            seconds += 2;
            sem_getvalue(workA, &valW);
            sem_getvalue(hallA, &valA);
            sem_getvalue(hallB, &valB);
            fprintf(stdout, "\tvisitors: hallA = %d,\thallB = %d\t(workA = %d)\ttime: %lu seconds\n", Na - valA, Nb - valB, Na - 1 - valW, seconds);
        }
    } else
        fprintf(stdout, "Na must be greater than Nb\n");
}

void createTime(const int Na, const int Nb) {
    if (Na <= 1 || Nb < 1) {
        fprintf(stdout, "Nb must be greater than 0, and Na must be greater than Nb\n");
        return;
    }
    if (Nb < Na) {
        sem_t *hallA = sem_open("/hallA", O_CREAT, 0600, Na);
        if (hallA == SEM_FAILED) {
            fprintf(stdout, "sem_open /workA\n");
            exit(10);
        }
        sem_t *hallB = sem_open("/hallB", O_CREAT, 0600, Nb);
        if (hallB == SEM_FAILED) {
            fprintf(stdout, "sem_open /workA\n");
            exit(10);
        }
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