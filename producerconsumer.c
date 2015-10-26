#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/errno.h>

#include "buffer.h"

buffer_item buffer[BUFFER_SIZE];
//buffer counter
int count;

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
		printf("mutex aquired: mutex=%d\n", mutex);

		if (insert_item(item) != 0) {
			fprintf(stderr, " Producer report error condition %d\n", errno);
		}
		else {
			printf("produced %d succesfully. mutex=%d\n", item, mutex);
		}

		/* release the mutex lock */
		pthread_mutex_unlock(&mutex);
		printf("mutex released: mutex=%d\n", mutex);

		/* signal full */
		sem_post(&full);
   }
}

void* consumer(void *ptr) {
	buffer_item item;

	while(1) {
		/* sleep for a random period of time */
		int rNum = 10; //rand() % 10 + 1000;
		sleep(rNum);

		/* aquire the full lock */
		sem_wait(&full);
		/* aquire the mutex lock */
		pthread_mutex_lock(&mutex);
		if (remove_item(&item) != 0) {
			fprintf(stderr, "Consumer report error condition %d\n", errno);
		}
		else {
			printf("produced %d succesfully. mutex=%d\n", item, mutex);
		}
		/* release the mutex lock */
		pthread_mutex_unlock(&mutex);
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

	int timeToSleep = argv[1];
	int numProducerThreads = argv[2];
	int numConsumerThreads = argv[3];


	//create mutex
   pthread_mutex_init(&mutex, NULL);

	//create 'full' semaphore == 0
	if (sem_init(&full, 0, 0) == -1) {
		fprintf(stderr, "Error creating full semaphore %d\n", errno);
		return -1;
	}
	else {
		printf("created full semaphore %d\n", full);
	}

   //create 'empty' semaphore == BUFFER_SIZE
	if (sem_init(&empty, 0, BUFFER_SIZE) == -1) {
		fprintf(stderr, "Error creating empty semaphore %d\n", errno);
		return -1;
	}
	else {
		printf("created empty semaphore %d\n", empty);
	}

   //initialize buffer counter
   count = 0;	
	
	/* 3. Create producer thread(s) */
	/* 4. Create consumer thread(s) */
	// TODO create multiple threads

	pthread_t ptid, ctid;
	pthread_create(&ptid, NULL, producer, NULL);
	pthread_create(&ctid, NULL, consumer, NULL);
	
	/* 5. Sleep */
	sleep(timeToSleep);

	/* 6. Exit */
	return 0;
}

int insert_item(buffer_item item) {
	printf("Producer inserting item: %i...", item);
   if(count < BUFFER_SIZE) {
      buffer[count] = item;
      count++;
	   printf("...Success. Count=%d\n", count);
      return 0;
   }
   else {
	   printf("...Failed. Count=%d\n", count);
      return -1;
   }
}

int remove_item(buffer_item *item) {
	printf("Consumer removing item: %i...", item);
   if(count > 0) {
      *item = buffer[(count-1)];
      count--;
	   printf("...Success. Count=%d\n", count);
      return 0;
   }
   else {
	   printf("...Failed. Count=%d\n", count);
      return -1;
   }
}