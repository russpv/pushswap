/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_vprintf.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpeavey <rpeavey@student.42singapore.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/27 15:50:33 by rpeavey           #+#    #+#             */
/*   Updated: 2024/11/07 13:24:35 by rpeavey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/* VPRINTF
 * cspdiuxX% flags handled
 * variadic list args accepted
 */

/* Parses minwidth or minprec. Expects to be on a digit.
** Flag determines either normal numbers or precision after '.' 
** check: return false for illegal additional '.' 
*/
static int	_parse_qty(const char **s, size_t *var, int check, int *flag)
{
	unsigned int	qty;

	qty = 0;
	while (ft_isdigit(**s))
	{
		*flag = TRUE;
		qty = qty * 10 + (**s - '0');
		(*s)++;
	}
	*var = qty;
	if (check == TRUE)
	{
		*flag = TRUE;
		if (*((*s)) == '.')
			return (FALSE);
	}
	return (TRUE);
}

/* Reads any permutation of flags and quantities 
** or resets the string back to one after '%' for printing
** Ends on char after flag(s)/digit(s)/dot
*/
static inline int	_parse_specs_or_reset(const char **s, t_spec *specs, \
											unsigned int *bytes)
{
	while (in_set(*s, FLAGS))
	{
		if (**s == '-')
			specs->ljstflg = TRUE;
		if (**s == '+')
			specs->signflg = TRUE;
		if (**s == ' ' && specs->signflg == FALSE)
			specs->leadflg = TRUE;
		if (**s == '0' && specs->ljstflg == FALSE)
			specs->pch = '0';
		if (**s == '#')
			specs->altflg = TRUE;
		(*s)++;
	}
	if (ft_isdigit(**s))
		_parse_qty(s, &specs->minwidth, FALSE, &specs->mwflg);
	if (*((*s)) == '.')
	{
		(*s)++;
		if (_parse_qty(s, &specs->minprec, TRUE, &specs->mpflg) == FALSE)
			return (return_false_and_resetpos(s, specs, bytes));
	}
	return (TRUE);
}

/* Processes specifier printout. If s not a specifier, print 
** text as normal. 
*/
static inline int	_do_formats(const char **s, va_list *args, \
		t_spec *specs, unsigned int *bytes)
{
	if (!**s)
		return (FALSE);
	if (!in_set(*s, SPECS) && !in_set_twodig(*s, SPECS2))
		return (return_false_and_resetpos(s, specs, bytes));
	do_idu_formats(*s, args, specs, bytes);
	do_cs_formats(*s, *args, specs, bytes);
	do_xx_formats(*s, *args, specs, bytes);
	do_pc(*s, bytes);
	if (1 == do_ldzu_formats(*s, args, specs, bytes))
		(*s) += 2;
	else
		(*s)++;
	return (TRUE);
}

int	ft_vprintf(const char *s, va_list *args)
{
	t_spec				specs;
	unsigned int		bytes;

	bytes = 0;
	while (*s)
	{
		if (*s != '%')
			bytes += putchar_with_return((unsigned int)*s++);
		else
		{
			mark_init_and_advance(&s, &specs);
			if (in_set(s, FLAGS) == TRUE || ft_isdigit(*s) || *s == '.')
				if (_parse_specs_or_reset(&s, &specs, &bytes) == FALSE)
					continue ;
			_do_formats(&s, args, &specs, &bytes);
		}
	}
	return (bytes);
}
