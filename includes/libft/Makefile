# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jihalee <jihalee@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/05/01 02:52:38 by jihalee           #+#    #+#              #
#    Updated: 2023/08/17 06:38:45 by jihalee          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libft.a

SRCS = ft_atoi.c ft_isprint.c ft_lstlast.c ft_memmove.c ft_split.c ft_strlen.c ft_tolower.c\
ft_bzero.c ft_itoa.c ft_lstmap.c ft_memset.c ft_strchr.c ft_strmapi.c ft_toupper.c\
ft_calloc.c ft_lstadd_back.c ft_lstnew.c ft_printf.c ft_strdup.c ft_strncmp.c get_next_line.c\
ft_isalnum.c ft_lstadd_front.c ft_lstsize.c ft_putchar_fd.c ft_striteri.c ft_strnstr.c get_next_line_utils.c\
ft_isalpha.c ft_lstclear.c ft_memchr.c ft_putendl_fd.c ft_strjoin.c ft_strrchr.c\
ft_isascii.c ft_lstdelone.c ft_memcmp.c ft_putnbr_fd.c ft_strlcat.c ft_strtrim.c print_formats.c\
ft_isdigit.c ft_lstiter.c ft_memcpy.c ft_putstr_fd.c ft_strlcpy.c ft_substr.c ft_atoi_base.c ft_atol.c\

OBJS = ${SRCS:.c=.o}

CC = gcc

CFLAGS = -Wextra -Werror -Wall

INCLUDE =

all : ${NAME}

.c.o :
	${CC} ${CFLAGS} -c $< -o $@

$(NAME) : ${OBJS}
	ar -rcs ${NAME} ${OBJS}

clean :
	rm -f *.o

fclean : clean
	rm -f ${NAME}

re : fclean all

.PHONY : clean fclean all re
