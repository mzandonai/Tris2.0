#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <ctype.h>
#include <string.h>
#include <signal.h>
#include <stdbool.h>
#include <time.h>

#define SHM_KEY 4321
#define SEM_KEY 8765
#define SIZE 1024

int semid;
int semval;
int shmid;
int *shared_memory;

void startup_controls(int argc, char *argv[])
{
    if (argc < 2)
    {
        fprintf(stderr, "Utilizzo: ./TriClient <nome utente>\n");
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char *argv[])
{
    startup_controls(argc, argv);

    shmid = shmget(SHM_KEY, SIZE, 0600);
    shared_memory = (int *)shmat(shmid, NULL, SHM_RND);

    semid = semget();
    semval = semctl(semid, 0, GETVAL);

    return 0;
}