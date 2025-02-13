/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gderoyqn <gderoyqn@student.42london.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 18:39:18 by gderoyqn          #+#    #+#             */
/*   Updated: 2025/02/13 18:43:04 by gderoyqn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft/inc/libft.h"
#include "minitalk.h"

struct metadata	mt;
void	send_bit(void);

void	signal_handler(int signal)
{
	if (signal == SIGUSR1)
		send_bit();
	if (signal == SIGUSR2)
	{
		write(1, "done!\n", 6);
		exit(0);
	}

}

void	send_bit(void)
{
	uint8_t	curr_bit;

	curr_bit = (mt.str[mt.c] >> (7 - mt.i)) & 1;
	if (curr_bit)
		kill(mt.pid, SIGUSR2);
	else
		kill(mt.pid, SIGUSR1);
	if (mt.i == 7)
	{
		mt.i = 0;
		mt.c++;
	}
	else
		mt.i++;
}

void	set_signal_action(void)
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

    mt.pid = ft_atoi(av[1]);
    mt.i = 0;
    mt.c = 0;
	mt.str = av[2];
    set_signal_action();
    send_bit();
    while (1)
        pause();
    return (0);
}
