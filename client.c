#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "segdef.h"

#define NB_SEMAPHORE 3 

struct sembuf sop;
int shmid, semid;
segment *seg;

void init_client(void) {
    // 1. Récupérer l'identifiant du segment mémoire
    shmid = shmget(cle, segsize, 0);
    if (shmid == -1) {
        perror("Erreur : shmget");
        exit(EXIT_FAILURE);
    }

    // 2. Récupérer l'identifiant des sémaphores
    semid = semget(cle, NB_SEMAPHORE, 0);
    if (semid == -1) {
        perror("Erreur : semget");
        exit(EXIT_FAILURE);
    }

    // 3. Attacher le segment mémoire
    seg = (segment *)shmat(shmid, NULL, 0);
    if (seg == (segment *)-1) {
        perror("Erreur shmat");
        exit(EXIT_FAILURE);
    }

    // 4. Initialiser le générateur aléatoire
    init_rand();
}

int main(void) {
    // 0. Récupère les identifiant du segment, des sémaphores et initialise le générateur.
    int nb_req = 10;
    int mon_pid = getpid();

    init_client();

    for (int req = 1; req <= nb_req; req++) {
        // 1. Demander à acquérir seg_dispo
        acq_sem(semid, seg_dispo);

        // 2. Initialise segment et acquiert seg_init
        seg->pid = mon_pid;
        seg->req = req;
        long somme = 0;
        for (int i = 0; i < maxval; i++) {
            seg->tab[i] = getrand();
            somme += seg->tab[i];
        }
        long moy_client = somme / maxval;

        acq_sem(semid, seg_init);

        // 3. Attend que res_ok passe à 0
        wait_sem(semid, res_ok);

        // 4. Lire le résultat puis libère seg_init
        long moy_serveur = seg->result;
        int pid_recu = seg->pid;
        int req_recue = seg->req;

        lib_sem(semid, seg_init);
        
        // 5. attendre la libération de res_ok 
        wait_sem(semid, res_ok);

        // 6. Libérer seg_dispo pour les autres clients
        lib_sem(semid, seg_dispo);

        // 7. Afficher les résultats comparés
        printf("[PID %d | Req %d] Client: %ld | Serveur: %ld -> %s\n",
               mon_pid, req, moy_client, moy_serveur,
               (moy_client == moy_serveur && pid_recu == mon_pid && req_recue == req) ? "OK" : "ERREUR");
    }

    // Détacher le segment mémoire
    if (shmdt(seg) == -1) {
        perror("Erreur shmdt");
        exit(EXIT_FAILURE);
    }

    return 0;
}