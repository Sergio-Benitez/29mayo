/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbenitez <sbenitez@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 12:02:24 by sbenitez          #+#    #+#             */
/*   Updated: 2025/05/30 14:15:50 by sbenitez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	execute_builtin(t_shell *ms, t_cmd *cmd, int prevfd)
{
	if (!cmd->args[0])
		cmd->args[0] = ft_strdup("");
	if (!ft_strncmp(cmd->args[0], "echo", 5))
		ms->exit_status = exec_echo(cmd);
	else if (!ft_strncmp(cmd->args[0], "cd", 3))
		ms->exit_status = exec_cd(ms, cmd);
	else if (!ft_strncmp(cmd->args[0], "pwd", 4))
		ms->exit_status = exec_pwd();
	else if (!ft_strncmp(cmd->args[0], "export", 7))
		ms->exit_status = exec_export(ms, cmd);
	else if (!ft_strncmp(cmd->args[0], "unset", 6))
		ms->exit_status = exec_unset(ms, cmd);
	else if (!ft_strncmp(cmd->args[0], "env", 4))
		ms->exit_status = exec_env(ms);
	else if (!ft_strncmp(cmd->args[0], "exit", 5))
		exec_exit(ms, cmd, prevfd);
	else
		return ;
}

void	child_process(t_cmd *cmd, int prevfd, int pipefd[2], t_shell *ms)
{
	if (ft_redirections(ms, cmd) == 1)
		exit (1);
	if (prevfd != -1)
		dup2(prevfd, STDIN_FILENO);
	if (cmd->next && !cmd->outfile)
		dup2(pipefd[1], STDOUT_FILENO);
	if (prevfd != -1)
		close(prevfd);
	if (cmd->next)
	{
		close(pipefd[0]);
		close(pipefd[1]);
	}
	if (cmd->is_btn)
	{
		execute_builtin(ms, cmd, prevfd);
		exit(ms->exit_status);
	}
	else
		execute_command(ms, cmd);
	perror("Error executing\n");
	exit (126);
}

void	parent_process(t_shell *ms, int *prevfd, int pipefd[2])
{
	if (*prevfd != -1)
		close(*prevfd);
	if (ms->cmd_lst->next)
	{
		close(pipefd[1]);
		*prevfd = pipefd[0];
	}
	else
		*prevfd = -1;
	g_signal_flag = 1;
}

void	ft_wait_all_processes(pid_t *pids, t_shell *ms)
{
	int		status;
	int		i;
	t_cmd	*cmd;

	i = 0;
	cmd = ms->cmd_lst;
	while (cmd)
	{
		waitpid(pids[i], &status, 0);
		i++;
		if (!cmd->next)
			ft_check_exitstat(status, ms);
		cmd = cmd->next;
	}
}

void	ft_exec_commands(t_shell *ms)
{
	int		pipefd[2];
	int		prevfd;
	t_cmd	*cmd;
	pid_t	pids[LOL];
	int		i;

	i = 0;
	cmd = ms->cmd_lst;
	prevfd = -1;
	while (cmd)
	{
		if (cmd->is_btn && !cmd->next && (prevfd == -1))
		{
			execute_builtin(ms, cmd, prevfd);
			return ;
		}
		if (cmd->next && pipe(pipefd) == -1)
		{
			perror("Error creating pipe\n");
			exit(1);
		}
		pids[i] = fork();
		if (pids[i] == -1)
		{
			return (perror("Error creating child process.\n"));
			exit(1);
		}
		if (pids[i] == 0)
			child_process(cmd, prevfd, pipefd, ms);
		else
		{
			parent_process(ms, &prevfd, pipefd);
			if (cmd->is_btn && !ft_strncmp(cmd->args[0], "exit", 5) && (prevfd == -1))
				exit(ms->exit_status);
			cmd = cmd->next;
		}
		i++;
	}
	ft_wait_all_processes(pids, ms);
}
