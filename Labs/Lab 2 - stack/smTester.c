/**
 * @file smTester.c
 * @brief Example functions to test the stack machine
 *
 * NOTE: You are required to write additional tests in either this or another file
 */

#include <stdio.h>
#include <stdlib.h>
#include "stackm.h"

void sm_init(struct stackm_t *my_stack)
{
    my_stack->top = NULL;
}

int sm_size(struct stackm_t *my_stack)
{
    int size = 0;
    struct node_t *current = my_stack->top;

    while (current != NULL)
    {
        size++;
        current = current->next;
    }

    return size;
}

int sm_push(struct stackm_t *my_stack, int to_store)
{
    struct node_t *new_element = (struct node_t *)malloc(sizeof(struct node_t));
    if (new_element == NULL)
    {
        return -1; // Memory allocation failure
    }

    new_element->value = to_store;
    new_element->next = my_stack->top;
    my_stack->top = new_element;

    return 0; // Successful push
}

int sm_pop(struct stackm_t *my_stack)
{
    if (my_stack->top == NULL)
    {
        return -1; // Stack is empty
    }

    struct node_t *temp = my_stack->top;
    my_stack->top = my_stack->top->next;
    free(temp);

    return 0; // Successful pop
}

int sm_top(struct stackm_t *my_stack, int *to_store)
{
    if (my_stack->top == NULL || to_store == NULL)
    {
        return -1; // Stack is empty or to_store is NULL
    }

    *to_store = my_stack->top->value;
    printf("Stack Top: ", my_stack->top->value);
    return 0; // Successful top
}

void sm_clear(struct stackm_t *my_stack)
{
    while (my_stack->top != NULL)
    {
        struct node_t *temp = my_stack->top;
        my_stack->top = my_stack->top->next;
        free(temp);
    }
}

void sm_print(struct stackm_t *my_stack)
{
    struct node_t *current = my_stack->top;

    // Case: Stack is empty
    if (current == NULL)
    {
        printf("\nStack is empty\n\n");
        return;
    }

    printf("Stack Contents:\n");

    // Case: Stack Top
    printf("%d", current->value);
    if (current->next == NULL)
    {
        printf("\t<-Top & Bottom\n");
        printf("\n");
        return;
    }
    printf("\t<-Top\n");
    current = current->next;

    // Case: Stack Contents
    while (current->next != NULL)
    {
        printf("%d\n", current->value);
        current = current->next;
    }

    // Case: Last element in the stack
    printf("%d\t<-Bottom\n", current->value);
    printf("\n");
}

int sm_add(struct stackm_t *my_stack)
{
    if (my_stack->top == NULL || my_stack->top->next == NULL)
    {
        return -1; // Not enough elements on the stack
    }

    int result = my_stack->top->value + my_stack->top->next->value;

    // Remove the top two elements
    struct node_t *temp1 = my_stack->top;
    my_stack->top = my_stack->top->next;
    free(temp1);

    struct node_t *temp2 = my_stack->top;
    my_stack->top = my_stack->top->next;
    free(temp2);

    // Push the result onto the stack
    return sm_push(my_stack, result);
}

int sm_sub(struct stackm_t *my_stack)
{
    if (my_stack->top == NULL || my_stack->top->next == NULL)
    {
        return -1; // Not enough elements on the stack
    }

    int result = my_stack->top->value - my_stack->top->next->value;

    // Remove the top two elements
    struct node_t *temp1 = my_stack->top;
    my_stack->top = my_stack->top->next;
    free(temp1);

    struct node_t *temp2 = my_stack->top;
    my_stack->top = my_stack->top->next;
    free(temp2);

    // Push the result onto the stack
    return sm_push(my_stack, result);
}

int sm_mult(struct stackm_t *my_stack)
{
    if (my_stack->top == NULL || my_stack->top->next == NULL)
    {
        return -1; // Not enough elements on the stack
    }

    int result = my_stack->top->value * my_stack->top->next->value;

    // Remove the top two elements
    struct node_t *temp1 = my_stack->top;
    my_stack->top = my_stack->top->next;
    free(temp1);

    struct node_t *temp2 = my_stack->top;
    my_stack->top = my_stack->top->next;
    free(temp2);

    // Push the result onto the stack
    return sm_push(my_stack, result);
}

int sm_div(struct stackm_t *my_stack)
{
    if (my_stack->top == NULL || my_stack->top->next == NULL || my_stack->top->value == 0)
    {
        return -1; // Not enough elements on the stack or division by zero
    }

    int result = my_stack->top->value / my_stack->top->next->value;

    // Remove the top two elements
    struct node_t *temp1 = my_stack->top;
    my_stack->top = my_stack->top->next;
    free(temp1);

    struct node_t *temp2 = my_stack->top;
    my_stack->top = my_stack->top->next;
    free(temp2);

    // Push the result onto the stack
    return sm_push(my_stack, result);
}

int sm_rotate(struct stackm_t *my_stack, int depth)
{
    if (depth <= 1)
    {
        // No rotation needed for depth less than or equal to 1
        return 0;
    }

    if (depth < 0 || depth > sm_size(my_stack))
    {
        return -1; // Invalid depth
    }

    struct stackm_t temp_stack;
    temp_stack.top = NULL;

    struct node_t *node;

    int save_value = my_stack->top->value;
    sm_pop(my_stack);

    int i = depth;

    do
    {
        node = my_stack->top;
        sm_push(&temp_stack, node->value);
        sm_pop(my_stack);
        i--;
    } while (i > 1);

    sm_push(my_stack, save_value);

    i = sm_size(&temp_stack);
    do
    {
        node = my_stack->top;
        sm_push(&temp_stack, node->value);
        sm_pop(my_stack);
        i--;
    } while (i > 0);

    return 0; // Rotation successful
}

/**
 * @brief Program entry procedure
 * @return 0
 */
int main(void)
{

    // Initialize the stack
    struct stackm_t my_stack;
    sm_init(&my_stack);

    // Test pushing of values
    sm_push(&my_stack, 2);
    sm_push(&my_stack, 3);
    sm_push(&my_stack, 4);
    sm_print(&my_stack);

    // Test popping of values
    sm_pop(&my_stack);
    sm_print(&my_stack);

    // Test retrieving the top
    int value = 0;
    sm_top(&my_stack, &value);
    printf("%d\n", value);

    // Test clear
    sm_clear(&my_stack);
    sm_print(&my_stack);

    // Test addition
    sm_push(&my_stack, 2);
    sm_push(&my_stack, 3);
    sm_print(&my_stack);
    sm_add(&my_stack);
    sm_print(&my_stack);
    sm_pop(&my_stack);

    // Test subtraction
    sm_push(&my_stack, 10);
    sm_push(&my_stack, 5);
    sm_print(&my_stack);
    sm_sub(&my_stack);
    sm_print(&my_stack);
    sm_pop(&my_stack);

    // Test multiplication
    sm_push(&my_stack, 10);
    sm_push(&my_stack, 11);
    sm_print(&my_stack);
    sm_mult(&my_stack);
    sm_print(&my_stack);
    sm_pop(&my_stack);

    // Test division
    sm_push(&my_stack, 10);
    sm_push(&my_stack, 2);
    sm_print(&my_stack);
    sm_div(&my_stack);
    sm_print(&my_stack);
    sm_pop(&my_stack);

    // Test rotate
    sm_push(&my_stack, 10);
    sm_push(&my_stack, 11);
    sm_push(&my_stack, 12);
    sm_push(&my_stack, 13);
    sm_push(&my_stack, 14);
    sm_push(&my_stack, 15);
    sm_print(&my_stack);
    sm_rotate(&my_stack, 5);
    sm_print(&my_stack);

    return 0;
}