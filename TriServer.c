#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <signal.h>
#include <sys/signal.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>

#define BOARD_SIZE_DEF 3
#define SHM_KEY 4321
#define SEM_KEY 8765
#define SIZE 1024

// Prototipi
void startup_controls(int argc, char *argv[]);
void handle_ctrl(int sig);
void cleanup();
bool victory();
bool draw();

int timeout = 0;
char player1;
char player2;

int ctrl_count = 0;

int semid;
int shmid;
int *shared_memory;
struct sembuf sop = {0, 0, 0};

// Controllo iniziale
void startup_controls(int argc, char *argv[])
{
    if (argc < 4)
    {
        printf("Utilizzo: ./TriServer <timeout> <simbolo 1> <simbolo 2>\n");
        exit(EXIT_FAILURE);
    }
    else
    {
        timeout = atoi(argv[1]);
        if (timeout < 0)
        {
            printf("Timer non può essere negativo.\n");
            exit(EXIT_FAILURE);
        }
        else if (timeout == 0)
        {
            printf("Timer nullo inserito\n");
        }
        else if (timeout > 0)
        {
            printf("Timer inserito: %d\n", timeout);
        }

        if (strlen(argv[2]) != 1 || strlen(argv[3]) != 1)
        {
            printf("Il simbolo del giocatore deve essere singolo.\n");
            exit(EXIT_FAILURE);
        }
        else
        {
            player1 = argv[2][0];
            player2 = argv[3][0];
        }
    }

    printf("Player 1: %c\n", player1);
    printf("Player 2: %c\n", player2);
}

// Gestione del CTRL + C
void handle_ctrl(int sig)
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

// Cancellazione del segmento di memoria
void cleanup()
{
    // Deallocazione memoria condivisa
    if (shmdt(shared_memory) == -1)
    {
        perror("Errore durante la deallocazione della memoria condivisa (shmdt)");
    }
    else
    {
        printf("Memoria condivisa deallocata con successo.\n");
    }

    // Rimuove area di memoria condivisa
    if (shmctl(shmid, IPC_RMID, NULL) == -1)
    {
        perror("Errore durante la rimozione dell'area di memoria condivisa (shmctl)");
    }
    else
    {
        printf("Area di memoria condivisa rimossa con successo.\n");
    }

    // Deallocazione semaforo
    if (semctl(semid, 0, IPC_RMID) == -1)
    {
        perror("Errore durante la deallocazione del semaforo (semctl)");
    }
    else
    {
        printf("Semaforo deallocato con successo.\n");
    }
}

bool victory()
{
    // controllo orizzontale

    // controllo verticale

    // controllo diagonale
}

bool draw()
{
    // controllo le celle occupate della matrice

    // se tutte le celle sono occupate e nessuno ha visto c'è pareggio
}

// Main program
int main(int argc, char *argv[])
{
    signal(SIGINT, handle_ctrl);

    // Controllo iniziale dei parametri
    startup_controls(argc, argv);

    // Creazione memoria condivisa
    shmid = shmget(SHM_KEY, SIZE, IPC_CREAT | 0660);
    if (shmid == -1)
    {
        perror("shmget");
        exit(EXIT_FAILURE);
    }
    else
    {
        printf("Memoria creata\n");
    }

    shared_memory = (int *)shmat(shmid, NULL, SHM_RND);
    if (shared_memory == (int *)-1)
    {
        perror("shmat");
        exit(EXIT_FAILURE);
    }
    else
    {
        printf("Spazio di memoria creato\n");
    }

    // Creazione semaf
    semid = semget(SEM_KEY, 1, IPC_CREAT | 0666);
    if (semid < 0)
    {
        perror("semget");
        exit(EXIT_FAILURE);
    }

    /*
        imposto valore semaforo a -2, incremento quando un
        giocatore si connette alla partia
    */

    while (1)
    {
        /* code */
    }

    // Rimozione memoria, semaforo
    cleanup();
    return 0;
}
