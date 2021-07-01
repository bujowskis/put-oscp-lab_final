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
            fprintf(stdout, "sem_open /hallA\n");
            exit(10);
        }
        sem_t *hallB = sem_open("/hallB", O_CREAT, 0600, Nb);
        if (hallB == SEM_FAILED) {
            fprintf(stdout, "sem_open /hallB\n");
            exit(10);
        }
        fprintf(stdout, "Museum opened (no. of visitors)\n");
        int valA, valB, valW;
        size_t seconds = 0;
        while (1) {
            sleep(2);
            seconds += 2;
            sem_getvalue(workA, &valW);
            sem_getvalue(hallA, &valA);
            sem_getvalue(hallB, &valB);
            fprintf(stdout, "\tvisitors:\thallA = %d/%d,\thallB = %d/%d,\tworkA = %d/%d\ttime: %lus\n", Na - valA, Na, Nb - valB, Nb, Na - 1 - valW, Na - 1, seconds);
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
        fprintf(stdout, "Museum opened (time of exiting B)\n");
        int valA, valB;
        size_t seconds = 0;
        while (1) {
            sleep(2);
            seconds += 2;
            sem_getvalue(hallA, &valA);
            sem_getvalue(hallB, &valB);
            fprintf(stdout, "\tvisitors:\thallA = %d/%d,\thallB = %d/%d\t(A+reserved = %d/%d)\ttime: %lus\n", Na - valA - (Nb - valB), Na, Nb - valB, Nb, Na - valA, Na, seconds);
        }
    } else
        fprintf(stdout, "Na must be greater than Nb\n");
}

void museumAddA(const int x) {
    int f = 0;
    switch (x) {
        case 1:
            break;
        case 2:
            f++;
            break;
        case 4:
            f += 2;
            break;
        case 8:
            f += 3;
            break;
        default:
            fprintf(stdout, "Wrong number of visitors\n\n");
            return;
    }

    sem_t *hallA = sem_open("/hallA", O_EXCL);
    if (hallA == SEM_FAILED) {
        fprintf(stdout, "Museum not opened (hallA not found)\n");
        return;
    }
    sem_t *hallB = sem_open("/hallB", O_EXCL);
    if (hallB == SEM_FAILED) {
        fprintf(stdout, "Museum not opened (hallB not found)\n");
        return;
    }

    fprintf(stdout, "(press ctrl+D to start)\n\n");
    char c;
    scanf("%c", &c);

    // Check what version of the museum is opened
    sem_t *workA = sem_open("/workA", O_EXCL);
    if (workA == SEM_FAILED) {
        // time of leaving B
        while (f > 0) {
            fork();
            f--;
        }
        pid_t pid = getpid();
        srand(pid);

        int time;
        sleep(rand() % 5);
        while (1) {
            fprintf(stdout, "[V#%d] waiting A\n", pid);
            sem_wait(hallA);

            time = rand() % 10 + 2;
            fprintf(stdout, "[V#%d] entered A, watching for %ds\n", pid, time);
            sleep(time);

            fprintf(stdout, "[V#%d] leaving A\n", pid);
            sleep(1);
            sem_post(hallA);

            fprintf(stdout, "[V#%d] left A\n", pid);
            sleep(rand() % 10 + 2);
        }
    } else {
        // no. of visitors
        while (f > 0) {
            fork();
            f--;
        }
        pid_t pid = getpid();
        srand(pid);

        int time;
        sleep(rand() % 5);
        while (1) {
            fprintf(stdout, "[V#%d] waiting A\n", pid);
            sem_wait(workA);
            sem_wait(hallA);

            time = rand() % 10 + 2;
            fprintf(stdout, "[V#%d] entered A, watching for %ds\n", pid, time);
            sleep(time);

            fprintf(stdout, "[V#%d] leaving A\n", pid);
            sleep(1);
            sem_post(hallA);
            sem_post(workA);

            fprintf(stdout, "[V#%d] left A\n", pid);
            sleep(rand() % 10 + 2);
        }
    }
}

void museumAddB(const int x) {
    int f = 0;
    switch (x) {
        case 1:
            break;
        case 2:
            f++;
            break;
        case 4:
            f += 2;
            break;
        case 8:
            f += 3;
            break;
        default:
            fprintf(stdout, "Wrong number of visitors\n\n");
            return;
    }

    sem_t *hallA = sem_open("/hallA", O_EXCL);
    if (hallA == SEM_FAILED) {
        fprintf(stdout, "Museum not opened (hallA not found)\n");
        return;
    }
    sem_t *hallB = sem_open("/hallB", O_EXCL);
    if (hallB == SEM_FAILED) {
        fprintf(stdout, "Museum not opened (hallB not found)\n");
        return;
    }

    fprintf(stdout, "(press ctrl+D to start)\n\n");
    char c;
    scanf("%c", &c);

    // Check what version of the museum is opened
    sem_t *workA = sem_open("/workA", O_EXCL);
    if (workA == SEM_FAILED) {
        // time of leaving B
        while (f > 0) {
            fork();
            f--;
        }
        pid_t pid = getpid();
        srand(pid);

        int time;
        time_t start, stop;
        sleep(rand() % 5);
        while (1) {
            fprintf(stdout, "[V#%d] waiting A\n", pid);
            sem_wait(hallA);

            time = rand() % 10 + 2;
            fprintf(stdout, "[V#%d] entered A, watching for %ds\n", pid, time);
            sleep(time);

            fprintf(stdout, "[V#%d] waiting B\n", pid);
            sem_wait(hallB);

            time = rand() % 10 + 2;
            fprintf(stdout, "[V#%d] entered B, watching for %ds\n", pid, time);
            sleep(time);

            fprintf(stdout, "[V#%d] leaving B and A\n", pid);
            start = clock();
            sleep(1);
            sem_post(hallB);
            sleep(1);
            sem_post(hallA);

            stop = clock();
            fprintf(stdout, "[V#%d] left B and A in %.5f\n", pid, (float) (stop - start) / CLOCKS_PER_SEC + 2); // +2 to account for `sleep`
            sleep(rand() % 10 + 2);
        }
    } else {
        // no. of visitors
        while (f > 0) {
            fork();
            f--;
        }
        pid_t pid = getpid();
        srand(pid);

        int time;
        sleep(rand() % 5);
        while (1) {
            fprintf(stdout, "[V#%d] waiting A\n", pid);
            sem_wait(workA);
            sem_wait(hallA);

            time = rand() % 10 + 2;
            fprintf(stdout, "[V#%d] entered A, watching for %ds\n", pid, time);
            sleep(time);

            fprintf(stdout, "[V#%d] waiting B\n", pid);
            sem_wait(hallB);

            time = rand() % 10 + 2;
            fprintf(stdout, "[V#%d] entered B, watching for %ds\n", pid, time);
            sem_post(workA);
            sem_post(hallA);
            sleep(time);

            fprintf(stdout, "[V#%d] leaving B\n", pid);
            sleep(1);
            sem_wait(hallA);

            sem_post(hallB);
            fprintf(stdout, "[V#%d] leaving A (from B)\n", pid);
            sleep(1);
            sem_post(hallA);

            fprintf(stdout, "[V#%d] left A\n", pid);
            sleep(rand() % 10 + 2);
        }
    }
}