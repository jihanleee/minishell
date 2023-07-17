/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jihalee <jihalee@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/03 12:52:47 by jihalee           #+#    #+#             */
/*   Updated: 2023/06/24 19:41:43 by jihalee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void	ft_putstr_count(char *str, int *n_bytes)
{
	int	i;

	if (str == 0)
	{
		ft_putstr_count("(null)", n_bytes);
		return ;
	}
	i = 0;
	while (str[i])
	{
		ft_putchar_count(str[i], n_bytes);
		i++;
	}
}

void	ft_putchar_count(char c, int *n_bytes)
{
	write(1, &c, 1);
	(*n_bytes)++;
}

int	is_in_argset(const char c)
{
	char	*argset;
	int		i;

	argset = "cspdiuxX";
	i = 0;
	while (argset[i])
	{
		if (c == argset[i])
			return (1);
		i++;
	}
	return (0);
}

void	dispatch(const char format, va_list ap, int *n_bytes)
{
	char	*decimal;
	char	*hex_l;
	char	*hex_u;

	decimal = "0123456789";
	hex_l = "0123456789abcdef";
	hex_u = "0123456789ABCDEF";
	if (format == 'c')
		ft_putchar_count(va_arg(ap, int), n_bytes);
	if (format == 'd' || format == 'i')
		ft_putnbr_base_count(va_arg(ap, int), decimal, n_bytes);
	if (format == 's')
		ft_putstr_count(va_arg(ap, char *), n_bytes);
	if (format == 'p')
		print_p(va_arg(ap, void *), n_bytes);
	if (format == 'u')
		print_nbr_base_u(va_arg(ap, unsigned int), 10, decimal, n_bytes);
	if (format == 'x')
		print_nbr_base_u(va_arg(ap, unsigned int), 16, hex_l, n_bytes);
	if (format == 'X')
		print_nbr_base_u(va_arg(ap, unsigned int), 16, hex_u, n_bytes);
}

int	ft_printf(const char *s, ...)
{
	va_list	ap;
	int		i;
	int		n_bytes;

	n_bytes = 0;
	va_start(ap, s);
	i = 0;
	while (s[i])
	{
		if (s[i] == '%' && is_in_argset(s[i + 1]))
			dispatch(s[i++ + 1], ap, &n_bytes);
		else if (s[i] == '%' && s[i + 1] == '%')
			ft_putchar_count(s[i++], &n_bytes);
		else
			ft_putchar_count(s[i], &n_bytes);
		i++;
	}
	va_end(ap);
	return (n_bytes);
}
