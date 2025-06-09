/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   safe_free.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgamarra <jgamarra@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/28 17:04:21 by jgamarra          #+#    #+#             */
/*   Updated: 2025/05/24 22:11:18 by jgamarra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	safe_free_vector(char **split)
{
	int	i;

	i = -1;
	while (split[++i])
	{
		free(split[i]);
	}
	if (split)
		free(split);
}

void	safe_free_minishell(t_minishell *minishell)
{
	if (minishell->path_env)
		safe_free_vector(minishell->path_env);
	if (minishell->env)
		safe_free_vector(minishell->env);
	free(minishell->exported);
}

char *strdup_safe(const char *s)
{
	char *dup;

	dup = ft_strdup(s);
	if (!dup)
	{
		perror("strdup");
		exit(EXIT_FAILURE);
	}
	return (dup);
}

char *join_and_free(char *a, char *b)
{
	char *res;

	res = ft_strjoin(a, b);
	if (!res)
	{
		free(a);
		free(b);
		perror("ft_strjoin");
		exit(EXIT_FAILURE);
	}
	free(a);
	free(b);
	return (res);
}

void	free_cmd(t_cmd *cmd)
{
	int	i;

	if (!cmd)
		return ;
	if (cmd->type == EXEC)
	{
		t_execcmd *ecmd = (t_execcmd *)cmd;
		i = 0;
		while (ecmd->argv[i])
		{
			free(ecmd->argv[i]);
			i++;
		}
	}
	else if (cmd->type == PIPE)
	{
		t_pipecmd *pcmd = (t_pipecmd *)cmd;
		free_cmd(pcmd->left);
		free_cmd(pcmd->right);
	}
	else if (cmd->type == REDIR)
	{
		t_redircmd *rcmd = (t_redircmd *)cmd;
		free_cmd(rcmd->cmd);
		free(rcmd->file);
		free(rcmd->efile);
		free(rcmd->hdoc);
	}
	free(cmd);
}
