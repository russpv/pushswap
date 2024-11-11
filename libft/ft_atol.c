/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpeavey <rpeavey@student.42singapore.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/27 20:01:44 by rpeavey           #+#    #+#             */
/*   Updated: 2024/05/27 20:16:36 by rpeavey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/* ATOL
** Returns long value of byte string.
** Skips any isspace(3)
** No error checking.
*/

long	ft_atol(const char *s)
{
	long	num;
	int		flag;

	flag = 1;
	num = 0;
	while (ft_isspace(*s))
		s++;
	if (*s == '-' || *s == '+')
	{
		if (*s == '-')
			flag *= -1;
		s++;
	}
	while (ft_isdigit(*s))
	{
		num *= 10;
		num += *s - '0';
		s++;
	}
	return (flag * num);
}
