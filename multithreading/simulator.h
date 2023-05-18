#include <pthread.h>

#ifndef SIMULATOR
#define SIMULATOR
	void createThread(pthread_t thread[],void* args);
	void writeBuffer2(int result, char* schedul_type);
	void* thread_A();
	void* thread_B();
	void* thread_C();
	void* thread_D();
	void* thread_E();
	void* thread_F();
	#define THREAD_SIZE 6
#endif

#ifndef STRUCT_HGUARD
#define STRUCT_HGUARD
    typedef struct
	{
		int n;/*array length*/
		int mov;/*(seek time)to store total number of head movements*/
		int* arr;
	}Args;
#endif
