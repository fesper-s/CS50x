NAME = sbash

CFLAGS = -g -Wall -Wextra -Werror

CPPFLAGS = -I /opt/homebrew/Cellar/readline/8.2.13/include

LDFLAGS = -L /opt/homebrew/Cellar/readline/8.2.13/lib

CPPFLAGS += -I ~/.brew/opt/readline/include

LDFLAGS += -L ~/.brew/opt/readline/lib

SRCS	= ./main.c ./error.c ./more_error.c ./signal.c ./utils.c \
		  ./sbash.c ./check.c ./init_struct.c ./list_utils.c ./builtins.c \
		  ./echo.c ./chdir.c ./export.c ./unset.c ./builtins_utils.c \
		  ./memory.c ./init_expand.c ./expand.c ./expand_utils.c ./exec.c \
		  ./heredoc.c ./file_utils.c ./check_utils.c

OBJS	= $(SRCS:.c=.o)

$(NAME):	$(OBJS)
			cc $^ $(CFLAGS) -o $@ -lreadline $(CPPFLAGS) $(LDFLAGS)

all:		$(NAME)

clean:
			rm -f $(OBJS)

fclean:		clean
			rm -f $(NAME)
			rm -rf $(NAME).dSYM

re:			fclean all

.PHONY:		all clean fclean re
