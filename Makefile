
NAME		= minishell
LIBFT		= libft.a

CC			= gcc
CFLAGS		= -Wall -Wextra -Werror
RM			= rm -f

I_PATH		= ./includes/
L_PATH		= ./includes/libft/

INCLUDES	= -I ${I_PATH} -I ${L_PATH}

SRCS		= ./srcs/main.c \
				./srcs/
OBJS		= ${SRCS:.c=.o}

all: ${NAME}

${NAME}: ${OBJS}
	@${MAKE} -C ${L_PATH}
	${CC} ${CLFAGS} ${OBJS} ${INCLUDES} -o ${NAME}

clean:
	${RM} ${OBJS}
	${RM} ${L_PATH}*.o

#혹시 위의 clean이 안되면 이 아래에 있는거 사용하세요
#둘 다 안되면... 연락 주세요..
#clean:
#	${RM} ${OBJS}
#	@${MAKE} -C ${L_PATH} clean

fclean: clean
	${RM} ${NAME}
	${RM} ${L_PATH}${LIBFT}

#혹시 위의 fclean이 안되면 이 아래에 있는거 사용하세요
#둘 다 안되면... 연락 주세요..
#fclean: clean
#	${RM} ${NAME}
#	${RM} ${L_PATH} fclean

re: clean all

.PHONY: all clean fclean re