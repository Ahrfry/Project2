#ifndef __HELPER_H
#define __HELPER_H

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>

#define READY 1
#define  GOT_IT 0 
#define NOT_READY -1

typedef struct msgbuf
{
	long   mtype;
	int shm_key;
} msgbuf_t ;

typedef struct _mem_struct{
	int status;
	int data[4];
} mem_struct_t;

#endif
