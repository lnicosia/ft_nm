/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnicosia <lnicosia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/08 23:07:11 by lnicosia          #+#    #+#             */
/*   Updated: 2021/03/29 12:04:25 by lnicosia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int		ft_strcmp(const char *s1, const char *s2)
{
	int	c1;
	int	c2;

	while (*s1 && *s2)
	{
		if (*s1 != *s2)
		{
			c1 = *s1;
			c2 = *s2;
			if ((unsigned char)*s1 >= 0xC0)
				c1 = 260;
			else if ((unsigned char)*s2 >= 0xC0)
				c2 = 260;
			return (c1 - c2);
		}
		s1++;
		s2++;
	}
	c1 = *s1;
	c2 = *s2;
	if ((unsigned char)*s1 >= 0xC0)
		c1 = 260;
	else if ((unsigned char)*s2 >= 0xC0)
		c2 = 260;
	return (c1 - c2);
}
