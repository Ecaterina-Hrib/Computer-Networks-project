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
#define max 10024
using namespace std;
int verify_song(char song[30],sqlite3* db);
int verify_user(char name[30],sqlite3* db);
int verifiy_restrict_user(char username[30], sqlite3 *db);
int login_name(char name[30],sqlite3* db);
int login_passwd(char name[30],char password[30],sqlite3* db);
void name_split(char ceva[150],char* out);
 void name_split2(char ceva[150],char* out);
static int callback_single(void *str, int argc, char **argv, char **azColName);
static int callback(void *str, int argc, char **argv, char **azColName);
int is_admin(char username[30], sqlite3 *db);
void top_genre_style(char style[20], char* outer,sqlite3 *db);
void top_genre_votes(char* outer, sqlite3 *db);
int vote(char name[30], char* outer,sqlite3 *db);
int unvote(char song[30], char* outer,sqlite3 *db);
void about_song(char name[30],char* outer, sqlite3* db);
void show_comments_song(char song[30],char *outer,sqlite3* db);
void restrict_user_vote(char name[30],char* outer,sqlite3* db);
void restrict_user_comment(char name[30],char*outer,sqlite3* db);
void show_comments_user(char name[30],char* outer,sqlite3* db);
void make_admin(char name[30],char* outer, sqlite3*db);
void remove_song(char song[30],char* outer,sqlite3*db);
//not finished
void insert_comment(char name[30],char song[30],char comment[150],int id,sqlite3* db);//id din main ca sa tina minte numarul unic al comentariului

void login(char name[30],char password[30],sqlite3* db);




// signup
//make admin
//add top
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
  //open database
   sqlite3 *db;
   int record;
    record = sqlite3_open("Info.db", &db);

    if (record)
        printf("Error at opening database!\n");
    else
        printf("Succes at opening database\n");
         
        int admin;
        int log; 
        int n,p;
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
          log=0; 
          admin=0;
           n=0; 
          p=0;
          while(1)
          {
              fflush(stdout);
          bzero(primesc,max);
          bzero(trimit,max);
          char raspuns[max]; char nume[30];
          bzero(raspuns,max); 
         
        char *outer=(char*)malloc(sizeof(char)*max);
       cout<<"[server]Read the message"<<endl;
         fflush(stdout);
             if (read (client, primesc, sizeof (primesc)) <= 0)	//citire mesaj
                {
                    perror ("[server]Eroare la read () de la client.\n");
                    close (client);	// inchidem conexiunea cu clientul
                    continue;		// continuam sa ascultam
                }
             
             fflush(stdout);
             bzero(mesaj,max);
             strcpy(mesaj,primesc);
             printf (" %s",mesaj);
            
            if(strstr(mesaj,"login")!=0&& n==0)
            {
                
                 fflush(stdout);
                bzero(outer,max);
            name_split(mesaj,outer);
             
                
                bzero(raspuns,max);
               bzero(nume,30);
               
                    strcpy(nume,outer);
                    nume[strlen(nume)-1]='\0';
                    cout<<nume<<endl;
                    int nr=strlen(nume);
                    cout<<nr<<endl;
                    
                  n= login_name(nume,db);
                   cout<<n;
                   if(n)
                   {if(verifiy_restrict_user(nume,db))
                       {
                           strcpy(raspuns,"Write your password: password X");
                       if(is_admin(nume,db)) admin=1; 

                       }
                       else 
                       cout<<"The user is restricted to perform any command";
                   }
                   else 
                   strcpy(raspuns,"Your username doesn't exist!");
                 

            }
           
           else 
           {if(strstr(mesaj,"password")&& n==1)
                    { fflush(stdout);
                        char password[30];
                        bzero(outer,max);
                         name_split(mesaj,outer);
                         bzero(password,30);
                         strcpy(password,outer);
                        password[strlen(password)-1]='\0';
                        if(login_passwd(nume,password,db))
                             {
                             p=1;
                             log=1; 
                             if(admin)
                            { strcpy(raspuns,"Log in succesfully. The user is admin!");
                            strcat(raspuns,"\n\n The commands are: \n top_all\n remove_song <song name>\n top_style <style >\n about_song <song name>\n show_comments_song <song name>\n show_comments_user <username>\n unvote <song>\n vote<song>\n restrict_vote <user>\n restrict_comment <user>\n restrict_user <username>\n make_admin <username>\n logout\n");}                                     
                             else strcpy(raspuns,"Log in succesfully. The user is not admin!");
                             }
                             else 
                              strcpy(raspuns,"Your password is incorrect");
                    }
            else
           { if(log==1)
               {
                   if(strstr(mesaj,"top_all"))
                {   
                    bzero(outer,max);
                    top_genre_votes(outer,db);
                    bzero(mesaj,max);
                    strcpy(raspuns,outer);

                } else if (strstr(mesaj,"top_style"))
                {  char style[20];
                bzero(style,20);
                 name_split(mesaj,outer);
                 strcpy(style,outer);
                 cout<<style;
                 bzero(raspuns,max);
                 strcat(raspuns,"Style top for ");
                 strcat(raspuns,style);
                 strcat(raspuns," are: ");
                 strcat(raspuns, "\n");
                     bzero(outer,max);
                    bzero(mesaj,max);
                    top_genre_style(style,outer,db);
                    strcat(raspuns,outer);

                } else if (strstr(mesaj,"remove_song"))
                {
                    bzero(outer,max);
                   char song[30];
                   bzero(song,30);
                   name_split2(mesaj,outer);
                   strcpy(song,outer);
                   cout<<song<<endl;
                    bzero(outer,max);
                   if(verify_song(song,db))
                   { remove_song(song,outer,db);
                   strcpy(raspuns,outer);
                   }
                   else strcpy(raspuns,"This song doesn't exist");
                bzero(mesaj,max);

                }
                else if (strstr(mesaj,"make_admin"))
                {
                  if(admin==1)
                  { char name_user[30];
                   bzero(name_user,30);
                   bzero(outer,max);
                   name_split2(mesaj,outer);
                   strcpy(name_user,outer);
                   cout<<name_user;
                   if(verify_user(name_user,db))
                   { bzero(outer,max);
                       make_admin(name_user,outer,db);
                       strcpy(raspuns,outer);
                   }
                   else strcpy(raspuns,"The user doesn't exist!");
                     
                  } else strcpy(raspuns,"This user is not admin so he can't use this command!");
                  bzero(mesaj,max);
                }
                else if (strstr(mesaj,"about_song"))
                {
                    //tot cu outer
                    
                     bzero(outer,max);
                   char song[30];
                   bzero(song,30);
                   name_split2(mesaj,outer);
                   strcpy(song,outer);
                   cout<<song<<endl;
                    bzero(outer,max);
                   if(verify_song(song,db))
                   { about_song(song,outer,db);
                   strcpy(raspuns,outer);
                   }
                   else strcpy(raspuns,"This song doesn't exist");
                bzero(mesaj,max);


                }
                else if (strstr(mesaj,"show_comments_song"))
                {
                     bzero(outer,max);
                   char song[30];
                   bzero(song,30);
                   name_split2(mesaj,outer);
                   strcpy(song,outer);
                   bzero(outer,max);
                   if(verify_song(song,db))
                   {
                       //cv cv cu show_comment song
                       show_comments_song(song,outer,db);
                       strcpy(raspuns,outer);
                       
                   }
                  else strcpy(raspuns,"This song doesn't exist");
                      bzero(mesaj,max);

                }
                else if (strstr(mesaj,"show_comments_user"))
                {
                   char name_user[30];
                   bzero(name_user,30);
                   bzero(outer,max);
                   name_split2(mesaj,outer);
                   strcpy(name_user,outer);
                   cout<<outer;
                   if(verify_user(name_user,db))
                   { bzero(outer,max);
                       show_comments_user(name_user,outer,db);
                       strcpy(raspuns,outer);
                   }
                   else strcpy(raspuns,"The user doesn't exist!");


                   bzero(mesaj,max);
                   
                }
                else
                   if(strstr(mesaj,"unvote"))
                    { bzero(outer,max);
                   char song[30];
                   bzero(song,30);
                   name_split2(mesaj,outer);
                   strcpy(song,outer);
                   bzero(outer,max);
                   if(verify_song(song,db))
                   {
                       unvote(song,outer,db);
                       strcpy(raspuns,outer);
                   }
                   else strcpy(raspuns,"The song doesn't exist!");

                        bzero(mesaj,max);

                    }
                    else
                    if(strstr(mesaj,"restrict_vote"))
                    { 
                        if(admin==1)
                        { char name_user[30];
                           bzero(name_user,30);
                           bzero(outer,max);
                           name_split2(mesaj,outer);
                           strcpy(name_user,outer);
                          cout<<outer;
                   if(verify_user(name_user,db))
                   { bzero(outer,max);
                       restrict_user_vote(name_user,outer,db);
                       strcpy(raspuns,outer);
                   }
                   else strcpy(raspuns,"The user doesn't exist!");

                        }
                        else strcpy(raspuns,"You are not an admin to perform this action!");
                            
                        
                        bzero(mesaj,max);
                       
                    }
                    else 
                     if(strstr(mesaj,"logout"))
                    { bzero(mesaj,max);
                        n=0; p=0; log=0; admin=0;cout<<mesaj;
                       
                        strcpy(raspuns,"Log out succesfully!");
                    }
                    else if(strstr(mesaj,"restrict_comment"))
                    {  if(admin==1)
                        { char name_user[30];
                           bzero(name_user,30);
                           bzero(outer,max);
                           name_split2(mesaj,outer);
                           strcpy(name_user,outer);
                          cout<<outer;
                   if(verify_user(name_user,db))
                   { bzero(outer,max);
                       restrict_user_comment(name_user,outer,db);
                       strcpy(raspuns,outer);
                   }
                   else strcpy(raspuns,"The user doesn't exist!");

                        }
                        else strcpy(raspuns,"You are not an admin to perform this action!");
                            bzero(mesaj,max);
                        
                    }
                    else if(strstr(mesaj,"vote"))
                    { 
                   bzero(outer,max);
                   char song[30];
                   bzero(song,30);
                   name_split2(mesaj,outer);
                   strcpy(song,outer);
                   bzero(outer,max);
                   if(verify_song(song,db))
                   {
                       vote(song,outer,db);
                       strcpy(raspuns,outer);
                   }
                   else strcpy(raspuns,"The song doesn't exist!");
                        bzero(mesaj,max);
                       
                    }
                    else if(strstr(mesaj,"comment"))
                    {
                        bzero(mesaj,max);
                        strcpy(raspuns,"comment x");
                    }
                     else if(strstr(mesaj,"restrict_user"))
                     {
                        bzero(mesaj,max);
                        strcpy(raspuns,"restrict user x");
                     }
                    else 
                    if(strstr(mesaj,"logout")==0&&strstr(mesaj,"unvote")==0&&strstr(mesaj,"vote")==0&&strstr(mesaj,"top")==0&&strstr(mesaj,"quit")==0)strcpy(raspuns,"Nuj ce vrei de la mn");
                   }
                  else strcpy(raspuns,"You need to log in to have acces to the commands");
            }}
           
            //fflush(stdout);
       
         int nrc=strlen(raspuns);
         cout<<"[server]Sending the message back..."<<endl;
         cout<<raspuns<<endl;

         if( write(client,raspuns,nrc)<=0)
         {printf("[server]Eroare la write() catre client.\n");
         continue;
         }
          else printf("trimit la client\n");//baza de date
          fflush(stdout);
          }
        }
      
    }
    sqlite3_close(db);
 return 0;
 }
 void name_split(char ceva[150],char* out)
{ char name[150];
char* p;
int nr=0;
p=strtok(ceva," ");
while(p!=NULL)
{ nr++;
    if(nr==2)
{
strcpy(out,p);
}
p=strtok(NULL," ");
}
}
 void name_split2(char ceva[150],char* out)
{ char name[150];
char* p;
int nr=0;
p=strtok(ceva," ");
while(p!=NULL)
{ nr++;
    if(nr==2)
{
strcpy(out,p);
}
p=strtok(NULL,"\n");
}
}

static int callback_single(void *str, int argc, char **argv, char **azColName)
{
    int i; int pa=1;
    char *data = (char *)str;
    
    for (i = 0; i < argc; i++)
    {pa=pa*2;
        
        if (argv[i])
            strcat(data, argv[i]);
        else
            strcat(data, "NULL");
        pa=pa-1;
    }

    
    return 0;
}
static int callback(void *str, int argc, char **argv, char **azColName)
{
    int i; int pa=0;
    char *data = (char *)str;
    for (i = 0; i < argc; i++)
    { pa=pa+1;
        
        if (argv[i])
            strcat(data, argv[i]);
        else
            strcat(data, "NULL");
        strcat(data, "\n");
        pa=0;
    }
    return 0;
}

int is_admin(char username[30], sqlite3 *db)
{
    int rc;
    char *zErrMsg = 0;
    char sql[150];
    char str[max];
    bzero(str, max);
    sprintf(sql, "SELECT admin FROM user WHERE username = '%s';", username);
    rc = sqlite3_exec(db, sql, callback, str, &zErrMsg);

    if (rc == SQLITE_OK)
    { //cout<<str<<endl;
        //cout<<rc<<endl;
        if (strchr(str, '1'))
        {
            printf("Utilizatorul %s este admin\n", username);
            return 1;
        }
        else
            printf("Utilizatorul %s nu este admin\n", username);
    }
    else
    {
        printf("%s\n", sql);
        printf("error: %s\n ", zErrMsg);
        sqlite3_free(zErrMsg);
        return 0;
    }

    return 0;
}
void top_genre_style(char style[20], char* outer,sqlite3 *db)
{
    int rc;
    char *zErrMsg = 0;
    char sql[150];
    char str[max];
    bzero(str, max);
    cout << style;
    bzero(str, max);
    sprintf(sql, "SELECT name FROM song s inner join style_song st on s.id_song=st.id where style = '%s';", style);
    rc = sqlite3_exec(db, sql, callback, str, &zErrMsg);
    if (rc != SQLITE_OK)
    {
        printf("%s\n", sql);
        printf("error: %s\n ", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    else
    {
        cout << str << endl;
        strcpy(outer,str);
        
    }
}
void top_genre_votes(char* outer,sqlite3 *db)
{
    char *zErrMsg = 0;
    char sql[150];
    char str[max];
    bzero(str, max);
    int rc;
    sprintf(sql, "SELECT name,votes FROM song where votes!=0 order by votes desc;");
    rc = sqlite3_exec(db, sql, callback, str, &zErrMsg);
    if (rc != SQLITE_OK)
    {
        printf("%s\n", sql);
        printf("error: %s\n ", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    else
    {
        cout << str << endl;
        strcpy(outer,str);
        
    }
}

int vote(char name[30], char* outer,sqlite3 *db)
{
    char *zErrMsg = 0;
    char sql[150];
    char str[max];
    bzero(str, max);
    int rc;
    sprintf(sql, "UPDATE song set votes = votes + 1 where name = '%s';",name);
    rc = sqlite3_exec(db, sql, callback, str, &zErrMsg);
    if (rc != SQLITE_OK)
    {
        printf("%s\n", sql);
        printf("error: %s\n ", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    else
    {
        cout << "Song voted succesfully!\n";
        strcpy(outer,"Song voted succesfully!\n");
        return 1;
    }
    return 0;
}

int unvote(char song[30], char* outer,sqlite3 *db)
{
    char *zErrMsg = 0;
    char sql[150];
    char str[max];
    bzero(str, max);
    int rc;
    sprintf(sql, "UPDATE song set votes = votes - 1 where name = '%s';",song);
    rc = sqlite3_exec(db, sql, callback, str, &zErrMsg);
    if (rc != SQLITE_OK)
    {
        printf("%s\n", sql);
        printf("error: %s\n ", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    else
    {
        strcpy(outer,"Song unvoted succesfully!");
        return 1;
    }
    return 0;
}
void about_song(char name[30], char* outer,sqlite3* db)
{
    char id[10];
    char *zErrMsg = 0;
    char sql[150];
    char str[max];
    bzero(str, max);
    int rc;
    sprintf(sql, "SELECT name from song where name = '%s';",name);
    rc = sqlite3_exec(db, sql, callback, str, &zErrMsg);
    if (rc != SQLITE_OK)
    {
        printf("%s\n", sql);
        printf("error: %s\n ", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    else
    {   cout<<"Name"<<endl;
        cout << str<<endl;
        strcat(outer,"Name: ");
        strcat(outer,"\n"); 
        strcat(outer,str);
        strcat(outer,"\n");
        
        
    }
     bzero(sql,150);
     bzero(str,max);
    sprintf(sql, "SELECT link from song where name = '%s';",name);
    rc = sqlite3_exec(db, sql, callback, str, &zErrMsg);
    if (rc != SQLITE_OK)
    {
        printf("%s\n", sql);
        printf("error: %s\n ", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    else
    {   cout<<"Link"<<endl;
        cout << str<<endl;
        strcat(outer,"Link: ");
         strcat(outer,"\n");
        strcat(outer,str);
        strcat(outer,"\n");
       
    }
    bzero(sql,150);
     bzero(str,max);
    sprintf(sql, "SELECT description from song where name = '%s';",name);
    rc = sqlite3_exec(db, sql, callback, str, &zErrMsg);
    if (rc != SQLITE_OK)
    {
        printf("%s\n", sql);
        printf("error: %s\n ", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    else
    {   cout<<"Description"<<endl;
        cout << str<<endl;
          strcat(outer,"Description: ");
           strcat(outer,"\n");
        strcat(outer,str);
        strcat(outer,"\n");
        
    }
    bzero(sql,150);
     bzero(str,max);
    sprintf(sql, "SELECT id_song from song where name = '%s';",name);
    rc = sqlite3_exec(db, sql, callback, str, &zErrMsg);
    if (rc != SQLITE_OK)
    {
        printf("%s\n", sql);
        printf("error: %s\n ", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    else
    {   
    strcpy(id,str);

    }
 bzero(sql,150);
     bzero(str,max);
    sprintf(sql, "SELECT style from style_song where id = '%s';",id);
    rc = sqlite3_exec(db, sql, callback, str, &zErrMsg);
    if (rc != SQLITE_OK)
    {
        printf("%s\n", sql);
        printf("error: %s\n ", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    else
    {   cout<<"Style"<<endl;
        cout << str<<endl;
          strcat(outer,"Style: ");
           strcat(outer,"\n");
        strcat(outer,str);
        strcat(outer,"\n");
       
    }

     bzero(sql,150);
     bzero(str,max);
    sprintf(sql, "SELECT votes from song where name = '%s';",name);
    rc = sqlite3_exec(db, sql, callback, str, &zErrMsg);
    if (rc != SQLITE_OK)
    {
        printf("%s\n", sql);
        printf("error: %s\n ", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    else
    {   cout<<"Votes"<<endl;
        cout << str<<endl;
          strcat(outer,"Votes: ");
           strcat(outer,"\n");
        strcat(outer,str);
        strcat(outer,"\n");
        
    }


    bzero(sql,150);
     bzero(str,max);
    sprintf(sql, "SELECT comments from song where name = '%s';",name);
    rc = sqlite3_exec(db, sql, callback, str, &zErrMsg);
    if (rc != SQLITE_OK)
    {
        printf("%s\n", sql);
        printf("error: %s\n ", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    else
    {   cout<<"Nr of comments"<<endl;
        cout << str<<endl;
          strcat(outer,"Number of comments: ");
           strcat(outer,"\n");
        strcat(outer,str);
        strcat(outer,"\n");
        
    }
}
void show_comments_song(char song[30],char *outer,sqlite3* db)
{
char sql[150];
    char str[max];
    bzero(str, max); char *zErrMsg = 0;
    int rc;
    sprintf(sql,"SELECT comment from comms c inner join song s on c.id_song=s.id_song where s.name = '%s';",song);
    rc = sqlite3_exec(db, sql, callback, str, &zErrMsg);
    if (rc != SQLITE_OK)
    {
        printf("%s\n", sql);
        printf("error: %s\n ", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    else
    {   
    cout<<"The song comments are:"<<endl;
    cout<<str;
strcpy(outer,"The song comments are: ");
 strcat(outer,"\n");
    strcat(outer,str);
     strcat(outer,"\n");
    
    }
}
void restrict_user_vote(char name[30],char* outer,sqlite3* db)
{
    char sql[150];
    char str[max];
    bzero(str, max); 
   // bzero(sql,150);
    char *zErrMsg = 0;
    int rc;
    sprintf(sql,"select restrict_vote from user where username='%s';",name);
 rc = sqlite3_exec(db, sql, callback, str, &zErrMsg);
//printf("%s",str);

     if(strchr(str,'0'))
     {
        char sql2[max]; char str2[max];
        int rc2; char *zErrMsg2 = 0;
        sprintf(sql2,"update user set restrict_vote = 1 where username = '%s';",name);

rc = sqlite3_exec(db, sql2, callback, str2, &zErrMsg2);
cout<<str<<endl<<"The user "<<name<<" has no vote right!";
strcat(outer,str); strcat(outer,"\0");
strcat(outer,"The user "); strcat(outer,name); strcat(outer," has been vote restricted");
     }
     else strcpy(outer,"This user has already vote restriction!");
 
 
}
int login_name(char name[30],sqlite3* db)
{
    char *zErrMsg = 0;
    char sql[150];
    char str[max];
    bzero(str, max);
    int rc;
    sprintf(sql, "SELECT username from user where username = '%s';",name);
    rc = sqlite3_exec(db, sql, callback_single, str, &zErrMsg);
    if (rc != SQLITE_OK)
    {
        printf("%s\n", sql);
        printf("error: %s\n ", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    else
       if(strcmp(str,name)==0) return 1;
    cout<<str;
   return 0;
    
}
int login_passwd(char name[30],char password[30],sqlite3* db)
{
//verif passwd
char *zErrMsg = 0;
    char sql[150];
    char str[max];
    bzero(str, max);
    int rc;
    sprintf(sql, "SELECT username,password from user where password = '%s' and username= '%s';",password,name);
    rc = sqlite3_exec(db, sql, callback, str, &zErrMsg);
    if (rc != SQLITE_OK)
    {
        printf("%s\n", sql);
        printf("error: %s\n ", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    else
      if(strstr(str,password)) 
    {cout<<str;
    return 1;}
    
   return 0;
    
}
int verifiy_restrict_user(char username[30], sqlite3 *db)
{
    int rc;
    char *zErrMsg = 0;
    char sql[150];
    char str[max];
    bzero(str, max);
    sprintf(sql, "SELECT restrict_user FROM user WHERE username = '%s';", username);
    rc = sqlite3_exec(db, sql, callback_single, str, &zErrMsg);
    if(strstr(str,"1"))
    { cout<<"userul este restrictionat"<<endl;
        return 0;
    }
    cout<<"userul nu este restrictionat"<<endl;
return 1;
}
int verify_song(char song[30],sqlite3* db)
{
//
char *zErrMsg = 0;
    char sql[150];
    char str[max];
    bzero(str, max);
    int rc;
    sprintf(sql, "SELECT name from song where name='%s';",song);
    rc = sqlite3_exec(db, sql, callback, str, &zErrMsg);
    if(strstr(str,song))
    {cout<<str;
        return 1;
    }
    cout<<str;
return 0;
}
void show_comments_user(char name[30],char* outer,sqlite3* db)
{
char sql[150];
    char str[max];
    bzero(str, max); char *zErrMsg = 0;
    int rc;
    sprintf(sql,"SELECT comment from comms c inner join user u on c.id_user=u.id where u.username = '%s';",name);
    rc = sqlite3_exec(db, sql, callback, str, &zErrMsg);
    if (rc != SQLITE_OK)
    {
        printf("%s\n", sql);
        printf("error: %s\n ", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    else
    {   
    cout<<"The user comments are:"<<endl<<str;
    strcat(outer,"The user comments are: ");
    strcat(outer,"\0");
    strcat(outer,str);
    strcat(outer,"\0");

    
    }
}
int verify_user(char name[30],sqlite3* db)
{
    char sql[150];
    char str[max];
   
    bzero(str, max); char *zErrMsg = 0;
    int rc;
     sprintf(sql,"select username from user where username = '%s';",name);
    rc = sqlite3_exec(db, sql, callback_single, str, &zErrMsg);
    cout<<name<<" "<<str;
    cout<<strlen(name);
    if(strstr(str,name))
    {return 1;}
    return 0;
}
void restrict_user_comment(char name[30],char*outer,sqlite3* db)
{
//ceva
 char sql[150];
    char str[max];
    bzero(str, max); 
   // bzero(sql,150);
    char *zErrMsg = 0;
    int rc;
    sprintf(sql,"select restrict_comment from user where username='%s';",name);
 rc = sqlite3_exec(db, sql, callback, str, &zErrMsg);
//printf("%s",str);

     if(strchr(str,'0'))
     {
        char sql2[max]; char str2[max];
        int rc2; char *zErrMsg2 = 0;
        sprintf(sql2,"update user set restrict_comment = 1 where username = '%s';",name);

rc = sqlite3_exec(db, sql2, callback, str2, &zErrMsg2);
cout<<str<<endl<<"The user "<<name<<" has no comment right!";
strcat(outer,str); strcat(outer,"\0");
strcat(outer,"The user "); strcat(outer,name); strcat(outer," has been comment restricted");
     }
     else strcpy(outer,"This user has already comment restriction!");
 

}
void make_admin(char name[30],char* outer, sqlite3*db)
{
    char sql[150];
    char str[max];
    bzero(str, max); 
   // bzero(sql,150);
    char *zErrMsg = 0;
    int rc;
    sprintf(sql,"select admin from user where username='%s';",name);
 rc = sqlite3_exec(db, sql, callback_single, str, &zErrMsg);
 strcpy(outer,"wadafac");
 if(strstr(str,"0"))
 {
     char sql2[max]; char str2[max];
        int rc2; char *zErrMsg2 = 0;
        sprintf(sql2,"update user set admin = 1 where username = '%s';",name);

rc2 = sqlite3_exec(db, sql2, callback, str2, &zErrMsg2);
strcpy(outer,"From now this user has admin flag!");

 }
 else strcpy(outer,"This user is already an admin!");
}
void remove_song(char song[30],char* outer,sqlite3*db)
{
 char *zErrMsg = 0;
    char sql[150];
    char str[max];
    bzero(str, max);
    int rc;
    sprintf(sql, "update song set votes=0 where name='%s';", song );
     rc = sqlite3_exec(db, sql, callback, str, &zErrMsg);
     strcpy(outer,"The song has been removed succesfully!");
}
