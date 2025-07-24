# === Project Settings ===
NAME       = minishell
CC         = cc
CFLAGS     = -Wall -Wextra -Werror
LIBFT_DIR  = libft
SRC_DIR    = src
INC_DIR    = $(SRC_DIR)/includes
OBJDIR     = objs
LIBFT      = $(LIBFT_DIR)/libft.a
INCLUDES   = -I$(LIBFT_DIR) -I$(INC_DIR) -I.

# === Source Files ===
SRCS       = $(SRC_DIR)/main.c \
             $(SRC_DIR)/executor/executor.c \
			 $(SRC_DIR)/executor/executor_helper.c \
			 $(SRC_DIR)/executor/executor_utils.c \
			 $(SRC_DIR)/executor/path.c \
             $(SRC_DIR)/pipes/pipes.c \
			 $(SRC_DIR)/pipes/pipes_helper.c \
             $(SRC_DIR)/parser.c \
             $(SRC_DIR)/utils.c \
			 $(SRC_DIR)/signal.c \
			 $(SRC_DIR)/redirection.c

# === Object Files ===
OBJS       = $(SRCS:$(SRC_DIR)/%.c=$(OBJDIR)/%.o)

# === Commands ===
RM         = rm -f
MKDIR      = mkdir -p
MAKE_LIBFT = make -C $(LIBFT_DIR)

# === Build Rules ===
all: $(NAME)

$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) -lreadline -o $(NAME)

$(LIBFT):
	$(MAKE_LIBFT)

$(OBJDIR)/%.o: $(SRC_DIR)/%.c
	@$(MKDIR) $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# === Clean Rules ===
clean:
	$(MAKE_LIBFT) clean
	$(RM) -r $(OBJDIR)

fclean: clean
	$(MAKE_LIBFT) fclean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
