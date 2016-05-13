#include "common.h"
#include <unistd.h>

//Anurag Kalra
//ID: 260631195

int fd;
int errno;
int MY_LEN = 5;
Shared* shared_mem;

static int keepRunning = 1;

//PORTIONS OF EXAMPLE CODE TAKEN FROM SLIDES


//CREATES SHARED MEMORY SPACE
int setup_shared_memory(){
    fd = shm_open(MY_SHM, O_CREAT | O_RDWR, 0666);
    if(fd == -1){
        printf("shm_open() failure");
        exit(1);
    }
    ftruncate(fd, sizeof(Shared) + MY_LEN*sizeof(int));
}

//ATTACH SHARED MEMORY
int attach_shared_memory(){
    shared_mem = (Shared*)  mmap(NULL, sizeof(Shared) + MY_LEN*sizeof(Job), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if(shared_mem == MAP_FAILED){
        printf("mmap() failure\n");
        exit(1);
    }
    return 0;
}

//INITIALIZE SHARED MEMORY
int init_shared_memory() {
	//The data variable will keep track of the current length of the queue
    shared_mem->data = 0;
    //The max variable will hold the queue's max length
    shared_mem->max = MY_LEN;
    //Initialise the semaphores for accessing shared memory, underflow and overflow
    sem_init(&(shared_mem->binary), 1, 1);
    sem_init(&(shared_mem->under), 1, 0);
    sem_init(&(shared_mem->over), 1, 0);

    //Offset the overflow semaphore by the amount of space we have for the queue
    int i;
    for(i = 0; i < MY_LEN; i++){
    	sem_post(&shared_mem->over);
    }
}




//GIVEN JOB STRUCT, PRINT FOR SPECIFIED TIME
void go_to_sleep(Job* j){
    sleep((*j).duration);
    printf("Finished printing: CLIENT_NUMBER %i, %i pages.\n", (*j).clientID, (*j).duration);
}





//GIVEN JOB STRUCT, PRINT PAGES <simulation>
void print_a_msg(Job* j){
    printf("CLIENT_NUMBER %i has %i pages to print...\n", (*j).clientID, (*j).duration);
}



//THIS DEQUEUE METHOD IS NECESSERAY TO REMOVE FRONTMOST ELEMENT IN THE QUEUE
void dequeue(){
    int c;
    for(c = 0; c < shared_mem->data; c++){
        shared_mem->arr[c] = shared_mem->arr[c+1];
    }
    shared_mem->data--;
}

int main(int argc, char* argv[]) {



	//BASIC EXCEPTION HANDLING
    if(argc < 2){
        printf("Default buffer size: 5\n");
        //SETS UP DEFAULT
    }
    else{
        int tmp = atoi(argv[1]);
        if(tmp > 0){
            MY_LEN = tmp;
        }
        else{
            printf("INVALID LENGTH: using default buffer length of 5\n");
        }
    }

    int lol;
    int smh_i;
    for (smh_i = 1; smh_i<1000; smh_i++){
    	lol = lol + smh_i;
    }

    setup_shared_memory();
    attach_shared_memory();
    init_shared_memory();

    int i;
    for (i=0; i < MY_LEN; i++) {
        Job JOKEJOB = {1, 1};
        shared_mem->arr[i] = JOKEJOB;
        printf("set %d\n", i);
    }

    //Infinite loop
    while (keepRunning == 1) {
        
        sem_wait(&shared_mem->under);

        sem_wait(&shared_mem->binary);

        Job tmpJob = shared_mem->arr[0];






        //THIS ACTUALLY DOES THE PROPER PRINTING IN THE BUFFER
        int i;
        printf("\nCurrent buffer:\n");
        printf(">>>>>>>>>>>\n\n");
        for(i = 0; i < shared_mem->data; i++){
            printf("> Job: %i,		ID: %i,		dur: %i\n", i, shared_mem->arr[i].clientID, shared_mem->arr[i].duration);
        }
        printf(">>>>>>>>>>>\n\n");

        //PRINT AND DEQUEUE
        print_a_msg(&tmpJob);
        go_to_sleep(&tmpJob);
        dequeue();

        //PRINT ALERT WHEN EMPTY
        if(shared_mem->data == 0){
            printf("\nBuffer has emptied. Waiting...\n\n");
        }



        sem_post(&shared_mem->binary);
        sem_post(&shared_mem->over);
        

    }

    return 0;
}

