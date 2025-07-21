NAME    = libft.a
CC      = cc
CFLAGS  = -Wall -Wextra -Werror
SRCS    = ft_split.c ft_strdup.c \
		  ft_strlen.c ft_strncmp.c \
		  ft_putstr_fd.c ft_strdup.c \
		  ft_strlcat.c ft_strlcpy.c
OBJDIR  = objs
OBJS    = $(SRCS:%.c=$(OBJDIR)/%.o)
AR      = ar
RM      = rm -f
MKDIR   = mkdir -p

all: $(NAME)
#rcs - flags for ar - archive command:
# r = insert or replace
# c = create
# s = create symbol index (faster linking)
#$@	- the target (e.g. libft.a)
#$^	- all prerequisites (e.g. the .o files to include in the archive)
$(NAME): $(OBJS)
	$(AR) rcs $@ $^

$(OBJDIR)/%.o: %.c
	$(MKDIR) $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) -r $(OBJDIR)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re