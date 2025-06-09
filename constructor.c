/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   constructor.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgamarra <jgamarra@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 20:03:21 by jgamarra          #+#    #+#             */
/*   Updated: 2025/06/03 22:07:28 by jgamarra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_cmd	*execcmd(void)
{
	t_execcmd	*cmd;

	cmd = safe_malloc(sizeof(*cmd));
	ft_memset(cmd, 0, sizeof(*cmd));
	cmd->type = EXEC;
	return ((t_cmd *)cmd);
}

t_cmd	*redircmd(t_cmd *subcmd, char *file, char *efile, char redir)
{
	t_redircmd	*rcmd;

	rcmd = safe_malloc(sizeof(*rcmd));
	ft_memset(rcmd, 0, sizeof(*rcmd));
	rcmd->type = REDIR;
	rcmd->cmd = subcmd;
	// printf("file : %s, efile: %s, redir: %c\n", file, efile, redir);
	rcmd->file = file;
	rcmd->efile = efile;
	rcmd->right = 0644;
	rcmd->fd = 1;
	if (redir == '<')
	{
		rcmd->mode = O_RDONLY;
		rcmd->right = 0;
		rcmd->fd = 0;
	}
	else if (redir == '>')
		rcmd->mode = O_CREAT | O_WRONLY | O_TRUNC;
	else if (redir == '+')
		rcmd->mode = O_CREAT | O_WRONLY | O_APPEND;
	rcmd->hdoc = NULL;
	return ((t_cmd *)rcmd);
}

t_cmd	*redircmd_hdoc(t_cmd *subcmd, char *hdoc)
{
	t_redircmd	*cmd;

	cmd = safe_malloc(sizeof(*cmd));
	ft_memset(cmd, 0, sizeof(*cmd));
	cmd->type = REDIR;
	cmd->cmd = subcmd;
	cmd->file = NULL;
	cmd->efile = NULL;
	cmd->right = 0;
	cmd->fd = 0;
	cmd->mode = 0;
	cmd->hdoc = hdoc;
	return ((t_cmd *)cmd);
}

t_cmd	*pipecmd(t_cmd *left, t_cmd *right)
{
	t_pipecmd	*cmd;

	cmd = safe_malloc(sizeof(*cmd));
	ft_memset(cmd, 0, sizeof(*cmd));
	cmd->type = PIPE;
	cmd->left = left;
	cmd->right = right;
	return ((t_cmd *)cmd);
}
