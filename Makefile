NAME = ircserv
CC = c++
FLAGS = -Wall -Wextra -Werror -g #-fsanitize=address
F98 = -std=c++98
D_BONUS = -DBONUS=1
L_BONUS = 
SRC =	main.cpp \
	Server.cpp \
	Commands.cpp \
	Channel.cpp \
	Connexion.cpp \
	Client.cpp \
	Utils.cpp \
	UtilsCmds.cpp

SRC_BONUS = ServerBot.cpp \
	Bot.cpp \
	BotMsg.cpp \
	UserData.cpp

OBJ = $(SRC:.cpp=.o)

OBJ_BONUS = $(SRC_BONUS:.cpp=.o)

$(NAME): $(OBJ)
	$(CC) $(FLAGS) $(F98) -o $(NAME) $(OBJ) $(L_BONUS)

%.o:%.cpp
	$(CC) $(FLAGS) $(F98) -I. -c $< -o $@

bonus:
	$(MAKE) FLAGS="$(FLAGS) $(D_BONUS)" L_BONUS="-lcurl" SRC="$(SRC) $(SRC_BONUS)" re

bonus: re

all: $(NAME)

clean:
	rm -f $(OBJ) $(OBJ_BONUS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re bonus
