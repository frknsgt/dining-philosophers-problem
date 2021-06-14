#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<semaphore.h>
#include<unistd.h>
                          
sem_t masa;
sem_t cubuk[5];

void * filozof(void *);
void yemek(int);

int main()
{
	int i,a[5];
	pthread_t tid[5];
	
	sem_init(&masa,0,4);
	
	for(i=0;i<5;i++)
		sem_init(&cubuk[i],0,1);
		
	for(i=0;i<5;i++){
		a[i]=i;
		pthread_create(&tid[i],NULL,filozof,(void *)&a[i]);
	}
	for(i=0;i<5;i++)
		pthread_join(tid[i],NULL);
}

void * filozof(void * num)
{
	int phil=*(int *)num;

	sem_wait(&masa);
	printf("\nFilozof %d masaya oturdu",phil+1);
	sem_wait(&cubuk[phil]);
	sem_wait(&cubuk[(phil+1)%5]);

	yemek(phil);
	sleep(2);
	printf("\nFilozof %d yemegini bitirdi ve gitti",phil+1);

	sem_post(&cubuk[(phil+1)%5]);
	sem_post(&cubuk[phil]);
	sem_post(&masa);
}

void yemek(int phil)
{
	printf("\nFilozof %d is yiyor",phil+1);
}