/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgamarra <jgamarra@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 21:27:25 by jgamarra          #+#    #+#             */
/*   Updated: 2025/05/23 19:32:06 by jgamarra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"




void valid_command(t_execcmd *ecmd, t_minishell *minishell)
{
  if (strchr("./", ecmd->argv[0][0]))
  {
    // #valid if command is executable
    if (access(ecmd->argv[0], F_OK) == -1)
    {
      ft_putstr_fd("minishell: ", 2);
      ft_putstr_fd(ecmd->argv[0], 2);
      ft_putstr_fd(": command not found\n", 2);
      exit(127);
    }
  }
  else
  {
    // loop into minishell->list_path to find the command path
    int i = -1;
    while (minishell->path_env[++i])
    {
      char *command = ft_strjoin("/", ecmd->argv[0]);
      char *full_command = ft_strjoin(minishell->path_env[i], command);
      if (access(full_command, F_OK) == 0)
      {
        ecmd->argv[0] = full_command;
        break;
      }
      free(command);
      free(full_command);
    }
    if (!strchr("./", ecmd->argv[0][0]))
    {
      ft_putstr_fd("minishell: ", 2);
      ft_putstr_fd(ecmd->argv[0], 2);
      ft_putstr_fd(": command not found\n", 2);
      exit(127);
    }
  }
}

char *read_stdin_to_str(void)
{
	char buffer[1024];
	char *result = malloc(1);
	ssize_t bytes_read;
	size_t total = 0;

	if (!result)
		return (NULL);
	result[0] = '\0';

	while ((bytes_read = read(STDIN_FILENO, buffer, sizeof(buffer))) > 0)
	{
		char *tmp = malloc(total + bytes_read + 1);
		if (!tmp)
			return (NULL);
		for (size_t i = 0; i < total; i++)
			tmp[i] = result[i];
		for (ssize_t j = 0; j < bytes_read; j++)
			tmp[total + j] = buffer[j];
		tmp[total + bytes_read] = '\0';
		free(result);
		result = tmp;
		total += bytes_read;
	}
	return result;
}

// Execute cmd.  Never returns.
void runcmd(struct cmd *cmd, t_minishell *minishell)
{
  int p[2];
  // struct backcmd *bcmd;
  struct execcmd *ecmd;
  // struct listcmd *lcmd;
  struct pipecmd *pcmd;
  struct redircmd *rcmd;
	pid_t pid1, pid2;

  if (cmd == 0)
    exit(0);

  switch (cmd->type)
  {
  default:
    panic("runcmd");

  case EXEC:
    ecmd = (struct execcmd *)cmd;
    if (ecmd->argv[0] == 0)
      exit(0);
	if (valid_builtins(cmd))
	{
		run_internal(cmd, minishell);
	}
	else
	{
    valid_command(ecmd, minishell);
    int idx = 1;
    while (ecmd->argv[idx])
    {
        ecmd->argv[idx] = expand_variables(ecmd->argv[idx], minishell);
        remove_quotes(ecmd, idx);
        idx++;
    }
    exec_command(ecmd->argv[0], ecmd->argv);
		ft_putstr_fd("exec failed ", 2);
		ft_putstr_fd(ecmd->argv[0], 2);
		ft_putchar_fd('\n', 2);
	}
    break;

  case REDIR:
    rcmd = (struct redircmd *)cmd;
    if (rcmd->hdoc) {
        int pipefd[2];
        if (pipe(pipefd) < 0)
            panic("pipe error");
        // write heredoc content to pipe
        write(pipefd[1], rcmd->hdoc, ft_strlen(rcmd->hdoc));
        close(pipefd[1]);

        // redirect stdin
        dup2(pipefd[0], STDIN_FILENO);
        close(pipefd[0]);
        free(rcmd->hdoc);

    }
     else {
        int fd;
        if (rcmd->mode == O_RDONLY) {
            fd = open(rcmd->file, O_RDONLY);
            if (fd < 0) {
                perror("open redirection failed");
                exit(1);
            }
            dup2(fd, STDIN_FILENO);
            close(fd);
        } else if (rcmd->mode == (O_WRONLY | O_CREAT | O_TRUNC)) {
            fd = open(rcmd->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd < 0) {
                perror("open redirection failed");
                exit(1);
            }
            dup2(fd, STDOUT_FILENO);
            close(fd);
        } else if (rcmd->mode == (O_WRONLY | O_CREAT | O_APPEND)) {
            fd = open(rcmd->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
            if (fd < 0) {
                perror("open redirection failed");
                exit(1);
            }
            dup2(fd, STDOUT_FILENO);
            close(fd);
        } else {
            perror("unknown redirection mode");
            exit(1);
        }
    }
    runcmd(rcmd->cmd, minishell);
    break;
  case PIPE:
    pcmd = (struct pipecmd *)cmd;
    if (pipe(p) < 0)
      panic("pipe");
    if ((pid1 = fork1()) == 0)
    {
      close(1);
    //   dup(p[1]);
		dup2(p[1], 1);
      close(p[0]);
      close(p[1]);
      runcmd(pcmd->left, minishell);
    }
    if ((pid2 = fork1()) == 0)
    {
      close(0);
    //   dup(p[0]);
		dup2(p[0], 0);
      close(p[0]);
      close(p[1]);
      runcmd(pcmd->right, minishell);
    }
    close(p[0]);
    close(p[1]);
    waitpid(pid1, NULL, 0);
	waitpid(pid2, NULL, 0);
    break;
  }
  exit(0);
}

void exec_command(char *command, char **args)
{
  if (execvp(command, args) == -1)
  {
    perror("exec failed");
    exit(EXIT_FAILURE);
  }
}

void control_cmd(t_cmd *cmd, t_minishell *minishell)
{
  if (minishell->error_syntax)
    return ;
	if (valid_builtins(cmd))
	{
		run_internal(cmd, minishell);
	}
	else
	{
		run_external(cmd, minishell);
	}
}
