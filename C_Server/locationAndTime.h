#ifndef __TIMELOCATION_H__
#define __TIMELOCATION_H__

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>

typedef struct{
    char location[200];
    char time[45];
    char str[1000];
}LocationAndTime;

LocationAndTime* createLTEmpty(){
    LocationAndTime* timeLocation = (LocationAndTime*) malloc(sizeof(LocationAndTime));
    return timeLocation;
}

#endif