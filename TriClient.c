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

int ctrl_count = 0; // CTRL + C contatore

bool bot; // giocatore automatico

// Cancellazione del segmento di memoria
void cleanup()
{
    // Deallocazione memoria condivisa
    if (shared_memory != NULL)
    {
        if (shmdt(shared_memory) == -1)
        {
            perror("Errore durante distacco mem. condivisa");
            exit(EXIT_FAILURE);
        }
        else
        {
            printf("Mem. condivisa distaccata\n");
        }
    }

    if (semctl(semid, 0, IPC_RMID) == -1)
    {
        perror("Errore durante la rimozione semaforo\n");
        exit(EXIT_FAILURE);
    }
    else
    {
        printf("Semaforo rimosso\n");
    }
}

void sig_handle_ctrl(int sig)
{
    if (ctrl_count == 0)
    {
        printf("\nHai premuto CTRL+C, premi di nuovo per terminare\n");
        ctrl_count++;
    }
    else
    {
        printf("\nProgramma terminato\n");
        // Rimozione
        cleanup();
        exit(0);
    }
}

void sig_handle_timer(int sig)
{
    // gestore del timer di inserimento mossa
}

void sig_server_closed(int sig)
{
    if (sig == SIGUSR1)
    {
        printf("\nIl server è stato disconnesso.\n");
    }

    if (sig == SIGUSR2)
    {
        printf("Il tuo avversario ha abbandonato.\n");
        exit(0);
    }
}

void startup_controls(int argc, char *argv[])
{
    if (argc < 2)
    {
        fprintf(stderr, "Utilizzo: ./TriClient <nome utente>\n");
        exit(EXIT_FAILURE);
    }
}

void correct_move()
{
    if (bot)
    {
        // Genero una mossa casuale
    }
    else
    {
        // Chiedo all'utente l'inserimento di una mossa
    }
}

int main(int argc, char *argv[])
{
    startup_controls(argc, argv);    // Controlli di startup
    signal(SIGINT, sig_handle_ctrl); // Gestore del CTRL + C

    // Memoria condivisa
    shmid = shmget(SHM_KEY, SIZE, 0600);
    if (shmid == -1)
    {
        perror("Errore durante la connessione alla memoria\n");
        exit(EXIT_FAILURE);
    }
    shared_memory = (int *)shmat(shmid, NULL, 0);
    if (shared_memory == (int *)-1)
    {
        perror("Errore nel collegamento alla memoria\n");
        exit(EXIT_FAILURE);
    }

    // Semafori
    semid = semget(SEM_KEY, 1, 0600);
    if (semid == -1)
    {
        perror("Errore nella connessione al semaforo\n");
        exit(EXIT_FAILURE);
    }

    struct sembuf sb;
    sb.sem_num = 0;
    sb.sem_op = 1;
    sb.sem_flg = 0;

    if (semop(semid, &sb, 1) == -1)
    {
        perror("Errore nell'incremento\n");
        cleanup();
        exit(EXIT_FAILURE);
    }

    printf("Connesso al server...\n");

    /*
        Gioco in coppia
        // ogni client che viene eseguito fa un'operazione di incremento sul semaforo semid,
        // quando vengono eseguiti 2 client allora il semaforo va a 0 e il server continua
    */

    // Il while controlla e dice chi ha visto o se c'è pareggio
    while (1)
    {
        /* code */
    }

    cleanup();
    return 0;
}