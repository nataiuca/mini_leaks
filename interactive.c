/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interactive.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natferna <natferna@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/15 12:28:48 by jgamarra          #+#    #+#             */
/*   Updated: 2025/06/04 23:52:25 by natferna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	disable_echoctl(void)
{
	struct termios	term;

	if (tcgetattr(STDIN_FILENO, &term) == -1)
	{
		perror("tcgetattr");
		exit(EXIT_FAILURE);
	}
	term.c_lflag &= ~ECHOCTL;
	if (tcsetattr(STDIN_FILENO, TCSANOW, &term) == -1)
	{
		perror("tcsetattr");
		exit(EXIT_FAILURE);
	}
}

/*
* catch control-c and print value ^C in each line.
*/
static void	sigint_handler(int signal)
{
	(void)signal;
	rl_replace_line("", 0);
	rl_on_new_line();
	printf("\n");
	rl_redisplay();
}

/*
* catch control-c with handler and control-\ ignoring.
* SIGINT control-c
* SIGQUIT control-\
*/
void	catch_signal(void)
{
	signal (SIGINT, sigint_handler);
	signal (SIGQUIT, SIG_IGN);
}

/*
* catch control-d, get null input and print exit.
*/
void	catch_interactive(t_minishell *minishell, char *input, char *prompt)
{
    if (!input)
    {
        (void)prompt;
        printf("exit\n");
        safe_free_minishell(minishell);
        exit(0);
    }
}

/*
* save input to history filtering empty lines
*/
void	save_history(char *input)
{
	if (input[0] != '\0')
		add_history(input);
}

/*
* Check if the input is valid
* Ignore empty lines
* Ignore empty lines with spaces or tabs
* 0 false 1 true
*/
char	*check_input_valid(char *input)
{
	if (input[0] != '\0')
	{
		input = trim_space_char(input);
		save_history(input);
	}
	return (input);
}
