#ifndef TTSH_H
#define TTSH_H

#define INPUT_MAX 256
#define CMD_MAX 5

// Function declarations
int read_cmd_string(char dest[INPUT_MAX]);
int parse_commands(char input[INPUT_MAX], char cmd_strs[CMD_MAX][INPUT_MAX]);
void execute_commands(char cmd_strs[CMD_MAX][INPUT_MAX], int cmd_count);

#endif /* TTSH_H */
