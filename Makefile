NAME = ft_irc

CPP = c++
CFLAGS = -Wall -Wextra -Werror -std=c++98

SRCS = Src/main.cpp Src/Server[MB].cpp Src/Client.cpp


OBJS = $(SRCS:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CPP) $(CFLAGS) $(OBJS) -o $(NAME)

clean:
	rm -f $(OBJS)

fclean: clean


re: fclean all

.PHONY: all clean fclean re
.SECONDARY: $(OBJS)