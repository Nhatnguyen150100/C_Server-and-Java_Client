#ifndef __MYSQL_H__
#define __MYSQL_H__

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include <mysql/mysql.h>
#include "user.h"
#include "locationAndTime.h"
#include "account.h"
#include "locationOftude.h"
#include "formdeclare.h"


// Account* makeAccount(char* nameAccount, char* passwordAccount);

LocationAndTime* getHistory(User* user, MYSQL *con, MYSQL_RES *result);
User* listIdOfF0(MYSQL *con, MYSQL_RES *result);
bool checkPasswordAcount(char* nameAccoount, char* passwordAccount,MYSQL *con, MYSQL_RES *result);
bool insertInforAccount(User *user, MYSQL *con, MYSQL_RES *result);
bool updateInforAccount(User *user, MYSQL *con, MYSQL_RES *result);
bool insertLocationAndTime(LocationAndTime *locationAndTime, User *user, int codeLocation,MYSQL *con, MYSQL_RES *result);
bool insertFormDecalre(Formdeclare *formDeclare, MYSQL *con, MYSQL_RES *result);
bool updateStateF0forUser(int idUser ,MYSQL *con, MYSQL_RES *result);
bool updateStateNormalforUser(int idUser ,MYSQL *con, MYSQL_RES *result);
bool updateState(char* idUser,MYSQL *con, MYSQL_RES *result);
char* checkNameAccount(char* name, char* password, MYSQL *con, MYSQL_RES *result);
char* checkNamePasswordAccount(char* name, char* password, MYSQL *con, MYSQL_RES *result);
char* removeEnterCharacter(char* str);
char* removeEnterCharacterFromString(char* str);
char* getLocationOfF0(MYSQL *con, MYSQL_RES *result);
int createNewAccount(char* name, char* password, MYSQL *con, MYSQL_RES *result);
void showAllOfInformationsUser(MYSQL *con, MYSQL_RES *result);
void finish_with_error(MYSQL *con);



bool updateState(char* idUser,MYSQL *con, MYSQL_RES *result){
    char query[1000];
    sprintf(query,"UPDATE mydb.infor set state = \"F1\" where idUser = %s", idUser);
    if (mysql_query(con, query)){
        printf("Update State ERROR!\n");
      finish_with_error(con);
        return false;
    }else{ 
        // printf("Update State Successful!\n");
        return true;
    }
}

char* getLocationOfF0(MYSQL *con, MYSQL_RES *result){
    int indexOfListF0 = 0;
    int indexListLocation = 0;
    char *list;
    char stringLocation[2000]="";
    User* listId;
    LocationOftude* listLocation;
    listLocation = (LocationOftude*)malloc(500*sizeof(LocationOftude));
    listId = (User*)malloc(10*sizeof(User*));
    if (mysql_query(con, "SELECT idUser FROM infor where state = 'F0'")){
        printf("check error0\n");
        finish_with_error(con);
        exit(1);
    }
    result = mysql_store_result(con);
    if (result == NULL){
      finish_with_error(con);
      exit(1);
    }
    int num_fields = mysql_num_fields(result);
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result))){
        for(int i = 0; i < num_fields; i++){
            // printf("i: %d\n", i);
            sprintf(listId[indexOfListF0].idUser,"%s",row[i]);
            indexOfListF0++;
            strcpy(listId[indexOfListF0].idUser,"");
            // printf("F0:%s ", row[i] ? row[i] : "NULL");
        }
        // printf("\n");
    }
    if(!strcmp(listId[0].idUser,"")==0){
        for(int i = 0; i < 20; i++){
            if(!strcmp(listId[i].idUser,"")==0){
                char query[200];
                sprintf(query,"select distinct locationName,longitude,latitude from mydb.location,mydb.check where mydb.location.idlocation = mydb.check.idlocation and mydb.check.idaccount = \'%s\'",listId[i].idUser);
                if (mysql_query(con, query)){
                    printf("check error1");
                    finish_with_error(con);
                    exit(1);
                }
                result = mysql_store_result(con);
                if (result == NULL){
                    finish_with_error(con);
                }
                int num_fields = mysql_num_fields(result);
                MYSQL_ROW row1;
                while ((row1 = mysql_fetch_row(result))){
                    bool check = false;                                      
                        for(int  k = 0; k < indexListLocation; k++){
                            if(strcmp(listLocation[k].location, row1[0])==0){
                                check = true;
                            }
                        }
                        if(check==false){
                            for(int j = 0; j < num_fields; j++){
                                if(j==0){
                                    sprintf(listLocation[indexListLocation].location,"%s",row1[0]);
                                }else if(j==1){
                                    sprintf(listLocation[indexListLocation].longitude,"%s",row1[1]);
                                }else{
                                    sprintf(listLocation[indexListLocation].latitude,"%s",row1[2]);
                                }                                
                            }
                            indexListLocation++; 
                        }               
                }
            }
        }
        for(int i = 0; i < indexListLocation; i++){
            strcat(stringLocation, listLocation[i].location);
            strcat(stringLocation,"@");
            strcat(stringLocation,listLocation[i].longitude);
            strcat(stringLocation,"!");
            strcat(stringLocation,listLocation[i].latitude);
            if(i!=indexListLocation-1){
                strcat(stringLocation,"_");
            }     
            // printf("listLocation[i].location: %s\n",stringLocation);  
        }
        sprintf(list,"%s",stringLocation);
        // printf("list %s\n", stringLocation);
        // stringLocation = removeEnterCharacter(stringLocation);
        free(listLocation);
        free(listId);
        return list;
    }
    free(listLocation);
    free(listId); 
    return "";
}


User* listIdOfF0(MYSQL *con, MYSQL_RES *result){
    int indexOfListF0 = 0;
    User* listId;
    User* listF1 ;
    listId = (User*)malloc(10*sizeof(User*));
    listF1 = (User*)malloc(500*sizeof(User*));
    if (mysql_query(con, "SELECT idUser FROM infor where state = 'F0'")){
        printf("check error0\n");
        finish_with_error(con);
        exit(1);
    }
    result = mysql_store_result(con);
    if (result == NULL){
      finish_with_error(con);
      exit(1);
    }
    int num_fields = mysql_num_fields(result);
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result))){
        for(int i = 0; i < num_fields; i++){
            // printf("i: %d\n", i);
            sprintf(listId[indexOfListF0].idUser,"%s",row[i]);
            indexOfListF0++;
            strcpy(listId[indexOfListF0].idUser,"");
            printf("F0:%s ", row[i] ? row[i] : "NULL");
        }
        printf("\n");
    }
    if(!strcmp(listId[0].idUser,"")==0){               
        int indexOfListF1 = 0;
        for(int i = 0; i < 20; i++){
            // printf("i : %d", i);

            if(!strcmp(listId[i].idUser,"")==0){
                // printf("id: %s\n", listId[i].idUser);
                LocationAndTime* listLocation;
                listLocation = (LocationAndTime*)malloc(500*sizeof(LocationAndTime));
                char query[200];
                sprintf(query,"select distinct idlocation from mydb.check where idaccount = \'%s\'",listId[i].idUser);
                if (mysql_query(con, query)){
                    printf("check error1");
                    finish_with_error(con);
                    exit(1);
                }
                result = mysql_store_result(con);
                if (result == NULL){
                    finish_with_error(con);
                }
                int num_fields = mysql_num_fields(result);
                MYSQL_ROW row1;
                int indexListLocation = 0;
                while ((row1 = mysql_fetch_row(result))){                
                    for(int j = 0; j < num_fields; j++){
                        // printf("idlocation: %s\n", row1[j]);
                        sprintf(listLocation[indexListLocation].location,"%s",row1[j]);

                    }
                    indexListLocation++;
                }
                for(int k = 0; k < 500; k++){
                    // printf("listLocation : %s\n", listLocation[k].location);
                    if(!(strcmp(listLocation[k].location,"")==0)){
                        char query2[500];
                        sprintf(query2,"select Max(time) from mydb.check where idaccount = \"%s\" and idlocation = \"%s\"", listId[i].idUser, listLocation[k].location);
                        if (mysql_query(con, query2)){
                            printf("check error2");
                            finish_with_error(con);
                            exit(1);
                        }
                        result = mysql_store_result(con);
                        if (result == NULL){
                            finish_with_error(con);
                        }
                        MYSQL_ROW row2;
                        row2 = mysql_fetch_row(result);
                        char time[30];
                        sprintf(time,"%s",row2[0]);
                        // printf("time : %s\n", time);
                        // printf("location %s\n", listLocation[k].location);
                        // printf("listId: %s\n", listId[i].idUser);
                        char quey3[500];
                        sprintf(quey3,"select distinct idaccount from mydb.check where time > date_add(\'%s\', interval 1 day) and idlocation = %s and idaccount != %s ", time, listLocation[k].location, listId[i].idUser);
                        if (mysql_query(con, quey3)){
                            printf("check error3");
                            finish_with_error(con);
                            exit(1);
                        }
                        result = mysql_store_result(con);
                        if (result == NULL){
                            finish_with_error(con);
                        }
                        MYSQL_ROW row3;
                        while ((row3 = mysql_fetch_row(result))){
                            for(int h = 0; h < num_fields; h++){
                                // printf("F1: %s\n", row3[h]);
                                sprintf(listF1[indexOfListF1].idUser,"%s",row3[h]);
                                // printf("F1: %s\n", listF1[indexOfListF1].idUser);
                                indexOfListF1++;
                                // strcpy(listF1[indexOfListF1].idUser,"");
                            }
                        }
                        bzero(time,sizeof(time));
                    }else{
                        // printf("Wrong\n");
                        break;
                    }
                }
                // free(listLocation);
            }else{
                // printf("Wrong\n");
                break;
            }
        }
        indexOfListF1 = 0;
    }
    
    for(int i = 0; i < 500; i++){
        if(!(strcmp(listF1[i].idUser,"")==0)){
            for(int j = 0; j < indexOfListF0; j++){
                if(strcmp(listId[j].idUser,listF1[i].idUser)==0){
                    sprintf(listF1[i].idUser,"%s","-1");
                }
            }
        }else{
            break;
        }       
    }
            // for(int i = 0; i < 500; i++){
            //     if(!(strcmp(listF1[i].idUser,"")==0) && strlen(listF1[i].idUser)<5){           
            //         // printf("idUser F1:%s\n",listF1[i].idUser);                    
            //     }else{
            //         break;
            //     }       
            // }
    free(listId);
    return listF1;
}



void showAllOfInformationsUser(MYSQL *con, MYSQL_RES *result){
    if (mysql_query(con, "SELECT * FROM infor")){
      finish_with_error(con);
    }
    result = mysql_store_result(con);
    if (result == NULL){
      finish_with_error(con);
    }
    int num_fields = mysql_num_fields(result);

    MYSQL_ROW row;
    printf("id\tFirst name  Last Name\t    CCCD\t\tNgay sinh\tGioi tinh   Sdt\t\t    Email\t\t\tDia chi\t\t\t\tTinh trang\n\n");
    while ((row = mysql_fetch_row(result))){
      for(int i = 0; i < num_fields; i++){
        if(i==0){
            printf("%s\t", row[i] ? row[i] : "NULL");
        }else if(i==1){
            printf("%-7s\t    ", row[i] ? row[i] : "NULL");
        }
        else if(i==2){
            printf("%-15s ", row[i] ? row[i] : "NULL");
        }
        else if(i==3){
            printf("%-15s\t", row[i] ? row[i] : "NULL");
        }else if(i==5){
            printf("%s\t    ", row[i] ? row[i] : "NULL");
        }else if(i==6){
            printf("%s\t    ", row[i] ? row[i] : "NULL");
        }else if(i==7){
            printf("%-27s\t", row[i] ? row[i] : "NULL");
        }else if(i==8){
            printf("%-30s\t", row[i] ? row[i] : "NULL");
        }
        else{
            printf("%-5s\t", row[i] ? row[i] : "NULL");
        }         
      }
      printf("\n");
    }
}

bool updateStateF0forUser(int idUser ,MYSQL *con, MYSQL_RES *result){
    char query[200];
    sprintf(query,"UPDATE mydb.infor set state = \'F0\' where idUser = %d",idUser);
    if (mysql_query(con, query)){
      finish_with_error(con);
      return false;
    }else{
        return true;
    }
}

bool updateStateNormalforUser(int idUser ,MYSQL *con, MYSQL_RES *result){
    char query[200];
    sprintf(query,"UPDATE mydb.infor set state = \'normal\' where idUser = %d",idUser);
    if (mysql_query(con, query)){
      finish_with_error(con);
      return false;
    }else{
        return true;
    }
}

char* checkNameAccount(char* name, char* password, MYSQL *con, MYSQL_RES *result){
    char query[1000];
    sprintf(query,"SELECT idaccount FROM account where userName = \'%s\'", name);
    // printf("Query check account:%s\n", query);
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

char* checkNamePasswordAccount(char* name, char* password, MYSQL *con, MYSQL_RES *result){
    char query[1000];
    sprintf(query,"SELECT idaccount FROM account where userName = \'%s\' and password = \'%s\'",name,password); 
    // printf("Query check account:%s\n", query);
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

LocationAndTime* getHistory(User* user, MYSQL *con, MYSQL_RES *result){
    char query[1000];
    char messageHistory[1000] = "";
    sprintf(query,"SELECT locationName,time from mydb.location,mydb.check where mydb.location.idlocation = mydb.check.idlocation and idaccount = %s", user->idUser);
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
      for(int i = 0; i < num_fields; i++){
          if(row[i]!=NULL){
            strcat(messageHistory,row[i]);
            printf("%s ", row[i] ? row[i] : "NULL");
            strcat(messageHistory,"_");
          }
      }
      printf("\n");
    }
    sprintf(messageHistory,"%s",removeEnterCharacter(messageHistory));
    // printf("%s\n", messageHistory);
    LocationAndTime* locationAndTime = createLTEmpty();
    sprintf(locationAndTime->str,"%s",messageHistory);
    // printf("------------%s\n",locationAndTime->str);
    return locationAndTime;
}

void addInforUser(User *user,char* idaccount, MYSQL *con, MYSQL_RES *result){
    char query[1000];
    sprintf(query,"SELECT * FROM infor WHERE idUser = %s",idaccount);
    // printf("query: %s\n", query);
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

int getLocation(char codeLocation[45],MYSQL *con, MYSQL_RES *result){
    char *index;
    char query[1000];
    sprintf(query,"SELECT idLocation FROM location where qrCode = \'%s\'",codeLocation);
    if(mysql_query(con, query)){
        finish_with_error(con);
        return -1;
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
    return value;
}

bool insertLocationAndTime(LocationAndTime *locationAndTime, User *user, int codeLocation,MYSQL *con, MYSQL_RES *result){
    char query[1000];
    sprintf(query,"INSERT INTO mydb.check VALUES(%d,%s,\'%s\')", codeLocation, user->idUser, locationAndTime->time);
    printf("qery insert time and location: %s\n", query);
    if (mysql_query(con, query)){
        printf("INSERT LOCATION AND TIME ERROR!\n");
      finish_with_error(con);
      return false;
    }else{
        return true;
    }
}

bool insertFormDecalre(Formdeclare *formDeclare, MYSQL *con, MYSQL_RES *result){
    char query[1000];
    sprintf(query,"INSERT INTO mydb.formdeclare VALUES(%s,\'%s\',\'%s\',\'%s\',\'%s\',\'%s\',\'%s\',\'%s\',\'%s\',\'%s\')", formDeclare->idUser, formDeclare->numberOfVehicle, formDeclare->addressCome, formDeclare->addressTo, formDeclare->localDate, formDeclare->cityCome, formDeclare->cityTo, formDeclare->districtCome, formDeclare->districtTo, formDeclare->vehicle);
    printf("query insert formDeclare: %s\n", query);
    if (mysql_query(con, query)){
        printf("INSERT FORMDECLARE ERROR!\n");
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