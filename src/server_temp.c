/*
 * shm-client - client program to demonstrate shared memory.
 */
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>

#define SHMSZ     27

#define READY 1
#define GOT_IT 0 
#define NOT_READY -1

typedef struct _mem_struct{
	int status;
	int data[4];
} mem_struct_t;

main()
{
	int shm_id;
	key_t key;
	mem_struct_t *load;

	/*
	* We need to get the segment named
	* "5678", created by the server.
	*/
	key = 5678;

	/*
	* Locate the segment.
	*/
	if ((shm_id = shmget(key, sizeof(load), 0666)) < 0) {
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
		printf("Printing load[%d] \n" , load->data[i]);
	}

	load->status = GOT_IT;
	printf("Client has finished receiving data \n");
	exit(0);
}
