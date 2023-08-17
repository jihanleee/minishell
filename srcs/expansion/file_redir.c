#include "minishell.h"

void	create_heredoc(const char *delim, bool hereq)
{
	int		fd;
	char	*line;

	fd = open("/tmp/heredoc.tmp", O_RDWR | O_CREAT | O_TRUNC, 0777);
	while (1)
	{
		line = readline_nl("> ");
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

void	heredoc_child(t_token *current)
{
	pid_t	cpid;

	sigaction_set_heredoc_parent();
	g_exit_stat = 0;
	cpid = fork();
	if (cpid == 0)
	{
		sigaction_set_heredoc();
		create_heredoc(current->str, current->hereq);
		clear_tokens(get_token_address(0), free);
		rl_clear_history();
		clear_env();
		exit(0);
	}
	wait(0);
}

int	open_file_redir(t_token **tokens)
{
	t_token	*current;
	int		fd;

	current = *tokens;
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
			heredoc_child(current);
			free(current->str);
			current->str = ft_strdup("/tmp/heredoc.tmp");
			if (g_exit_stat == 130)
				return ((clear_tokens(get_token_address(0), free), 0));
		}
		open_file_errors(&current, fd);
		if (current)
			current = current->next;
	}
	return (0);
}
