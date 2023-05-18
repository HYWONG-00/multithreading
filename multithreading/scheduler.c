#include <stdio.h>
#include <stdlib.h>
#include "scheduler.h"

int fcfs(int n,int arr[]){
	int i;
  	int mov;
  	mov = 0;
  	/* "next position"- "current position of the disk"*/
    	mov = mov + abs(arr[3]-arr[1]);
    	/*printf("mov: %d distance:%d\n",*mov,arr[3]-arr[1]);*/
    	
    	/*[5] - [4],[6]-[5]...[10]-[9]*/
    for(i = 4;i < n;i++)
    {	
    	/*printf("index:%d mov:%d distance:%d\n",i,*mov,abs(arr[i]-arr[i-1]));*/
        mov = mov + abs(arr[i]-arr[i-1]);
    }
   	return mov;
}

/*keep looping and find the closest cylinder from current position*/
int sstf(int n,int arr[]){	
	int min,closestIndex,count,currentPos,mov;
	/*Make a copy of an array as I dun want to change the original array*/
	int* tempArray,tempSize,i;
	tempSize = n - 3;
	tempArray = (int*)malloc(tempSize * sizeof(int));
	
	mov = 0;
	for(i = 0;i < tempSize;i++)
        {
           tempArray[i] = arr[i + 3];
        }
     
     	/*start your sstf algorithm*/
	count = 0;
	currentPos = arr[1];
    while(count < tempSize)/*size = 11, loop 8 times only*/
    {      
        min = MAX;
        findClosest(tempSize,tempArray,&min,&closestIndex,&currentPos);
        /*printf("%d -> ",currentPos);*/
        mov = mov + min;
        /*printf("min: %d\n",min);*/
        currentPos = tempArray[closestIndex];
        tempArray[closestIndex] = MAX;
        count++;
    }
    /*printf("%d -> ",currentPos);*/
    
    free(tempArray);
    
    return mov;
}

/*SSTF: find minimum distance (currentPos to its closest cylinder)*/
void findClosest(int n,int arr[],int* min, int* closestIndex,int* currentPos){
	int distance,i;
	
	for(i = 0;i < n;i++)
        {
           distance=abs(arr[i] - *currentPos);
           if(*min > distance)
           {
               *min = distance;
               *closestIndex = i;
           }     
        }
}

int scan(int n,int arr[]){

	/*make an temporary array for "scan" cuz easier*/
	int* tempArray,tempSize,i,currentPos,currentPosIndex;
	int mov;
  	mov = 0;
	tempArray = (int*)malloc((n-2) * sizeof(int));
	tempSize = n - 2;
	currentPos = arr[1];
	
	tempArray[0] = arr[1];/*insert current position into tempArray*/
	for(i = 1;i < tempSize;i++)
        {
           tempArray[i] = arr[i + 2];
        }
        
        /*sort the tempArray*/
        sort(tempSize,tempArray);
	
	/*find the index of current position in array*/
	i = 0;
	while(currentPos >= tempArray[i]){
	    currentPosIndex = i;
	    i++;
	}
	
	/*53 -> 37 -> 14*/
	for(i = currentPosIndex; i >= 0 ; i--)
        {
            /*printf("%d -> ",tempArray[i]);*/
            mov = mov + abs(currentPos - tempArray[i]);
            currentPos = tempArray[i];          
        }
        
        /*14 -> 0*/
        mov = mov + abs(currentPos - 0);
        currentPos = 0;
        /*printf("0 -> ");*/
        
        /*0 -> 65 -> 67 -> 98 -> 122 -> 124 -> 183*/
        for(i = currentPosIndex + 1; i < tempSize;i++)
        {
            /*printf("%d -> ",tempArray[i]);*/
            mov = mov + abs(currentPos - tempArray[i]);
            currentPos = tempArray[i];         
        }

	free(tempArray);
	
	return mov;
}

/*SCAN: sorting array*/
void sort(int size,int tempArray[]){
    int i,j,temp;
    for(i = 0;i < size;i++)
    {
        for(j = 0;j < size;j++)
        {
            if(tempArray[i] < tempArray[j])
            {
                temp = tempArray[i];
                tempArray[i] = tempArray[j];
                tempArray[j] = temp;
            }
        }
    }
    
    
}

/* 53 -> 37 -> 14 -> 0 -> 199 -> 183 -> 124 -> 122 -> 98 -> 67 -> 65*/
/*Diff with scan: need to move to the end of the disk(arr[0] - 1) after reach 0*/
int cscan(int n,int arr[]){
	/*make an temporary array for "cscan" cuz easier*/
	int* tempArray,tempSize,i,currentPos,currentPosIndex;
	int mov;
  	mov = 0;
	tempArray = (int*)malloc((n-2) * sizeof(int));
	tempSize = n - 2;
	currentPos = arr[1];
	
	tempArray[0] = arr[1];/*insert current position into tempArray*/
	for(i = 1;i < tempSize;i++)
        {
           tempArray[i] = arr[i + 2];
        }
        
        /*sort the tempArray*/
        sort(tempSize,tempArray);
	
	/*find the index of current position in array*/
	i = 0;
	while(currentPos >= tempArray[i]){
	    currentPosIndex = i;
	    i++;
	}
        
	/*53 -> 37 -> 14*/
	for(i = currentPosIndex; i >= 0 ; i--)
        {
            /*printf("%d -> ",tempArray[i]);*/
            mov = mov + abs(currentPos - tempArray[i]);
            /*printf("dis: %d\n",abs(currentPos - tempArray[i]));*/
            currentPos = tempArray[i];            
        }
       
        /*14 -> 0*/
        mov = mov + abs(currentPos - 0);
        currentPos = 0;
        /*printf("0 -> ");*/
         
        
        /*CHANGE FROM NOW ONWARDS (b4 this line all the same as scan)*/ 
        
        /*0 -> 199*/
        mov = mov + abs(currentPos - (arr[0] - 1));
        currentPos = arr[0] - 1;
        /*printf("%d -> ",arr[0] - 1);*/
              
        /*199 -> 183 -> 124 -> 122 -> 98 -> 67 -> 65*/
        for(i = tempSize - 1; i >= currentPosIndex + 1; i--)
        {
            /*printf("%d -> ",tempArray[i]);*/
            mov = mov + abs(currentPos - tempArray[i]);
            currentPos = tempArray[i];         
        }
        
	free(tempArray);
	
	return mov;
}

int look(int n,int arr[]){

	/*make an temporary array for "scan" cuz easier*/
	int* tempArray,tempSize,i,currentPos,currentPosIndex;
	int mov;
  	mov = 0;
	tempArray = (int*)malloc((n-2) * sizeof(int));
	tempSize = n - 2;
	currentPos = arr[1];
	
	tempArray[0] = arr[1];/*insert current position into tempArray*/
	for(i = 1;i < tempSize;i++)
        {
           tempArray[i] = arr[i + 2];
        }
        
        /*sort the tempArray*/
        sort(tempSize,tempArray);
	
	/*find the index of current position in array*/
	i = 0;
	while(currentPos >= tempArray[i]){
	    currentPosIndex = i;
	    i++;
	}
	
	/*53 -> 37 -> 14*/
	for(i = currentPosIndex; i >= 0 ; i--)
        {
            /*printf("%d -> ",tempArray[i]);*/
            mov = mov + abs(currentPos - tempArray[i]);
            currentPos = tempArray[i];          
        }
        
        /*14 -> 65 -> 67 -> 98 -> 122 -> 124 -> 183*/
        for(i = currentPosIndex + 1; i < tempSize;i++)
        {
            /*printf("%d -> ",tempArray[i]);*/
            mov = mov + abs(currentPos - tempArray[i]);
            currentPos = tempArray[i];         
        }
        
	free(tempArray);
	
	return mov;
}

int clook(int n,int arr[]){

	/*make an temporary array for "scan" cuz easier*/
	int* tempArray,tempSize,i,currentPos,currentPosIndex;
	int mov;
  	mov = 0;
	tempArray = (int*)malloc((n-2) * sizeof(int));
	tempSize = n - 2;
	currentPos = arr[1];
	
	tempArray[0] = arr[1];/*insert current position into tempArray*/
	for(i = 1;i < tempSize;i++)
        {
           tempArray[i] = arr[i + 2];
        }
        
        /*sort the tempArray*/
        sort(tempSize,tempArray);
	
	/*find the index of current position in array*/
	i = 0;
	while(currentPos >= tempArray[i]){
	    currentPosIndex = i;
	    i++;
	}
	
	/*53 -> 37 -> 14*/
	for(i = currentPosIndex; i >= 0 ; i--)
        {
            /*printf("%d -> ",tempArray[i]);*/
            mov = mov + abs(currentPos - tempArray[i]);
            currentPos = tempArray[i];          
        }
        
        /*14 -> 183 -> 124 -> 122 -> 98 -> 67 -> 65*/
        for(i = tempSize - 1; i >= currentPosIndex + 1; i--)
        {
            /*printf("%d -> ",tempArray[i]);*/
            mov = mov + abs(currentPos - tempArray[i]);
            currentPos = tempArray[i];         
        }
        
	free(tempArray);
	
	return mov;

}
