CC = gcc
CFLAGS = -Wall -Wextra -std=c99

SRC = ttsh.c parser.c executer.c utils.c
OBJ = $(SRC:.c=.o)

ttsh: $(OBJ)
    $(CC) $(CFLAGS) -o $@ $^

%.o: %.c
    $(CC) $(CFLAGS) -c -o $@ $^

clean:
    rm -f $(OBJ) ttsh
