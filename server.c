/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gderoyqn <gderoyqn@student.42london.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 18:39:04 by gderoyqn          #+#    #+#             */
/*   Updated: 2025/02/13 18:55:19 by gderoyqn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minitalk.h"

struct current_byte curr_b;

void	signal_handler(int signum, siginfo_t *info, void *context)
{
	if (signum == SIGUSR1)
		curr_b.counter++;
	if (signum == SIGUSR2)
	{
		curr_b.bytes |= (1 << (8 - curr_b.counter));
		curr_b.counter++;
	}
	curr_b.received_signal = 1;
	curr_b.current_pid = info->si_pid;
}

void	set_signal_action(void)
{
	struct sigaction	act;

	ft_memset(&act, 0, sizeof(act));
	act.sa_sigaction = &signal_handler;
	act.sa_flags = SA_SIGINFO;
	sigaction(SIGUSR1, &act, NULL);
	sigaction(SIGUSR2, &act, NULL);
}

int	main(void)
{
	ft_printf("%i\n", getpid());
	curr_b.bytes = 0;
	curr_b.counter = 1;
	curr_b.received_signal = 0;
	set_signal_action();
	while (1)
	{
		while (!curr_b.received_signal)
			pause();
		curr_b.received_signal = 0;
		if (curr_b.counter == 9)
		{
			write(1, &curr_b.bytes, 1);
			if (!curr_b.bytes)
			{
				kill(curr_b.current_pid, SIGUSR2);
				write(1, "\n", 1);
				curr_b.bytes = 0;
				curr_b.counter = 1;
				continue ;
			}
			curr_b.bytes = 0;
			curr_b.counter = 1;
		}
		kill(curr_b.current_pid, SIGUSR1);
	}
	return (0);
}
