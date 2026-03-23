NAME = minishell

CC = cc
CFLAGS = -Wall -Wextra -Werror -g3
INCLUDES = -I includes -I libft

SRCS = src/main.c \
       src/lexer.c \
       src/parser.c \
       src/parser_utils.c \
       src/lexer_utils.c \
       src/env_get.c \
       src/env_set.c \
       src/exec_find.c \
       src/expander.c \
       src/expander_utils.c \
       src/expander_quotes.c \
       src/executor.c \
       src/executor_child.c \
       src/executor_utils.c \
       src/builtins.c \
       src/builtins_env.c \
       src/builtins_ctrl.c \
       src/builtins_utils.c \
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
