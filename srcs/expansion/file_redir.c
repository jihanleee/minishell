#include "minishell.h"

void	create_heredoc(const char *delim)
{
	int		fd;
	char	*line;

	fd = open("heredoc.tmp", O_RDWR | O_CREAT | O_TRUNC, 0777);
	while (1)
	{
		ft_printf("heredoc> ");
		line = get_next_line(0);
		if (line == 0 || line[ft_strlen(line) - 1] != '\n')
		{
			if (line)
				free(line);
			ft_putstr_fd("warning: heredoc delimeted by EOF\n", 2);
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

void	open_file_errors(t_token **current, int fd)
{
	if ((*current)->type == amb_redir)
	{
		perror("ambiguous redirection");
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
			create_heredoc(current->str);
			free(current->str);
			current->str = ft_strdup("heredoc.tmp");
		}
		open_file_errors(&current, fd);
		if (current)
			current = current->next;
	}
}