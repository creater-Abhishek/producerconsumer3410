#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <unistd.h>

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
		// sleep each iteration
		sleep(1000);

		//generate random number to insert
		item = rand();

		//wait to acquire sem
		sem_wait(&empty);
		//acquire mutex
		pthread_mutex_lock(&mutex);

		if(insert_item(item)) {
			fprintf(stderr, " Producer report error condition\n");
		}
		else {
         printf("producer produced %d\n", item);
		}

		/* release the mutex lock */
		pthread_mutex_unlock(&mutex);

		/* signal full */
		sem_post(&full);
   }
}

void* consumer(void *ptr) {
	buffer_item item;

	while(1) {
		/* sleep for a random period of time */
		int rNum = rand() % 10 + 1;
		sleep(rNum);

		/* aquire the full lock */
		sem_wait(&full);
		/* aquire the mutex lock */
		pthread_mutex_lock(&mutex);
		if(remove_item(&item)) {
			fprintf(stderr, "Consumer report error condition\n");
		}
		else {
			printf("consumer consumed %d\n", item);
		}
		/* release the mutex lock */
		pthread_mutex_unlock(&mutex);
		/* signal empty */
		sem_post(&empty);
	}
}


int main(int argc, char *argv[]) {

	printf("buffer size: %i\n", BUFFER_SIZE);
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
	full = (sem_t) sem_open("full", O_CREAT);
	printf("created full semaphore %d\n", full);

   //create 'empty' semaphore == BUFFER_SIZE
	empty = (sem_t) sem_open("empty", O_CREAT);
	printf("created empty semaphore %d\n", empty);

   //initialize buffer counter
   count = 0;	
	
	/* 3. Create producer thread(s) */
	/* 4. Create consumer thread(s) */
	// TODO create multple threads

	pthread_t ptid, ctid;
	pthread_create(&ptid, NULL, producer, NULL);
	pthread_create(&ctid, NULL, consumer, NULL);
	
	/* 5. Sleep */
	sleep(timeToSleep);

	/* 6. Exit */
	return 0;
}

int insert_item(buffer_item item) {
	printf("inserting item: %i", item);
   if(count < BUFFER_SIZE) {
      buffer[count] = item;
      count++;
      return 0;
   }
   else {
      return -1;
   }
}

int remove_item(buffer_item *item) {
	printf("removing item: %i", item);
   if(count > 0) {
      *item = buffer[(count-1)];
      count--;
      return 0;
   }
   else {
      return -1;
   }
}