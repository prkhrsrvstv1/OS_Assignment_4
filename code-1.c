#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct {
	int m, k, n;
	int **A, **B, **C;
} SharedMem;

typedef struct {
	int r, c;
	SharedMem *shared_mem;
} WorkerArgs;

void *worker(void *args);
void *subthread(void *args);

int main() {

	pthread_t stid;
	SharedMem *shared_mem = (SharedMem *) malloc(sizeof(SharedMem));
	// Input dimensions
	scanf("%d", &(shared_mem->m));
	scanf("%d", &(shared_mem->k));
	scanf("%d", &(shared_mem->n));

	// Allocate dynamic memory for the matrices
	shared_mem->A = (int **) malloc(sizeof(int *) * shared_mem->m);
	for(int i = 0; i < shared_mem->m; ++i)
		shared_mem->A[i] = (int *) malloc(sizeof(int) * shared_mem->k);
	
	shared_mem->B = (int **) malloc(sizeof(int *) * shared_mem->k);
	for(int i = 0; i < shared_mem->k; ++i)
		shared_mem->B[i] = (int *) malloc(sizeof(int) * shared_mem->n);

	shared_mem->C = (int **) malloc(sizeof(int *) * shared_mem->m);
	for(int i = 0; i < shared_mem->m; ++i)
		shared_mem->C[i] = (int *) malloc(sizeof(int) * shared_mem->n);
	
	// Input the matrices A & B
	for(int i = 0; i < shared_mem->m; ++i)
		for(int j = 0; j < shared_mem->k; ++j)
			scanf("%d", &shared_mem->A[i][j]);
	for(int i = 0; i < shared_mem->k; ++i)
		for(int j = 0; j < shared_mem->n; ++j)
			scanf("%d", &(shared_mem->B)[i][j]);

	// Start the sub-thread and wait till it finishes
	pthread_create(&stid, NULL, subthread, shared_mem);
	pthread_join(stid, NULL);

	// Print the product matrix C
	printf("\nProduct:\n");
	for(int i = 0; i < shared_mem->m; ++i) {
		for(int j = 0; j < shared_mem->n; ++j)
			printf("%d ", (shared_mem->C)[i][j]);
		printf("\n");
	}

	// Free the dynamically allocated memory
	free(shared_mem);
	return 0;
}

void *worker(void *worker_args) {
	
	int r = ((WorkerArgs *) worker_args)->r;
	int c = ((WorkerArgs *) worker_args)->c;
	SharedMem *shared_mem = ((WorkerArgs *) worker_args)->shared_mem;
	int *value = (int *) malloc(sizeof(int));
	*value = 0;
	
	for(int i = 0; i < shared_mem->k; ++i)
		*value += shared_mem->A[r][i] * shared_mem->B[i][c];

	pthread_exit((void *)value);
}

void *subthread(void *subthread_args) {

	void *temp;
	pthread_t **tid;
	WorkerArgs *args;
	SharedMem *shared_mem = (SharedMem *) subthread_args;

	tid = (pthread_t **) malloc(sizeof(pthread_t *) * shared_mem->m);
	for(int i = 0; i < shared_mem->m; ++i)
		tid[i] = (pthread_t *) malloc(sizeof(pthread_t) * shared_mem->n);

	for(int i = 0; i < shared_mem->m; ++i)
		for(int j = 0; j < shared_mem->n; ++j) {
			args = (WorkerArgs *) malloc(sizeof(WorkerArgs));
			args->r = i;
			args->c = j;
			args->shared_mem = shared_mem;
			pthread_create(&(tid[i][j]), NULL, worker, args);
		}
	
	for(int i = 0; i < shared_mem->m; ++i)
		for(int j = 0; j < shared_mem->n; ++j) {
			pthread_join(tid[i][j], &temp);
			shared_mem->C[i][j] = *(int *) temp;
		}

	for(int i = 0; i < shared_mem->m; ++i)
		free(tid[i]);
	free(tid);
	free(args);
	pthread_exit(0);
}
