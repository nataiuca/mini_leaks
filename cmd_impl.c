/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_impl.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgamarra <jgamarra@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 13:45:13 by jgamarra          #+#    #+#             */
/*   Updated: 2025/06/03 22:04:43 by jgamarra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


// void	debug_cmd_chain2(t_cmd *cmd)
// {
// 	int		i;
// 	t_redircmd	*redir;

// 	i = 0;
// 	while (cmd && cmd->type == REDIR)
// 	{
// 		redir = (t_redircmd *)cmd;
// 		ft_printf("redir[%d]: file = %s\n", i, redir->file);
// 		cmd = redir->cmd;
// 		i++;
// 	}
// 	if (cmd && cmd->type == EXEC)
// 	{
// 		t_execcmd *exec = (t_execcmd *)cmd;
// 		ft_printf("exec: argv[0] = %s\n", exec->argv[0]);
// 	}
// }


// Execute cmd.  Never returns.
void	runcmd(t_cmd *cmd, t_minishell *minishell)
{

	// debug_cmd_chain2(cmd);

	// printf("cmd->type: %d\n", cmd->type);

	// exit(0);
	if (!cmd)
		exit(0);
	// cmd = invert_redirection_chain(cmd);
	if (cmd->type == EXEC)
	{
		// exit(0);
		// printf("runcmd: EXEC\n");
		handle_cmd_exec((t_execcmd *)cmd, minishell);
	}
	else if (cmd->type == REDIR)
	{
		handle_cmd_redir((t_redircmd *)cmd, minishell);
	}
	else if (cmd->type == PIPE)
		handle_cmd_pipe((t_pipecmd *)cmd, minishell);
	else
		panic("runcmd: unknown type");
	exit(0);
}

t_cmd	*invert_redirection_chain2(t_cmd *cmd)
{
	t_redircmd	*prev;
	t_redircmd	*current;
	t_redircmd	*next;
	t_cmd		*tail;

	if (!cmd || cmd->type != REDIR)
		return (cmd);
	current = (t_redircmd *)cmd;
	prev = NULL;
	while (current && current->cmd && current->cmd->type == REDIR)
	{
		next = (t_redircmd *)current->cmd;
		current->cmd = (t_cmd *)prev;
		prev = current;
		current = next;
	}
	tail = current->cmd;
	current->cmd = (t_cmd *)prev;
	return ((t_cmd *)current);
}

t_cmd	*get_last_cmd_node2(t_cmd *cmd)
{
	t_redircmd	*redir;

	redir = (t_redircmd *)cmd;
	while (redir && redir->cmd && redir->cmd->type == REDIR)
		redir = (t_redircmd *)redir->cmd;
	return (redir->cmd);
}

void	run_external(t_cmd *cmd, t_minishell *minishell)
{
	int		status;
	pid_t	pid;
// t_cmd	*exec_cmd;

	// if (cmd && cmd->type == REDIR)
	// {
	// 	// exec_cmd = get_last_cmd_node2(cmd);
	// 	printf("exec_cmd->rcmd->file: %s\n", ((t_redircmd *)exec_cmd)->file);
	// 	cmd = invert_redirection_chain2(cmd);
	// 	// ((t_redircmd *)cmd)->cmd = exec_cmd;
	// }

	pid = fork1();
	if (pid == 0)
		runcmd(cmd, minishell);
	waitpid (pid, &status, 0);
	if (WIFEXITED(status))
	{
		minishell->status = WEXITSTATUS(status);
	}
}
