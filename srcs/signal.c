/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jihalee <jihalee@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/05 17:09:14 by hesong            #+#    #+#             */
/*   Updated: 2023/08/16 17:19:04 by jihalee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	sig_handler_prompt(int signum, siginfo_t *info, void *context)
{
	(void)info;
	(void)context;
	if (signum == SIGINT)
	{
		g_exit_stat = 130;
		write(1, "\n", 1);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
	}
	else if (signum == SIGQUIT)
		return ;
	return ;
}

int	sigaction_set_prompt(void)
{
	static struct sigaction	sig_struct;

	sigemptyset(&sig_struct.sa_mask);
	sig_struct.sa_flags = SA_SIGINFO;
	sig_struct.sa_sigaction = &sig_handler_prompt;
	if (sigaction(SIGINT, &sig_struct, NULL) == -1)
		return (0);
	if (sigaction(SIGQUIT, &sig_struct, NULL) == -1)
		return (0);
	signal(SIGQUIT, SIG_IGN);
	return (1);
}

void	sig_handler_parent(int signum, siginfo_t *info, void *context)
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
	{
		waitpid(-1, &stat, 0);
		g_exit_stat = get_child_status(stat);
		if (g_exit_stat == 131)
			ft_putstr_fd("quit (core dumped)\n", 2);
	}
	return ;
}

int	sigaction_set_parent(void)
{
	static struct sigaction	sig_struct;

	sigemptyset(&sig_struct.sa_mask);
	sig_struct.sa_flags = SA_SIGINFO;
	sig_struct.sa_sigaction = &sig_handler_parent;
	if (sigaction(SIGINT, &sig_struct, 0) == -1)
		return (0);
	if (sigaction(SIGQUIT, &sig_struct, 0) == -1)
		return (0);
	return (1);
}
void	sig_handler_heredoc(int signum, siginfo_t *info, void *context)
{
	(void)info;
	(void)context;
	if (signum == SIGINT)
	{
		clear_tokens(get_token_address(0), free);
		rl_clear_history();
		clear_env();
		exit(130);
	}
	else if (signum == SIGQUIT)
		return ;
	return ;
}

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
	return (1);
}
