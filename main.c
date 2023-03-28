#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include "ts_hashmap.h"

// Globals
pthread_spinlock_t *lock;
void *runTest(void *);
ts_hashmap_t *hashmap;

void* runTest(void *args) {
	// Pick a number between 1 and 3 to assign the thread's work
	int randomizer = rand() % 3 + 1;

	if (randomizer == 1) {
		// Thread runs put

		// Lock CS (hashmap)
		pthread_spin_lock(lock);

		// Generate random key/val 1-99
		int randKey = (int) (rand() * 99.0 / RAND_MAX);

		put(hashmap, randKey, randKey); // Put the key/val in the hashmap
		
		// Release lock
		pthread_spin_unlock(lock);
	} else if (randomizer == 2) {
		// Thread runs get

		// Lock CS (hashmap)
		pthread_spin_lock(lock);

		// Generate random key/val 1-99
		int randKey = (int) (rand() * 99.0 / RAND_MAX);

		get(hashmap, randKey); // Get the random key from the hashmap

		// Release lock
		pthread_spin_unlock(lock);
	} else {
		// Thread runs delete

		// Lock CS (hashmap)
		pthread_spin_lock(lock);

		// Generate random key/val 1-99
		int randKey = (int) (rand() * 99.0 / RAND_MAX);

		del(hashmap, randKey); // Delete random key

		// Release lock
		pthread_spin_unlock(lock);
	}

	return NULL;
}

int main(int argc, char *argv[]) {
	if (argc < 3) {
		printf("Usage: %s <num threads> <hashmap capacity>\n", argv[0]);
		return 1;
	}

	srand(time(NULL));
	int num_threads = atoi(argv[1]);
	int capacity = (unsigned int) atoi(argv[2]);

	// TODO: Write your test
	hashmap = initmap(capacity);

	// Allocate space for the lock and initialize it
	lock = (pthread_spinlock_t*)malloc(sizeof(pthread_spinlock_t));
	pthread_spin_init(lock, PTHREAD_PROCESS_PRIVATE);
	
	// Allocate space to hold threads
	pthread_t *threads = (pthread_t*) malloc(sizeof(pthread_t) * num_threads);

	for (int i=0; i<num_threads; i++) {
		// Create the threads
		pthread_create(&threads[i], NULL, runTest, NULL);
	}

	// Join the threads
	for (int i=0; i<num_threads; i++) {
		// Join the threads
		pthread_join(threads[i], NULL);
	}
	
	// Print out the final hashmap
	printmap(hashmap);

	// Memeory cleanup
	pthread_spin_destroy(lock);

	for (int i=0; i<hashmap->capacity; i++) {
		free(hashmap->table[i]);
		hashmap->table[i] = NULL;
	}
	
	free(hashmap->table);
	hashmap->table = NULL;

	free(hashmap);
	hashmap = NULL;

	free(threads);
	threads = NULL;
	return 0;
}

