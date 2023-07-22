
NAME		= minishell
LIBFT		= libft.a

CC			= gcc
CFLAGS		= -Wall -Wextra #-Werror commented out for now, erase it before submission
RM			= rm -f

I_PATH		= ./includes/
L_PATH		= ./includes/libft/ #libft should be replaced by 'your teammate's libft' before submission to match the 42 header author name.

INCLUDES	= -I ${I_PATH} -I ${L_PATH}

SRCS		= ./srcs/tokenizer.c # change or add files if needed
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

#혹시 위의 clean이 안되면 이 아래에 있는거 사용하세요
#둘 다 안되면... 연락 주세요..
#clean:
#	${RM} ${OBJS}
#	@${MAKE} -C ${L_PATH} clean

fclean: clean
	${RM} ${NAME}
	${RM} ./includes/libft/libft.a

#혹시 위의 fclean이 안되면 이 아래에 있는거 사용하세요
#둘 다 안되면... 연락 주세요..
#fclean: clean
#	${RM} ${NAME}
#	${RM} ${L_PATH} fclean

re: clean all

.PHONY: all clean fclean re
