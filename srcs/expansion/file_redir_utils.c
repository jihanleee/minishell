/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_redir_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jihalee <jihalee@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/17 12:21:45 by solee2            #+#    #+#             */
/*   Updated: 2023/08/21 00:23:27 by jihalee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

char	*readline_nl(char *str)
{
	char	*line;
	char	*withnl;

	line = readline(str);
	withnl = ft_strjoin(line, "\n");
	free(line);
	return (withnl);
}

void	set_random_filename(char *randname)
{
	char	*tmp;
	char	buf;
	int		i;
	int		fd;

	tmp = "/tmp/";
	fd = open("/dev/random", O_RDONLY);
	i = 0;
	while (tmp[i])
	{
		randname[i] = tmp[i];
		i++;
	}
	while (i < 6)
	{
		read(fd, &buf, 1);
		randname[i] = buf % 50 + 50;
		i++;
	}
	randname[i] = '\0';
	close(fd);
}
