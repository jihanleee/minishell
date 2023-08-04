
NAME		= minishell
LIBFT		= libft.a

CC			= gcc
CFLAGS		= -Wall -Wextra -g3 #-Werror commented out for now, erase it before submission
RM			= rm -f

I_PATH		= ./includes/
L_PATH		= ./includes/libft/ #libft should be replaced by 'your teammate's libft' before submission to match the 42 header author name.

INCLUDES	= -I ${I_PATH} -I ${L_PATH}

SRCS		= ./srcs/tokenizer.c \
				./srcs/parser.c \
				./srcs/find_param.c \
				./srcs/expander.c \
				./srcs/signal.c \
				./srcs/echo.c \
				./srcs/env.c \
				./srcs/exit.c \
				./srcs/export.c \
				./srcs/pwd.c \
				./srcs/unset.c \
				./srcs/cd.c \
				./srcs/exec.c \
				./srcs/main.c
				#./srcs/builtin/cd.c \
				#./srcs/main.c \
				# change or add files if needed
OBJS		= ${SRCS:.c=.o}

all: ${NAME}

%.o: %.c 
	${CC} ${CFLAGS} ${INCLUDES} -c $< -o $@

${NAME}: ${OBJS}
	@${MAKE} -C ${L_PATH}
	echo ${SRCS}
	${CC} ${CLFAGS} ${OBJS} ${INCLUDES} -o ${NAME} -L${L_PATH} -lft -lreadline

clean:
	${RM} ./*.o
	${RM} ./*/*.o
	${RM} ./*/*/*.o

fclean: clean
	${RM} ${NAME}
	${RM} ./includes/libft/libft.a

re: clean all

.PHONY: all clean fclean re
