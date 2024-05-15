/**
 * @file flagger.h
 * @brief Declarations for the flagger thread
 *
 * Course: CSC3210
 * Section: N/A
 * Assignment: Flagger Ahead
 * Name: Pezewski Solution
 */

#ifndef _FLAGGER_H
#define _FLAGGER_H

#include <pthread.h>

/**
 * @brief Flagger thread parameters
 * @property allowed_drive_time - amount of time cars travel in one direction
 */
struct flagger_params_t {
	int allowed_drive_time;
};

/**
 * @brief Creates a new flagger thread
 * @param p - the flagger thread parameters
 * @return the thread identifier for the flagger thread
 */
pthread_t start_flagger(struct flagger_params_t* p);

#endif // _FLAGGER_H
