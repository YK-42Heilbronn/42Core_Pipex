# NAME = pipex.a
# NAME = pipex.out
NAME = pipex_asan.out
# CFLAGS = -Wall -Wextra -Werror -g
CFLAGS = -Wall -Wextra -Werror -fsanitize=address -g

SCRS = utils.c pipex.c
OBJS = $(SCRS:.c=.o)  # creates .o file names as .c, does not compile

LIBFT_DIR = ./libft
LIBFT = $(LIBFT_DIR)/libft.a

PRINTF_DIR = ./printf
PRINTF = $(PRINTF_DIR)/libftprintf.a

all: $(NAME)

%.o: %.c  # .o files compilation happens in this rule
	@cc $(CFLAGS) -c $< -o $@

$(LIBFT):
	@make -C $(LIBFT_DIR) bonus

$(PRINTF):
	@make -C $(PRINTF_DIR)

$(NAME): $(OBJS) $(LIBFT) $(PRINTF)
# 	LINK: 1
#   NOTE: following command is used to run the specific script
	@cc $(CFLAGS) $(OBJS) $(LIBFT) $(PRINTF) -o $(NAME) 

# $(NAME): $(OBJS) $(LIBFT) $(PRINTF)
# #   NOTE: following commands used to create the static lib of the specific project
# # 	@cp $(LIBFT) $(NAME)
# # 	@cp $(PRINTF) $(NAME)
# 	ar rcs $(NAME) $(LIBFT) $(PRINTF) $(OBJS)    
# 	ranlib $(NAME)

clean:
	@rm -f $(OBJS)
	@make -C $(LIBFT_DIR) clean
	@make -C $(PRINTF_DIR) clean
	@rm -f *.o

fclean: clean
	@rm -f $(NAME)
	@make -C $(LIBFT_DIR) fclean
	@make -C $(PRINTF_DIR) fclean

re: fclean all

.PHONY: clean fclean re

run:
	@chmod 755 $(NAME)
	@./pipex.out file1.txt "ls -la" "wc -w" file2.txt
	@cat file2.txt


TEXT_FILES = file1.txt file2.txt outfile infile
clean_text_files:
	@rm -f $(TEXT_FILES)
	