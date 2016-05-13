#ifndef _INCLUDE_COMMON_H_
#define _INCLUDE_COMMON_H_

//Anurag Kalra
//ID: 260631195

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>

// from `man shm_open`
#include <sys/mman.h>
#include <sys/stat.h>        /* For mode constants */
#include <fcntl.h>           /* For O_* constants */



#define MY_SHM "/tmp"

//THIS IS THE JOB STRUCT THAT HOLDS THE PRINT ID AND DURATION TIME
typedef struct {
	int clientID;
	int duration;
} Job;


//SHARED MEMORY STRUCT
typedef struct {
    sem_t binary;
    sem_t under;
    sem_t over;
    int data;
    int max;
    Job arr[0];
} Shared;

#endif //_INCLUDE_COMMON_H_

