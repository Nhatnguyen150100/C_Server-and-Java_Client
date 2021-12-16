#ifndef __MYSQL_H__
#define __MYSQL_H__

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include <mysql/mysql.h>

#define MAX_IDENT_LEN_NAME 100
#define MAX_IDENT_LEN_PASSWORD 30

typedef struct {
    char name[MAX_IDENT_LEN_NAME];
    char password[MAX_IDENT_LEN_PASSWORD];
} Account;

// Account* makeAccount(char* nameAccount, char* passwordAccount);
bool checkNameAccount(char* name, char* password, MYSQL *con, MYSQL_RES *result);
int createNewAccount(char* name, char* password, MYSQL *con, MYSQL_RES *result);
bool checkPasswordAcount(char* nameAccoount, char* passwordAccount,MYSQL *con, MYSQL_RES *result);
void finish_with_error(MYSQL *con);
char* removeEnterCharacter(char* str);
char* removeEnterCharacterFromString(char* str);

bool checkNameAccount(char* name, char* password, MYSQL *con, MYSQL_RES *result){
    char query[1000];
    sprintf(query,"SELECT userName FROM account where userName = \'%s\' and password = %s", name, password);
    printf("Query check account:%s\n", query);
    if (mysql_query(con, query)){
      finish_with_error(con);
    }
    result = mysql_store_result(con);
    if (result == NULL){
      finish_with_error(con);
    }
    MYSQL_ROW row;
    // int num_fields = mysql_num_fields(result);
    // printf("num_fields: %d\n", num_fields);
    
    if(row = mysql_fetch_row(result)){
        return true;
    }else{
        return false;
    }
}

int indexOfRowAcount(MYSQL *con, MYSQL_RES *result){
    char *index;
    char inValue[0];
    if (mysql_query(con, "SELECT MAX(idaccount) FROM account")){
      finish_with_error(con);
    }
    result = mysql_store_result(con);
    if (result == NULL){
      finish_with_error(con);
    }

    int num_fields = mysql_num_fields(result);
    MYSQL_ROW row;
    row = mysql_fetch_row(result);

    index = row[0];
    int value = atoi(index);
    return value + 1;
}

int createNewAccount(char* name, char* password, MYSQL *con, MYSQL_RES *result){
    int index = indexOfRowAcount(con,result);
    printf("index: %d\n", index);
    // printf("start!");
    char query[1000];
    printf("query insert: %s\n", query);
    sprintf(query,"INSERT INTO account values (%d,\'%s\',%s)", index,name,password);
    printf("query insert: %s\n", query);
    mysql_query(con,query);
    return index;
}

// bool checkPasswordAccount(char* nameAccoount,  char* passwordAccount, MYSQL *con, MYSQL_RES *result){
//     char name[102] = "\'";
//     char name_x[1] = "\'";
//     char nameac[100] = "Nhatnguyen150100";
//     strcat(name, nameac);
//     strcat(name, "\'");
//     char password[100] = "12345";
//     char query[10000] = "SELECT password FROM account Where ";
//     char query_1[1000] = "userName = ";
//     strcat(query_1, name);
//     char query_2[100] = " and password = ";
//     strcat(query_2, password);
//     strcat(query_1, query_2);
//     strcat(query, query_1);
//     if (mysql_query(con, query)){
//       finish_with_error(con);
//     }
//     result = mysql_store_result(con);
//     if (result == NULL){
//       finish_with_error(con);
//     }
//     MYSQL_ROW row;
//     row = mysql_fetch_row(result);
//     return row;
// }

void finish_with_error(MYSQL *con)
{
  fprintf(stderr, "%s\n", mysql_error(con));
  mysql_close(con);
  exit(1);
}

char* removeEnterCharacter(char* str){
    const int len = strlen(str);
    if( len == 0 ) {
        return '\0';      
    } else {
        str[len-1] = '\0';
        return str;// Trả về chuỗi kết quả
    }
}

char* removeEnterCharacterFromString(char* str){
    const int len = strlen(str);
    for(int i = 0; i < len; i++){
        if( str[i] == '\n' ){
            str[i] = '\0';
        }
    }
    return str;
}


#endif