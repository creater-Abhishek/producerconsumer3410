//
// Created by Steven Lyall on 15-10-26.
//

#include "buffer.h"

int main(int argc, char *argv[]) {
/* 1. Get command line arguments argv[1],argv[2],argv[3] */
    if (argc != 3) {
        printf("Incorrect number of aguments. Usage:\n");
        printf("producerconsumer <time to sleep before terminating> <number of producers> <number of consumers\n");
        return -1;
    }

    int timeToSleep = argv[1];
    int numProducerThreads = argv[2];
    int numConsumerThreads = argv[3];

/* 2. Initialize buffer */

/* 3. Create producer thread(s) */
/* 4. Create consumer thread(s) */
/* 5. Sleep */
/* 6. Exit */
}