#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <pthread.h>
#include <string>
#include <vector>
#include <iostream>
#include <arpa/inet.h>
#include <netdb.h>
#include "Commands/CommandInvoker.h"
#include "Commands/Data.h"
#include "Commands/additionals.h"
#include <time.h>
#include "files/tinyxml.h"
//#include "files/tinyxml.h"

/* portul folosit */
#define PORT 1111

/* codul de eroare returnat de anumite apeluri */

static void *treat(void *); /* functia executata de fiecare thread ce realizeaza comunicarea cu clientii */
void raspunde(void *);
Data *Data::instance = 0;

typedef struct thData
{
    int idThread; // id-ul thread-ului tinut in evidenta de acest program
    int cl;       // descriptorul intors de accept
} thData;

pthread_mutex_t lock;

int main()
{

    struct sockaddr_in server; // structura folosita de server
    struct sockaddr_in from;
    int nr; // mesajul primit de trimis la client
    int sd; // descriptorul de socket
    int pid;

    pthread_mutex_init(&lock, NULL);
    pthread_t th[100]; // Identificatorii thread-urilor care se vor crea
    int i = 0;
    // queue.resize(100);
    /* crearea unui socket */
    if ((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("[server]Eroare la socket().\n");
        return errno;
    }
    /* utilizarea optiunii SO_REUSEADDR */
    int on = 1;
    setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

    /* pregatirea structurilor de date */
    bzero(&server, sizeof(server));
    bzero(&from, sizeof(from));

    /* umplem structura folosita de server */
    /* stabilirea familiei de socket-uri */
    server.sin_family = AF_INET;
    /* acceptam orice adresa */
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    /* utilizam un port utilizator */
    server.sin_port = htons(PORT);

    /* atasam socketul */
    if (bind(sd, (struct sockaddr *)&server, sizeof(struct sockaddr)) == -1)
    {
        perror("[server]Eroare la bind().\n");
        return errno;
    }

    /* punem serverul sa asculte daca vin clienti sa se conecteze */
    if (listen(sd, 2) == -1)
    {
        perror("[server]Eroare la listen().\n");
        return errno;
    }
    /* servim in mod concurent clientii folosind thread-uri */

    while (1)
    {
        int client;
        thData *td; // parametru functia executata de thread
        socklen_t length = sizeof(from);

        printf("[server]Asteptam la portul %d...\n", PORT);
        fflush(stdout);

        // client= malloc(sizeof(int));
        /* acceptam un client (stare blocanta pina la realizarea conexiunii) */

        /* s-a realizat conexiunea, se astepta mesajul */
        if ((client = accept(sd, (struct sockaddr *)&from, &length)) < 0)
        {
            perror("[server]Eroare la accept().\n");
            continue;
        }
        // int idThread; //id-ul threadului
        // int cl; //descriptorul intors de accept

        td = (struct thData *)malloc(sizeof(struct thData));
        td->idThread = i++;
        td->cl = client;

        pthread_create(&th[i], NULL, &treat, td);
        pthread_mutex_destroy(&lock);

    } // while
};
static void *treat(void *arg)
{
    pthread_t qThread;
    struct thData tdL;
    tdL = *((struct thData *)arg);
    printf("[thread]- %d - Asteptam mesajul...\n", tdL.idThread);
    fflush(stdout);
    pthread_detach(pthread_self());
    raspunde((struct thData *)arg);
    /* am terminat cu acest client, inchidem conexiunea */
    close((intptr_t)arg);
    return (NULL);
};
void raspunde(void *arg)
{
    bool loggedThread = false;
    int nr, i = 0;
    char comm[256];
    memset(comm, 0, sizeof(comm));

    struct thData tdL;
    tdL = *((struct thData *)arg);
    int x = 0;

    std::string sched = showSchedule();

    sched += "To access the information about our trains, please use the following commands: \n";
    sched = sched + "schedule <train> : info on specific train \n" + "\t - departures : trains leaving in the next hour\n" + "\t - arrivals   : trains arriving in the next hour\n" + "\t - departures <station> : trains leaving in the next hour from <station>\n" + "\t - arrivals <station> : trains arriving in the next hour from <station>\n" + "\t - departure <train> <station>\n" + "\t - arrival <train> <station>\n" + "Our operators are updating any possible delays during the day.\n" + "The schedule is modified accordingly and the delays are mentioned.\n";

    const char *prog = sched.c_str();
    if (write(tdL.cl, prog, strlen(prog)) <= 0)
    {
        perror("[Thread]Eroare la write() showSchedule() catre client.\n");
    }

    while ((read(tdL.cl, &x, sizeof(int)) > 0))
    {

        printf("[Thread %d]Mesajul a fost receptionat. %s\n", tdL.idThread, comm);
        printf("[Thread %d]Trimitem mesajul inapoi. %s\n", tdL.idThread, comm);
        if (read(tdL.cl, &comm, x) <= 0)
        {
            perror("[client]Eroare la prefixare read() spre server.\n");
        }
        if (strlen(comm) > 1)
        {
            comm[strlen(comm) - 1] = '\0';
        }
        CommandInvoker attempt(tdL.cl, comm, loggedThread, lock);
        attempt.setcmd();
        attempt.exec();
        //  prefixare: trb comparat lungimea comenzii de la client cu spatiul disponibil

        memset(comm, 0, sizeof(comm));
    }
}
