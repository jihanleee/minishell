/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jihalee <jihalee@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/17 12:13:28 by solee2            #+#    #+#             */
/*   Updated: 2023/10/18 14:40:46 by jihalee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	read_jobs(t_job *jobs)
{
	int	i;

	while (jobs)
	{
		ft_printf("\tintype\t%d\n", jobs->in);
		ft_printf("\touttype\t%d\n", jobs->out);
		if (jobs->cmd)
			ft_printf("\tcmd\t%s\n", jobs->cmd);
		i = -1;
		if (jobs->arg)
		{
			while (jobs->arg[++i])
				ft_printf("\targ[%d]\t%s\n", i, jobs->arg[i]);
		}
		if (jobs->infile)
			ft_printf("\tinfile\t%s\n", jobs->infile);
		if (jobs->outfile)
		{
			ft_printf("\toutfile\t%s\n", jobs->outfile);
		}
		if (jobs->next != NULL)
			jobs = jobs->next;
		else
			return ;
	}
}

void	read_lexemes(t_lexeme *crnt)
{
	int		i;

	i = 0;
	while (crnt)
	{
		ft_printf("\tindex:\t%d\n", i);
		ft_printf("\tstr:\t%s\n", crnt->str);
		ft_printf("\ttype:\t%d\n", crnt->type);
		if (crnt->exp == 1)
			ft_printf("\texp:\tTRUE\n");
		else
			ft_printf("\texp:\tFALSE\n");
		ft_printf("p_found %d\n", crnt->p_found);
		crnt = crnt->next;
		i++;
	}
}

void	read_tokens(t_token *current)
{
	int		i;

	printf("---------------------------------------\n");
	printf("....tokens read....\n");
	if (current == NULL)
		printf("\t(NULL)\n");
	i = 0;
	while (current)
	{
		printf("\tindex:\t%d\n", i);
		printf("\tstr:\t%s\n", current->str);
		printf("\ttype:\t%d\n", current->type);
		current = current->next;
		i++;
	}
}

void	read_env(void)
{
	t_env	*current;

	current = *get_env_address();
	while (current)
	{
		printf("%s\n", current->str);
		current = current->next;
	}
}
