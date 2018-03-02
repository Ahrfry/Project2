#include <sys/msg.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <stdio.h>
#include <pthread.h>
#include "helper.h"
#include "queue.h"

#define NUM_OF_CLIENTS 9

#define READY 1
#define  GOT_IT 0 
#define NOT_READY -1


void queue_init(int queue[]){
	
	queue[0] = 30;
	queue[1] = 90;
	queue[2] = 40;
	queue[3] = 20;
	queue[4] = 50;
	queue[5] = 70;
	queue[6] = 60;
	queue[7] = 80; 
	queue[8] = 10; 
}


int main()
{
	int queue[4];	
	int counter = 0;
	queue_init(queue);	
	/*message vars*/	
	int msqid;
    	key_t key;
    	msgbuf_t rcvbuffer;
	
	/*shared mem vars*/
	int shm_id;
	key_t shm_key;
	mem_struct_t *load;
	
	
	int flag;	
    	key = 1234;
	while(1){	
		
		flag = queue[counter];	
		sleep(1);
		if ((msqid = msgget(key, 0666)) < 0)
			die("msgget()");

		printf("message type %d \n" , counter);
		//Receive an answer of message type that has credit.
		if (msgrcv(msqid, &rcvbuffer, sizeof(rcvbuffer), flag , IPC_NOWAIT) == -1){
			printf("msgrcv didnt die \n");
		}else{
		
					

			/**************** SHMMMMMMMM **************/

			

			shm_key = rcvbuffer.shm_key;

			if ((shm_id = shmget(shm_key, sizeof(load), 0666)) < 0) {
				perror("shmget");
				//exit(1);
			}

			if ((load = (mem_struct_t *) shmat(shm_id, NULL, 0)) == -1) {
				perror("shmat");
				//exit(1);
			}

			for (int i=0; i < 4; i++){
				load->data[i] = load->data[i]*2;
				//printf("Printing load[%d] \n" , load->data[i]);
			}

			load->status = GOT_IT;
			printf("Client has finished receiving data \n");
			shmdt(load);

		}

		if(counter >= 8){
			counter = 0;
		}else{
			counter++;
		}
		
	}
	exit(0); 




}
