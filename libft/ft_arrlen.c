/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_arrlen.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpeavey <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 17:58:01 by rpeavey           #+#    #+#             */
/*   Updated: 2024/11/12 17:58:34 by rpeavey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/* Returns number of strings in a null-
 * terminated array of arrays.
 * Excludes NULL.
 */
size_t	ft_arrlen(const char**arr)
{
	size_t	count;

	count = 0;
	while (arr[count] != NULL)
		count++;
	return (count);
}
