#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "segdef.h"

/* --- 1. Fonctions de manipulation des sémaphores --- */

void acq_sem(int semid, int sem) {
    struct sembuf sop;
    sop.sem_num = sem;
    sop.sem_op = -1;  // Acquérir (-1)
    sop.sem_flg = 0;
    if (semop(semid, &sop, 1) == -1) {
        perror("Erreur acq_sem");
        exit(EXIT_FAILURE);
    }
}

void lib_sem(int semid, int sem) {
    struct sembuf sop;
    sop.sem_num = sem;
    sop.sem_op = 1;   // Libérer (+1)
    sop.sem_flg = 0;
    if (semop(semid, &sop, 1) == -1) {
        perror("Erreur lib_sem");
        exit(EXIT_FAILURE);
    }
}

void wait_sem(int semid, int sem) {
    struct sembuf sop;
    sop.sem_num = sem;
    sop.sem_op = 0;   // Attendre le passage à 0
    sop.sem_flg = 0;
    if (semop(semid, &sop, 1) == -1) {
        perror("Erreur wait_sem");
        exit(EXIT_FAILURE);
    }
}

void init_rand(void) {
    // Initialise la graine avec l'heure + le PID
    srand(time(NULL) ^ getpid());
}

long getrand(void) {
    return rand();
}