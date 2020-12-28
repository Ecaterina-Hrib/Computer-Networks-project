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
    cout << endl;
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
     bzero(sql,max);
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
    bzero(sql,max);
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
    bzero(sql,max);
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
 bzero(sql,max);
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

     bzero(sql,max);
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


    bzero(sql,max);
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
void show_comments()
{
    //ceva cu file descriptori
    //update la nr de comentarii
}
int main()
{
    sqlite3 *db;
    char name[30] = "katy";
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
    
    sqlite3_close(db);
    return 0;
}