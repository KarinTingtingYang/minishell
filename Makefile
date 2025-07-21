# NAME       = pipex
# CC         = cc
# CFLAGS     = -Wall -Wextra -Werror
# LIBFT_DIR  = libft
# SRC_DIR    = src
# OBJDIR     = objs
# LIBFT      = $(LIBFT_DIR)/libft.a
# INCLUDES   = -I$(LIBFT_DIR) -I$(SRC_DIR) -I.

# SRCS       = $(SRC_DIR)/main.c \
# 			 $(SRC_DIR)/pipex_helper.c \
# 			 $(SRC_DIR)/utils.c

# OBJS       = $(SRCS:$(SRC_DIR)/%.c=$(OBJDIR)/%.o)

# RM         = rm -f
# MKDIR      = mkdir -p
# MAKE_LIBFT = make -C $(LIBFT_DIR)

# all: $(NAME)

# $(NAME): $(OBJS) $(LIBFT)
# 	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) -o $(NAME)

# $(LIBFT):
# 	$(MAKE_LIBFT)

# $(OBJDIR)/%.o: $(SRC_DIR)/%.c
# 	@$(MKDIR) $(dir $@)
# 	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# clean:
# 	$(MAKE_LIBFT) clean
# 	$(RM) -r $(OBJDIR)

# fclean: clean
# 	$(MAKE_LIBFT) fclean
# 	$(RM) $(NAME)

# re: fclean all

# .PHONY: all clean fclean re


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
             $(SRC_DIR)/pipex_helper.c \
             $(SRC_DIR)/utils.c \
             $(SRC_DIR)/executor.c

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
