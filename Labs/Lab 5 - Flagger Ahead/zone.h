/**
 * @file zone.h
 * @brief Declarations for the construction zone
 *
 * Course: CSC3210
 * Section: N/A
 * Assignment: Flagger Ahead
 * Name: Pezewski Solution
 */

#ifndef _ZONE_H
#define _ZONE_H

// Safe direction to drive
#define UNSAFE       -1
#define LEFT_TO_RIGHT  0
#define RIGHT_TO_LEFT  1

/**
 * @brief Initializes the construction zone
 * @param capacity - the number of cars that fit in the zone
 */
void init_construction_zone(int capacity);

/**
 * @brief signal all cars that it is afe to drive in the given direction
 * @param direction - the direction of travel
 */
void signal_safe_to_drive(int direction);

/**
 * @brief signal all cars that it is not safe to drive in either direction
 */
void signal_unsafe_to_drive();

/**
 * @brief Indicate that the zone needs to wait until the cars have cleared the zone
 */
void wait_until_construction_zone_cleared();

/**
 * @brief Causes a car to wait until it is safe to drive in the given direction
 * @param direction - the direction of travel
 * @return the amount of time the car waited
 */
unsigned long wait_until_safe_to_drive(int direction);

/**
 * @brief signals to the zone that a car has entered the zone
 * @param id - the identifier of the car
 * @param direction - the direction of travel for the car
 * @param crossing_count  - the number of crossings left for this car
 */
void signal_enter_construction_zone(int id, int direction, int crossing_count);

/**
 * @brief signals that a car is leaving the zone
 * @param id - the identifier of the car
 * @param direction - the direction of travel for the car
 */
void signal_exit_construction_zone(int id, int direction);

/**
 * @brief Simulate the car crossing the construction zone
 * @param id - identifier of the car
 * @param direction - the direction of travel
 * @param crossing_count - the current remaining crossing count
 * @param cross_time - the time to crosss
 */
void cross_zone(int id, int direction, int crossing_count, int cross_time);

#endif // _ZONE_H
