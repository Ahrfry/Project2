#ifndef __HELPER_H
#define __HELPER_H

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>

#define READY 1
#define  GOT_IT 0 
#define NOT_READY -1
#define Q_SIZE 2


typedef struct msgbuf
{
	long   mtype;
	int shm_key;
	int shm_keys[4];
} msgbuf_t ;

typedef struct _mem_struct{
	int status;
	int data[4];
	int shm_key;
	int client_id;
} mem_struct_t;

void die(char *s){
	perror(s);
  	exit(1);
}

init_arr(int A[]){
	for(int i=0 ; i<4; i++){
		A[i] = i;
	}
}

void *sync_service(void *args){
	
	char c;
	int shm_id;
	key_t shm_key;
	mem_struct_t *load;
	
	int msqid;
	int msgflg = IPC_CREAT | 0666;
    	key_t msg_key;
    	msgbuf_t sbuf;
    	size_t buflen;

	// Shared memory ID
	shm_key = (int *) args;

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
	
	init_arr(load->data);	
	
	
	load->status = READY;
	
	/****************MESSAGE**********************/	
	
	

	msg_key = 1234;

	if ((msqid = msgget(msg_key, msgflg )) < 0){
		die("msgget");
	}

	//Message Type
	sbuf.mtype = (int *) args;
	sbuf.shm_key = shm_key;

	if (msgsnd(msqid, &sbuf, sizeof(sbuf), IPC_NOWAIT) < 0){
		die("msgsnd");
	}else{
		printf("Message Sent\n");
	}

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
}

	

void *monitor(void *arg){
	mem_struct_t *load = (mem_struct_t *) arg;
	int shm_id;
	while(load->status != GOT_IT){
		//printf("Client ID %d waiting for server to get load \n", load->client_id);
		sleep(1);
	}
	
	for(int i=0; i<4; i++){
		printf("Pring load[%d] \n" , load->data[i]);
	}
	
		
	//locate segment to be removed
	if ((shm_id = shmget(load->shm_key, sizeof(load), 0666)) < 0) {
			perror("shmget");
			exit(1);
	}	
	
	printf("Client detected that server has received the data of type %d \n" , load->client_id);
	shmdt((void *) load);
	shmctl(shm_id , IPC_RMID , NULL);
	printf("Client has detached itself from memory \n");

	return;
}

void *async_service(void *args){
	
	char c;
	int shm_id;
	key_t shm_key;
	mem_struct_t *load;
	
	int msqid;
	int msgflg = IPC_CREAT | 0666;
    	key_t msg_key;
    	msgbuf_t sbuf;
    	size_t buflen;
	pthread_t p_arr[4];
	// Shared memory ID
	shm_key = (int *) args;
	for(int i = 0; i<4; i++){
		/*
		* Create the segment.
		*/
		printf("About to get shm \n");
		if ((shm_id = shmget(shm_key, sizeof(load), IPC_CREAT | 0666)) < 0) {
			perror("shmget");
			exit(1);
		}

		/*
		* Now we attach the segment to our data space.
		*/
		printf("About to attach \n");

		if ((load = shmat(shm_id, NULL, 0)) == -1) {
			perror("shmat");
			exit(1);
		}
		
		load->status = NOT_READY;
		
		init_arr(load->data);	
		load->shm_key = shm_key;	
		load->client_id = (int *) args;	
		load->status = READY;
		
		/****************MESSAGE**********************/	
		
		

		msg_key = 1234;

		if ((msqid = msgget(msg_key, msgflg )) < 0){
			die("msgget");
		}

		//Message Type
		sbuf.mtype = (int *) args;
		printf("Client ID  %d \n" , sbuf.mtype);
		sbuf.shm_key = shm_key;
		printf("About to send message \n");
		if (msgsnd(msqid, &sbuf, sizeof(sbuf), IPC_NOWAIT) < 0){
			die("msgsnd");
		}else{
			printf("Message Sent\n");
		}

		pthread_t p;
		p_arr[i] = p;
		pthread_create(&p, NULL, monitor, load);
		shm_key++; 
		
	}
	for(int i=0; i<4; i++){
		
		pthread_join(p_arr[i], NULL);
	}
}


#endif
