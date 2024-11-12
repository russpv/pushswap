/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_freearr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpeavey <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 17:58:53 by rpeavey           #+#    #+#             */
/*   Updated: 2024/11/12 17:59:06 by rpeavey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_freearr(char **arr)
{
	size_t	i;
	size_t	len;

	i = -1;
	if (arr == NULL)
		return (FAILURE);
	len = ft_arrlen((const char **)arr);
	while (++i < len)
		free(arr[i]);
	free(arr);
	return (SUCCESS);
}
