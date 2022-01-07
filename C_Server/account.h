#ifndef __ACCOUNT_H__
#define __ACCOUNT_H__

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>

#define MAX_IDENT_LEN_NAME 50
#define MAX_IDENT_LEN_PASSWORD 30

typedef struct {
    char name[MAX_IDENT_LEN_NAME];
    char password[MAX_IDENT_LEN_PASSWORD];
} Account;

Account* createAccountEmpty(){
    Account* account = (Account*) malloc(sizeof(Account));
    return account;
}

#endif