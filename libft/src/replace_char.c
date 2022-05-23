/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   replace_char.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnicosia <lnicosia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/08 11:39:13 by lnicosia          #+#    #+#             */
/*   Updated: 2022/05/10 14:24:06 by lnicosia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	count_find(char* str, char find)
{
	size_t	i;
	int		count;

	i = 0;
	count = 0;
	while (str[i])
	{
		if (str[i] == find)
			count++;
		i++;
	}
	return (count);
}

char*	replace_char(char* str, char find, char* replace)
{
	size_t	str_incr;
	size_t	res_incr;
	size_t	replace_len;
	char*	res;
	int		replaces_to_do;
	int		replace_size;

	str_incr = 0;
	res_incr = 0;
	res = NULL;
	replace_len = ft_strlen(replace);
	replaces_to_do = count_find(str, find);
	replace_size = replaces_to_do * (replace_len - 1);
	if (!(res = ft_strnew(ft_strlen(str) + replace_size)))
		return (NULL);
	while (str[str_incr])
	{
		if (str[str_incr] == find)
		{
			ft_strcpy(res + res_incr, replace);
			res_incr += replace_len;
		}
		else
		{
			res[res_incr] = str[str_incr];
			res_incr++;
		}
		str_incr++;
	}
	return (res);
}