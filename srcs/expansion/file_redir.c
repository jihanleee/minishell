#include "minishell.h"

char	*gnl_heredoc(bool hereq, char **line)
{
	t_lexeme	*lexemes;
	t_lexeme	*current;
	int			i;
	int			j;

	if (hereq || !*line)
		return (*line);
	lexemes = new_lexeme(ft_strdup(*line), 0);
	split_expansions((free(*line), lexemes));
	replace_params(lexemes);
	*line = (char *)ft_calloc(lexemelen(lexemes, 0) + 1, sizeof (char));
	i = 0;
	current = lexemes;
	while (*line && current)
	{
		j = 0;
		while (current->str[j] && !(!current->p_found && current->exp))
			(*line)[i++] = current->str[j++];
		current = current->next;
	}
	clear_lexemes(&lexemes, free);
	return (*line);
}

void	create_heredoc(const char *delim, bool hereq)
{
	int		fd;
	char	*line;

	fd = open("heredoc.tmp", O_RDWR | O_CREAT | O_TRUNC, 0777);
	while (1)
	{
		line = ft_printf("heredoc> ") * 0 + get_next_line(0);
		if (line == 0 || line[ft_strlen(line) - 1] != '\n')
		{
			if (line)
				free(line);
			ft_putstr_fd("warning: heredoc delimeted by EOF\n", 2);
			break ;
		}
		if (ft_strncmp(line, delim, ft_strlen(line) - 1) == 0 \
		&& (ft_strlen(line) - 1 == ft_strlen(delim)))
		{
			free(line);
			break ;
		}
		gnl_heredoc(hereq, &line);
		write(fd, line, ft_strlen(line));
		free(line);
	}
	close(fd);
}

void	open_file_errors(t_token **current, int fd)
{
	if ((*current)->type == amb_redir)
	{
		ft_putstr_fd("ambiguous redirection\n", 2);
		g_exit_stat = 1;
	}
	if (fd == -1)
	{
		perror((*current)->str);
		g_exit_stat = 1;
	}
	if (fd == -1 || (*current)->type == amb_redir)
	{
		while ((*current) && (*current)->type != pipe_op)
		{
			(*current)->type = amb_redir;
			*current = (*current)->next;
		}
	}
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
		if (current->type == out || current->type == append)
			fd = open(current->str, O_WRONLY | O_CREAT | \
			O_TRUNC * (current->type != append), 0777);
		if (current->type == heredoc)
		{
			create_heredoc(current->str, current->hereq);
			free(current->str);
			current->str = ft_strdup("heredoc.tmp");
		}
		open_file_errors(&current, fd);
		if (current)
			current = current->next;
	}
}
