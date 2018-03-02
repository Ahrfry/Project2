#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include "matrix.h"


void matrix_init(int M[M_SIZE][M_SIZE])
{
	int i,j;
	for(i = 0; i < M_SIZE; i++){
		for( j = 0; j < M_SIZE; j++)
		{
			M[i][j] = 1;
		}
	}
	return;
}

void matrix_struct_init(matrix_t *matrices){
	matrix_init(matrices->A);
	matrix_init(matrices->B);
	matrix_init(matrices->C);
	

}


void matrix_mult(matrix_t *m_struct){
	
	int i , j, k, sum;	
	for (i = 0; i < M_SIZE; i++)
    	{
		for (j = 0; j < M_SIZE; j++)
		{
			sum = 0;
		    
			for (k = 0; k < M_SIZE; k++){
				sum += m_struct->A[i][k]*m_struct->B[k][j];
			}
			
			m_struct->C[i][j] = sum;	
		}
    	}

}

void print_matrix(int M[M_SIZE][M_SIZE])
{
	int i, j;

	for(i=0;i<M_SIZE;i++)
	{
		for(j=0;j<M_SIZE;j++)
			printf(" %d ",M[i][j]);
		printf("\n");
	}

	return;
}

int main(){
	char c;
	int shm_id;
	key_t shm_key;
	matrix_t *matrices;	
	
	if ((shm_id = shmget(shm_key, sizeof(matrices), IPC_CREAT | 0666)) < 0) {
			perror("shmget");
	}

	/*
	* Now we attach the segment to our data space.
	*/
	printf("About to attach \n");

	if ((matrices = shmat(shm_id, NULL, 0)) == -1) {
		perror("shmat");
	}
	
	matrix_struct_init(matrices);
	matrix_mult(matrices);	
	
	print_matrix(matrices->C);
}
