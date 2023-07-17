/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jihalee <jihalee@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 15:43:31 by jihalee           #+#    #+#             */
/*   Updated: 2023/06/22 18:11:31 by jihalee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H
# include <stdarg.h>
# include <string.h>
# include <unistd.h>

void	ft_putchar_count(char c, int *n_bytes);
void	ft_putstr_count(char *str, int *n_bytes);
void	print_p(void *addr, int *n_bytes);
void	print_nbr_base(int nbr, int radix, char *base, int *n_bytes);
void	ft_putnbr_base_count(int nbr, char *base, int *n_bytes);
void	print_ulong(unsigned long nbr, int radix, char *base, int *n_bytes);
void	print_nbr_base_u(unsigned int nbr, int radix, char *base, int *n_bytes);

int		is_in_argset(const char c);
void	dispatch(const char format, va_list ap, int *n_bytes);
int		ft_printf(const char *s, ...);

#endif
