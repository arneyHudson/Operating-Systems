/**
 * @file memory_manager.c
 *
 * @brief Implementation of memory manager functions
 *
 * Course: CSC3210
 * Section: 003
 * Assignment: Memory Manager
 * Name: Hudson Arney
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include "memory_manager.h"

/*
 * Using static causes the compiler to
 * limit visibility of the variables to this file only
 * This can be used to simulate 'private' variables in c
 */
static int allocation_count = 0;
static int malloc_count = 0;

// Global mutex
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

// Data Structure for Memory Block
typedef struct Node
{
    void *start;       // Start address of the block
    int size;          // Size of the block
    int allocated;     // Flag indicating if the block is allocated
    struct Node *next; // Pointer to the next block in the list
} Node;

// Global Variables
static Node *memory_list = NULL;

// Helper Function
static Node *find_free_block(int nbytes)
{
    Node *curr = memory_list;
    while (curr != NULL)
    {
        if (!curr->allocated && curr->size >= nbytes)
        {
            return curr;
        }
        curr = curr->next;
    }
    return NULL;
}

void *allocate_memory(Node *block, int nbytes)
{
    if (block->size > nbytes)
    {
        // Split the block into two
        Node *new_block = (Node *)malloc(sizeof(Node));
        new_block->start = block->start + nbytes;
        new_block->size = block->size - nbytes;
        new_block->allocated = 0;
        new_block->next = block->next;
        block->next = new_block;
        block->size = nbytes;
    }

    // Allocate from the block
    block->allocated = 1;
    allocation_count++;

    // Return the start address of the allocated block
    return block->start;
}

/**
 * @brief Initialize the memory manager to "manage" the given location
 *        NOTE: Do NOT malloc space for the memory to manage
 *              The invoker of this function will provide the memory
 *              See the sample for how to use this function
 * @param start - the start of the memory to manage
 * @param size - the size of the memory to manage
 */
void mm_init(void *start, int size)
{
    allocation_count = 0;
    malloc_count = 0;

    // Initialize memory list with the entire memory block as one free block
    memory_list = (Node *)malloc(sizeof(Node));
    memory_list->start = start;
    memory_list->size = size;
    memory_list->allocated = 0;
    memory_list->next = NULL;
}

// Memory Manager Cleanup
/**
 * @brief Cleans up any storage used by the memory manager
 *        After a call to mmDestroy:
 *            all allocated spaces become invalid
 *            future allocation attempts fail
 *            future frees result in segmentation faults
 *        NOTE: after a call to mmDestroy a call to mmInit
 *            reinitializes the memory manager to allow allocations
 *            and frees
 */
void mm_destroy()
{
    // Free all nodes in the memory list
    Node *curr = memory_list;
    while (curr != NULL)
    {
        Node *temp = curr;
        curr = curr->next;
        free(temp);
    }
    memory_list = NULL;
}

/**
 * @brief Requests a block of memory be allocated using first fit placement algorithm
 *        The memory manager must be initialized (mm_init) for this call to succeed
 * @param nbytes - the number of bytes in the requested memory
 * @return a pointer to the start of the allocated space
 */
void *mymalloc_ff(int nbytes)
{
    Node *block = find_free_block(nbytes);
    if (block == NULL)
    {
        return NULL; // No suitable free block found
    }

    return allocate_memory(block, nbytes);
}

/**
 * @brief Requests a block of memory be allocated using worst fit placement algorithm
 *        The memory manager must be initialized (mm_init) for this call to succeed
 * @param nbytes - the number of bytes in the requested memory
 * @return a pointer to the start of the allocated space
 */
void *mymalloc_wf(int nbytes)
{
    Node *block = NULL;
    Node *curr = memory_list;

    // Find the largest free block that can accommodate the requested size
    while (curr != NULL)
    {
        if (!curr->allocated && curr->size >= nbytes)
        {
            if (block == NULL || curr->size > block->size)
            {
                block = curr;
            }
        }
        curr = curr->next;
    }

    if (block == NULL)
    {
        return NULL; // No suitable free block found
    }

    return allocate_memory(block, nbytes);
}

/**
 * @brief Requests a block of memory be allocated using best fit placement algorithm
 *        The memory manager must be initialized (mm_init) for this call to succeed
 * @param nbytes - the number of bytes in the requested memory
 * @return a pointer to the start of the allocated space
 */
void *mymalloc_bf(int nbytes)
{
    Node *block = NULL;
    Node *curr = memory_list;

    // Find the smallest free block that can accommodate the requested size
    while (curr != NULL)
    {
        if (!curr->allocated && curr->size >= nbytes)
        {
            if (block == NULL || curr->size < block->size)
            {
                block = curr;
            }
        }
        curr = curr->next;
    }

    if (block == NULL)
    {
        return NULL; // No suitable free block found
    }

    return allocate_memory(block, nbytes);
}

// Memory Deallocation
/**
 * @brief Requests a block of memory be freed and the storage made available for future allocations
 *        The memory manager must be initialized (mm_init) for this call to succeed
 * @param ptr - a pointer to the start of the space to be freed
 *        Signals a SIGSEGV if a free is not valid
 *            - memory manager is not initialized
 *            - memory manager has been destroyed
 *            - ptr is not allocated (e.g. double free)
 */
void myfree(void *ptr)
{
    if (ptr == NULL)
    {
        return; // Invalid pointer
    }

    pthread_mutex_lock(&mutex);
    Node *curr = memory_list;
    Node *prev = NULL;

    while (curr != NULL)
    {
        if (curr->start == ptr)
        {
            if (curr->allocated)
            {
                malloc_count++;
                curr->allocated = 0;
                allocation_count--;

                // Merge with the previous free block if applicable
                if (prev != NULL && !(prev->allocated))
                {
                    prev->size += curr->size;
                    prev->next = curr->next;
                    curr = prev; // Move back to the merged block
                }

                // Merge with the next free block if applicable
                if (curr->next != NULL && !(curr->next->allocated))
                {
                    curr->size += curr->next->size;
                    curr->next = curr->next->next;
                }
                pthread_mutex_unlock(&mutex);

                return;
            }
            else
            {
                // Error: Trying to free an unallocated block
                pthread_mutex_unlock(&mutex);
                exit(EXIT_FAILURE);
            }
        }
        prev = curr;
        curr = curr->next;
    }

    pthread_mutex_unlock(&mutex);
    // Error: Pointer not found in memory list
    printf("Segmentation fault\n\n");
    exit(EXIT_FAILURE);
}

// Retrieve the current amount of space allocated by the memory manager
/**
 * @brief Retrieve the current amount of space allocated by the memory manager (in bytes)
 * @return the current number of allocated bytes
 */
int get_allocated_space()
{
    int allocated_space = 0;
    Node *curr = memory_list;
    while (curr != NULL)
    {
        if (curr->allocated)
        {
            allocated_space += curr->size;
        }
        curr = curr->next;
    }
    return allocated_space;
}

// Retrieve the current amount of available space in the memory manager
/**
 * @brief Retrieve the current amount of available space in the memory manager (in bytes)
 *        (e.g. sum of all free blocks)
 * @return the current number of free bytes
 */
int get_remaining_space()
{
    int free_space = 0;
    Node *curr = memory_list;
    while (curr != NULL)
    {
        if (!curr->allocated)
        {
            free_space += curr->size;
        }
        curr = curr->next;
    }
    return free_space;
}

// Retrieve the current number of free blocks
/**
 * @brief Retrieve the current amount of free blocks (i.e. the count of all the block, not the size)
 * @return the current number of free blocks
 */
int get_fragment_count()
{
    int fragment_count = 0;
    pthread_mutex_lock(&mutex);
    Node *curr = memory_list;
    while (curr != NULL)
    {
        if (!curr->allocated)
        {
            fragment_count++;
        }
        curr = curr->next;
    }
    pthread_mutex_unlock(&mutex);
    return fragment_count;
}

/**
 * @brief Retrieve the number of successful malloc calls (for all placement types)
 * @return the total number of successful mallocs
 */
int get_mymalloc_count()
{
    return malloc_count;
}