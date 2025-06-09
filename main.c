/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natferna <natferna@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 22:34:17 by jgamarra          #+#    #+#             */
/*   Updated: 2025/06/04 23:54:07 by natferna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void check_leaks(void)
{
	system("leaks minishell");
}

int main(int argc, char **argv, char **envp)
{
    t_minishell ms;
    char *input;

    prepare_minishell(&ms);
    valid_inital_param(argc, envp, &ms);
    disable_echoctl();
    catch_signal();
    init_history(&ms);

    while (1)
    {
        input = get_input(&ms);

        if (!input)
        {
            printf("exit\n");
            safe_free_minishell(&ms);
            exit(0);
        }

        size_t len = ft_strlen(input);
        if (len > 0 && input[len - 1] == '\r')
            input[len - 1] = '\0';

        ms.error_syntax = false;
        input = check_input_valid(input);

        if (*input)
        {
            save_history_file(&ms, input);
            t_cmd *cmd = parsecmd(input, &ms);
            control_cmd(cmd, &ms);
            free_cmd(cmd); // ✅ Libera memoria del comando
        }

        free(input);
    }

    return 0;
}
