/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strtol.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gderoyqn <gderoyqn@student.42london.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 23:27:21 by gderoyqn          #+#    #+#             */
/*   Updated: 2024/12/21 00:19:12 by gderoyqn         ###   ########.fr       */
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

#define RED "\033[0;31m"
#define GREEN "\033[0;32m"
#define RESET "\033[0m"

void test_strtoll(const char *input, int base, long long expected, int expected_errno) {
    long long result;
    int original_errno = errno;

    errno = 0;
    result = mini_strtoll(input, base);

    printf("Input: \"%s\", Base: %d, expected errno = %d\n", input, base, expected_errno);
    if (result == expected && errno == expected_errno) {
        printf(GREEN "  Passed: " RESET "Result = %lld, errno = %d\n", result, errno);
    } else {
        printf(RED "  Failed: " RESET);
        printf("Expected Result = %lld, Actual Result = %lld\n", expected, result);
        printf("Expected errno = %d, Actual errno = %d\n", expected_errno, errno);
    }
    errno = original_errno;
}

/*
int main() {
    printf("Running tests for mini_strtoll...\n\n");

    // Test cases based on the man page description

    // Basic decimal conversion
    test_strtoll("12345", 10, 12345, 0);
    test_strtoll("+12345", 10, 12345, 0);
    test_strtoll("-12345", 10, -12345, 0);
    test_strtoll("   12345", 10, 12345, 0);
    test_strtoll("   +12345", 10, 12345, 0);
    test_strtoll("   -12345", 10, -12345, 0);
    
    // basic octal conversion
    test_strtoll("010", 0, 8, 0);
    test_strtoll("010", 8, 8, 0);
    test_strtoll("  -010", 0, -8, 0);

    // Basic hexadecimal conversion
    test_strtoll("0x1A", 16, 26, 0);
    test_strtoll("0X1a", 16, 26, 0);
    test_strtoll("0x1A", 0, 26, 0);
    test_strtoll("   -0x1A", 16, -26, 0);
    test_strtoll("   -0x1a", 0, -26, 0);
    

    // Other bases
    test_strtoll("101101", 2, 45, 0);
    test_strtoll("FF", 16, 255, 0);
    test_strtoll("ff", 16, 255, 0);
    test_strtoll("z", 36, 35, 0);
    test_strtoll("Z", 36, 35, 0);
    

    // Invalid input
    test_strtoll("abc", 10, 0, EINVAL);
    test_strtoll("   ", 10, 0, EINVAL);
    test_strtoll("", 10, 0, EINVAL);
    test_strtoll("12ab", 10, 12, 0);
    test_strtoll("0xxyz", 16, 0, EINVAL);
    test_strtoll("0xxyz", 0, 0, EINVAL);

    // Invalid base
    test_strtoll("123", 1, 0, EINVAL);
    test_strtoll("123", 37, 0, EINVAL);
    test_strtoll("123", -1, 0, EINVAL);

    // Overflow and underflow
    test_strtoll("9223372036854775807", 10, LLONG_MAX, 0);  // LLONG_MAX
    test_strtoll("9223372036854775808", 10, LLONG_MAX, ERANGE);
    test_strtoll("-9223372036854775808", 10, LLONG_MIN, 0); // LLONG_MIN
    test_strtoll("-9223372036854775809", 10, LLONG_MIN, ERANGE);

    printf("\nTests completed.\n");

    return 0;
}
*/
