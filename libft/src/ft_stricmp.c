/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_stricmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnicosia <lnicosia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/29 11:39:55 by lnicosia          #+#    #+#             */
/*   Updated: 2021/03/29 12:05:34 by lnicosia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
**	Compares two strings ignoring case
*/

int		ft_stricmp(const char *s1, const char *s2)
{
	char	c1;
	char	c2;

	while (*s1 && *s2)
	{
		c1 = ft_tolower(*s1);
		c2 = ft_tolower(*s2);
		if (c1 != c2)
		{
			return (c1 - c2);
		}
		s1++;
		s2++;
	}
	c1 = ft_tolower(*s1);
	c2 = ft_tolower(*s2);
	return (c1 - c2);
}
