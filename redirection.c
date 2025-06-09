/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natferna <natferna@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 21:29:04 by jgamarra          #+#    #+#             */
/*   Updated: 2025/06/05 01:20:33 by natferna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_cmd	*handle_redir_token(t_cmd *cmd, char *q, char *eq, int tok)
{
	cmd = redircmd(cmd, q, eq, tok);
	return (cmd);
}

static t_cmd	*handle_heredoc_token(t_cmd *cmd, char *q, char *eq, t_minishell *minishell)
{
	char	*hdoc;

	hdoc = process_heredoc(q, eq, minishell);
	if (!hdoc)
		panic("heredoc failed");
	minishell->status = 0;
	cmd = redircmd_hdoc(cmd, hdoc);
	return (cmd);
}

t_cmd	*invert_redirection_chain(t_cmd *cmd)
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
		// printf("current->file: %s\n", current->file);
		next = (t_redircmd *)current->cmd;
		current->cmd = (t_cmd *)prev;
		prev = current;
		current = next;
	}
	tail = current->cmd;
	current->cmd = (t_cmd *)prev;
	return ((t_cmd *)current);
}

t_cmd	*get_last_cmd_node(t_cmd *cmd)
{
	t_redircmd	*redir;

	while (cmd && cmd->type == REDIR)
	{
		redir = (t_redircmd *)cmd;
		cmd = redir->cmd;
	}
	return (cmd);
}

// t_cmd	*parseredirs(t_cmd *cmd, char **ps, char *es, t_minishell *minishell)
// {
// 	int		tok;
// 	char	*q;
// 	char	*eq;

// 	printf("here0\n");
// 	// printf("ps: %s\n", *ps);
// 	while (peek(ps, es, "<>") && !minishell->error_syntax)
// 	{
// 		// printf("here1\n");
// 		tok = gettoken(ps, es, &q, &eq);
// 		if (gettoken(ps, es, &q, &eq) != 'a')
// 		{
// 			panic("syntax error near unexpected token.\n");
// 			minishell->status = 258;
// 			minishell->error_syntax = true;
// 			break ;
// 		}
// 		if (tok == HDOC)
// 			cmd = handle_heredoc_token(cmd, q, eq);
// 		else if (tok == '<')
// 		{
// 			// printf("here2\n");
// 			// while (ft_strcountchr(q, '<')!=0)
// 			// {
// 			// 	tok = gettoken(ps, es, &q, &eq);
// 			// 	if (gettoken(ps, es, &q, &eq) != 'a')
// 			// 	{
// 			// 		panic("syntax error near unexpected token.\n");
// 			// 		minishell->status = 258;
// 			// 		minishell->error_syntax = true;
// 			// 		break ;
// 			// 	}
// 			// }
// 			// return (handle_redir_token(cmd, q, eq, tok));
// 			cmd = handle_redir_token(cmd, q, eq, tok);
// 		}
// 		else if (tok == '>' || tok == '+')
// 		{
// 			// printf("here3\n");
// 			// return (handle_redir_token(cmd, q, eq, tok)); //LEAK POR EL PRIMER < ??????
// 			// 	TEST
// 			// while (ft_strcountchr(q, '>')!=0)
// 			// {
// 			// 	tok = gettoken(ps, es, &q, &eq);
// 			// 	if (gettoken(ps, es, &q, &eq) != 'a')
// 			// 	{
// 			// 		panic("syntax error near unexpected token.\n");
// 			// 		minishell->status = 258;
// 			// 		minishell->error_syntax = true;
// 			// 		break ;
// 			// 	}
// 			// }
// 			// TEST
// 			cmd = handle_redir_token(cmd, q, eq, tok); //LEAK POR EL PRIMER < ??????
// 			// printf("file -> %s\n", q);
// 		}
// 	}
// 	if (cmd->type == REDIR)
// 		cmd = invert_redirection_chain(cmd);

// 	return (cmd);
// }

t_cmd	*parseredirs(t_cmd *cmd, char **ps, char *es, t_minishell *minishell)
{
	int		tok;
	char	*q;
	char	*eq;
	t_cmd	*exec_cmd;

	while (peek(ps, es, "<>") && !minishell->error_syntax)
	{
		tok = gettoken(ps, es, &q, &eq);
		if (gettoken(ps, es, &q, &eq) != 'a')
		{
			panic("syntax error near unexpected token.\n");
			minishell->status = 258;
			minishell->error_syntax = true;
			break ;
		}
		// printf("q: %s, eq: %s, tok: %d\n", q, eq, tok);
		if (tok == HDOC)
			cmd = handle_heredoc_token(cmd, q, eq, minishell);
		else if (tok == '<' || tok == '>' || tok == '+')
			cmd = handle_redir_token(cmd, q, eq, tok);
	}

	if (cmd && cmd->type == REDIR)
{
	exec_cmd = get_last_cmd_node(cmd);      // Save exec
	cmd = invert_redirection_chain(cmd);     // Invert redirs
	t_redircmd *last = (t_redircmd *)cmd;    // Find last redir node
	while (last->cmd && last->cmd->type == REDIR)
		last = (t_redircmd *)last->cmd;
	last->cmd = exec_cmd;                   // Attach exec at end
}
	return (cmd);
}

