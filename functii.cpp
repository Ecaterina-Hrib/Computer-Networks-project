#include <iostream>
#include <sqlite3.h>
#include <string.h>
using namespace std;
#define max 1024
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
void top_genre_style(char style[20], sqlite3 *db)
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
    }
}
void top_genre_votes(sqlite3 *db)
{
    char *zErrMsg = 0;
    char sql[150];
    char str[max];
    bzero(str, max);
    int rc;
    sprintf(sql, "SELECT name FROM song order by votes desc;");
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
    }
}

void vote(char name[30], sqlite3 *db)
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
    }
}
void unvote(char name[30], sqlite3 *db)
{
    char *zErrMsg = 0;
    char sql[150];
    char str[max];
    bzero(str, max);
    int rc;
    sprintf(sql, "UPDATE song set votes = votes - 1 where name = '%s';",name);
    rc = sqlite3_exec(db, sql, callback, str, &zErrMsg);
    if (rc != SQLITE_OK)
    {
        printf("%s\n", sql);
        printf("error: %s\n ", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    else
    {
        cout << "Song unvoted succesfully!\n";
    }
}
void about_song(char name[30], sqlite3* db)
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
    }
}
void login(char name[30],char password[30],sqlite3* db)//nu ii terminata da nu mai am chef sa fac nica la ea azi
{
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
    {   if(strcmp(str,name)==0) 
    cout<<"Correct"<<endl;
    else 
    cout<<"The user doesn't exist"<<endl;
    }
}
void show_comments_user(char name[30],sqlite3* db)
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
    
    }

    //ceva cu file descriptori
    //update la nr de comentarii
}
void insert_comment(char name[30],char song[30],char comment[150],int id,sqlite3* db)//id din main ca sa tina minte numarul unic al comentariului
{ char sql[150];
    char str[max];
    char str1[max];
    char str2[max];
    bzero(str, max); char *zErrMsg = 0;
    int rc;
     sprintf(sql,"select id from user where username = '%s';",name);
    rc = sqlite3_exec(db, sql, callback_single, str1, &zErrMsg);
    rc=-1;
    bzero(sql,150);
    sprintf(sql,"select id_song from song where name = '%s';",song);
    rc = sqlite3_exec(db, sql, callback_single, str2, &zErrMsg);
    rc=-1;
     bzero(sql,150);
   // sprintf("insert into comms values ('%d','%s','%s','%s');",id,str1,comment,str2);




}

void show_comments_song(char song[30],sqlite3* db)
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
    
    }
}
void restrict_user_vote(char name[30],sqlite3* db)
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
        sprintf(sql2,"update user set vote = 1 where name = '%s';",name);

rc = sqlite3_exec(db, sql2, callback, str2, &zErrMsg2);
cout<<str<<endl<<"The user "<<name<<" has no vote right!";
     }
     else cout<<"The user vote is already restricted!"<<endl;
 
 
}
void restrict_user_comment(char name[30],sqlite3* db)
{
//ceva
}

int main()
{
    sqlite3 *db;
    char name[30] = "alexandru00";
    //opendb(db);
    int record;
    char song_name[30]="Bubblegum - Jason Derulo";

    record = sqlite3_open("Info.db", &db);

    if (record)
        printf("Error at opening database!\n");
    else
        printf("Succes at opening database\n");

    char style[20] = "rock";
    char style2[20] = "jazz";
    //is_admin(name,db);
    //top_genre_votes(db);
    //top_genre_style(style, db);
    //top_genre_style(style2, db);
    //unvote(song_name,db);
    //about_song(song_name,db);
    //show_comments_song(song_name,db);
    //show_comments_user(name,db);
    restrict_user_vote(name,db);
    sqlite3_close(db);
    return 0;
}
