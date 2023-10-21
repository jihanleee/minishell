
NAME		= minishell
LIBFT		= ./includes/libft/libft.a

CC			= cc
CFLAGS		= -Wall -Wextra -g3 -Werror
RM			= rm -f

I_PATH		= ./includes/
L_PATH		= ./includes/libft/

INCLUDES	= -I ${I_PATH} -I ${L_PATH}

SRCS		=	./srcs/parsing/parser.c \
				./srcs/signal/signal.c \
				./srcs/signal/signal_heredoc.c \
				./srcs/main.c \
				./srcs/debug.c \
				./srcs/expansion/find_param.c \
				./srcs/expansion/assign_quotes.c \
				./srcs/expansion/file_redir.c \
				./srcs/expansion/file_redir_utils.c \
				./srcs/expansion/clear.c \
				./srcs/expansion/expander.c \
				./srcs/expansion/expansion_utils.c \
				./srcs/expansion/jobs.c \
				./srcs/expansion/lexemes.c \
				./srcs/expansion/replace.c \
				./srcs/expansion/token_conversion1.c \
				./srcs/expansion/token_conversion2.c \
				./srcs/execution/exec_utils1.c \
				./srcs/execution/exec_utils2.c \
				./srcs/execution/exec_free.c \
				./srcs/execution/execution.c \
				./srcs/tokenization/create_tokens.c \
				./srcs/tokenization/token_substr_len.c \
				./srcs/tokenization/token_utils.c \
				./srcs/builtins/echo/echo.c \
				./srcs/builtins/env/env.c \
				./srcs/builtins/exit/exit.c \
				./srcs/builtins/exit/exit_utils.c \
				./srcs/builtins/export/export.c \
				./srcs/builtins/export/export_utils.c \
				./srcs/builtins/export/export_unset.c \
				./srcs/builtins/pwd/pwd.c \
				./srcs/builtins/unset/unset.c \
				./srcs/builtins/cd/cd.c \
				./srcs/builtins/cd/cd_paths.c \
				./srcs/builtins/cd/cd_utils.c \
				./srcs/builtins/cd/cd_error.c \
				./srcs/env_var.c \


OBJS		= ${SRCS:.c=.o}

all: ${NAME}

%.o: %.c
	${CC} ${CFLAGS} ${INCLUDES} -c $< -o $@

${NAME}: ${OBJS} ${LIBFT}
	echo ${SRCS}
	${CC} ${CLFAGS} ${OBJS} ${INCLUDES} -o ${NAME} -L${L_PATH} -lft -lreadline

${LIBFT}:
	@${MAKE} -C ${L_PATH}

clean:
	${RM} ./*.o
	${RM} ./*/*.o
	${RM} ./*/*/*.o
	${RM} ./*/*/*/*.o

fclean: clean
	${RM} ${NAME}
	${RM} ./includes/libft/libft.a

re: clean all

.PHONY: all clean fclean re
