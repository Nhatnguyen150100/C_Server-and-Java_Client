#ifndef __FORMDECLARE_H__
#define __FORMDECLARE_H__

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>


typedef struct{
    char idUser[10];
    char numberOfVehicle[20];
    char addressCome[255];
    char addressTo[255];
    char localDate[45];
    char cityCome[45];
    char cityTo[45];
    char districtCome[45];
    char districtTo[45];
    char vehicle[45];
}Formdeclare;


Formdeclare* createFormdeclareEmpty(){
    Formdeclare* formdeclare = (Formdeclare*) malloc(sizeof(Formdeclare));
    return formdeclare;
}


#endif