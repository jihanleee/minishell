#include "minishell.h"

void	replace_params(t_lexeme *lexemes, char **envp)
{
	t_lexeme	*current;
	char		*newstr;

	current = lexemes;
	while (current)
	{
		ft_printf("%s\n", current->str);
		newstr = 0;
		if (current->exp == TRUE)
		{
			newstr = find_param(&(current->str[1]), envp);
			if (newstr)
			{
				current->p_found = TRUE;
				free(current->str);
				current->str = newstr;
			}
		}
		current = current->next;
	}
}
