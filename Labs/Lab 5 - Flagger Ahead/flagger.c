/**
 * @file flagger.c
 * @brief Implementation for the flagger thread
 *
 * Course: CSC3210
 * Section: N/A
 * Assignment: Flagger Ahead
 * Name: Pezewski Solution
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#include "zone.h"
#include "flagger.h"

extern int start;

/**
 * @brief flagger thread function
 * @param args - parameters for the flagger thread
 * @return NULL
 */
void* flagger_thread(void* args) {

    // Retrieve the flagger parameters
    struct flagger_params_t* params = (struct flagger_params_t*)args;
	int allowed_drive_time = params->allowed_drive_time;

    // Parameters were passed in from the heap - free them to avoid a memory leak
	free(params);

    // Wait for the simulation to being
	while(start == 0);

	int direction = LEFT_TO_RIGHT;
	while(start == 1) {

        // Allow cars to flow through the zone
		signal_safe_to_drive(direction);
		usleep(allowed_drive_time);

        // Signal that it is not safe
		signal_unsafe_to_drive();

        // Wait for all cars to exit the zone
		wait_until_construction_zone_cleared();

        // Flip the traveling direction of the car
        if(direction == LEFT_TO_RIGHT) {
            direction = RIGHT_TO_LEFT;
        } else {
            direction = LEFT_TO_RIGHT;
        }
	}

    printf("Flagger has finished\n");
	return NULL;
}

/**
 * @brief Creates a new flagger thread
 * @param p - the flagger thread parameters
 * @return the thread identifier for the flagger thread
 */
pthread_t start_flagger(struct flagger_params_t* p) {

	// Copy parameters to send to thread
	struct flagger_params_t* fp = malloc(sizeof(struct flagger_params_t));
	memcpy(fp, p, sizeof(struct flagger_params_t));
	
	// Create the flagger thread
	pthread_t flagger_id;
	if(pthread_create(&flagger_id, NULL, flagger_thread, (void*)fp) == -1) {
        printf("Error: Could not create a flagger thread: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	return flagger_id;
}
