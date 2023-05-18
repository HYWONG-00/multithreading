/*function prototype in header file*/

#ifndef SCHEDULER
#define SCHEDULER
	int fcfs(int n,int arr[]);
	int sstf(int n,int arr[]);
	void findClosest(int n,int arr[],int* min, int* closestIndex,int* currentPos);
	int scan(int n,int arr[]);
	void sort(int size,int tempArray[]);
	int cscan(int n,int arr[]);
	int look(int n,int arr[]);
	int clook(int n,int arr[]);
	#define MAX 1000000 /*impossible / maximum value for the cylinder numbers*/	
#endif


#ifndef HGUARD/*If the HGUARD is not defined*/
#define HGUARD/*then u define it*/
    #define FALSE 0 /*This is boolean.False is 0*/
    #define TRUE !FALSE /*True is 1*/
#endif/*This is end of header guard*/


