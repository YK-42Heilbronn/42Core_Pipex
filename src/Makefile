# NAME = pipex.a
# NAME = pipex_asan.out
NAME = pipex
BONUS_NAME = pipex_bonus

CFLAGS = -Wall -Wextra -Werror -g
# CFLAGS = -Wall -Wextra -Werror -fsanitize=address -g

MANDATORY_DIR = mandatory
LIBS_DIR = libs
BONUS_DIR = bonus
OBJS_DIR = objs
SRC_OBJS_DIR = $(OBJS_DIR)/$(MANDATORY_DIR)
BONUS_OBJS_DIR = $(OBJS_DIR)/$(BONUS_DIR)

# ------- Mandatory -------
SRC_HEADER_FILES = pipex.h
SRC_HEADER = $(MANDATORY_DIR)/$(SRC_HEADER_FILES)
SRC_FILES = utils.c get_exe_path.c pipex.c
SRCS = $(addprefix $(MANDATORY_DIR)/,$(SRC_FILES))
OBJ_FILES = $(SRCS:.c=.o)  # creates .o file names as .c, does not compile
OBJS = $(addprefix $(OBJS_DIR)/,$(OBJ_FILES))

# ------- Bonus -------
BONUS_HEADER_FILES = pipex_bonus.h
BONUS_HEADER = $(BONUS_DIR)/$(BONUS_HEADER_FILES)
BONUS_SRC_FILES = utils_bonus.c file_utils_bonus.c get_exe_path_bonus.c \
				  child_processes_bonus.c pipex_bonus.c
BONUS_SRCS = $(addprefix $(BONUS_DIR)/,$(BONUS_SRC_FILES))
BONUS_OBJ_FILES = $(BONUS_SRCS:.c=.o)
BONUS_OBJS = $(addprefix $(OBJS_DIR)/,$(BONUS_OBJ_FILES))

# -------- Libs --------
LIBFT_DIR = $(LIBS_DIR)/libft
LIBFT = $(LIBFT_DIR)/libft.a

GETNEXTLINE_DIR = $(LIBS_DIR)/getnextline
GETNEXTLINE = $(GETNEXTLINE_DIR)/get_next_line.a

# PRINTF_DIR = $(LIBS_DIR)/printf
# PRINTF = $(PRINTF_DIR)/libftprintf.a

# --------- Functions -----------
# ===Colors===
PURPLE	= \033[95m
GREEN		= \033[92m
RED 		= \033[91m
YELLOW	= \033[33m
RESET		= \033[0m

define print_green
	@printf "$(GREEN)$(1)\n$(RESET)"
endef

define print_red
	@printf "$(RED)$(1)\n$(RESET)"
endef

# --------- Rules ---------
all: $(NAME)

$(SRC_OBJS_DIR)/%.o: $(MANDATORY_DIR)/%.c  $(SRC_HEADER)# .o files compilation happens in this rule
# 	$(call print_green,Compiling Mandatory Objects ...)
	@mkdir -p $(@D)
	@cc $(CFLAGS) -c $< -o $@

$(LIBFT):
	$(call print_green,Compiling Libft ...)
	@make -C $(LIBFT_DIR) bonus

# $(PRINTF):
# 	@make -C $(PRINTF_DIR)

$(NAME): $(OBJS) $(LIBFT)
# 	LINK: 1
#   NOTE: following command is used to run the specific script
	$(call print_green,Compiling Mandatory ...)
	@cc $(CFLAGS) $(OBJS) $(LIBFT) -o $(NAME) 

# $(NAME): $(OBJS) $(LIBFT) $(PRINTF)
# #   NOTE: following commands used to create the static lib of the specific project
# # 	@cp $(LIBFT) $(NAME)
# # 	@cp $(PRINTF) $(NAME)
# 	ar rcs $(NAME) $(LIBFT) $(PRINTF) $(OBJS)    
# 	ranlib $(NAME)

bonus: $(BONUS_NAME)

$(BONUS_OBJS_DIR)/%.o: $(BONUS_DIR)/%.c  $(BONUS_HEADER)# .o files compilation happens in this rule
# 	$(call print_green,Compiling Bonus Objects ...)
	@mkdir -p $(@D)
	@cc $(CFLAGS) -c $< -o $@

$(GETNEXTLINE):
	$(call print_green,Compiling GetNextLine ...)
	@make -C $(GETNEXTLINE_DIR)

$(BONUS_NAME): $(BONUS_OBJS) $(LIBFT) $(GETNEXTLINE)
	$(call print_green,Compiling Bonus ...)
	@cc $(CFLAGS) $(BONUS_OBJS) $(LIBFT) $(GETNEXTLINE) -o $(BONUS_NAME)

# -------- Phonies --------
clean:
	$(call print_red,Cleaning Objects ...)
	@rm -rf $(OBJS_DIR)
	@make -C $(LIBFT_DIR) clean
	@make -C $(GETNEXTLINE_DIR) clean
# 	@make -C $(PRINTF_DIR) clean

fclean: clean
	$(call print_red,Cleaning Objects & Executables ...)
	@rm -f $(NAME)
	@rm -f $(BONUS_NAME)
	@make -C $(LIBFT_DIR) fclean
	@make -C $(GETNEXTLINE_DIR) fclean
# 	@make -C $(PRINTF_DIR) fclean

re: fclean all

.PHONY: all clean fclean re bonus

run:
	@chmod 755 $(NAME)
	@./pipex.out file1.txt "ls -la" "wc -w" file2.txt
	@cat file2.txt


TEXT_FILES = file1.txt file2.txt outfile infile
clean_text_files:
	@rm -f $(TEXT_FILES)
	