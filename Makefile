NAME = ft_irc

CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98

SRCS = Src/main.cpp \
		Src/Client.cpp \
		Src/Channel.cpp \
		Src/Server/Server.cpp \
		Src/Server/ServerCore.cpp \
		Src/Server/ServerPoll.cpp \
		Src/Server/ServerIO.cpp\
		Src/Server/ServerInit.cpp


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