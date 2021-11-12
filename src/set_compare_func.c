/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_compare_func.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnicosia <lnicosia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/06 15:03:22 by lnicosia          #+#    #+#             */
/*   Updated: 2021/09/22 18:25:13 by lnicosia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm.h"
#include "options.h"

int	(*set_compare_func(int type, int opt))(void *s1, void *s2)
{
	if (opt & OPT_P)
	{
		if (opt & OPT_R)
			return (rno_sort);
		return (no_sort);
	}
	if (opt & OPT_N)
	{
		if (type == 64)
		{
			if (opt & OPT_R)
				return (rcompare_addresses64);
			return (compare_addresses64);
		}
		else if (type == 32)
		{
			if (opt & OPT_R)
				return (rcompare_addresses32);
			return (compare_addresses32);
		}
	}
	if (type == 64)
	{
		if (opt & OPT_R)
			return (rcompare_names64);
		return (compare_names64);
	}
	else if (type == 32)
	{
		if (opt & OPT_R)
			return (rcompare_names32);
		return (compare_names32);
	}
	return (0);
}
