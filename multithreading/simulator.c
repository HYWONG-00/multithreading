#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "scheduler.h"
#include "simulator.h"

extern char* filename;
extern pthread_cond_t condFilename;
extern pthread_mutex_t mutexFilename;/* mutex for synchronization*/

extern int num;
extern pthread_cond_t canconsume1;/*all consumer waits on this condition variable()*/
extern pthread_cond_t canproduce1;/*all producer waits on this condition variable()*/
extern pthread_mutex_t mutexBuffer1;/* mutex for synchronization*/


extern int* buffer1;/*for reading by all child thread*/
extern int arraySize;

/*buffer2: value == -1 means it is empty*/
extern int num2;
extern char* schedul_name;
extern int* buffer2;/*for writing the seek time by all child thread*/
extern pthread_mutex_t mutexBuffer2;/* mutex for synchronization*/
extern pthread_cond_t canconsume2;/*all consumer waits on this condition variable()*/
extern pthread_cond_t canproduce2;/*all producer waits on this condition variable()*/


void createThread(pthread_t thread[],void* args){


/*address of thread, NULL = default behaviour, pointer to function,argument passed to the function(MUST BE IN void*)*/
	/*initialize the thread FIRST*/
	if(pthread_create(&thread[0],NULL, &thread_A,NULL) != 0){
		perror("Thread creation failed");
	}
	
	if(pthread_create(&thread[1],NULL, &thread_B,NULL) != 0){
		perror("Thread creation failed");
	}
	if(pthread_create(&thread[2],NULL, &thread_C,NULL) != 0){
		perror("Thread creation failed");
	}
	if(pthread_create(&thread[3],NULL, &thread_D,NULL) != 0){
		perror("Thread creation failed");
	}
	if(pthread_create(&thread[4],NULL, &thread_E,NULL) != 0){
		perror("Thread creation failed");
	}
	if(pthread_create(&thread[5],NULL, &thread_F,NULL) != 0){
		perror("Thread creation failed");
	}
	
	/*
	for(j = 0; j < THREAD_SIZE ; j++){
		printf("in create method: Thread %d (ID: %ld) .\n",j,thread[j]);	
    	}*/
}

void writeBuffer2(int result, char* schedul_type){
	
	pthread_mutex_lock(&mutexBuffer2);
						
	/*num2 == 1 means buffer 2 is full*/
	while(num2 == 1){
		pthread_cond_wait(&canproduce2, &mutexBuffer2);
	}
	/*printf("Writing buffer 2...\n");*/

	/*if buffer 2 is empty,then the child thread can write the result to buffer 2*/	
	strcpy(schedul_name,schedul_type);
	*buffer2 = result;

	num2 = 1;

	pthread_mutex_unlock(&mutexBuffer2);/*done writing*/
	pthread_cond_signal(&canconsume2);
	

}

/*describe an operation of thread_A (after create, keep looping until user enter "QUIT", need to wait for reading buffer1, then call the function get the result,then check is we want to end the stop, if haven't stop, go back n wait for the buffer 1 again)*/
void* thread_A(){
	int result;
	char* name;
	name = "FCFS\0";
	/*1. terminate itself when the user enter "QUIT" so that we can join the thread in main*/
	while(filename[0] == '\0')
	{}
	if(strcmp(filename,"QUIT") == 0)
	{
		/*printf("USER ENTER %s, TERMINATE MYSELF : A\n",filename);*/
		pthread_exit(0);
	}
	

	/*2.consume buffer 1*/
	pthread_mutex_lock(&mutexBuffer1);
	
	/*wait when there is no element in buffer / when buffer is still null*/
	while(num == 0 || buffer1 == NULL){		
		pthread_cond_wait(&canconsume1, &mutexBuffer1);
	}
	
	/*start yr reading buffer 1*/
	/*n do calculation*/
	result = fcfs(arraySize,buffer1);
	
	/*(process synchronization will be done in the method)
	3. write the result / seek time into buffer 2*/
	writeBuffer2(result,name);
	
	
	buffer1 = NULL; /*later on will be malloc again when the program read the file?*/
	num = 0;
	
	/*printf ("Consume buffer1: buffer1 now is null\n");*/
	pthread_mutex_unlock(&mutexBuffer1);/*done reading*/
	pthread_cond_signal(&canproduce1);/*producer can produce now*/

	return NULL;
}

void* thread_B(){
	int result;
	char* name;
	name = "SSTF\0";
	/*1. terminate itself when the user enter "QUIT" so that we can join the thread in main*/
	while(filename[0] == '\0')
	{}
	if(strcmp(filename,"QUIT") == 0)
	{
		/*printf("USER ENTER %s, TERMINATE MYSELF : B\n",filename);*/
		pthread_exit(0);
	}
	

	/*2.consume buffer 1*/
	pthread_mutex_lock(&mutexBuffer1);
	
	/*wait when there is no element in buffer / when buffer is still null*/
	while(num == 0 || buffer1 == NULL){		
		pthread_cond_wait(&canconsume1, &mutexBuffer1);
	}
	
	/*start yr reading buffer 1*/
	/*n do calculation*/
	result = sstf(arraySize,buffer1);
	
	/*(process synchronization will be done in the method)
	3. write the result / seek time into buffer 2*/
	writeBuffer2(result,name);
	
	buffer1 = NULL; /*later on will be malloc again when the program read the file?*/
	num = 0;
	
	/*printf ("Consume buffer1: buffer1 now is null\n");*/ 
	pthread_mutex_unlock(&mutexBuffer1);/*done reading*/
	pthread_cond_signal(&canproduce1);/*producer can produce now*/

	return NULL;
}

void* thread_C(){
	int result;
	char* name;
	name = "SCAN\0";
	/*1. terminate itself when the user enter "QUIT" so that we can join the thread in main*/
	while(filename[0] == '\0')
	{}
	if(strcmp(filename,"QUIT") == 0)
	{
		/*printf("USER ENTER %s, TERMINATE MYSELF : C\n",filename);*/
		pthread_exit(0);
	}
	

	/*2.consume buffer 1*/
	pthread_mutex_lock(&mutexBuffer1);
	
	/*wait when there is no element in buffer / when buffer is still null*/
	while(num == 0 || buffer1 == NULL){		
		pthread_cond_wait(&canconsume1, &mutexBuffer1);
	}
	
	/*start yr reading buffer 1*/
	/*n do calculation*/
	result = scan(arraySize,buffer1);
	
	/*(process synchronization will be done in the method)
	3. write the result / seek time into buffer 2*/
	writeBuffer2(result,name);
	
	buffer1 = NULL; /*later on will be malloc again when the program read the file?*/
	num = 0;
	
	/*printf ("Consume buffer1: buffer1 now is null\n");*/
	pthread_mutex_unlock(&mutexBuffer1);/*done reading*/
	pthread_cond_signal(&canproduce1);/*producer can produce now*/

	return NULL;
}

void* thread_D(){
	int result;
	char* name;
	name = "CSCAN\0";
	/*1. terminate itself when the user enter "QUIT" so that we can join the thread in main*/
	while(filename[0] == '\0')
	{}
	if(strcmp(filename,"QUIT") == 0)
	{
		/*printf("USER ENTER %s, TERMINATE MYSELF : D\n",filename);*/
		pthread_exit(0);
	}
	

	/*2.consume buffer 1*/
	pthread_mutex_lock(&mutexBuffer1);
	
	/*wait when there is no element in buffer / when buffer is still null*/
	while(num == 0 || buffer1 == NULL){		
		pthread_cond_wait(&canconsume1, &mutexBuffer1);
	}
	
	/*start yr reading buffer 1*/
	/*n do calculation*/
	result = cscan(arraySize,buffer1);
	
	/*(process synchronization will be done in the method)
	3. write the result / seek time into buffer 2*/
	writeBuffer2(result, name);
	
	buffer1 = NULL; /*later on will be malloc again when the program read the file?*/
	num = 0;
	
	/*printf ("Consume buffer1: buffer1 now is null\n");*/
	pthread_mutex_unlock(&mutexBuffer1);/*done reading*/
	pthread_cond_signal(&canproduce1);/*producer can produce now*/

	return NULL;
}

void* thread_E(){
	int result;
	char* name;
	name = "LOOK\0";
	/*1. terminate itself when the user enter "QUIT" so that we can join the thread in main*/
	while(filename[0] == '\0')
	{}
	if(strcmp(filename,"QUIT") == 0)
	{
		/*printf("USER ENTER %s, TERMINATE MYSELF : E\n",filename);*/
		pthread_exit(0);
	}
	

	/*2.consume buffer 1*/
	pthread_mutex_lock(&mutexBuffer1);
	
	/*wait when there is no element in buffer / when buffer is still null*/
	while(num == 0 || buffer1 == NULL){		
		pthread_cond_wait(&canconsume1, &mutexBuffer1);
	}
	
	/*start yr reading buffer 1*/
	/*n do calculation*/
	result = look(arraySize,buffer1);
	
	/*(process synchronization will be done in the method)
	3. write the result / seek time into buffer 2*/
	writeBuffer2(result, name);
	
	buffer1 = NULL; /*later on will be malloc again when the program read the file?*/
	num = 0;
	
	/*printf ("Consume buffer1: buffer1 now is null\n");*/
	pthread_mutex_unlock(&mutexBuffer1);/*done reading*/
	pthread_cond_signal(&canproduce1);/*producer can produce now*/

	return NULL;
}

void* thread_F(){
	int result;
	char* name;
	name = "CLOOK\0";
	/*1. terminate itself when the user enter "QUIT" so that we can join the thread in main*/
	while(filename[0] == '\0')
	{}
	if(strcmp(filename,"QUIT") == 0)
	{
		/*printf("USER ENTER %s, TERMINATE MYSELF : F\n",filename);*/
		pthread_exit(0);
	}
	

	/*2.consume buffer 1*/
	pthread_mutex_lock(&mutexBuffer1);
	
	/*wait when there is no element in buffer / when buffer is still null*/
	while(num == 0 || buffer1 == NULL){		
		pthread_cond_wait(&canconsume1, &mutexBuffer1);
	}
	
	/*start yr reading buffer 1*/
	/*n do calculation*/
	result = clook(arraySize,buffer1);
	
	/*(process synchronization will be done in the method)
	3. write the result / seek time into buffer 2*/
	writeBuffer2(result, name);
	
	buffer1 = NULL; /*later on will be malloc again when the program read the file?*/
	num = 0;
	
	/*printf ("Consume buffer1: buffer1 now is null\n");*/
	pthread_mutex_unlock(&mutexBuffer1);/*done reading*/
	pthread_cond_signal(&canproduce1);/*producer can produce now*/

	return NULL;
}
