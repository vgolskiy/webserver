NAME = webserv

HDR_DIR = ./includes/
HDR_LIST =	webserv.hpp \
			Serv.hpp \
			Sock.hpp \
			ErrnoException.hpp \
			Client.hpp \
			Request.hpp \
			Response.hpp \
			Http.hpp \
			error_handler.hpp \
			conf_parser.hpp \
			conf_utils.hpp \
			webserv_utils.hpp \
			signal_handler.hpp
HDR = $(addprefix $(HDR_DIR), $(HDR_LIST))

SRC_DIR = ./
SRC_DIR_SRCS = srcs/
SRC_LIST =	srcs/main.cpp \
			srcs/Serv.cpp \
			srcs/Sock.cpp \
			srcs/ErrnoException.cpp \
			srcs/Client.cpp \
			srcs/Request.cpp \
			srcs/Response.cpp \
			srcs/Http.cpp \
			srcs/error_handler.cpp \
			srcs/conf_parser.cpp \
			srcs/conf_utils.cpp \
			srcs/webserv_utils.cpp \
			srcs/signal_handler.cpp

SRC = $(addprefix $(SRC_DIR), $(SRC_LIST))

OBJ_DIR = ./obj/
OBJ_LIST = $(patsubst %.cpp, $(OBJ_DIR)%.o, $(SRC_LIST))

CL = clang++
FLAGS = -std=c++98 -Wall -Wextra -Werror
INCLUDE = -I$(HDR_DIR)

.PHONY: all parrot clean fclean re

all: parrot $(NAME)

parrot:
	@curl -s -m 5 parrot.live || true
	@clear

$(NAME): $(OBJ_DIR) $(OBJ_LIST)
	@$(CL) $(OBJ_LIST) -o $@ $(INCLUDE)
	@echo "$(NAME) created"

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(OBJ_DIR)$(SRC_DIR_SRCS)

$(OBJ_DIR)%.o : $(SRC_DIR)%.cpp $(HDR)
	@$(CL) $(FLAGS) -c $< -o $@ $(INCLUDE)

clean:
	@rm -rf $(OBJ_DIR)
	@echo "$(OBJ_DIR) deleted"
	@rm -rf content/file_should_exist_after
	@rm -rf content/multiple_same

fclean: clean
	@rm -rf $(NAME)
	@echo "$(NAME) deleted"

re: fclean all
