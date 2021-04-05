NAME = webserv

INCLUDE = ./includes/
HEADERS = Client.hpp	Server.hpp	Config.hpp	Socket.hpp
#HEADERS = $(addprefix $(INCLUDE), $(HEADERS))

SRC_DIR = ./srcs/
SRCS = utils.cpp	Socket.cpp	convertConfig.cpp	readConfig.cpp	Server.cpp	Client.cpp	main.cpp
#SRC = $(addprefix $(SRC_DIR), $(SRC))

OBJ_DIR = ./obj/
OBJECTS = $(patsubst %.cpp, $(OBJ_DIR)%.o, $(SRCS))

CL = clang++
FLAGS = -std=c++98 -Wall -Wextra -Werror

all: $(NAME)

$(NAME): $(OBJ_DIR) $(OBJECTS)
	@$(CL) $(OBJECTS) -o $@ -I $(INCLUDE)

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)
	#@mkdir -p $(OBJ_DIR)$(SRCS)

#$(OBJ_DIR)%.o : $(SRC_DIR)%.cpp $(HEADERS)
$(OBJ_DIR)%.o : %.cpp $(HEADERS)
	@$(CL) $(FLAGS) -c $< -o $@ -I $(INCLUDE)

clean:
	@rm -rf $(OBJ_DIR)

fclean: clean
	@rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re
