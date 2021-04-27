cmake_minimum_required(VERSION 3.19)
project(web_collab)

set(CMAKE_CXX_STANDARD 98)

include_directories(.)
include_directories(libft)

add_executable(web_collab
        libft/ft_atoi.c
        libft/ft_bzero.c
        libft/ft_calloc.c
        libft/ft_isalnum.c
        libft/ft_isalpha.c
        libft/ft_isascii.c
        libft/ft_isdigit.c
        libft/ft_isprint.c
        libft/ft_isspace.c
        libft/ft_itoa.c
        libft/ft_lstadd_back.c
        libft/ft_lstadd_front.c
        libft/ft_lstclear.c
        libft/ft_lstdelone.c
        libft/ft_lstiter.c
        libft/ft_lstlast.c
        libft/ft_lstmap.c
        libft/ft_lstnew.c
        libft/ft_lstsize.c
        libft/ft_memccpy.c
        libft/ft_memchr.c
        libft/ft_memcmp.c
        libft/ft_memcpy.c
        libft/ft_memmove.c
        libft/ft_memset.c
        libft/ft_nbrlen.c
        libft/ft_nbrlen_base.c
        libft/ft_putchar.c
        libft/ft_putchar_fd.c
        libft/ft_putendl_fd.c
        libft/ft_putnbr.c
        libft/ft_putnbr_base.c
        libft/ft_putnbr_fd.c
        libft/ft_putnstr.c
        libft/ft_putstr.c
        libft/ft_putstr_fd.c
        libft/ft_rev_arr.c
        libft/ft_split.c
        libft/ft_strchr.c
        libft/ft_strcmp.c
        libft/ft_strdup.c
        libft/ft_strjoin.c
        libft/ft_strlcat.c
        libft/ft_strlcpy.c
        libft/ft_strlen.c
        libft/ft_strmapi.c
        libft/ft_strncmp.c
        libft/ft_strndup.c
        libft/ft_strnew.c
        libft/ft_strnstr.c
        libft/ft_strrchr.c
        libft/ft_strtrim.c
        libft/ft_substr.c
        libft/ft_tolower.c
        libft/ft_toupper.c
        libft/ft_uitoa_base.c
        libft/libft.h
#        2_client_test.cpp
        Client.cpp
        Client.hpp
#        client_test.cpp
        Config.hpp
        convertConfig.cpp
        main.cpp
        readConfig.cpp
        Request.cpp
        Request.hpp
        Response.hpp
        Server.cpp
        Server.hpp
#        server_test.cpp
        Socket.cpp
        Socket.hpp
        utils.cpp)
