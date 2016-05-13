#include "common.h"

//Anurag Kalra
//ID: 260631195

int fd;
Shared* shared_mem;
int ID = 0;
int duration = 1;

//CREATE SHARED MEMORY
int setup_shared_memory(){
    fd = shm_open(MY_SHM, O_RDWR, 0666);
    if(fd == -1){
        printf("shm_open() failure\n");
        exit(1);
    }
}




//ATTACH SHARED MEMORY
int attach_shared_memory(){
    shared_mem = (Shared*) mmap(NULL, sizeof(Shared), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if(shared_mem == MAP_FAILED){
        printf("mmap() failure\n");
        exit(1);
    }
    return 0;
}

//ADD JOB TO QUEUE
void put_a_job(Job* j){
    (*shared_mem).arr[shared_mem->data] = (*j);
    shared_mem->data += 1;
}

int main(int argc, char* argv[]) {

    //BASIC ARGUMENT HANDLING
	if(argc < 3){
		printf("USING DEFAULT CLIENT_id AND DURATION\n");
	}
	else{
		int tmpID = atoi(argv[1]);
		int tmpDur = atoi(argv[2]);

		if(tmpID < 1 || tmpDur < 1){
			printf("PARAMETERS FORMATTED INCORRECTLY\n NEED CLIEND ID > 0 AND DURATION > 0 \n Using default client ID of 0 and duration of 1\n");
		}
		else{
			ID = tmpID;
			duration = tmpDur;
			printf("Using id: %i and duration: %i\n", ID, duration);
		}
	}

    setup_shared_memory();
    attach_shared_memory();
    




    Job j = {ID, duration};
    

    sem_wait(&shared_mem->over);

    sem_wait(&shared_mem->binary);
    
    //ADDS JOB
    put_a_job(&j);
    printf("CLIENT JOB HAS BEEN ADDED TO BUFFER -       ID: %i,       dur: %i\n", j.clientID, j.duration);

    int lolC;
    int smh_iC;
    for (smh_iC = 1; smh_iC < 1500; smh_iC++){
        lolC = lolC + smh_iC;
    }

    sem_post(&shared_mem->under);


    sem_post(&shared_mem->binary);

    return 0;
}
