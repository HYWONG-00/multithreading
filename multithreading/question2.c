#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "scheduler.h"
#include "simulator.h"
#include "question2.h"

char* filename;
pthread_cond_t condFilename;
pthread_mutex_t mutexFilename;/* mutex for synchronization*/

pthread_mutex_t mutexBuffer1;/* mutex for synchronization*/
pthread_cond_t canconsume1;/*all consumer waits on this condition variable()*/
pthread_cond_t canproduce1;/*all producer waits on this condition variable()*/
int num;/*number of element in the buffer 1, min = 0, max = after reading file, arraySize*/
int* buffer1;/*for reading by all child thread*/
int arraySize;

int num2;/*number of element in the buffer 2, min = 0, max = 1*/
char* schedul_name;
int* buffer2;/*for writing the seek time by all child thread*/
pthread_mutex_t mutexBuffer2;/* mutex for synchronization*/
pthread_cond_t canconsume2;/*all consumer waits on this condition variable()*/
pthread_cond_t canproduce2;/*all producer waits on this condition variable()*/

int main(int argc, char* argv[])
{
	int i,j;
    FILE* sPtr;
    Args* func_args;
	char* token;
	char line[50], line1[50],line2[50];
    char** printResult;/*for printing the result in order*/
    
    /*for pthreads*/
    pthread_t thread[THREAD_SIZE];
    
    /*MUST initialize mutex lock and condition variable*/
    pthread_mutex_init(&mutexBuffer1, NULL);
    pthread_cond_init(&canconsume1, NULL);
    pthread_cond_init(&canproduce1, NULL);
    
    pthread_mutex_init(&mutexBuffer2, NULL);
    pthread_cond_init(&canconsume2, NULL);
    pthread_cond_init(&canproduce2, NULL);
 
 	filename = (char*)malloc(20 * sizeof(char));
	schedul_name = (char*)malloc(20 * sizeof(char));
	
	printResult = (char**)malloc(THREAD_SIZE * sizeof(char*));
	for(i = 0; i < THREAD_SIZE ; i++)
	{
		printResult[i] = (char*)malloc(20 * sizeof(char));
	}
	
    do
    {	
   		filename[0] = '\0';/*filename is null*/
    		num = 0; /*initially buffer1 have no element*/
    		num2 = 0; /*initially buffer2 have no element*/
		arraySize = 0;
		buffer1 = NULL;
		buffer2 = (int*)malloc(sizeof(int));/*bounded buffer with size 1*/
		sPtr = NULL;
    	
    	
    	func_args = (Args*)malloc(sizeof(Args));
		func_args->n = 20;
		func_args->mov = 0;
		func_args->arr = buffer1;
		
    	/*create the threads first*/
		createThread(thread,(void*)func_args);
		
		/*read the filename from the user*/
		printf("Enter your filename: ");
		scanf("%s",filename);
		
		
		/*if user wants to quit, make sure terminate all of the thread and print the id of the child threads*/
		if(strcmp(filename,"QUIT") == 0)
		{
    		/*join(terminate) all threads*/		
    		for(j = 0; j < THREAD_SIZE ; j++){
				if(pthread_join(thread[j],NULL) == 0){
					printf("Thread %ld  has terminated.\n",thread[j]);				
				}	
    		}
    		
    		/*then terminate itself*/
    		pthread_exit(0);/*the thread won't continue execute the below code, but it will let the child threads done their execution then terminate the child thread*/
		}
		else
		{
			/*open the text file*/
			sPtr = fopen(filename,"r");
			
			/*if sPtr == NULL means sPtr does not points to input.txt*/
			if(sPtr == NULL)
			{
				/*print error */
				perror("Could not open files");
			}
			else
			{	
				
				/*EDIT arraySize here as we only write arraySize once(b4 any process read it), allow many reader to access arraySize at the same time(just get value only ma)*/
				/*read the line from the file*/
				fgets(line,sizeof(line),sPtr);
				
				strcpy(line1,line);/*copy "line" to "line1"*/
				
				
				/*Just to get the size(for initializing the array)*/
				token = strtok(line," ");
				while(token != NULL){
					arraySize++;
					token = strtok(NULL," ");		
				}
				
				
				/*reading the buffer 1: they have been created at the beginning there*/
				
				/*writing the buffer 1*/
				
				j = 0;
				while(j < THREAD_SIZE){/*I will only loop 6 times, for that 6 threads*/
					pthread_mutex_lock(&mutexBuffer1);
					
					/*num == arraySize means buffer 1 is full*/
					while(num == arraySize){
						pthread_cond_wait(&canproduce1, &mutexBuffer1);/**/
					}
					strcpy(line2,line1);/*copy "line" to "line1"*/
					
					/*if buffer is not full yet, do writing to buffer 1*/
					writingBuffer1(line2);
					
					num = arraySize;
					
					/*for(i = 0; i < arraySize ; i++){
						printf ("producer: inserted %d\n",buffer1[i]);  
					
					}*/
					
					j++;
					pthread_mutex_unlock(&mutexBuffer1);/*done writing*/
					pthread_cond_signal(&canconsume1);
					
					/*after that, parent thread blocks while waiting for results from child threads*/
					readBuffer2(printResult);
					
				}
				/*print the result*/
				printf ("For %s:\n",filename);  
				for(i = 0; i < THREAD_SIZE ; i++)
				{
					printf("%s\n",printResult[i]);
				}
							
				/*close the file*/
				fclose(sPtr);
				free(buffer1);
				free(func_args);
				free(buffer2);
			}
		}
		
	}while(strcmp(filename,"QUIT") != 0);
	
	
	free(filename);
	for(i = 0; i < THREAD_SIZE ; i++)
	{
		free(printResult[i]);
	}
	free(printResult);
	/*destroy the mutex lock and condition variable */   	
	pthread_mutex_destroy(&mutexBuffer1);
   	pthread_cond_destroy(&canconsume1);
   	pthread_cond_destroy(&canproduce1);
   	
   	pthread_mutex_destroy(&mutexBuffer2);
   	pthread_cond_destroy(&canconsume2);
   	pthread_cond_destroy(&canproduce2);
	return 0;
}

void writingBuffer1(char* line){
	char* token;
	int i;
	
	i = 0;
	token = NULL;
	
	/*printf("Writing buffer 1...\n");*/
		
	/*writing the buffer1*/
	buffer1 = (int*)malloc(arraySize * sizeof(int));
				
	/*Extract the first token*/		
	token = strtok(line," ");
	buffer1[i] = atoi(token);
	/* loop through the string to extract all other tokens*/
	while(token != NULL){/*&buffer1[i]*/
		i++;
		token = strtok(NULL," ");
		/*store the token only when it is not null(when it is last integer in the line, the next token will be NULL so we need checking)*/
		if(token != NULL){
			buffer1[i] = atoi(token);
		}
	}
}

void readBuffer2(char** printResult){

	pthread_mutex_lock(&mutexBuffer2);
		
	/*wait when there is no element in buffer*/
	while(num2 == 0){		
		pthread_cond_wait(&canconsume2, &mutexBuffer2);
	}
	
	/*reading buffer2*/
	/*printf("%s: %d\n",schedul_name,*buffer2);*/
	storeResult(printResult);
	
	*buffer2 = -1; /*-1 means buffer2 is empty now*/
	num2 = 0;

	/*printf ("Consume buffer2: buffer2 is now empty\n"); */
	pthread_mutex_unlock(&mutexBuffer2);/*done reading*/
	pthread_cond_signal(&canproduce2);/*producer can produce now*/
}

/*to store the result into a char** array then print the result in order(in main function)*/
void storeResult(char** printResult){
	char* line;
	char text[5];
	line = (char*)malloc(20 * sizeof(char));
	
	/*schedul_name + ":" + *buffer2;*/
	sprintf(text, "%d", *buffer2);
	
	strcpy(line, schedul_name);
	strcat(line, " :");
    strcat(line, text);
	
	if(strcmp(schedul_name,"FCFS\0") == 0){
		strcpy(printResult[0],line);
	}
	else if(strcmp(schedul_name,"SSTF\0") == 0){
		strcpy(printResult[1],line);
	}
	else if(strcmp(schedul_name,"SCAN\0") == 0){
		strcpy(printResult[2],line);
	}
	else if(strcmp(schedul_name,"CSCAN\0") == 0){
		strcpy(printResult[3],line);
	}
	else if(strcmp(schedul_name,"LOOK\0") == 0){
		strcpy(printResult[4],line);
	}
	else if(strcmp(schedul_name,"CLOOK\0") == 0){
		strcpy(printResult[5],line);
	}
	
	free(line);
}




