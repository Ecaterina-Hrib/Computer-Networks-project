#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cerrno>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>
using namespace std;
#define max 1024
#define PORT 2908
struct user{
char username[30];
char parola[30];
}p;
string convertstring(char a[30])
{
string s;
for(int i=0;i<strlen(a);i++)
s=s+a[i];
return s;
}
int main(int argc, char* argv[])
{
 //client + server
    int sd;			//descriptorul de socket
    struct sockaddr_in server;	//structura folosita pentru conectare
    char primit[max];		//mesajul trimis
    char trimit[max];           //mesaj de trimis

    if ((sd = socket (AF_INET, SOCK_STREAM, 0)) == -1)	//creare socket
    {
        perror ("Eroare la socket().\n");
        return errno;
    }

    server.sin_family = AF_INET;			//familia socket-ului
    server.sin_addr.s_addr = inet_addr ("127.0.0.1");	//adresa IP a serverului
    server.sin_port = htons (PORT);			//portul de conectare

    if (connect (sd, (struct sockaddr *) &server, sizeof (struct sockaddr)) == -1)	//conectare la server
    {
        perror ("[client]Eroare la connect().\n");
        return errno;
    }
    //ceva culori ca sa dea mai bine
    printf("Welcome to the TopMusic app!\n If u want to have acces to use the program you need to login or sign up!");
    printf("\n 1.Sign Up\n 2.Log in\n Write your name: ");
    fflush(stdout);
    
    

   while(1)
    {
         fflush(stdout);
     bzero(p.username,30);
       read (0, p.username, sizeof (p.username));//citire tastatura
      write(sd,p.username,sizeof(p.username)); //scriere server
      fflush(stdout);
      printf("trimit la server : %s",p.username);
       
        
        read(sd,primit,sizeof(primit));
        if(strstr(primit,"Quit")==0) break;

    }
    close(sd);

}