#include <sys/msg.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include "helper.h"

void die(char *s)
{
	perror(s);
  	exit(1);
}

main()
{
	char c;
	int shm_id;
	key_t shm_key;
	mem_struct_t *load;

	/*
	* We'll name our shared memory segment
	* "5678".
	*/
	shm_key = 5678;

	/*
	* Create the segment.
	*/
	if ((shm_id = shmget(shm_key, sizeof(load), IPC_CREAT | 0666)) < 0) {
		perror("shmget");
		exit(1);
	}

	/*
	* Now we attach the segment to our data space.
	*/
	if ((load = shmat(shm_id, NULL, 0)) == -1) {
		perror("shmat");
		exit(1);
	}

	/*
	* Now put some things into the memory for the
	* other process to read.
	*/
	
	load->status = NOT_READY;
	
	load->data[0] = 1;
	load->data[1] = 2;
	load->data[2] = 3;
	load->data[3] = 4;
	
	
	load->status = READY;
	
	/****************MESSAGE**********************/	
	
	int msqid;
	int msgflg = IPC_CREAT | 0666;
    	key_t msg_key;
    	msgbuf_t sbuf;
    	size_t buflen;

    	msg_key = 1234;

    	if ((msqid = msgget(msg_key, msgflg )) < 0){
    		die("msgget");
	}

    	//Message Type
    	sbuf.mtype = 1;
	sbuf.shm_key = shm_key;

    	if (msgsnd(msqid, &sbuf, sizeof(sbuf), IPC_NOWAIT) < 0)
    	{
        	die("msgsnd");
    	}

    	else
        	printf("Message Sent\n");
	
	while(load->status != GOT_IT){
		printf("waiting for server to get load \n");
		sleep(1);
	}
	
	for(int i=0; i<4; i++){
		printf("Pring load[%d] \n" , load->data[i]);
	}
		
	printf("Client detected that server has received the data \n");
	shmdt((void *) load);
	shmctl(shm_id , IPC_RMID , NULL);
	printf("Client has detached itself from memory \n");
		
	exit(0);

}
