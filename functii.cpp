#include <iostream>
#include <sqlite3.h>
#include <string.h>
using namespace std;
#define max 10024
static int callback(void *str, int argc, char **argv, char **azColName)
{
    int i;
    char *data = (char *)str;
    //strcat (data, "\n");
    //cout << endl;
    for (i = 0; i < argc; i++)
    {
        //strcat (data, azColName[i]);
        //strcat (data, " = ");
        if (argv[i])
            strcat(data, argv[i]);
        else
            strcat(data, "NULL");
        strcat(data, "\n");
    }

    //strcat (data, "\n");
    return 0;
}
static int callback_single(void *str, int argc, char **argv, char **azColName)
{
    int i;
    char *data = (char *)str;
    
    for (i = 0; i < argc; i++)
    {
        
        if (argv[i])
            strcat(data, argv[i]);
        else
            strcat(data, "NULL");
        
    }

    
    return 0;
}
static int callback2(void *str, int argc, char **argv, char **azColName)
{
    int i;
    char *data = (char *)str;
    //strcat (data, "\n");
    cout << endl;
    for (i = 0; i < argc; i++)
    {
        strcat (data, azColName[i]);
        strcat (data, " = ");
        if (argv[i])
            strcat(data, argv[i]);
        else
            strcat(data, "NULL");
        strcat(data, "\n");
    }

    //strcat (data, "\n");
    return 0;
}
int opendb(sqlite3 *db1)
{
    int record;

    record = sqlite3_open("Info.db", &db1);

    if (record)
        printf("Error at opening database!\n");
    else
        printf("Succes at opening database\n");
    return 1;
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
    { cout<<str<<endl;
        return 0;
    }
    cout<<str<<endl;
return 1;
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
void top_genre_style(char style[20],char* outer, sqlite3 *db)
{
    int rc;
    char *zErrMsg = 0;
    char sql[150];
    char str[max];
    bzero(str, max);
    cout <<"The style songs "<< style<< " are:"<<endl<<endl;
    bzero(str, max);
    sprintf(sql, "SELECT  name FROM song s inner join style_song st on s.id_song=st.id where style = '%s';", style);
    rc = sqlite3_exec(db, sql, callback, str, &zErrMsg);
   
    
        
      strcpy(outer,str);
       
        
    
}
int verify_vote(char name[30],sqlite3*db)
{
    char *zErrMsg = 0;
    char sql[150];
    char str[max];
    bzero(str, max);
    int rc;
    sprintf(sql, "SELECT restrict_vote from song where name = '%s';",name);
    rc = sqlite3_exec(db, sql, callback, str, &zErrMsg);
    if(strstr(str,"0"))
    {return 1;}
    return 0;

}
int remove_song(char song[30],char* outer,sqlite3*db)
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
char* top_genre_votes(sqlite3 *db)
{
    char *zErrMsg = 0;
    char sql[150];
    char str[max];
    bzero(str, max);
    int rc;
    sprintf(sql, "SELECT name FROM song where votes!= 0 order by votes desc;");
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
        return str;
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
int login_name(char name[30],sqlite3* db)
{
    char *zErrMsg = 0;
    char sql[150];
    char str[max];
    bzero(str, max);
    int rc;
    sprintf(sql, "SELECT username from user where username = '%s';",name);
    rc = sqlite3_exec(db, sql, callback, str, &zErrMsg);
       
    if(strcmp(str,name)==0) return 1;
    
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
      if(strcmp(str,password)==0) 
    {cout<<str;
    return 1;}
    
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
    strcat(outer,"The user comments are:");
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
    if(strstr(str,name))
    {return 1;}
    cout<<str;
    cout<<name;
    return 0;
}
void insert_comment(char name[30],char song[30],char comment[150],char* outer,sqlite3* db)//id din main ca sa tina minte numarul unic al comentariului
{ char sql[max];
    char str[max];
    char str1[max];
    char str2[max];
    char str3[max];
    char str_final[max];
     char str_select[max];
    bzero(str, max); char *zErrMsg = 0;
    int rc;
    char id;
     sprintf(sql,"select id from user where username = '%s';",name);
    rc = sqlite3_exec(db, sql, callback_single, str1, &zErrMsg);
    rc=-1;
    bzero(sql,max);
    sprintf(sql,"select id_song from song where name = '%s';",song);
    rc = sqlite3_exec(db, sql, callback_single, str2, &zErrMsg);
    rc=-1;
     bzero(sql,max);
     sprintf(sql,"select count(comment)+1 from comms group by id_comment;");
     rc = sqlite3_exec(db, sql, callback_single, str3, &zErrMsg);
    bzero(sql,max);
    rc=-1;
   sprintf(sql,"insert into comms values ('%s','%s','%s','%s');",str3,str1,comment,str2);
rc = sqlite3_exec(db, sql, callback_single, str_final, &zErrMsg);




}

void show_comments_song(char song[30],sqlite3* db, char* outer)
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
    strcat(outer,"The song comments are: ");
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

void make_admin(char name[30],char* outer, sqlite3*db)
{
    char sql[150];
    char str[max];
    bzero(str, max); 
   // bzero(sql,150);
    char *zErrMsg = 0;
    int rc;
    sprintf(sql,"select admin from user where username='%s';",name);
 rc = sqlite3_exec(db, sql, callback, str, &zErrMsg);
 if(strstr(str,"0"))
 {
     char sql2[max]; char str2[max];
        int rc2; char *zErrMsg2 = 0;
        sprintf(sql2,"update user set admin = 1 where username = '%s';",name);

rc = sqlite3_exec(db, sql2, callback, str2, &zErrMsg2);
strcpy(outer,"From now this user has admin flag!");

 }
 else strcpy(outer,"This user is already an admin!");
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
p=strtok(NULL,"\n");
}
}
int main()
{
    sqlite3 *db;
    char name_p[30] = "ion";
    //opendb(db);
    int record;
    char *outer=(char*)malloc(sizeof(char)*max);
   char comment[]="I love this song, because it's giving me vibes of summer!";
    char song_name[]="Some Say - Nea";
    char password[30]="kamasis";

    record = sqlite3_open("Info.db", &db);

    if (record)
        printf("Error at opening database!\n");
    else
        printf("Succes at opening database\n");

    char style[20] = "rock";
    char style2[20] = "jazz";
    //int i=verify_song(song_name,db);
    //cout<<" afisez cv "<<i<<endl;
   //name_split(song_name,outer);
   //cout<<outer;
    //is_admin(name,db);
    //char* c=top_genre_votes(db);
    //cout<<c;
    bzero(outer,max);

  // top_genre_style(style,outer, db);
   //cout<<outer;
    //top_genre_style(style2, db);
    //unvote(song_name,db);
   // about_song(song_name,outer,db);
    // cout<<outer;
    //show_comments_song(song_name,db);
    //show_comments_user(name,db);
  /* bzero(outer,max);
    name_split(name_p,outer);
    bzero(name_p,30);
    strcpy(name_p,outer);
    name_p[strlen(name_p)]='\0';
    cout<<strlen(name_p)<<endl;
    cout<<name_p<<endl;
    //int p=login_name(name_p,db);
   // cout<<p<<endl;
    if(verifiy_restrict_user(name_p,db))
    cout<<"Userul nu este restrictionat"<<endl;*/
   //if (p)
   // cout<<login_passwd(name_p,password,db)<<endl;
//nt flag= verify_user(name_p,db);
//cout<<flag<<"<- exista/nu exista";
//vote(song_name,outer,db);
//cout<<outer;
//restrict_user_comment(name_p,outer,db);
//make_admin(name_p,outer,db);

cout<<verify_vote(name_p,db);
    //restrict_user_vote(name,db);
    sqlite3_close(db);
    return 0;
}
