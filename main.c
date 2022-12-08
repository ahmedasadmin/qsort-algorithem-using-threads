/**** c4.2.c: quiksort by threads ******/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>


typedef struct{
	int upperbound;
	int lowerbound;
}PARM;

#define N 10
int a[N] = {4, 3, 5, 7, 6, 1, 0, 8, 9, 2};

int print(){
	int i;
	printf("[ ");
	for (i=0; i<N; i++)
		printf("%d ", a[i]);
	printf("]\n");
}
void *Qsort(void *aptr){
	PARM *ap, aleft, aright;
	int pivot, pivotIndex, left, right, temp;
	int upperbound, lowerbound;

	pthread_t me, leftThread, rightThread;
	me = pthread_self();
	ap = (PARM *)aptr;
	upperbound = ap->upperbound;
	lowerbound = ap->lowerbound;
	pivot = a[upperbound];
	left = lowerbound - 1;
	right = upperbound - 1;
	if(lowerbound >= upperbound)
		pthread_exit(NULL);
	while(left < right){
	do {left++;} while(a[left] < pivot);
	do {right --;} while (a[right] > pivot);
	if (left < right){
	temp = a[left];
	a[left] = a[right];
	a[right] = temp;
	}
	}
	print();
	pivotIndex = left;  // put pivot back
	temp = a[pivotIndex];
	a[pivotIndex] = pivot;
	a[upperbound] = temp;
	// start recursive threads
	aleft.upperbound = pivotIndex - 1;
	aleft.lowerbound = lowerbound;
	aright.upperbound = upperbound;
	aright.lowerbound = pivotIndex;
	printf("%lu: create left and right threads\n", me);
	pthread_create(&leftThread, NULL, Qsort, (void*)&aleft);
	pthread_create(&rightThread, NULL, Qsort, (void*)&aright);
	// wait for left and right threads to finish
	pthread_join(leftThread, NULL);
	pthread_join(rightThread, NULL);
	printf("%lu: joined with left & right threads\n", me);

}
int main (int argc, char *argv[]){
	PARM arg;
	int i, *array;
	pthread_t me, thread;
	me = pthread_self();
	printf("main %lu: unsorted array = ", me);
	print();

	arg.upperbound = N-1;
	arg.lowerbound = 0;
	printf("main %lu create a thread to QS\n", me);
	pthread_create(&thread, NULL, Qsort, (void *)&arg);
	// wait for QS thread to finish 
	pthread_join(thread, NULL);
	printf("main %lu sorted array = ", me);
	print();
	
}
