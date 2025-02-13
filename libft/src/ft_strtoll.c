/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtoll.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gderoyqn <gderoyqn@student.42london.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 23:27:21 by gderoyqn          #+#    #+#             */
/*   Updated: 2025/02/13 19:27:24 by gderoyqn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/libft.h"

int	ft_isspace(int c)
{
	const unsigned char	cc = (unsigned char)c;

	return ((cc >= 9 && cc <= 13) || cc == 32);
}

int	base_index(char c, int radix)
{
	const char	*base = "0123456789abcdefghijklmnopqrstuvwxyz";
	int			index;

	index = 0;
	c = ft_tolower(c);
	while (base[index] && index < radix)
	{
		if (base[index] == c)
			return (index);
		index++;
	}
	return (-1);
}

int	handle_overflow(int sign, long long *n, int digit, int radix)
{
	if (sign == 1 && *n > (LLONG_MAX - digit) / radix)
	{
		errno = ERANGE;
		*n = LLONG_MAX;
		return (true);
	}
	if (sign == -1 && -(*n) < (LLONG_MIN + digit) / radix)
	{
		errno = ERANGE;
		*n = LLONG_MIN;
		return (true);
	}
	return (false);
}

void	skip_lead(const char **nptr, int *sign, int *radix)
{
	errno = 0;
	while (ft_isspace(**nptr))
		(*nptr)++;
	*sign = 1;
	if (**nptr == '-')
	{
		*sign = -1;
		(*nptr)++;
	}
	else if (**nptr == '+')
		(*nptr)++;
	if ((*radix == 0 || *radix == 16) && **nptr == '0'
		&& (ft_tolower(*(*nptr + 1)) == 'x'))
	{
		*nptr += 2;
		*radix = 16;
	}
	else if (*radix == 0 && **nptr == '0')
	{
		(*nptr)++;
		*radix = 8;
	}
	else if (*radix == 0)
		*radix = 10;
}

long long	mini_strtoll(const char *nptr, int radix)
{
	int			sign;
	int			index;
	int			empty;
	long long	n;

	sign = 1;
	n = 0;
	empty = 1;
	skip_lead(&nptr, &sign, &radix);
	if (radix < 2 || radix > 36)
		return (errno = EINVAL, 0);
	while (*nptr && (base_index(*nptr, radix) > -1))
	{
		empty = 0;
		index = base_index(*nptr, radix);
		if (handle_overflow(sign, &n, index, radix))
			return (n);
		n = n * radix + index;
		nptr++;
	}
	if (empty)
		return (errno = EINVAL, 0);
	return (n * sign);
}
