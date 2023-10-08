CC = clang
CFLAGS = -Wall -Werror -Wextra  -I./includes -I./src/libft

SRC = src/main.c src/helpOptions.c \
		src/parser.c \
		src/creationEchoRequest.c \

EXECUTABLE = ft_ping
LIBFTF = ./src/libft
LDFLAGS = -L$(LIBFTF)
LIBFT = -lft
OBJ = $(SRC:.c=.o)

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJ)
	@make -C $(LIBFTF)
	$(CC) $(CFLAGS) -o $@ $(OBJ) $(LDFLAGS) $(LIBFT)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	@make clean -C $(LIBFTF)
	rm -f $(OBJ)

fclean: clean
	@make fclean -C $(LIBFTF)
	rm -f $(EXECUTABLE)

re:				fclean $(EXECUTABLE)

.PHONY: all clean fclean re
