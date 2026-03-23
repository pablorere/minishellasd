NAME := minishell

CC := cc
CFLAGS := -Wall -Wextra -Werror
SRC := minishell.c
OBJ := $(SRC:.c=.o)

.PHONY: all clean fclean flean re

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

flean: fclean

re: fclean all
