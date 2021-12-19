#ifndef __MYSQL_H__
#define __MYSQL_H__

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include <mysql/mysql.h>
#include "user.h"

#define MAX_IDENT_LEN_NAME 100
#define MAX_IDENT_LEN_PASSWORD 30

typedef struct {
    char name[MAX_IDENT_LEN_NAME];
    char password[MAX_IDENT_LEN_PASSWORD];
} Account;

// Account* makeAccount(char* nameAccount, char* passwordAccount);
char* checkNameAccount(char* name, char* password, MYSQL *con, MYSQL_RES *result);
int createNewAccount(char* name, char* password, MYSQL *con, MYSQL_RES *result);
bool checkPasswordAcount(char* nameAccoount, char* passwordAccount,MYSQL *con, MYSQL_RES *result);
bool insertInforAccount(User *user, MYSQL *con, MYSQL_RES *result);
bool updateInforAccount(User *user, MYSQL *con, MYSQL_RES *result);
void finish_with_error(MYSQL *con);
char* removeEnterCharacter(char* str);
char* removeEnterCharacterFromString(char* str);

char* checkNameAccount(char* name, char* password, MYSQL *con, MYSQL_RES *result){
    char query[1000];
    sprintf(query,"SELECT idaccount FROM account where userName = \'%s\' and password = \'%s\'", name, password);
    printf("Query check account:%s\n", query);
    if (mysql_query(con, query)){
      finish_with_error(con);
    }
    result = mysql_store_result(con);
    if (result == NULL){
      finish_with_error(con);
    }
    MYSQL_ROW row;
    char *idaccount;
    // int num_fields = mysql_num_fields(result);
    // printf("num_fields: %d\n", num_fields);
    
    if(row = mysql_fetch_row(result)){
        idaccount = row[0];
        return idaccount;
    }else{
        return "false";
    }
}

void addInforUser(User *user,char* idaccount, MYSQL *con, MYSQL_RES *result){
    char query[1000];
    sprintf(query,"SELECT * FROM infor WHERE idUser = %s",idaccount);
    printf("query: %s\n", query);
    if (mysql_query(con, query)){
      finish_with_error(con);
    }
    result = mysql_store_result(con);
    if (result == NULL){
      finish_with_error(con);
    }
    int num_fields = mysql_num_fields(result);
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result))){
        // printf("So truong: %d\n", num_fields);
        for(int i = 0; i < num_fields; i++){
            if(i==0){
                sprintf(user->idUser,"%s",row[i]);
            }else if(i==1){
                sprintf(user->firstName,"%s",row[i]);
            }else if(i==2){
                sprintf(user->lastName,"%s",row[i]);
            }else if(i==3){
                sprintf(user->cardId,"%s",row[i]);
            }else if(i==4){
                sprintf(user->birthday,"%s",row[i]);
            }else if(i==5){
                sprintf(user->gender,"%s",row[i]);
            }else if(i==6){
                sprintf(user->numberPhone,"%s",row[i]);
            }else if(i==7){
                sprintf(user->address,"%s",row[i]);
            }else if(i==8){
                sprintf(user->email,"%s",row[i]);
            }else if(i==9){
                sprintf(user->state,"%s",row[i]);
            }
        }
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
    sprintf(query,"INSERT INTO account values (%d,\'%s\',\'%s\')", index,name,password);
    printf("query insert: %s\n", query);
    mysql_query(con,query);
    return index;
}

bool insertInforAccount(User *user, MYSQL *con, MYSQL_RES *result){
    char query[1000];
    sprintf(query,"INSERT INTO infor VALUES(%s,\'%s\',\'%s\',\'%s\',\'%s\',\'%s\',\'%s\',\'%s\',\'%s\',\'%s\')", user->idUser,user->firstName,user->lastName,user->cardId,user->birthday,user->gender,user->numberPhone,user->address,user->email,user->state);
    if (mysql_query(con, query)){
        printf("INSERT INFORMATION ERROR!\n");
      finish_with_error(con);
      return false;
    }else{
        return true;
    }
}

bool updateInforAccount(User *user, MYSQL *con, MYSQL_RES *result){
    char query[1000];
    sprintf(query,"UPDATE infor set firstName = \'%s\', lastName = \'%s\', cardId = \'%s\', birthOfDay = \'%s\', gender = \'%s\', numberPhone = \'%s\', email = \'%s\', address = \'%s\', state = \'%s\' where idUser = %s", user->firstName,user->lastName,user->cardId,user->birthday,user->gender,user->numberPhone,user->address,user->email,user->state, user->idUser);
    if (mysql_query(con, query)){
        printf("Update INFORMATION ERROR!\n");
      finish_with_error(con);
        return false;
    }else{
        return true;
    }
}


void finish_with_error(MYSQL *con)
{
  fprintf(stderr, "%s\n", mysql_error(con));
  mysql_close(con);
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