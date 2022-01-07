#ifndef __LOCATIONOFTUDE_H__
#define __LOCATIONOFTUDE_H__

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>

typedef struct{
    char location[200];
    char longitude[45];
    char latitude[45];
    char str[1000];
}LocationOftude;

LocationOftude* createLOTEmpty(){
    LocationOftude* locationOftude = (LocationOftude*) malloc(sizeof(LocationOftude));
    return locationOftude;
}

#endif