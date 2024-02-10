/* cliTCPIt.c - Exemplu de client TCP
   Trimite un numar la server; primeste de la server numarul incrementat.

   Autor: Lenuta Alboaie  <adria@infoiasi.ro> (c)2009
*/
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

/* codul de eroare returnat de anumite apeluri */
// extern int errno;

/* portul de conectare la server*/
int port;

int main(int argc, char *argv[])
{
    int sd;                    // descriptorul de socket
    struct sockaddr_in server; // structura folosita pentru conectare
                               // mesajul trimis
    int nr = 0;
    char buf[256];
    memset(buf, 0, sizeof(buf));

    /* exista toate argumentele in linia de comanda? */
    if (argc != 3)
    {
        printf("Sintaxa: %s <adresa_server> <port>\n", argv[0]);
        return -1;
    }

    /* stabilim portul */
    port = atoi(argv[2]);

    /* cream socketul */
    if ((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("Eroare la socket().\n");
        return errno;
    }

    /* umplem structura folosita pentru realizarea conexiunii cu serverul */
    /* familia socket-ului */
    server.sin_family = AF_INET;
    /* adresa IP a serverului */
    server.sin_addr.s_addr = inet_addr(argv[1]);
    /* portul de conectare */
    server.sin_port = htons(port);

    /* ne conectam la server */
    if (connect(sd, (struct sockaddr *)&server, sizeof(struct sockaddr)) == -1)
    {
        perror("[client]Eroare la connect().\n");
        return errno;
    }
    char *program;
    memset(&program, 0, sizeof(program));
    program = (char *)malloc(7168);

    if (read(sd, program, 7168) < 0)
    {
        perror("[client]Eroare la read() de la server.\n");
    }
    printf("%s\n", program);
    memset(&program, 0, sizeof(program));
    free(program);

    while (1)
    {
        /* citirea mesajului */
        printf("Enter a command: ");
        fflush(stdout);
        int x = read(0, buf, sizeof(buf));
     
        int y = 0;
        y = strlen(buf);
        if (write(sd, &y, sizeof(int)) <= 0)
        {
            perror("[client] Eroare la prefixare write() spre server.\n");
            return errno;
        }
        /* trimiterea mesajului la server */

        if (write(sd, buf, strlen(buf)) <= 0)
        {
            perror("[client] Eroare la write() spre server.\n");
            return errno;
        }

        char *rasp;
        memset(&rasp, 0, sizeof(rasp));
        int size;
        if (read(sd, &size, sizeof(size)) < 0)
        {
            perror("[client] Eroare la read() de la server.\n");
            return errno;
        }

        rasp = (char *)malloc(size+1);

        if (read(sd, rasp, size) < 0)
        {

            perror("[client] Eroare la read() de la server.\n");
            return errno;
        }
        /* afisam mesajul primit */
        printf("%s\n", rasp);
        if (strcmp("quit", rasp) == 0)
        {
            exit(0);
        }
        memset(buf, 0, sizeof(buf));
        memset(&rasp, 0, sizeof(rasp));
        free(rasp);
    }

    /* inchidem conexiunea, am terminat */
    close(sd);
}