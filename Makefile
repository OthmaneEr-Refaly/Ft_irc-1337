# NAME = ft_irc

# CXX = c++
# CXXFLAGS = -Wall -Wextra -Werror -std=c++98

# SRCS = Src/main.cpp \
		# Src/Client.cpp \
		# Src/Channel.cpp \
		# Src/Server/Server.cpp \
		# Src/Server/ServerCore.cpp \
		# Src/Server/ServerPoll.cpp \
		# Src/Server/ServerIO.cpp\
		# Src/Server/ServerInit.cpp


# OBJS = $(SRCS:.cpp=.o)

# all: credit $(NAME)

# $(NAME): $(OBJS)
	# $(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)

# clean:
	# rm -f $(OBJS)

# fclean: clean
	# rm -f $(NAME)

# re: fclean all

# .PHONY: all clean fclean re
# .SECONDARY: $(OBJS)

# #Print Credit
# credit:
	# @echo " ┏━━━┓┏━━━━┓    ┏━━┓┏━━━┓┏━━━┓"
	# @echo " ┃┏━━┛┃┏┓┏┓┃    ┗┫┣┛┃┏━┓┃┃┏━┓┃"
	# @echo " ┃┗━━┓┗┛┃┃┗┛     ┃┃ ┃┗━┛┃┃┃ ┗┛"
	# @echo " ┃┏━━┛  ┃┃       ┃┃ ┃┏┓┏┛┃┃ ┏┓"
	# @echo " ┃┃    ┏┛┗┓┏━━━┓┏┫┣┓┃┃┃┗┓┃┗━┛┃"
	# @echo " ┗┛    ┗━━┛┗━━━┛┗━━┛┗┛┗━┛┗━━━┛"
	# @echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
	# @echo "━━━━━━━━━┃ Leader d project howa : \033[1;91mMontasiiiir\033[m ┃━━━━━━━━━"
	# @echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"

NAME        = ft_irc
CXX         = c++
CXXFLAGS    = -Wall -Wextra -Werror -std=c++98

# Colors
RESET       = \033[0m
BOLD        = \033[1m
DIM         = \033[2m
UNDERLINE   = \033[4m
BLINK       = \033[5m

# Regular Colors
BLACK       = \033[30m
RED         = \033[31m
GREEN       = \033[32m
YELLOW      = \033[33m
BLUE        = \033[34m
MAGENTA     = \033[35m
CYAN        = \033[36m
WHITE       = \033[37m

# Bold Colors
BRED        = \033[1;31m
BGREEN      = \033[1;32m
BYELLOW     = \033[1;33m
BBLUE       = \033[1;34m
BMAGENTA    = \033[1;35m
BCYAN       = \033[1;36m
BWHITE      = \033[1;37m

# Background Colors
BG_BLACK    = \033[40m
BG_RED      = \033[41m
BG_GREEN    = \033[42m
BG_YELLOW   = \033[43m
BG_BLUE     = \033[44m
BG_MAGENTA  = \033[45m
BG_CYAN     = \033[46m
BG_WHITE    = \033[47m

# Source Files
SRCS        = Src/main.cpp \
              Src/Client.cpp \
              Src/Channel.cpp \
              Src/Server/Server.cpp \
              Src/Server/ServerCore.cpp \
              Src/Server/ServerPoll.cpp \
              Src/Server/ServerIO.cpp \
              Src/Server/ServerInit.cpp

OBJS        = $(SRCS:.cpp=.o)
TOTAL_FILES = $(words $(SRCS))
COMPILED    = 0

# ═══════════════════════════════════════════════════════════════════════════ #
#                                  RULES                                      #
# ═══════════════════════════════════════════════════════════════════════════ #

all: banner $(NAME) success

$(NAME): $(OBJS)
	@echo "$(BMAGENTA)"
	@echo "╔════════════════════════════════════════════════════════════════╗"
	@echo "║                        🔗 LINKING PHASE 🔗                    ║"
	@echo "╚════════════════════════════════════════════════════════════════╝"
	@echo "$(RESET)"
	@printf "$(BCYAN)⚡ Creating executable: $(BWHITE)$(NAME)$(RESET)\n"
	@$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)
	@echo "$(BGREEN)✅ Executable created successfully!$(RESET)"

%.o: %.cpp
	@$(eval COMPILED=$(shell echo $$(($(COMPILED)+1))))
	@printf "$(BYELLOW)[$(COMPILED)/$(TOTAL_FILES)]$(RESET) "
	@printf "$(BCYAN)🔨 Compiling: $(RESET)$(DIM)$<$(RESET) "
	@$(CXX) $(CXXFLAGS) -c $< -o $@
	@printf "$(BGREEN)✅$(RESET)\n"

clean:
	@echo "$(BRED)"
	@echo "╔════════════════════════════════════════════════════════════════╗"
	@echo "║                        🧹 CLEANING UP 🧹                      ║"
	@echo "╚════════════════════════════════════════════════════════════════╝"
	@echo "$(RESET)"
	@printf "$(BYELLOW)🗑️  Removing object files...$(RESET)\n"
	@rm -f $(OBJS)
	@echo "$(BGREEN)✅ Object files cleaned!$(RESET)"

fclean: clean
	@printf "$(BYELLOW)🗑️  Removing executable: $(BWHITE)$(NAME)$(RESET)\n"
	@rm -f $(NAME)
	@echo "$(BGREEN)✅ Everything cleaned!$(RESET)"

re: fclean all

# ═══════════════════════════════════════════════════════════════════════════ #
#                              DECORATIVE RULES                              #
# ═══════════════════════════════════════════════════════════════════════════ #

banner:
	@clear
	@echo "$(BMAGENTA)"
	@echo "╔══════════════════════════════════════════════════════════════════════════╗"
	@echo "║                                                                          ║"
	@echo "║    $(BWHITE)███████╗████████╗      ██╗██████╗  ██████╗$(BMAGENTA)                      ║"
	@echo "║    $(BWHITE)██╔════╝╚══██╔══╝      ██║██╔══██╗██╔════╝$(BMAGENTA)                      ║"
	@echo "║    $(BWHITE)█████╗     ██║         ██║██████╔╝██║     $(BMAGENTA)                      ║"
	@echo "║    $(BWHITE)██╔══╝     ██║         ██║██╔══██╗██║     $(BMAGENTA)                      ║"
	@echo "║    $(BWHITE)██║        ██║███████╗ ██║██║  ██║╚██████╗$(BMAGENTA)                      ║"
	@echo "║    $(BWHITE)╚═╝        ╚═╝╚══════╝ ╚═╝╚═╝  ╚═╝ ╚═════╝$(BMAGENTA)                      ║"
	@echo "║                                                                          ║"
	@echo "║    $(BCYAN)━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━$(BMAGENTA)    ║"
	@echo "║                                                                          ║"
	@echo "║    $(BGREEN)🚀 Internet Relay Chat Server Implementation$(BMAGENTA)                   ║"
	@echo "║    $(BYELLOW)👨‍💻 Project Leader howa: $(BRED)Montasiiiir$(BMAGENTA)                                ║"
	@echo "║    $(BCYAN)📅 Built with: C++98 Standard$(BMAGENTA)                                    ║"
	@echo "║    $(BWHITE)⚡ Compiler: $(CXX) with $(CXXFLAGS)$(BMAGENTA)                ║"
	@echo "║                                                                          ║"
	@echo "╚══════════════════════════════════════════════════════════════════════════╝"
	@echo "$(RESET)"
	@echo ""
	@echo "$(BBLUE)📦 Starting compilation process...$(RESET)"
	@echo ""

success:
	@echo ""
	@echo "$(BGREEN)"
	@echo "╔══════════════════════════════════════════════════════════════════════════╗"
	@echo "║                                                                          ║"
	@echo "║    $(BWHITE)🎉 COMPILATION SUCCESSFUL! 🎉$(BGREEN)                                    ║"
	@echo "║                                                                          ║"
	@echo "║    $(BCYAN)✨ Your ft_irc server is ready to rock! ✨$(BGREEN)                       ║"
	@echo "║                                                                          ║"
	@echo "║    $(BYELLOW)Usage: ./$(NAME) <port> <password>$(BGREEN)                              ║"
	@echo "║                                                                          ║"
	@echo "╚══════════════════════════════════════════════════════════════════════════╝"
	@echo "$(RESET)"

info:
	@echo "$(BCYAN)"
	@echo "╔══════════════════════════════════════════════════════════════════════════╗"
	@echo "║                          📋 PROJECT INFO 📋                             ║"
	@echo "╚══════════════════════════════════════════════════════════════════════════╝"
	@echo "$(RESET)"
	@echo "$(BWHITE)Project:$(RESET)     $(BGREEN)$(NAME)$(RESET)"
	@echo "$(BWHITE)Compiler:$(RESET)    $(BYELLOW)$(CXX)$(RESET)"
	@echo "$(BWHITE)Flags:$(RESET)       $(BRED)$(CXXFLAGS)$(RESET)"
	@echo "$(BWHITE)Files:$(RESET)       $(BCYAN)$(TOTAL_FILES) source files$(RESET)"
	@echo "$(BWHITE)Author:$(RESET)      $(BMAGENTA)Montasiiiir$(RESET)"
	@echo ""

help:
	@echo "$(BBLUE)"
	@echo "╔══════════════════════════════════════════════════════════════════════════╗"
	@echo "║                            🆘 HELP MENU 🆘                              ║"
	@echo "╚══════════════════════════════════════════════════════════════════════════╝"
	@echo "$(RESET)"
	@echo "$(BWHITE)Available commands:$(RESET)"
	@echo ""
	@echo "  $(BGREEN)make$(RESET) or $(BGREEN)make all$(RESET)   - Build the project"
	@echo "  $(BRED)make clean$(RESET)        - Remove object files"
	@echo "  $(BRED)make fclean$(RESET)       - Remove object files and executable"
	@echo "  $(BYELLOW)make re$(RESET)           - Rebuild everything (fclean + all)"
	@echo "  $(BCYAN)make info$(RESET)         - Show project information"
	@echo "  $(BMAGENTA)make help$(RESET)         - Show this help menu"
	@echo ""

# Progress bar function (just for show)
progress:
	@echo -n "$(BGREEN)Building: $(RESET)["
	@for i in {1..50}; do echo -n "█"; sleep 0.02; done
	@echo "] $(BGREEN)100%$(RESET)"

.PHONY: all clean fclean re banner success info help progress
.SECONDARY: $(OBJS)
