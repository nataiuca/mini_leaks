/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_controller.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natferna <natferna@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 21:50:39 by jgamarra          #+#    #+#             */
/*   Updated: 2025/06/04 23:16:11 by natferna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	valid_builtins(t_cmd *cmd)
{
	t_execcmd	*ecmd;

	ecmd = (t_execcmd *)cmd;
	return (ft_strcmp(ecmd->argv[0], "echo") == 0
		|| ft_strcmp(ecmd->argv[0], "cd") == 0
		|| ft_strcmp(ecmd->argv[0], "pwd") == 0
		|| ft_strcmp(ecmd->argv[0], "export") == 0
		|| ft_strcmp(ecmd->argv[0], "unset") == 0
		|| ft_strcmp(ecmd->argv[0], "env") == 0
		|| ft_strcmp(ecmd->argv[0], "exit") == 0
		|| ft_strcmp(ecmd->argv[0], "history") == 0);
}

void run_internal(t_cmd *cmd, t_minishell *minishell)
{
	t_execcmd *ecmd;
	
	ecmd = (t_execcmd *)cmd;
	if (ft_strstr(ecmd->argv[0], "echo"))
		echo_impl(cmd, minishell);
	else if (ft_strstr(ecmd->argv[0], "pwd"))
		pwd_impl(cmd, minishell);
	else if (ft_strstr(ecmd->argv[0], "cd"))
		cd_impl(cmd, minishell);
	else if (ft_strstr(ecmd->argv[0], "env"))
		env_impl(cmd, minishell);
	else if (ft_strstr(ecmd->argv[0], "exit"))
		exit_impl(cmd, minishell);
	else if (ft_strstr(ecmd->argv[0], "export"))
		export_impl(cmd, minishell);
	else if (ft_strstr(ecmd->argv[0], "unset"))
		unset_impl(cmd, minishell);
	else if (ft_strcmp(ecmd->argv[0], "history") == 0)
		handle_history(ecmd, minishell);
	// free all variables from ecmd
	// atexit(check_leaks);
	// 	exit(0);
}
