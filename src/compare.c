/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   compare.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnicosia <lnicosia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/06 15:03:22 by lnicosia          #+#    #+#             */
/*   Updated: 2021/09/22 18:08:53 by lnicosia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "nm.h"

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
	return (ft_strcmp(name1, name2));
}