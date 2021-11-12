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
		|| c == '$' || c == ' ' || c == ';' || c == '{' || c == '}' || c == '-'
		|| c == '[' || c == ']' || c == ',' || c == '@')
		return (c);
	return (0);
}

/*
**	Function comparing the two file names that is going to be used
**	to go through our dlist
*/

int		no_sort(void *s1, void *s2)
{
	(void)s1;
	(void)s2;
	return (0);
}

int		compare_names32(void *s1, void *s2)
{
	return (ft_strcmp(((t_sym32*)s1)->name, ((t_sym32*)s2)->name));
}

int		compare_names64(void *s1, void *s2)
{
	return (ft_strcmp(((t_sym64*)s1)->name, ((t_sym64*)s2)->name));
}


int		compare_addresses32(void *s1, void *s2)
{
	t_sym32	*sym1 = (t_sym32*)s1;
	t_sym32	*sym2 = (t_sym32*)s2;
	uint32_t value1 = sym1->sym.st_value;
	uint32_t value2 = sym2->sym.st_value;
	if (sym1->sym.st_shndx == 0 && sym2->sym.st_shndx != 0)
		return (0);
	if (sym2->sym.st_shndx == 0 && sym1->sym.st_shndx != 0)
		return (1);
	if (value1 == value2)
		return (compare_names32(s1, s2));
	return (value1 > value2);
}

int		compare_addresses64(void *s1, void *s2)
{
	t_sym64	*sym1 = (t_sym64*)s1;
	t_sym64	*sym2 = (t_sym64*)s2;
	uint64_t value1 = sym1->sym.st_value;
	uint64_t value2 = sym2->sym.st_value;
	if (sym1->sym.st_shndx == 0 && sym2->sym.st_shndx != 0)
		return (0);
	if (sym2->sym.st_shndx == 0 && sym1->sym.st_shndx != 0)
		return (1);
	if (value1 == value2)
		return (compare_names64(s1, s2));
	return (value1 > value2);
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
	int			s1_first_real_char;
	int			s2_first_real_char;
	char		*s1, *s2;
	char		*str1, *str2;
	t_sym64		*sym1, *sym2;

	sym1 = (t_sym64*)ptr1;
	sym2 = (t_sym64*)ptr2;
	s1 = sym1->name;
	s2 = sym2->name;
	str1 = s1;
	str2 = s2;
	(void)str1;
	(void)str2;
	ignored = 0;
	case_diff = 0;
	s1_first_real_char = 0;
	s2_first_real_char = 0;
	(void)s1_first_real_char;
	(void)s2_first_real_char;
	s1_ignored = 0;
	s2_ignored = 0;
	(void)s1_ignored;
	(void)s2_ignored;
	while (*s1 && *s2)
	{
		// Skip the ignored chars in the strings
		// Known chars to ignore: _ * . ( ) / $
		// Count how many chars were ignored to remember priority
		/*while (ignore_char(*s1))
		{
		//	if (ft_strequ(str1, "runtime/cgo(.text)") && ft_strequ(str2, "runtime/cgo(.text)"))
		//		ft_printf("Ignoring %c in s1:\"%s\" (\"%s\")\n", *s1, str1, s1);
			s1++;
			//if (s1_first_real_char == 0)
				s1_ignored++;
		}
		s1_first_real_char = 1;
		while (ignore_char(*s2))
		{
		//	if (ft_strequ(str1, "runtime/cgo(.text)") && ft_strequ(str2, "runtime/cgo(.text)"))
		//		ft_printf("Ignoring %c in s2:\"%s\" (\"%s\")\n", *s2, str2, s2);
			s2++;
			//if (s2_first_real_char == 0)
				s2_ignored++;
		}
		s2_first_real_char = 1;*/
		while (ignore_char(*s1) && ignore_char(*s2))
		{
			s1++;
			s2++;
		}
		if (ignore_char(*s1))
		{
			if ((ft_strstr(str1, "(*FileActionRm).Size") && ft_strstr(str2, "(*FileAction_Rm).Size"))
			|| (ft_strstr(str1, "(*FileAction_Rm).Size") && ft_strstr(str2, "(*FileActionRm).Size")))
			{
				ft_printf("COUCOU1\n");
				ft_printf("str1 = %s\n", str1);
				ft_printf("str2 = %s\n", str2);
				ft_printf("Ignored = %d\n", ignored);
				ft_printf("Comparing '%c' (%d) with '%c' (%d)\n", *s1, *s1, *s2, *s2);
			}
			if ((ft_strstr(str1, "(*FileActionRm).Size") && ft_strstr(str2, "(*FileAction_Rm).Size"))
			|| (ft_strstr(str1, "(*FileAction_Rm).Size") && ft_strstr(str2, "(*FileActionRm).Size")))
			{
				ft_printf("COUCOU1\n");
				ft_printf("str1 = %s\n", str1);
				ft_printf("str2 = %s\n", str2);
				ft_printf("Ignored = %d\n", ignored);
				ft_printf("Comparing '%c' (%d) with '%c' (%d)\n", *s1, *s1, *s2, *s2);
			}
			int value = 0;
			while (ignore_char(*s1))
			{
				value += *s1;
				s1++;
			}
			if (ignored == 0)
				ignored = ft_tolower(*s2) - value;
		}
		if (ignore_char(*s2))
		{
			if ((ft_strstr(str1, "(*FileActionRm).Size") && ft_strstr(str2, "(*FileAction_Rm).Size"))
			|| (ft_strstr(str1, "(*FileAction_Rm).Size") && ft_strstr(str2, "(*FileActionRm).Size")))
			{
				ft_printf("COUCOU2\n");
				ft_printf("str1 = %s\n", str1);
				ft_printf("str2 = %s\n", str2);
				ft_printf("Ignored = %d\n", ignored);
				ft_printf("Comparing '%c' (%d) with '%c' (%d)\n", *s1, *s1, *s2, *s2);
			}
			if ((ft_strstr(str1, "(*FileActionRm).Size") && ft_strstr(str2, "(*FileAction_Rm).Size"))
			|| (ft_strstr(str1, "(*FileAction_Rm).Size") && ft_strstr(str2, "(*FileActionRm).Size")))
			{
				ft_printf("COUCOU2\n");
				ft_printf("str1 = %s\n", str1);
				ft_printf("str2 = %s\n", str2);
				ft_printf("Ignored = %d\n", ignored);
				ft_printf("Comparing '%c' (%d) with '%c' (%d)\n", *s1, *s1, *s2, *s2);
			}
			int value = 0;
			while (ignore_char(*s2))
			{
				value += *s2;
				s2++;
			}
			if (ignored == 0)
				ignored = value - ft_tolower(*s1);
		}
		//if (ft_strequ(str1, "runtime/cgo(.text)") && ft_strequ(str2, "runtime/cgo(.text)"))
		//	ft_printf("After ignored, s1 = \"%s\", s2 = \"%s\"\n", s1, s2);
		// The string with the less ignored chars comes first
		//if (s1_ignored > s2_ignored && ignored == 0)
		//	ignored = 2;
		//else if (s1_ignored < s2_ignored && ignored == 0)
		//	ignored = 1;
		// If they are equal but with different case,
		// priority to the low case
		if (*s1 != *s2 && ft_tolower(*s1) == ft_tolower(*s2) && case_diff == 0)
		{
			if (*s1 >= 'a' && *s1 <= 'z')
				case_diff = 2;
			else if (*s2 >= 'a' && *s2 <= 'z')
				case_diff = 1;
		}
		if (ft_tolower(*s1) != ft_tolower(*s2))
		{
			/*if ((ft_strequ(str1, "freeaddrinfo@@GLIBC_2.2.5") && ft_strequ(str2, "free@@GLIBC_2.2.5"))
			|| (ft_strequ(str2, "freeaddrinfo@@GLIBC_2.2.5") && ft_strequ(str1, "free@@GLIBC_2.2.5")))
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
		/*if (ft_strequ(str1, "github.com/docker/cli/vendor/golang.org/x/crypto/curve25519.basePoint")
		|| ft_strequ(str2, "github.com/docker/cli/vendor/golang.org/x/crypto/curve25519.basePoint"))
			//|| (ft_strequ(str1, "runtime.GC") && ft_strequ(str2, "runtime._GC")))
		{
			ft_printf("Case diff\n");
			ft_printf("str1 = %s\n", str1);
			ft_printf("str2 = %s\n", str2);
			ft_printf("case = %d\n", case_diff);
		}*/
		//if (ignored == 0)
		//	ft_printf("\"%s\" and \"%s\" are equal except from case. diff = %d\n", str1, str2, case_diff);
		//if ((ft_strequ(str1, "time.Time.date") && ft_strequ(str2, "time.Time.Date"))
		//|| (ft_strequ(str1, "time.Time.Date") && ft_strequ(str2, "time.Time.date")))
		if ((ft_strequ(str1, "time.Time.UTC.stkobj") && ft_strequ(str2, "time.(*Time).UTC.stkobj"))
		|| (ft_strequ(str1, "time.(*Time).UTC.stkobj") && ft_strequ(str2, "time.Time.UTC.stkobj")))
			ft_printf("Case diff\n");
		if (case_diff == 1)
			return (1);
		else if (case_diff == 2)
			return (-1);
		return (ignored);
	}
	if (ft_tolower(*s1) == ft_tolower(*s2) && ignored != 0)
	{
		/*if (ft_strequ(str1, "github.com/docker/cli/vendor/golang.org/x/crypto/curve25519.basePoint")
		|| ft_strequ(str2, "github.com/docker/cli/vendor/golang.org/x/crypto/curve25519.basePoint"))
			//|| (ft_strequ(str1, "runtime.GC") && ft_strequ(str2, "runtime._GC")))
		{
			ft_printf("Ingored diff\n");
			ft_printf("str1 = %s\n", str1);
			ft_printf("str2 = %s\n", str2);
			ft_printf("ignored = %d\n", ignored);
		}*/
		//if (ft_strequ(str1, "free@@GLIBC_2.2.5")
		//|| ft_strequ(str1, "free@@GLIBC_2.2.5"))
		//	ft_printf("Ignored diff\n");
		//if (sym1->sym.st_value != sym2->sym.st_value)
		//	return (sym1->sym.st_value > sym2->sym.st_value);
		//if (ft_strequ(str1, "time.Time.date") && ft_strequ(str2, "time.Time.date"))
		//	ft_printf("Ignored diff = %d\n", ignored);
		if ((ft_strstr(str1, "(*FileActionRm).Size") && ft_strstr(str2, "(*FileAction_Rm).Size"))
			|| (ft_strstr(str1, "(*FileAction_Rm).Size") && ft_strstr(str2, "(*FileActionRm).Size")))
		{
			ft_printf("Ingored diff\n");
			ft_printf("str1 = %s\n", str1);
			ft_printf("str2 = %s\n", str2);
			ft_printf("ignored = %d\n", ignored);
		}
		//if (ignored < 0)
		//	return (1);
		//else if (ignored > 0)
		//	return (-1);
	}
	if (ft_tolower(*s1) != ft_tolower(*s2))
		return (ft_tolower(*s1) - ft_tolower(*s2));
	//if (ELF64_ST_TYPE(sym1->sym.st_info) != ELF64_ST_TYPE(sym2->sym.st_info))
//		return (ELF64_ST_TYPE(sym1->sym.st_info) < ELF64_ST_TYPE(sym2->sym.st_info));
	//if (ft_strequ(str1, "runtime/cgo(.text)") && ft_strequ(str2, "runtime/cgo(.text)"))
	//	ft_printf("COUCOU\n");
	// Last comparison is between the addresses
	// For some reason, it's inversed when symbol is in the bss section
	//if (ft_strequ(str1, "free@@GLIBC_2.2.5")
	//|| ft_strequ(str1, "free@@GLIBC_2.2.5"))
	//	ft_printf("No diff\n");
	if (sym1->sym.st_info != sym2->sym.st_info)
		return (sym1->sym.st_info > sym2->sym.st_info);
	if (sym1->sym.st_value != sym2->sym.st_value)
	{
		//if (ft_strequ(str1, "time.Time.date") && ft_strequ(str2, "time.Time.date"))
		//	ft_printf("Address diff\n");
		if (sym1->type == 'b')
			return (sym1->sym.st_value < sym2->sym.st_value);
		return (sym1->sym.st_value > sym2->sym.st_value);
	}
	return (0);
}
