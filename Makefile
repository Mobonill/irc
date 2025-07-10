NAME = ircserv
CC = c++
FLAGS = -Wall -Wextra -Werror -g #-fsanitize=address
F98 = -std=c++98
D_BONUS = -DBONUS=1
L_BONUS = 
SRC =	main.cpp \
		Server.cpp \
		Client.cpp \
		Channel.cpp \
		Commands.cpp \
		Utils.cpp \
		UtilsCmds.cpp \
		Bot.cpp

OBJ = $(SRC:.cpp=.o)

$(NAME): $(OBJ)
	$(CC) $(FLAGS) $(F98) -o $(NAME) $(OBJ) $(L_BONUS)

%.o:%.cpp
	$(CC) $(FLAGS) $(F98) -I. -c $< -o $@

bonus: FLAGS += $(D_BONUS)
	L_BONUS += -lcurl

bonus: re

all: $(NAME)

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re bonus
