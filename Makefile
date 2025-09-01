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
			 $(SRC_DIR)/executor/precheck_line.c \
             $(SRC_DIR)/executor/executor.c \
             $(SRC_DIR)/executor/executor_helper.c \
             $(SRC_DIR)/executor/executor_cleanup.c \
			 $(SRC_DIR)/executor/executor_error.c \
             $(SRC_DIR)/executor/path.c \
			 $(SRC_DIR)/executor/prepare_command_execution.c \
			 $(SRC_DIR)/executor/prepare_pipeline_commands.c \
			 $(SRC_DIR)/executor/execute_prepared_command.c \
			 $(SRC_DIR)/executor/execute_command_types.c \
			 $(SRC_DIR)/builtins/builtins.c \
			 $(SRC_DIR)/builtins/builtins_io.c \
			 $(SRC_DIR)/builtins/cd_pwd.c \
			 $(SRC_DIR)/builtins/echo.c \
			 $(SRC_DIR)/builtins/exit.c \
			 $(SRC_DIR)/builtins/env.c \
			 $(SRC_DIR)/builtins/export.c \
			 $(SRC_DIR)/builtins/unset.c \
             $(SRC_DIR)/pipes/pipes.c \
             $(SRC_DIR)/pipes/pipes_helper.c \
             $(SRC_DIR)/parser/parser.c \
			 $(SRC_DIR)/parser/parser_helper.c \
             $(SRC_DIR)/parser/command_create.c \
             $(SRC_DIR)/parser/command_free.c \
			 $(SRC_DIR)/parser/lexer.c \
			 $(SRC_DIR)/parser/lexer_helper.c \
			 $(SRC_DIR)/parser/expander.c \
			 $(SRC_DIR)/parser/expander_helper_1.c \
			 $(SRC_DIR)/parser/expander_helper_2.c \
			 $(SRC_DIR)/parser/expander_helper_3.c \
			 $(SRC_DIR)/parser/expander_helper_4.c \
             $(SRC_DIR)/utils.c \
             $(SRC_DIR)/signal/signal.c \
             $(SRC_DIR)/signal/signal_utils.c \
             $(SRC_DIR)/redirection/redirection.c \
             $(SRC_DIR)/redirection/redirection_utils.c \
			 $(SRC_DIR)/redirection/heredoc.c \
             $(SRC_DIR)/env/env_utils.c \
			 $(SRC_DIR)/env/env_init.c

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



# # === Project Settings ===
# NAME       = minishell
# CC         = cc
# CFLAGS     = -Wall -Wextra -Werror
# LIBFT_DIR  = libft
# SRC_DIR    = src
# INC_DIR    = $(SRC_DIR)/includes
# OBJDIR     = objs
# LIBFT      = $(LIBFT_DIR)/libft.a
# INCLUDES   = -I$(LIBFT_DIR) -I$(INC_DIR) -I.

# # --- Platform detection ---
# UNAME_S := $(shell uname -s)

# # Readline (headers + libs)
# # On macOS: use Homebrew's readline
# ifeq ($(UNAME_S),Darwin)
# READLINE_PREFIX := $(shell \
# 	if command -v brew >/dev/null 2>&1; then brew --prefix readline; \
# 	elif [ -d /opt/homebrew/opt/readline ]; then echo /opt/homebrew/opt/readline; \
# 	elif [ -d /usr/local/opt/readline ]; then echo /usr/local/opt/readline; \
# 	fi)
# READLINE_INC    := -I$(READLINE_PREFIX)/include
# READLINE_LIB    := -L$(READLINE_PREFIX)/lib -lreadline -lncurses
# else
# # On Linux: system readline; ncurses helps satisfy termcap symbols on some distros
# READLINE_INC    :=
# READLINE_LIB    := -lreadline -lncurses
# endif

# # === Source Files ===
# SRCS       = $(SRC_DIR)/main.c \
#              $(SRC_DIR)/executor/executor.c \
#              $(SRC_DIR)/executor/executor_helper.c \
#              $(SRC_DIR)/executor/executor_utils.c \
#              $(SRC_DIR)/executor/path.c \
#              $(SRC_DIR)/builtins/builtins.c \
#              $(SRC_DIR)/builtins/builtins_io.c \
#              $(SRC_DIR)/builtins/cd_pwd.c \
#              $(SRC_DIR)/builtins/echo.c \
#              $(SRC_DIR)/builtins/exit.c \
#              $(SRC_DIR)/builtins/env.c \
#              $(SRC_DIR)/builtins/export.c \
#              $(SRC_DIR)/builtins/unset.c \
#              $(SRC_DIR)/pipes/pipes.c \
#              $(SRC_DIR)/pipes/pipes_helper.c \
#              $(SRC_DIR)/parser/parser.c \
#              $(SRC_DIR)/parser/parser_helper.c \
#              $(SRC_DIR)/parser/command_create.c \
#              $(SRC_DIR)/parser/command_free.c \
#              $(SRC_DIR)/parser/lexer.c \
#              $(SRC_DIR)/parser/lexer_helper.c \
#              $(SRC_DIR)/parser/expander.c \
#              $(SRC_DIR)/parser/expander_helper_1.c \
#              $(SRC_DIR)/parser/expander_helper_2.c \
#              $(SRC_DIR)/parser/expander_helper_3.c \
#              $(SRC_DIR)/parser/expander_helper_4.c \
#              $(SRC_DIR)/utils.c \
#              $(SRC_DIR)/signal/signal.c \
#              $(SRC_DIR)/signal/signal_utils.c \
#              $(SRC_DIR)/redirection/redirection.c \
#              $(SRC_DIR)/redirection/redirection_utils.c \
#              $(SRC_DIR)/redirection/heredoc.c \
#              $(SRC_DIR)/env/env_utils.c \
#              $(SRC_DIR)/env/env_init.c

# # === Object Files ===
# OBJS       = $(SRCS:$(SRC_DIR)/%.c=$(OBJDIR)/%.o)

# # === Commands ===
# RM         = rm -f
# MKDIR      = mkdir -p
# MAKE_LIBFT = make -C $(LIBFT_DIR)

# # === Build Rules ===
# all: $(NAME)

# $(NAME): $(OBJS) $(LIBFT)
# 	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) $(READLINE_LIB) -o $(NAME)

# $(LIBFT):
# 	$(MAKE_LIBFT)

# $(OBJDIR)/%.o: $(SRC_DIR)/%.c
# 	@$(MKDIR) $(dir $@)
# 	$(CC) $(CFLAGS) $(INCLUDES) $(READLINE_INC) -c $< -o $@

# # === Clean Rules ===
# clean:
# 	$(MAKE_LIBFT) clean
# 	$(RM) -r $(OBJDIR)

# fclean: clean
# 	$(MAKE_LIBFT) fclean
# 	$(RM) $(NAME)

# re: fclean all

# .PHONY: all clean fclean re
