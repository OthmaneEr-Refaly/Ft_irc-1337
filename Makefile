NAME = ft_irc

CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98

SRCS = Src/main.cpp \
		Src/Server.cpp \
		Src/Client.cpp \
		Src/Channel.cpp


OBJS = $(SRCS:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
.SECONDARY: $(OBJS)