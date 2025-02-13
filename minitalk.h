/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minitalk.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gderoyqn <gderoyqn@student.42london.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 19:19:45 by gderoyqn          #+#    #+#             */
/*   Updated: 2025/02/13 19:25:18 by gderoyqn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/inc/libft.h"
#include <sys/types.h>
#include <unistd.h>
#include <strings.h>
#include <signal.h>
#include <stdint.h>

struct s_current_byte
{
	u_int8_t				bytes;
	u_int8_t				counter;
	volatile sig_atomic_t	received_signal;
	int						current_pid;
};

struct s_metadata
{
	int		pid;
	int		i;
	int		c;
	char	*str;
};
