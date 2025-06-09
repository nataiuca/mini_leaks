/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natferna <natferna@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 17:48:19 by jgamarra          #+#    #+#             */
/*   Updated: 2025/06/05 00:39:42 by natferna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	handle_no_arguent(t_minishell *minishell)
{
	ft_putstr_fd("exit\n", STDERR_FILENO);
	ft_putstr_fd(PROMPT_ERROR, STDERR_FILENO);
	ft_putstr_fd("exit: too many arguments\n", STDERR_FILENO);
	minishell->status = 1;
}

int is_valid_numeric_arg_strtol(const char *arg, int *out_value)
{
    char *endptr;
    long val;

    errno = 0;
    val = strtol(arg, &endptr, 10);
    if (errno != 0)
        return 0;
    if (*endptr != '\0')
        return 0;
    if (val < INT_MIN || val > INT_MAX)
        return 0;

    *out_value = (int)val;
    return 1;
}

void	handle_numeric_exit(t_cmd *cmd, t_minishell *minishell)
{
	t_execcmd	*ecmd;
	char		*clean;
	int			val;

	(void)minishell;
	ecmd = (t_execcmd *)cmd;
	clean = remove_quotes_simple(ecmd->argv[1]);
	if (!is_valid_numeric_arg_strtol(clean, &val))
	{
		ft_putstr_fd("exit\n", STDERR_FILENO);
		ft_putstr_fd(PROMPT_ERROR, STDERR_FILENO);
		ft_putstr_fd("exit: numeric argument required\n", STDERR_FILENO);
		exit(255);
	}
	exit(val % 256);
}
/*
* Function to handle the exit command.
* It checks the number of arguments passed to the command.
* If there are no arguments, it exits with the last status.
*/
void    exit_impl(t_cmd *cmd, t_minishell *minishell)
{
    t_execcmd    *ecmd;

    ecmd = (t_execcmd *)cmd;
    if (ecmd->argv[1] && !ecmd->argv[2])
    {
        handle_numeric_exit(cmd, minishell);
    }
    else if (ecmd->argv[1] && ecmd->argv[2])
    {
        handle_no_arguent(minishell);
        return;
    }
    else
    {
        ft_putstr_fd("exit\n", STDERR_FILENO);
        exit(minishell->status);
    }
}
