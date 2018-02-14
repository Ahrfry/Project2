#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>

#define READY 1
#define  GOT_IT 0 
#define NOT_READY -1

extern typedef struct _mem_struct{
	int status;
	int data[SIZE][SIZE];
} mem_struct_t;


