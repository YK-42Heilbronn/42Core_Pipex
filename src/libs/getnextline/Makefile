CC = cc
CFLAGS = -Wall -Wextra -Werror

SRCS = get_next_line.c get_next_line_utils.c

OBJS = $(SRCS:%.c=%.o)

NAME = get_next_line.a

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJS) 
	ar rcs $(NAME) $(OBJS)

all: $(NAME)

clean:	
	rm -f $(OBJS) $(OBJSB)

fclean:	clean
	rm -f $(NAME) $(bonus)

re: clean fclean all

bonus: $(NAME) $(OBJSB)
	ar rcs $(NAME) $(OBJSB)

.PHONY: all clean fclean re


