/**
 * Hudson Arney
 * CSC 3320 003 - Operating Systems
 * Lab 2 - Stack Machine in C
 *
 * @file stackm.h
 *
 * @brief External (public) declarations for stack machine in C.
 *
 * This stack will know about the top.  Each element must point
 * to the one below it in the stack.
 *
 * Note that the pop operations do not return a reference to the
 * popped node.  This would require storage for the node to be
 * released by the user, which could lead to memory mishandling.
 * The stack size is unbounded so memory must be allocated and
 * freed as appropriate when operations are performed.
 *
 * This stack will only hold integers.  Mathematical operations are
 * performed on elements on the top of the stack as described in the
 * function definitions.
 *
 */

#ifndef STACKM_H
#define STACKM_H

/* Structures */

/**
 * @brief stack node
 * @property value - the value of the stack element
 * @property next - pointer to the next node (down) in the stack
 */
struct node_t
{
    int value;
    struct node_t *next;
};

/**
 * @brief structure for a stack
 * @property top - pointer to the first element in the stack
 */
struct stackm_t
{
    struct node_t *top;
};

/* Stack Machine methods
 *
 * These methods are used to create and operate on the stack machine as a whole.
 */

/**
 * @brief Initialize a stack machine structure.
 *        An empty stack will be characterized by top being NULL.
 * @param my_stack - a pointer to the structure to be init
 */
void sm_init(struct stackm_t *my_stack);

/**
 * @brief Reports the current size of the stack.
 *        Must iterate the stack to get this data size there is no size property.
 * @param my_stack - pointer to the stack
 * @return the size of the stack
 */
int sm_size(struct stackm_t *my_stack);

/**
 * @brief Add a new node with provided data to the top of the stack.
 *        This method should allocate memory as needed and check to
 *        make sure that the memory was allocated successfully.
 * @param my_stack - pointer to the stack
 * @param to_store - value to push on the stack
 * @return -1 if push could not be performed (i.e. failed to allocate memory)
 * @return 0 if push was successful
 */
int sm_push(struct stackm_t *my_stack, int to_store);

/**
 * @brief Removes top item in stack
 *        Note, this does not return any data from the stack.
 *        If the data in the node is needed it should be accessed
 *        prior to the pop (sm_top).
 * @param my_stack - pointer to the stack
 * @return -1 if pop could not be performed (i.e stack is empty)
 * @return 0 if pop was successful
 */
int sm_pop(struct stackm_t *my_stack);

/**
 * @brief "returns" the value at the top of the stack.
 *        NOTE: the value must be returned by dereferencing the passed
 *        in pointer.
 *        The function must make sure the pointer is not NULL prior to
 *        dereferencing to avoid memory violations.
 * @param my_stack - pointer to the stack
 * @param to_store - pointer to an integer to contain the top of the stack
 * @return -1 if the top of the stack could not be retrieved (i.e stack is empty or to_store is NULL)
 * @return 0 if the top of the stack was retrieved and stored in to_store
 */
int sm_top(struct stackm_t *my_stack, int *to_store);

/* smClear()
 *     Clears all nodes and releases all dynamic memory.  Stack
 *     structure should be NULLed and can be reused.
 *         Parameters:  myStack - the stack
 *         Returns: nothing
 */
/**
 * @brief Clears all nodes and releases all dynamic memory.  Stack should
 *        and able to be reused after clear
 * @param my_stack - pointer to the stack
 */
void sm_clear(struct stackm_t *my_stack);

/**
 * @brief Prints the contents of the stack machine to standard output.
 *        When printing it should be clear what value contains the top
 *        as well which values are stacked on top of other values
 * @param my_stack - pointer to the stack
 */
void sm_print(struct stackm_t *my_stack);

/* Stack Machine manipulation methods
 *
 * These methods perform operations that manipulate the stack
 * (math or element ordering).
 */

/**
 * @brief Add together the top two elements of the stack and push
 *        the result.
 *        The stack must contain at least 2 elements for this
 *        operation to be successful.
 *        Any removed element must be freed (no memory leaks)
 * @param my_stack - pointer to the stack
 * @return -1 if the operation could not be performed
 *             (i.e. not enough elements on the stack)
 * @return 0 if operation was successful
 */
int sm_add(struct stackm_t *my_stack);

/**
 * @brief Subtract the top two elements of the stack and push
 *        the result.
 *        The stack must contain at least 2 elements for this
 *        operation to be successful.
 *        Operation is TOS (top of stack) - 2nd from TOS
 *        if the stack contained the values:
 *        top ->    6
 *                  5
 *                  4
 *        bottom -> 3
 *        The result after an sm_sub operation would be:
 *        top ->    1
 *                  4
 *        bottom -> 3
 *        6 - 5 = 1
 * @param my_stack - pointer to the stack
 * @return -1 if the operation could not be performed
 *             (i.e. not enough elements on the stack)
 * @return 0 if operation was successful
 */
int sm_sub(struct stackm_t *my_stack);

/**
 * @brief Multiply the top two elements of the stack and push
 *        the result.
 *        The stack must contain at least 2 elements for this
 *        operation to be successful.
 *        Any removed element must be freed (no memory leaks)
 * @param my_stack - pointer to the stack
 * @return -1 if the operation could not be performed
 *             (i.e. not enough elements on the stack)
 * @return 0 if operation was successful
 */
int sm_mult(struct stackm_t *my_stack);

/**
 * @brief Divide the top two elements of the stack and push
 *        the result.
 *        NOTE: this performs integer division
 *        NOTE: dividing by zero is not allowed and the function
 *              must return -1 (and do nothing) if the divisor is zero
 *        The stack must contain at least 2 elements for this
 *        operation to be successful.
 *        Operation is TOS (top of stack) / 2nd from TOS
 *        if the stack contained the values:
 *        top ->    6
 *                  5
 *                  4
 *        bottom -> 3
 *        The result after an sm_sub operation would be:
 *        top ->    1
 *                  4
 *        bottom -> 3
 *        6 / 5 = 1
 * @param my_stack - pointer to the stack
 * @return -1 if the operation could not be performed
 *             (i.e. not enough elements on the stack)
 * @return 0 if operation was successful
 */
int sm_div(struct stackm_t *my_stack);

/**
 * @brief Rotate the top 'n' elements of the stack.  For example,
 *        if the stack contained the values:
 *        top ->    6
 *                  5
 *                  4
 *        bottom -> 3
 *        The result after a rotate 3 operation would be:
 *        top ->    5
 *                  4
 *                  6
 *        bottom -> 3
 *        NOTE: the top element on the stack goes to the 'nth'
 *        location and every element above the 'nth' location
 *        gets moved up one
 *        The stack must contain at least 'n' elements for this
 *        operation to be successful.
 *        'n' must be greater than or equal to 1.  A value of 0 or 1
 *        does nothing, but is still successful.
 *        Any removed element must be freed (no memory leaks)
 * @param my_stack - pointer to the stack
 * @param depth - the depth of rotation
 * @return 0 if the rotate was completed successfully
 * @return -1 if the rotate could not be performed
 *            (i.e. depth negative or the depth is greater than
 *             the number of elements on the stack)
 */
int sm_rotate(struct stackm_t *my_stack, int depth);

#endif