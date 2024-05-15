/**
 * @file main.c
 * @brief Implementation of the program entry procedure for the flagger ahead
 *
 * Course: CSC3210
 * Section: N/A
 * Assignment: Flagger Ahead
 * Name: Pezewski Solution
 */

#include <stdio.h>
#include <stdlib.h>

#include "zone.h"
#include "flagger.h"
//#include "car.h"
//#include "params.h"

volatile int start = 0;

/**
 * @brief program entry procedure
 * @return 0 on success
 * @return 1 on failure
 */
int main(int argc, char* argv[]) {
	struct params_t p;
	if(argc != 2) {
		printf("Usage: flagger param_file\n");
		return 1;
	}

	read_param_file(argv[1], &p);
	int total_cars = p.left_cars + p.right_cars;

	init_construction_zone(p.capacity);

    pthread_t flagger_id = start_flagger(&p.flagger);
	pthread_t* cars = malloc(total_cars * sizeof(pthread_t));
	unsigned long* wait_times = malloc(total_cars * sizeof(unsigned long));
	for(int i = 0; i < total_cars; i++) {
		p.cars[i].total_wait = &(wait_times[i]);
		cars[i] = start_car(&p.cars[i]);
	}

	print_params(&p);

	start = 1;
	for(int i = 0; i < total_cars; i++) {
		pthread_join(cars[i], NULL);
	}
    start = 0;
    pthread_join(flagger_id, NULL);

	printf("Simulation ended\n");
	for(int i = 0; i < total_cars; i++) {
		printf("Car %d waited a total of %lu ns\n", p.cars[i].identifier, wait_times[i]);
	}

	free(wait_times);
	free(cars);
	free_params(&p);

	return 0;
}
