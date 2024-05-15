/**
 * @file zone.h
 * @brief Implementation for the construction zone
 *
 * Course: CSC3210
 * Section: N/A
 * Assignment: Flagger Ahead
 * Name: Pezewski Solution
 */

#include <semaphore.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

#include "zone.h"

static int zone_direction = UNSAFE;
static int cars_in_zone = 0;
static sem_t zone_capacity;
static sem_t crossing_time;
static pthread_mutex_t zone_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t zone_empty = PTHREAD_COND_INITIALIZER;
static pthread_cond_t safe_to_drive[2] = { PTHREAD_COND_INITIALIZER, PTHREAD_COND_INITIALIZER };

#define NS_PER_SEC 1000000000
#define US_PER_NS 1000

/**
 * @brief gets the current time of the system in nanoseconds
 */
static inline unsigned long gettime_ns() {
	struct timespec ts;
	clock_gettime(CLOCK_REALTIME, &ts);
	return ts.tv_sec * NS_PER_SEC + ts.tv_nsec;
}

/**
 * @brief Initializes the construction zone
 * @param capacity - the number of cars that fit in the zone
 */
void init_construction_zone(int capacity) {
	sem_init(&zone_capacity, 0, capacity);
    sem_init(&crossing_time, 0, 1);
}

/**
 * @brief signal all cars that it is safe to drive in the given direction
 * @param direction - the direction of travel
 */
void signal_safe_to_drive(int direction) {
	pthread_mutex_lock(&zone_mutex);
	char* direction_str = direction == LEFT_TO_RIGHT ? "LEFT->RIGHT" : "RIGHT->LEFT";
	printf("Flagger indicating safe to drive %s\n", direction_str);
	zone_direction = direction;
	pthread_cond_broadcast(&safe_to_drive[zone_direction]);
	pthread_mutex_unlock(&zone_mutex);
}

/**
 * @brief signal all cars that it is not safe to drive in either direction
 */
void signal_unsafe_to_drive() {
	pthread_mutex_lock(&zone_mutex);
    printf("Flagger indicating unsafe to drive\n");
	zone_direction = UNSAFE;
}

/**
 * @brief Indicate that the zone needs to wait until the cars have cleared the zone
 */
void wait_until_construction_zone_cleared() {
	while(cars_in_zone > 0) {
		pthread_cond_wait(&zone_empty, &zone_mutex);
	}
	pthread_mutex_unlock(&zone_mutex);
}

/**
 * @brief Causes a car to wait until it is safe to drive in the given direction
 * @param direction - the direction of travel
 * @return the amount of time the car waited
 */
unsigned long wait_until_safe_to_drive(int direction) {
	pthread_mutex_lock(&zone_mutex);
	unsigned long start = gettime_ns();
	while(zone_direction != direction) {
		pthread_cond_wait(&safe_to_drive[direction], &zone_mutex);
	}
	return gettime_ns() - start;
}

/**
 * @brief Simulate the car crossing the construction zone
 * @param id - identifier of the car
 * @param direction - the direction of travel
 * @param crossing_count - the current remaining crossing count
 * @param cross_time - the time to crosss
 */
void cross_zone(int id, int direction, int crossing_count, int cross_time) {

    char* direction_str = direction == LEFT_TO_RIGHT ? "LEFT->RIGHT" : "RIGHT->LEFT";
    printf("Car %d entering construction zone traveling: %s Crossings Remaining %d\n", id, direction_str, crossing_count);
    usleep(cross_time);

// TESTING CODE - Attempt to get cars to exit the zone in order - is not always consistent
    unsigned long start_time = gettime_ns();
    sem_wait(&crossing_time);
    char* direction_str = direction == LEFT_TO_RIGHT ? "LEFT->RIGHT" : "RIGHT->LEFT";
    printf("Car %d entering construction zone traveling: %s Crossings Remaining %d\n", id, direction_str, crossing_count);
    unsigned long time_at_exit = start_time + cross_time;
    unsigned long end_time = gettime_ns();
    if(time_at_exit >= end_time) {
        usleep((time_at_exit - end_time) / US_PER_NS);
    }
    printf("Car %d has exited the construction zone\n", id);
    sem_post(&crossing_time);
}

/**
 * @brief signals to the zone that a car has entered the zone
 * @param id - the identifier of the car
 * @param direction - the direction of travel for the car
 * @param crossing_count  - the number of crossings left for this car
 */
void signal_enter_construction_zone(int id, int direction, int crossing_count) {
	cars_in_zone++;
	pthread_mutex_unlock(&zone_mutex);
	sem_wait(&zone_capacity);
//    char* direction_str = direction == LEFT_TO_RIGHT ? "LEFT->RIGHT" : "RIGHT->LEFT";
//    printf("Car %d entering construction zone traveling: %s Crossings Remaining %d\n", id, direction_str, crossing_count);
}

/**
 * @brief signals that a car is leaving the zone
 * @param id - the identifier of the car
 * @param direction - the direction of travel for the car
 */
void signal_exit_construction_zone(int id, int direction) {
	pthread_mutex_lock(&zone_mutex);
	sem_post(&zone_capacity);
	cars_in_zone--;
//    printf("Car %d has exited the construction zone\n", id);
	if(cars_in_zone == 0) {
		pthread_cond_signal(&zone_empty);
	}
	if(zone_direction == direction) {
		pthread_cond_signal(&safe_to_drive[direction]);
	}
	pthread_mutex_unlock(&zone_mutex);
}
