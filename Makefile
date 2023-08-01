NAME		:= ircserv

CXX			:= c++
CXXFLAGS	:= -Wall -Werror -Wextra -std=c++98

SRCS		:= main.cpp \
				Client.cpp \
				Channel.cpp \
				Server.cpp \
				Messages.cpp

HEADER		:= Client.hpp \
				Channel.hpp \
				Server.hpp \
				Messages.hpp

M_SRCS		= $(addprefix ./sources/, $(SRCS))
M_HEADER	= $(addprefix ./includes/, $(HEADER))

OBJS		:= $(M_SRCS:%.cpp=%.o)

# ************************************** #
#   COLORS                               #
# ************************************** #
RED			=	\e[91;5;118m
GREEN		=	\e[92;5;118m
YELLOW		=	\e[93;5;226m

# ************************************** #
#   RULES                                #
# ************************************** #

.PHONY: all clean fclean re

$(NAME) : $(OBJS) $(M_HEADER)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $@
	@printf "$(GREEN)Successful compilation\n$(RESET)"
#@printf "$(_SUCCESS) $(GREEN)Successful compilation\n$(RESET)"

%.o: %.cpp $(M_HEADER)
	$(CXX) $(CXXFLAGS) -c $< -o $@

all: $(NAME)

clean:
	$(RM) $(OBJS) 

fclean: clean
	$(RM) $(NAME)

re: fclean all