/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   compare.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnicosia <lnicosia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/06 15:03:22 by lnicosia          #+#    #+#             */
/*   Updated: 2021/09/22 18:25:13 by lnicosia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "nm.h"

char		ignore_char(char c)
{
	if (!c)
		return (0);
	if (c == '_' || c == '*' || c == '.' || c == '(' || c == ')' || c == '/'
		|| c == '$')
		return (c);
	return (0);
}

/*
**	Function comparing the two file names that is going to be used
**	to go through our dlist
*/

int		compare_names(void *s1, void *s2)
{
	return (ft_strcmp(((t_sym*)s1)->name, ((t_sym*)s2)->name));
}

/*
**	Function comparing the two file names that is going to be used
**	to go through our dlist
*/

int		compare_names_no_special(void *ptr1, void *ptr2)
{
	int			ignored;
	int			case_diff;
	int			s1_ignored;
	int			s2_ignored;
	char		*s1, *s2;
	char		*str1, *str2;
	t_sym		*sym1, *sym2;

	sym1 = (t_sym*)ptr1;
	sym2 = (t_sym*)ptr2;
	s1 = sym1->name;
	s2 = sym2->name;
	str1 = s1;
	str2 = s2;
	(void)str1;
	(void)str2;
	ignored = 0;
	case_diff = 0;
	while (*s1 && *s2)
	{
		s1_ignored = 0;
		s2_ignored = 0;
		// Skip the ignored chars in the strings
		// Known chars to ignore: _ * . ( ) / $
		// Count how many chars were ignored to remember priority
		while (ignore_char(*s1))
		{
		//	if (ft_strequ(str1, "runtime/cgo(.text)") && ft_strequ(str2, "runtime/cgo(.text)"))
		//		ft_printf("Ignoring %c in s1:\"%s\" (\"%s\")\n", *s1, str1, s1);
			s1++;
			s1_ignored++;
		}
		while (ignore_char(*s2))
		{
		//	if (ft_strequ(str1, "runtime/cgo(.text)") && ft_strequ(str2, "runtime/cgo(.text)"))
		//		ft_printf("Ignoring %c in s2:\"%s\" (\"%s\")\n", *s2, str2, s2);
			s2++;
			s2_ignored++;
		}
		//if (ft_strequ(str1, "runtime/cgo(.text)") && ft_strequ(str2, "runtime/cgo(.text)"))
		//	ft_printf("After ignored, s1 = \"%s\", s2 = \"%s\"\n", s1, s2);
		// The string with the less ignored chars comes first
		if (s1_ignored > s2_ignored)
			ignored = 2;
		else if (s1_ignored < s2_ignored)
			ignored = 1;
		// If they are equal but with different case,
		// priority to the low case
		if (*s1 != *s2 && ft_tolower(*s1) == ft_tolower(*s2))
		{
			if (*s1 >= 'a' && *s1 <= 'z')
				case_diff = 2;
			else if (*s2 >= 'a' && *s2 <= 'z')
				case_diff = 1;
		}
		if (ft_tolower(*s1) != ft_tolower(*s2))
		{
			/*if (ft_strequ(str1, "runtime/cgo(.text)") && ft_strequ(str2, "runtime/cgo(.text)"))
			{
				ft_printf("COUCOU\n");
				ft_printf("s1 = %s\n", s1);
				ft_printf("s2 = %s\n", s2);
				ft_printf("str1 = %s\n", str1);
				ft_printf("str2 = %s\n", str2);
			}*/
			return (ft_tolower(*s1) - ft_tolower(*s2));
		}
		// If the string ended with a ignored char, we're already at the \0
		// and thus advancing again would set us in the next symbol node
		if (*s1)
			s1++;
		if (*s2)
			s2++;
	}
	if (ft_tolower(*s1) == ft_tolower(*s2) && case_diff != 0)
	{
		if (case_diff == 1)
			return (1);
		else if (case_diff == 2)
			return (-1);
	}
	if (ft_tolower(*s1) == ft_tolower(*s2) && ignored != 0)
	{
		//if ((ft_strequ(str1, "runtime.funcname") && ft_strequ(str2, "runtime.(*Func).Name"))
		//	|| (ft_strequ(str1, "runtime.(*Func).Name") && ft_strequ(str2, "runtime.funcname")))
		//	ft_printf("COUCOU\n");
		if (ignored == 1)
			return (1);
		else if (ignored == 2)
			return (-1);
	}
	if (ft_tolower(*s1) != ft_tolower(*s2))
		return (ft_tolower(*s1) - ft_tolower(*s2));
	if (ELF64_ST_TYPE(sym1->sym->st_info) != ELF64_ST_TYPE(sym2->sym->st_info))
		return (ELF64_ST_TYPE(sym1->sym->st_info) > ELF64_ST_TYPE(sym2->sym->st_info));
	//if (ft_strequ(str1, "runtime/cgo(.text)") && ft_strequ(str2, "runtime/cgo(.text)"))
	//	ft_printf("COUCOU\n");
	// Last comparison is between the addresses
	// For some reason, it's inversed when symbol is in the bss section
	if (sym1->type == 'b')
		return (sym1->sym->st_value < sym2->sym->st_value);
	return (sym1->sym->st_value > sym2->sym->st_value);
}
