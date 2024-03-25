/**
 * @file main.c
 * @brief Program entry point.  Runs the teeny tiny shell
 *
 * Course: CSC3210
 * Section: 003
 * Assignment: Teeny Tiny Shell
 * Name: Hudson Arney
 */

#include "ttsh.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define INPUT_MAX 256
#define CMD_MAX 5

/**
 * @brief Reads a string of user input
 * @param dest - buffer for user input string
 * @return 0 on success
 * @return -1 on error
 */
int read_cmd_string(char dest[INPUT_MAX])
{

    // Read user input
    if (fgets(dest, INPUT_MAX, stdin) == NULL)
    {
        fprintf(stderr, "Unable to read user input\n");
        return -1;
    }

    // Remove trailing return character
    int len = strlen(dest);
    if (dest[len - 1] == '\n')
    {
        dest[len - 1] = '\0';
    }

    return 0;
}

/**
 * @brief Parses a string and divides it into individual commands
 * @param input - string containing user input
 * @param cmd_strs - the target array for command strings
 * @return the number of commands found in the input
 * @return -1 on error
 */
int parse_commands(char input[INPUT_MAX], char cmd_strs[CMD_MAX][INPUT_MAX])
{

    // Chop the input into command strings
    int cmd_count = 0;
    char *cmd_ptr = strtok(input, ";");
    while (cmd_ptr)
    {
        if (cmd_count >= CMD_MAX)
        {
            fprintf(stderr, "Too many commands\n");
            return -1;
        }
        strncpy(cmd_strs[cmd_count], cmd_ptr, INPUT_MAX);
        cmd_count++;
        cmd_ptr = strtok(NULL, ";");
    }

    return cmd_count;
}

/**
 * @brief Program entry procedure for the shell
 */
int main(int argc, char *argv[])
{

    char user_input[INPUT_MAX];
    char cmd_strs[CMD_MAX][INPUT_MAX];

    // TODO need to be able to get input from
    //    the user in a loop
    while (1)
    {
        // Print the input prompt
        printf("$> ");

        // Read user input
        if (read_cmd_string(user_input) == -1)
        {
            return 1;
        }

        // Check for quit command
        if (strcmp(user_input, "quit") == 0)
        {
            break;
        }

        // Chop the input into command strings
        int cmd_count = parse_commands(user_input, cmd_strs);
        if (cmd_count == -1)
        {
            return 1;
        }

        // Execute each command
        for (int i = 0; i < cmd_count; i++)
        {
            char *args[11]; // Array to hold command and its arguments
            int arg_count = 0;

            // Tokenize the command string into arguments
            char *token = strtok(cmd_strs[i], " ");
            while (token != NULL && arg_count < 10)
            {
                args[arg_count++] = token;
                token = strtok(NULL, " ");
            }
            args[arg_count] = NULL; // Set the last argument to NULL as execvp requires it

            // Fork a process
            /**/ int pid = fork();
            if (pid == -1)
            {
                fprintf(stderr, "Fork failed\n");
                return 1;
            }
            else if (pid == 0)
            {
                // Child process
                execvp(args[0], args);
                // If execvp returns, it means it failed
                perror("Execvp failed");
                return 1;
            }
            else
            {
                // Parent process
                // Wait for the child process to finish
                wait(NULL);
            }
        }
    }

    return 0;
}