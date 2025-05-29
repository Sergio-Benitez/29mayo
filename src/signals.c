/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pruiz-al <pruiz-al@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 17:52:12 by sbenitez          #+#    #+#             */
/*   Updated: 2025/05/29 20:05:10 by pruiz-al         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	g_signal_flag;

void	ft_check_exitstat(int status, t_shell *ms)
{
	if (WIFEXITED(status))
		ms->exit_status = WEXITSTATUS(status) % 256;
	else if (WIFSIGNALED(status))
		ms->exit_status = (128 + WTERMSIG(status)) % 256;
}

void	ft_handle_sigint(int signum)
{
	if (g_signal_flag == 0 || g_signal_flag == 130)
	{
		g_signal_flag = 128 + signum;
		write(1, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
	else if (g_signal_flag == 1)
	{
		g_signal_flag = 128 + signum;
		write(1, "\n", 1);
		rl_replace_line("", 0);
	}
	else if (g_signal_flag == 2)
	{
		g_signal_flag = 128 + signum;
		write(1, "\n", 1);
		exit(130);
	}
	else
		fprintf(stderr, "sefue: %i\n", g_signal_flag);
}

void	ft_setup_signals(void)
{
	g_signal_flag = 0;
	signal(SIGINT, ft_handle_sigint);
	signal(SIGQUIT, SIG_IGN);
}
