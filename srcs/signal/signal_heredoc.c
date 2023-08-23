/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_heredoc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jihalee <jihalee@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/17 13:57:51 by solee2            #+#    #+#             */
/*   Updated: 2023/08/17 14:14:19 by jihalee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	sigaction_set_heredoc(void)
{
	static struct sigaction	sig_struct;

	sigemptyset(&sig_struct.sa_mask);
	sig_struct.sa_flags = SA_SIGINFO;
	sig_struct.sa_sigaction = &sig_handler_heredoc;
	if (sigaction(SIGINT, &sig_struct, 0) == -1)
		return (0);
	if (sigaction(SIGQUIT, &sig_struct, 0) == -1)
		return (0);
	signal(SIGQUIT, SIG_IGN);
	return (1);
}

void	sig_handler_heredoc(int signum, siginfo_t *info, void *context)
{
	(void)info;
	(void)context;
	if (signum == SIGINT)
	{
		clear_tokens(get_token_address(), free);
		rl_clear_history();
		clear_env();
		exit(130);
	}
	else if (signum == SIGQUIT)
		return ;
	return ;
}

void	sig_handler_heredoc_parent(int signum, siginfo_t *info, void *context)
{
	int	stat;

	(void)info;
	(void)context;
	if (signum == SIGINT)
	{
		waitpid(-1, &stat, 0);
		g_exit_stat = get_child_status(stat);
		if (g_exit_stat == 130)
			ft_printf("\n");
	}
	else if (signum == SIGQUIT)
		return ;
	return ;
}

int	sigaction_set_heredoc_parent(void)
{
	static struct sigaction	sig_struct;

	sigemptyset(&sig_struct.sa_mask);
	sig_struct.sa_flags = SA_SIGINFO;
	sig_struct.sa_sigaction = &sig_handler_heredoc_parent;
	if (sigaction(SIGINT, &sig_struct, 0) == -1)
		return (0);
	if (sigaction(SIGQUIT, &sig_struct, 0) == -1)
		return (0);
	signal(SIGQUIT, SIG_IGN);
	return (1);
}
