NAME = minishell

CC = cc
CFLAGS = -Wall -Wextra -Werror
INCLUDES = -I includes -I libft

SRCS = src/main.c \
       src/lexer.c \
       src/parser.c \
       src/expander.c \
       src/executor.c \
       src/builtins.c \
       src/redirections.c \
       src/signals.c \
       src/utils.c

OBJS = $(SRCS:.c=.o)

LIBFT = libft/libft.a

all: $(NAME)

$(LIBFT):
	$(MAKE) -C libft

$(NAME): $(LIBFT) $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -L libft -lft -lreadline -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	$(MAKE) -C libft clean
	rm -f $(OBJS)

fclean: clean
	$(MAKE) -C libft fclean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
