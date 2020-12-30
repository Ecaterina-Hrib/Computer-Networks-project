//server concurrent cu forkuri 
#include <iostream>
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
#include <string.h>
#include <sqlite3.h>
#define max 1024
//trb sa fac functii de login si signup
//make admin
//genre
//comment
//restrict
// style
//add genre

/* portul folosit */
#define PORT 2908
int main(int argc, char* argv[])
{
    char primesc[max];
    char trimit[max];
    char mesaj[max];
    char username[30];
    char passwd[30];
    //defines pentru comenzi
    //deschid baza de date

     //server + client
    int sd;						                        //descriptor de socket

    if ((sd = socket (AF_INET, SOCK_STREAM, 0)) == -1)	//creare socket
    {
        perror ("[server]Eroare la socket ().\n");
        return errno;
    }
 int on=1;
  setsockopt(sd,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on));
    struct sockaddr_in server;			       	        //structura folosita de server
    server.sin_family = AF_INET;		              	//stabilirea familiei de socket-uri
    server.sin_addr.s_addr = htonl (INADDR_ANY);	    //acceptam orice adresa
    server.sin_port = htons (PORT);			            //utilizam un port utilizator

    if (bind (sd, (struct sockaddr *) &server, sizeof (struct sockaddr)) == -1)	//atasare socket
    {
        perror ("[server]Eroare la bind ().\n");
        return errno;
    }

    if (listen (sd, 5) == -1)	//punem serverul sa asculte daca vin clienti sa se conecteze
    {
        perror ("[server]Eroare la listen ().\n");
        return errno;
    }

    fflush (stdout);
    printf ("[server]Waiting at port: %d...\n\n",PORT);

 while (1)
    {
        int client = accept (sd, NULL, NULL); 	//acceptam un client (stare blocanta pina la realizarea conexiunii)

        if (client < 0) 		              	//eroare la acceptarea conexiunii de la un client
        {
            perror ("[server]Eroare la accept().\n");
            continue;
        }
        if (fork () == 0)
        { //login or sign up once
          
          while(1)
          {fflush(stdout);
          bzero(primesc,max);
          bzero(trimit,max);
int read_size;


          //bzero(username,30);
         // memset (username, 0, sizeof (username));

          //if(read (client, username, sizeof (username)) >0) //citire mesaj
          //{printf("am primit de la client username ul %s",username);}
             if (read (client, primesc, sizeof (primesc)) <= 0)	//citire mesaj
                {
                    perror ("[server]Eroare la read () de la client.\n");
                    close (client);	// inchidem conexiunea cu clientul
                    continue;		// continuam sa ascultam
                }
             printf ("[server]Message received...\n");
             fflush(stdout);
             bzero(mesaj,max);
             strcpy(mesaj,primesc);
             printf (" %s",mesaj);
             //if(login)
             char raspuns_login[6]="Yes";
            //message back 
         if( write(client,raspuns_login,sizeof(raspuns_login))<=0)
         {printf("[server]Eroare la write() catre client.\n");
         continue;
         }
          else printf("trimit la client raspunsul pt logare\n");//baza de date
          fflush(stdout);
          }
        }
      
    }
 return 0;
 }

