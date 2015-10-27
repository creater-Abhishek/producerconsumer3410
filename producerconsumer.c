#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <sys/errno.h>

#include "buffer.h"

buffer_item buffer[BUFFER_SIZE];
//buffer counter
int count, in, out;

//mutex lock
pthread_mutex_t mutex;

//semaphores
sem_t full, empty;

void* producer(void *ptr) {
	buffer_item item;
	while(1) {
		/* sleep for a random period of time */
		int rNum = 5;//rand() % 10 + 10; TODO make these randoms that make sense
		sleep(rNum);

		//generate random number to insert
		item = rand() / 1000000;

		//wait to acquire sem
		sem_wait(&empty);

		//acquire mutex
		pthread_mutex_lock(&mutex);
		//printf("mutex aquired: mutex=%d\n", mutex);

		insert_item(item);

		/* release the mutex lock */
		pthread_mutex_unlock(&mutex);
		//printf("mutex released: mutex=%d\n", mutex);

		/* signal full */
		sem_post(&full);
   }
}

void* consumer(void *ptr) {

	while(1) {
		/* sleep for a random period of time */
		int rNum = 10; //rand() % 10 + 1000;
		sleep(rNum);

		/* aquire the full lock */
		sem_wait(&full);
		/* aquire the mutex lock */
		pthread_mutex_lock(&mutex);
		remove_item();

		/* release the mutex lock */
		pthread_mutex_unlock(&mutex);
		//printf("mutex released: mutex=%d\n", mutex);

		/* signal empty */
		sem_post(&empty);
	}
}


int main(int argc, char *argv[]) {

	/* 1. Get command line arguments argv[1],argv[2],argv[3] */
	if (argc != 4) {
		printf("Incorrect number of aguments. Usage:\n");
		printf("producerconsumer <time to sleep before terminating> <number of producers> <number of consumers>\n");
		return -1;
	}

	int timeToSleep = atoi(argv[1]);
	int numProducerThreads = atoi(argv[2]);
	int numConsumerThreads = atoi(argv[3]);


	//create mutex
   pthread_mutex_init(&mutex, NULL);

	//create 'full' semaphore == 0
	if (sem_init(&full, 0, 0) == -1) {
		fprintf(stderr, "Error creating full semaphore %d\n", errno);
		return -1;
	}

   //create 'empty' semaphore == BUFFER_SIZE
	if (sem_init(&empty, 0, BUFFER_SIZE) == -1) {
		fprintf(stderr, "Error creating empty semaphore %d\n", errno);
		return -1;
	}

	//initialize buffer counter
	count = 0;
	in = 0;
	out = 0;
	
	/* 3. Create producer thread(s) */
	/* 4. Create consumer thread(s) */
	// TODO create multiple threads

	pthread_t ptid, ctid;
	pthread_create(&ptid, NULL, producer, NULL);
	pthread_create(&ctid, NULL, consumer, NULL);
	
	/* 5. Sleep */
	sleep(timeToSleep);

	// kill threads
	pthread_kill(ptid, 0);
	pthread_kill(ctid, 0);

	/* 6. Exit */
	return 0;
}

int insert_item(buffer_item item) {

	/* produce an item in next produced */
	while (count == BUFFER_SIZE) {
		/* do nothing */
	}

	buffer[in] = item;
	in = (in + 1) % BUFFER_SIZE;
	printf("Thread id: %u Producer inserting item: %i...", pthread_self(), item);
	count++;
	printf("...Success. Count=%d\n", count);
}

int remove_item() {
	buffer_item item;
	
	while(count == 0){
		//do nothing
	}
   
    item = buffer[out];
	out = (out+1) % BUFFER_SIZE;
	printf("Thread id: %u Consumer removing item: %i...", pthread_self(), item);
    count--;
	printf("...Success. Count=%d\n", count);
}