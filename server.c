/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gderoyqn <gderoyqn@student.42london.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 18:39:04 by gderoyqn          #+#    #+#             */
/*   Updated: 2025/02/13 19:25:45 by gderoyqn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minitalk.h"

struct s_current_byte	g_curr_b;

static void	signal_handler(int signum, siginfo_t *info, void *context)
{
	if (signum == SIGUSR1)
		g_curr_b.counter++;
	if (signum == SIGUSR2)
	{
		g_curr_b.bytes |= (1 << (8 - g_curr_b.counter));
		g_curr_b.counter++;
	}
	g_curr_b.received_signal = 1;
	g_curr_b.current_pid = info->si_pid;
}

static void	set_signal_action(void)
{
	struct sigaction	act;

	ft_memset(&act, 0, sizeof(act));
	act.sa_sigaction = &signal_handler;
	act.sa_flags = SA_SIGINFO;
	sigaction(SIGUSR1, &act, NULL);
	sigaction(SIGUSR2, &act, NULL);
}

static void	setup_struct(void)
{
	g_curr_b.bytes = 0;
	g_curr_b.counter = 1;
	g_curr_b.received_signal = 0;
}

int	main(void)
{
	ft_printf("%i\n", getpid());
	setup_struct();
	set_signal_action();
	while (1)
	{
		while (!g_curr_b.received_signal)
			pause();
		g_curr_b.received_signal = 0;
		if (g_curr_b.counter == 9)
		{
			write(1, &g_curr_b.bytes, 1);
			if (!g_curr_b.bytes)
			{
				kill(g_curr_b.current_pid, SIGUSR2);
				write(1, "\n", 1);
				g_curr_b.bytes = 0;
				g_curr_b.counter = 1;
				continue ;
			}
			g_curr_b.bytes = 0;
			g_curr_b.counter = 1;
		}
		kill(g_curr_b.current_pid, SIGUSR1);
	}
	return (0);
}
