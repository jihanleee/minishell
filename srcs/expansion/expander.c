#include "minishell.h"

void	chop_lexeme_str(t_lexeme **current, t_lexeme **next)
{
	int			i;
	t_lexeme	*new;

	i = 0;
	while ((*current)->type != 1 && (*current)->str[i])
	{
		if ((*current)->str[i] == '$' && (*current)->type != 1)
			i += len_lvname(&((*current)->str[i]));
		else
			while ((*current)->str[i] && (*current)->str[i] != '$')
				i++;
		if ((*current)->str[i])
		{
			new = new_lexeme(ft_strdup(&((*current)->str[i])), (*current)->type);
			if (new)
			{
				(*current)->next = new;
				(*current)->next->next = (*next);
				(*current)->str[i] = '\0';
			}
		}
	}
}

void	split_expansions(t_lexeme *lexemes)
{
	t_lexeme	*current;
	t_lexeme	*next;

	current = lexemes;
	while (current)
	{
		next = current->next;
		chop_lexeme_str(&current, &next);
		current = current->next;
	}
	while (lexemes)
	{
		if (lexemes->str[0] == '$' && lexemes->str[1] != '\0' && lexemes->type != 1)
			lexemes->exp = TRUE;
		lexemes = lexemes->next;
	}
}

void	expansion(t_token **tokens, char **envp)
{
	t_token	*current;
	t_token	*next;
	t_token	*prev;
	t_token	*expanded;
	int		i;

	i = 0;
	prev = 0;
	current = *tokens;
	while (current)
	{
		next = current->next;
		expanded = token_to_etoken(current, envp);
		if (expanded)
		{
			if (prev == 0)
				*tokens = expanded;
			else
				prev->next = expanded;
			while (expanded->next)
				expanded = expanded->next;
			expanded->next = next;
			prev = expanded;
			expanded->type = current->type;
		}
		else if (!expanded)
		{
			if (current->type == 1 || current->type == 3 || current->type == 4)
			{
				current->type = amb_redir;
				prev = current;
				current = next;
				continue ;
			}
			else if (prev == 0)
				*tokens = next;
			else
				prev->next = next;
		}
		free((free(current->str), current));
		current = next;
	}
}

char	**extract_arg(t_token **tokens)
{
	char		**result;
	int			size_result;
	t_token		*current;
	int			i;

	size_result = 0;
	current = *tokens;
	while (current && current->type != pipe_op)
	{
		if (current->type == word)
			size_result++;
		current = current->next;
	}
	result = (char **)ft_calloc(size_result + 1, sizeof (char *));
	if (result == NULL)
		return (NULL);
	i = 0;
	while (*tokens && (*tokens)->type != pipe_op)
	{
		if ((*tokens)->type == word)
			result[i++] = ft_strdup((*tokens)->str);
		*tokens = (*tokens)->next;
	}
	result[i] = 0;
	return (result);
}

void	create_heredoc(const char *delim)
{
	int		fd;
	char	*line;

	fd = open("heredoc.tmp", O_RDWR | O_CREAT | O_TRUNC, 0777);
	while (1)
	{
		printf("heredoc>");
		line = get_next_line(0);
		if (line == 0 || line[ft_strlen(line) - 1] != '\n')
		{
			if (line)
				free(line);
			printf("ERRRRORROR\n");
			break ;
		}
		else if (ft_strncmp(line, delim, ft_strlen(line) - 1) == 0 \
		&& (ft_strlen(line) - 1 == ft_strlen(delim)))
		{
			free(line);
			break ;
		}
		write(fd, line, ft_strlen(line));
		free(line);
	}
	close(fd);
}

void	open_file_redir(t_token *token)
{
	t_token	*current;
	int		fd;

	current = token;
	while (current)
	{
		fd = 0;
		if (current->type == in)
			fd = open(current->str, O_RDONLY);
		else if (current->type == out || current->type == append)
			fd = open(current->str, O_WRONLY | O_CREAT | \
			O_TRUNC * (current->type != append), 0777);
		else if (current->type == heredoc)
		{
			create_heredoc(current->str);
			free(current->str);
			current->str = ft_strdup("heredoc.tmp");
		}
		else if (current->type == amb_redir)
			perror("ambiguous redirection");
		if (fd == -1)
			perror(current->str);
		if (fd == -1 || current->type == amb_redir)
		{
			while (current && current->type != pipe_op)
			{
				current->type = amb_redir;
				current = current->next;
			}
		}
		if (current)
			current = current->next;
	}
}
