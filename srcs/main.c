#include "minishell.h"

int	main(int argc, char **argv, char **envp)
{
	t_token	*tokens;
	t_token	*temp;
	char	*line;
	int		*info;
	int		i;
	char	*tok;

	while (1)
	{	temp = tokens;
		line = readline("%");
		ft_printf("line : %s\n", line);
		tokens = create_tokens(line);
		if (check_tokens(tokens) != 0)
			exit_error("bash: syntax error\n", &temp);
	}
	return (0);
}

//token type
//0: word
//1: <
//2: heredoc
//3: >
//4: append
//5: pipe