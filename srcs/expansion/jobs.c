#include "minishell.h"

void	read_jobs(t_job *jobs)
{
	int	i;

	while (jobs)
	{
		printf("--------------------\n");
		printf("in read jobs function, expander\n");
		//ft_printf("\tcurrent pipe has command\n", jobs->cmd);
		ft_printf("\tintype\t%d\n", jobs->in);
		ft_printf("\touttype\t%d\n", jobs->out);
		if (jobs->cmd)
			ft_printf("\tcmd\t%s\n", jobs->cmd);
		i = 0;
		if (jobs->arg)
		{
			while (jobs->arg[i])
			{
				ft_printf("\targ[%d]\t%s\n", i, jobs->arg[i]);
				i++;
			}
		}
		//printf("before in/outfile\n");
		if (jobs->infile)
			ft_printf("\tinfile\t%s\n", jobs->infile);
		//printf("after infile + before outfile\n");
		if (jobs->outfile)
		{
			//printf("inside outfile\n");
			ft_printf("\toutfile\t%s\n", jobs->outfile);
		}
		ft_printf("--------------------\n");
		if (jobs->next != NULL)
			jobs = jobs->next;
		else
			return ;
	}
}

void	point_prev_job(t_job *jobs)
{
	while (jobs->next)
	{
		jobs->next->prev = jobs;
		jobs = jobs->next;
	}
}

t_job	*extract_jobs(t_token *tokens)
{
	t_job	*result;
	t_job	*cur_result;
	t_token	*current;

	if (tokens == NULL)
		return (NULL);
	result = (t_job *)calloc(1, sizeof(t_job));
	cur_result = result;
	current = tokens;
	while (current)
	{
		if (current->type == pipe_op)
		{
			cur_result->next = (t_job *)ft_calloc(1, sizeof (t_job));
			cur_result = cur_result->next;
		}
		else if (current->type == in || current->type == heredoc)
		{
			if (cur_result->infile)
				free(cur_result->infile);
			cur_result->infile = ft_strdup(current->str);
			cur_result->in = in;
		}
		else if (current->type == out || current->type == append)
		{
			if (cur_result->outfile)
				free(cur_result->outfile);
			cur_result->outfile = ft_strdup(current->str);
			cur_result->out = current->type;
		}
		if (current->type == amb_redir)
		{
			cur_result->in = -1;
			cur_result->out = -1;
			while (current->next && current->next->type != pipe_op)
				current = current->next;
		}
		current = current->next;
	}
	current = tokens;
	cur_result = result;
	int	i = 0;
	while (current && cur_result)
	{
		while (current && current->type != pipe_op && current->type != word)
			current = current->next;
		if (current == 0)
			break ;
		if (current->type == pipe_op)
		{
			ft_printf("index %d\n", i++);
			cur_result = cur_result->next;
			current = current->next;
		}
		else if (current->type == word)
		{
			if (cur_result->cmd == 0)
			{
				//ft_printf("str:%s\n", current->str);
				cur_result->cmd = ft_strdup(current->str);
				current = current->next;
			}
			else
				cur_result->arg = extract_arg(&current);
		}
	}
	point_prev_job(result);
	return (result);
}

void	clear_jobs(t_job **lst)
{
	t_job	*current;
	t_job	*next;
	
	if (lst == 0)
		return ;
	current = *lst;
	while (current)
	{
		next = current->next;
		if (current->cmd)
			free(current->cmd);
		if (current->outfile)
			free(current->outfile);
		if (current->infile)
			free(current->infile);
		if (current->arg)
		free_arrays(current->arg);
		free(current);
		current = next;
	}
	*lst = 0;
}
