/**
 * @file memory_manager.h
 *
 * @brief External (public) declarations for a memory manager in c.
 *
 * Clients begin by initializing the memory manager.  They can
 * then allocate and free blocks of memory.  The memory manager
 * keeps track of allocated and free blocks to maintain efficient
 * use of memory.
 */

#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H
 
/* Structures */
/* No "public" structure, the user of the memory manager
 * should have no knowledge of the structures used for managing
 * allocated and free blocks
 */

/* Memory Manager Methods */

/**
 * @brief Initialize the memory manager to "manage" the given location
 * @param start - the start of the memory to manage
 * @param size - the size of the memory to manage
 */
void mm_init(void* start, int size);

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
void mm_destroy();

/**
 * @brief Requests a block of memory be allocated using first fit placement algorithm
 *        The memory manager must be initialized (mm_init) for this call to succeed
 * @param nbytes - the number of bytes in the requested memory
 * @return a pointer to the start of the allocated space
 */
void* mymalloc_ff(int nbytes);

/**
 * @brief Requests a block of memory be allocated using worst fit placement algorithm
 *        The memory manager must be initialized (mm_init) for this call to succeed
 * @param nbytes - the number of bytes in the requested memory
 * @return a pointer to the start of the allocated space
 */
void* mymalloc_wf(int nbytes);

/**
 * @brief Requests a block of memory be allocated using best fit placement algorithm
 *        The memory manager must be initialized (mm_init) for this call to succeed
 * @param nbytes - the number of bytes in the requested memory
 * @return a pointer to the start of the allocated space
 */
void* mymalloc_bf(int nbytes);

/**
 * @brief Requests a block of memory be freed and the storage made available for future allocations
 *        The memory manager must be initialized (mm_init) for this call to succeed
 * @param ptr - a pointer to the start of the space to be freed
 *        Signals a SIGSEGV if a free is not valid
 *            - memory manager is not initialized
 *            - memory manager has been destroyed
 *            - ptr is not allocated (e.g. double free)
 */
void myfree(void* ptr);

/**
 * @brief Retrieve the current amount of space allocated by the memory manager (in bytes)
 * @return the current number of allocated bytes
 */
int get_allocated_space();

/**
 * @brief Retrieve the current amount of available space in the memory manager (in bytes)
 *        (e.g. sum of all free blocks)
 * @return the current number of free bytes
 */
int get_remaining_space();

/**
 * @brief Retrieve the current amount of free blocks (i.e. the count of all the block, not the size)
 * @return the current number of free blocks
 */
int get_fragment_count();

/**
 * @brief Retrieve the number of successful malloc calls (for all placement types)
 * @return the total number of successful mallocs
 */
int get_mymalloc_count();

#endif
