/*
parsing rules

if token.redirection == 1 (meaning there is redirection)
	and **files is empty ==> error
if token redirection is <
	and *file[0] is empty ==> error
if token redirection is >
	and *file[1] is empty ==> error

if token after pipe is not a word, redirection etc...
=if token after pipe is a pipe ==> error

*/
//receive tokens list
//go through each token
	//if redirection == 1 and **files are empty ==> error
	//if redirection is < and files[1] is empty ==> error
	// if redirection is > and files[2] is empty ==> error
	//if pipe is followed by a pipe ==> error
	//no file(word in this case) after redirections

void	exit_error(char *message)
{
	write(STDERR_FILENO, message, ft_strlen(message));
	exit(EXIT_FAILURE);
}


int	check_tokens(t_token *tokens)
{
	while (tokens && tokens->next != NULL)
	{
		if (tokens->type == in || tokens->type == out)
		{
			if (tokens->next->type != word)
				exit_error("bash: syntax error\n");	//check error message
		}
		if (tokens->type == pipe_op && tokens->next->type == pipe_op)
			exit_error("bash: syntax error\n");	//check error message
		tokens = tokens->next;
	}
	return (0);
}