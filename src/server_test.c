#include <sys/msg.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>

#define READY 1
#define  GOT_IT 0 
#define NOT_READY -1


void die(char *s)
{
  perror(s);
  exit(1);
}

typedef struct msgbuf
{
	long   mtype;
	int shm_key;
} msgbuf_t ;

typedef struct _mem_struct{
	int status;
	int data[4];
} mem_struct_t;


int main()
{
    	int msqid;
    	key_t key;
    	msgbuf_t rcvbuffer;

    	key = 1234;
	
	if ((msqid = msgget(key, 0666)) < 0)
		die("msgget()");


	//Receive an answer of message type 1.
	if (msgrcv(msqid, &rcvbuffer, sizeof(rcvbuffer), 1, 0) < 0)
		die("msgrcv");
	
	printf("Key value is= %d \n", rcvbuffer.shm_key);

	/**************** SHMMMMMMMM **************/

	int shm_id;
	key_t shm_key;
	mem_struct_t *load;

	/*
	* We need to get the segment named
	* "5678", created by the server.
	*/

	shm_key = rcvbuffer.shm_key;

	/*
	* Locate the segment.
	*/
	if ((shm_id = shmget(shm_key, sizeof(load), 0666)) < 0) {
		perror("shmget");
		exit(1);
	}

	/*
	* Now we attach the segment to our data space.
	*/
	if ((load = (mem_struct_t *) shmat(shm_id, NULL, 0)) == -1) {
		perror("shmat");
		exit(1);
	}

	/*
	* Now read what the server put in the memory.
	*/
	for (int i=0; i < 4; i++){
		load->data[i] = load->data[i]*2;
		//printf("Printing load[%d] \n" , load->data[i]);
	}

	load->status = GOT_IT;
	printf("Client has finished receiving data \n");
	exit(0); 




}
