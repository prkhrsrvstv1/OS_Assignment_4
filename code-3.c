#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

int N, M, *A, *B, *Result;

void *merge(void *);
void *A_worker(void *);
void *B_worker(void *);

int main() {
	pthread_t tid;
	pthread_create(&tid, NULL, merge, NULL);
	pthread_join(tid, NULL);
	for(int i = 0; i < N + M; ++i) printf("%d ", Result[i]);
	printf("\n");
	free(Result);
	return 0;
}

void *merge(void *not_used) {
	int *arg;
	scanf("%d", &N);
	scanf("%d", &M);
	A = (int *)malloc(sizeof(int) * N);
	B = (int *)malloc(sizeof(int) * M);
	Result = (int *)malloc(sizeof(int) * (M + N));
	pthread_t tid[M + N];
	for(int i = 0; i < N; ++i) scanf("%d", &A[i]);
	for(int i = 0; i < M; ++i) scanf("%d", &B[i]);
	for(int i = 0; i < N; ++i) printf("%d ", A[i]);
	printf("\n");
	for(int i = 0; i < M; ++i) printf("%d ", B[i]);
	printf("\n\n");
	for(int i = 0; i < N; ++i) {
		arg = (int *)malloc(sizeof(int));
		*arg = i;
		pthread_create(&tid[i], NULL, A_worker, arg);
	}
	for(int i = 0; i < M; ++i) {
		arg = (int *)malloc(sizeof(int));
		*arg = i;
		pthread_create(&tid[N + i], NULL, B_worker, arg);
	}
	for(int i = 0; i < N + M; ++i) pthread_join(tid[i], NULL);
	free(arg);
	free(A);
	free(B);
	pthread_exit(0);
}

void *A_worker(void *arg) {
	int A_index = *(int *)arg;
	int key = A[A_index];
	int B_index, B_index_lo = 0, B_index_hi = M;
	while(B_index_lo < B_index_hi) {
		B_index = (B_index_lo + B_index_hi) / 2;
		if(B[B_index] < key) B_index_lo = B_index + 1;
		else B_index_hi = B_index;
	}
	printf("Found out that %d numbers in B are smaller than A[%d]: %d\n", B_index, A_index, key);
	Result[A_index + B_index] = key;
	pthread_exit(0);
}

void *B_worker(void *arg) {
	int B_index = *(int *)arg;
	int key = B[B_index];
	int A_index, A_index_lo = 0, A_index_hi = N;
	while(A_index_lo < A_index_hi) {
		printf("lo : %d, hi : %d\n", A_index_lo, A_index_hi);
		A_index = (A_index_lo + A_index_hi) / 2;
		if(A[A_index] < key) A_index_lo = A_index + 1;
		else A_index_hi = A_index;
	}
	printf("lo : %d, hi : %d\n", A_index_lo, A_index_hi);
	printf("Found out that %d numbers in A are smaller than B[%d]: %d\n", A_index, B_index, key);
	Result[A_index + B_index] = key;
	pthread_exit(0);
}
