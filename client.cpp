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
#define max 10024
#define Color_Blue "\x1b[34m"// Color Start
#define Color_end "\33[0m\\]" // color end
struct mesaj{
char primit[max];
char trimis[max];
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
              
char mirobolant[]="Muhahhhhhhhhhhhhhha";
    if ((sd = socket (AF_INET, SOCK_STREAM, 0)) == -1)	//creare socket
    {
        perror ("Eroare la socket().\n");
        return errno;
    }
string my_spirit=convertstring(mirobolant);

    server.sin_family = AF_INET;			//familia socket-ului
    server.sin_addr.s_addr = inet_addr ("127.0.0.1");	//adresa IP a serverului
    server.sin_port = htons (2908);			//portul de conectare
printf("\nstressed for that connect guy\n");
    if (connect (sd, (struct sockaddr *) &server, sizeof (struct sockaddr)) == -1)	//conectare la server
    {
        perror ("[client]Eroare la connect().\n");
       printf ("[client]Uof iar ai gresit\n");
        return errno;
    }
    
    printf("\x1b[33m Welcome to the TopMusic app!\n If u want to have acces to use the program you need to login or sign up!");
    printf("\n\n      The steps to enter the app are:\n 1.login <your name>\n 2.password <your password>\n YOU CAN LOGIN AND LOGOUT MULTIPLE TIMES IN THE SAME CLIENT!\n\x1b[0m");
    fflush(stdout);
    //bool logat=0;
    
    cout<<endl;

   while(1) 
    { printf("\x1b[35m~~TopMusicApp$ \033[0m");
         fflush(stdout);
        
     bzero(p.trimis,max);
     bzero(p.primit,max);
read(0,p.trimis,sizeof(p.trimis));
send(sd,p.trimis,sizeof(p.trimis),0);
char frum[]="Am trimis info\n";
cout<<frum;
//if(strstr(primit,"quit"))close(sd);

cout<<endl;

bzero(p.primit,max);
read(sd,p.primit,sizeof(p.primit));

cout<<p.primit<<endl;
    }
    close(sd);
return 0;
}
