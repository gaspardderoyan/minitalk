/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gderoyqn <gderoyqn@student.42london.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 18:39:18 by gderoyqn          #+#    #+#             */
/*   Updated: 2025/02/13 19:22:22 by gderoyqn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft/inc/libft.h"
#include "minitalk.h"

struct s_metadata	g_mt;
static void	send_bit(void);

static void	signal_handler(int signal)
{
	if (signal == SIGUSR1)
		send_bit();
	if (signal == SIGUSR2)
	{
		write(1, "done!\n", 6);
		exit(0);
	}
}

static void	send_bit(void)
{
	uint8_t	curr_bit;

	curr_bit = (g_mt.str[g_mt.c] >> (7 - g_mt.i)) & 1;
	errno = 0;
	if (curr_bit)
		kill(g_mt.pid, SIGUSR2);
	else
		kill(g_mt.pid, SIGUSR1);
	if (errno)
	{
		ft_putstr_fd("dumb fuck\n", 2);
		exit(0);
	}
	if (g_mt.i == 7)
	{
		g_mt.i = 0;
		g_mt.c++;
	}
	else
		g_mt.i++;
}

static void	set_signal_action(void)
{
	struct sigaction	act;

	ft_memset(&act, 0, sizeof(act));
	act.sa_handler = &signal_handler;
	sigemptyset(&act.sa_mask);
	sigaction(SIGUSR1, &act, NULL);
	sigaction(SIGUSR2, &act, NULL);
}

int	main(int ac, char **av)
{
	if (ac != 3)
		return (ft_putstr_fd("Usage: %s <server_pid> <string>\n", 2), 1);
	g_mt.pid = ft_atoi(av[1]);
	if (!g_mt.pid)
	{
		ft_putstr_fd("dumb fuck\n", 2);
		exit(0);
	}
	g_mt.i = 0;
	g_mt.c = 0;
	g_mt.str = av[2];
	set_signal_action();
	send_bit();
	while (1)
		pause();
	return (0);
}
