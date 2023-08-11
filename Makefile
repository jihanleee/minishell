
NAME		= minishell
LIBFT		= libft.a

CC			= gcc
CFLAGS		= -Wall -Wextra -g3 #-Werror commented out for now, erase it before submission
RM			= rm -f

I_PATH		= ./includes/
L_PATH		= ./includes/libft/ #libft should be replaced by 'your teammate's libft' before submission to match the 42 header author name.

INCLUDES	= -I ${I_PATH} -I ${L_PATH}

SRCS		=	./srcs/parser.c \
				./srcs/find_param.c \
				./srcs/signal.c \
				./srcs/main.c \
				./srcs/expansion/assign_quotes.c \
				./srcs/expansion/clear.c \
				./srcs/expansion/expander.c \
				./srcs/expansion/expansion_utils.c \
				./srcs/expansion/jobs.c \
				./srcs/expansion/lexemes.c \
				./srcs/expansion/replace.c \
				./srcs/expansion/token_conversion.c \
				./srcs/execution/exec_utils1.c \
				./srcs/execution/exec_utils2.c \
				./srcs/execution/execution.c \
				./srcs/tokenization/create_tokens.c \
				./srcs/tokenization/token_substr_len.c \
				./srcs/tokenization/token_utils.c \
				./srcs/builtins/echo.c \
				./srcs/builtins/env.c \
				./srcs/builtins/exit.c \
				./srcs/builtins/export.c \
				./srcs/builtins/pwd.c \
				./srcs/builtins/unset.c \
				./srcs/builtins/cd.c \
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
