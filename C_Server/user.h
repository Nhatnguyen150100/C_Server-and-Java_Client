#ifndef __USER_H__
#define __USER_H__

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>


typedef struct{
    char idUser[10];
    char firstName[255];
    char lastName[255];
    char cardId[45];
    char birthday[45];
    char gender[45];
    char numberPhone[45];
    char address[45];
    char email[45];
    char state[45];
}User;

// User createUser(char* idUser, char* firstName, char* lastName, char* cardId, char* gender, char* numberPhone, char* address, char* email, char* state){
//     User user = (User) malloc(sizeof(User));
//     user.idUser = idUser;
//     user.firstName = firstName;
//     user.lastName = lastName;
//     user.cardId = cardId;
//     user.birthday = birthday;
//     user.gender = gender;
//     user.numberPhone = numberPhone;
//     user.address = address;
//     user.email = email;
//     user.state = state;
//     return user;
// }

User* createUserEmpty(){
    User*user = (User*) malloc(sizeof(User));
    return user;
}

User* createUserF1(){
    User* user;
    strcpy(user->idUser,"-1");
    return user;
}

#endif