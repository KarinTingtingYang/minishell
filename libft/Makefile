NAME    = libft.a
CC      = cc
CFLAGS  = -Wall -Wextra -Werror
SRCS = ft_isalpha.c ft_isdigit.c ft_isalnum.c ft_isascii.c \
       ft_isprint.c ft_strlen.c ft_memset.c ft_bzero.c \
       ft_memcpy.c ft_memmove.c ft_strlcpy.c ft_strlcat.c \
       ft_calloc.c ft_strdup.c ft_toupper.c ft_tolower.c \
       ft_strchr.c ft_strncmp.c ft_memchr.c ft_memcmp.c \
       ft_strnstr.c ft_atoi.c ft_strrchr.c ft_substr.c \
       ft_strjoin.c ft_strtrim.c ft_split.c ft_itoa.c \
       ft_strmapi.c ft_striteri.c ft_putchar_fd.c \
       ft_putstr_fd.c ft_putendl_fd.c ft_putnbr_fd.c
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