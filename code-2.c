#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <pthread.h>

char *num1, *num2;
int len_num1, len_num2;

void *worker(void *);

int main(int argc, char *argv[]) {
	assert(argc == 3);
	num1 = argv[1], num2 = argv[2];
	len_num1 = strlen(num1), len_num2 = strlen(num2);

	int num_threads = len_num1 + len_num2 - 1;
	void *retval;
	pthread_t tid[num_threads];
	char prod_digits[num_threads];
	int *arg;
	
	for(int i = num_threads - 1; i >= 0; --i) {
		arg = (int *)malloc(sizeof(int));
		*arg = i;
		pthread_create(&tid[i], NULL, worker, arg);
	}
	
	int carry = 0, temp_digit;
	for(int i = num_threads - 1; i >= 0; --i) {
		pthread_join(tid[i], &retval);
		temp_digit = *(int *)retval + carry;
		carry = temp_digit / 10;
		prod_digits[i] = temp_digit % 10 + '0';
	}

	if(carry != 0) printf("%d", carry);
	printf("%s\n", prod_digits);
	
	free(arg);
	free(retval);
	return 0;
}

void *worker(void *arg) {
	int thread_num = *(int *)arg;
	int* ret = (int *)malloc(sizeof(int));
	*ret = 0;
	int i, j;
	for(i = 0; i < len_num1; ++i) {
		j = thread_num - i;
		if(j >= 0 && j < len_num2)
			*ret += (num1[i] - '0') * (num2[j] - '0');
	}
	pthread_exit(ret);
}
