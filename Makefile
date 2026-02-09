NAME = pipex
BONUS_NAME = pipex_bonus

CC = cc
CFLAGS = -Wall -Wextra -Werror
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
BONUS_SRC_FILES = utils_bonus.c get_exe_path_bonus.c child_processes_bonus.c \
					pipex_bonus_utils_bonus.c pipex_bonus.c
BONUS_SRCS = $(addprefix $(BONUS_DIR)/,$(BONUS_SRC_FILES))
BONUS_OBJ_FILES = $(BONUS_SRCS:.c=.o)
BONUS_OBJS = $(addprefix $(OBJS_DIR)/,$(BONUS_OBJ_FILES))

# -------- Libs --------
LIBFT_DIR = $(LIBS_DIR)/libft
LIBFT = $(LIBFT_DIR)/libft.a

GETNEXTLINE_DIR = $(LIBS_DIR)/getnextline
GETNEXTLINE = $(GETNEXTLINE_DIR)/get_next_line.a

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

# .o files compilation happens in this rule
$(SRC_OBJS_DIR)/%.o: $(MANDATORY_DIR)/%.c  $(SRC_HEADER)
	@mkdir -p $(@D)
	@$(CC) $(CFLAGS) -c $< -o $@

$(LIBFT):
	$(call print_green,Compiling Libft ...)
	@make -C $(LIBFT_DIR) bonus

$(NAME): $(OBJS) $(LIBFT)
	$(call print_green,Compiling Mandatory ...)
	@$(CC) $(CFLAGS) $(OBJS) $(LIBFT) -o $(NAME)

bonus: $(BONUS_NAME)

# .o files compilation happens in this rule
$(BONUS_OBJS_DIR)/%.o: $(BONUS_DIR)/%.c  $(BONUS_HEADER)
	@mkdir -p $(@D)
	@$(CC) $(CFLAGS) -c $< -o $@

$(GETNEXTLINE):
	$(call print_green,Compiling GetNextLine ...)
	@make -C $(GETNEXTLINE_DIR)

$(BONUS_NAME): $(BONUS_OBJS) $(LIBFT) $(GETNEXTLINE)
	$(call print_green,Compiling Bonus ...)
	@$(CC) $(CFLAGS) $(BONUS_OBJS) $(LIBFT) $(GETNEXTLINE) -o $(BONUS_NAME)

# -------- Phonies --------
clean:
	$(call print_red,Cleaning Objects ...)
	@rm -rf $(OBJS_DIR)
	@make -C $(LIBFT_DIR) clean
	@make -C $(GETNEXTLINE_DIR) clean

fclean: clean
	$(call print_red,Cleaning Objects & Executables ...)
	@rm -f $(NAME)
	@rm -f $(BONUS_NAME)
	@make -C $(LIBFT_DIR) fclean
	@make -C $(GETNEXTLINE_DIR) fclean

re: fclean all

.PHONY: all clean fclean re bonus
