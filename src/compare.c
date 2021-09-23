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

int		ft_strcmp_no_case(char *s1, char *s2)
{
	int	underscore;

	underscore = 0;
	while (*s1 && *s2)
	{
		while (*s1 && *s1 == '_')
		{
			s1++;
			if (underscore == 0)
				underscore = 1;
		}
		while (*s2 && *s2 == '_')
		{
			s2++;
			if (underscore == 0)
				underscore = 2;
		}
		if (ft_tolower(*s1) != ft_tolower(*s2))
		{
			return (ft_tolower(*s1) - ft_tolower(*s2));
		}
		s1++;
		s2++;
	}
	if (ft_tolower(*s1) == ft_tolower(*s2) && underscore != 0)
	{
		if (underscore == 1)
			return (1);
		else if (underscore == 2)
			return (-1);
	}
	return (ft_tolower(*s1) - ft_tolower(*s2));
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

int		compare_names_no_special(void *s1, void *s2)
{
	char	*name1;
	char	*name2;

	name1 = ((t_sym*)s1)->name;
	name2 = ((t_sym*)s2)->name;
	while (name1 && *name1 == '_')
		name1++;
	while (name2 && *name2 == '_')
		name2++;
	return (ft_strcmp_no_case(name1, name2));
}
