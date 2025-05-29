/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pruiz-al <pruiz-al@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 12:06:03 by sbenitez          #+#    #+#             */
/*   Updated: 2025/05/29 20:26:35 by pruiz-al         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	exec_env(t_shell *shell)
{
	int	i;

	i = 0;
	ft_add_modify_env(shell->env, "_=/usr/bin/env", 1);
	while (shell->env[i])
	{
		if (ft_strchr(shell->env[i], '='))
			printf("%s\n", shell->env[i]);
		i++;
	}
	return (0);
}
