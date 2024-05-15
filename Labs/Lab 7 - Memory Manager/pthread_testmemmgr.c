/**
 * @file pthread_testmemmgr.c
 *
 * @brief Sample memory manager tester using pthreads
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#include "memory_manager.h"

#define MY_HEAP_SIZE 4096
#define ALLOC_SIZE   1024
#define THREAD_COUNT 10

/**
 * @brief Pthread routine allocates and frees memory
 * @param args - thread arguments - not used
 * @return NULL
 */
void* thread_routine(void* args) {

    // Loop 25 times and to allocate and free memory from the shared space
	for(int i = 0; i < 25; i++) {
		char* myptr = mymalloc_ff(ALLOC_SIZE);
		usleep(20);
		if(myptr != NULL) {
			printf("Thread %lu allocated %d byte(s): %p\n", pthread_self(), ALLOC_SIZE, myptr);
			*myptr = 'a' + i;
			printf("Thread %lu write %c\n", pthread_self(), *myptr);
			myfree(myptr);
			printf("Thread %lu freed %p\n", pthread_self(), myptr);
		} else {
			printf("Thread %lu could not allocate %d byte(s)\n", pthread_self(), ALLOC_SIZE);
		}
	}

	return NULL;
}

/**
 * @brief Program entry procedure - initializes the memory manager and creates threads to request memory
 * @return 0
 */
int main() {

    // Store thread IDs for future join
	pthread_t mythreads[THREAD_COUNT];

    // Initialize the shared heap space
	char my_heap[MY_HEAP_SIZE];
	mm_init(my_heap, MY_HEAP_SIZE);

    // Create threads to use the space
	for(int i = 0; i < THREAD_COUNT; i++) {
		if(pthread_create(&mythreads[i], NULL, thread_routine, NULL) == -1) {
			printf("COULD NOT CREATE A THREAD\n");
			exit(EXIT_FAILURE);
		}
	}

    // Wait for threads to finsih
	for(int i = 0; i < THREAD_COUNT; i++) {
		pthread_join(mythreads[i], NULL);
	}

    // Destroy the shared heap and return
	mm_destroy();
	
	return 0;
}
