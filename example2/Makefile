NAME = web_server
SRCS = ClassParser.cpp client.cpp Http.cpp parser_check.cpp parser_errors.cpp parser_set_data.cpp \
      parser_utils.cpp parser_mime_types.cpp ClassGenErrorPage.cpp big_changus.cpp Server.cpp Socket.cpp main.cpp HttpHeaders.cpp \
       RequestParser.cpp
OBJ = $(SRCS:.cpp=.o)
CXXFLAGS = -MMD -Wall -Wextra -Werror --std=c++98
CXX = clang++
INCLUDES = ./

all: $(NAME)

$(NAME): $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(NAME)
%.o:%.cpp
	$(CXX) $(CXXFLAGS) -I $(INCLUDES) -c $< -o $@
clean:
	/bin/rm -rf $(OBJ)
	/bin/rm -rf $(OBJ:.o=.d)
fclean: clean
	/bin/rm -rf $(NAME)
-include = $(OBJ:.o=.d)
re: fclean all