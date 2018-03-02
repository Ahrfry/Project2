#include <sys/msg.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <pthread.h>
#include "helper.h"



main()
{
	
        
	int key = 1;
	
	pthread_t p_arr[10];
	for(int i=1; i<10; i++){
		key = i * 10;
		//create 5 threads
		pthread_t p;
		pthread_create(&p, NULL, async_service, key);
		p_arr[i-1] = p;
	}
	
	for(int i=0; i<10; i++){
		pthread_join(p_arr[i], NULL);
	}

	exit(0);

}
