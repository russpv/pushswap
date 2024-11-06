#include "libft.h"

/* SIZET is same as UINT for flags
 * LONG is same as INT for flags
 */
int	do_ldzu_formats(const char *s, va_list *args, t_spec *specs,
		unsigned int *bytes)
{
	long long			value;
	unsigned long long	uvalue;

	if (0 == ft_strncmp(s, "ld", 2))
	{
		value = va_arg(*args, long);
		reset_specs(specs, INT);
		*bytes += print_output(type_switch(&value, LONG, specs), specs);
		return (1);
	}
	else if (0 == ft_strncmp(s, "zu", 2))
	{
		uvalue = va_arg(*args, size_t);
		reset_specs(specs, UINT);
		*bytes += print_output(type_switch(&uvalue, SIZET, specs), specs);
		return (1);
	}
	else
		return (0);
}
