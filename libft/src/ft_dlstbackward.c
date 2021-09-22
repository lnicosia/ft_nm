/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dlstbackward.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnicosia <lnicosia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/25 10:58:36 by lnicosia          #+#    #+#             */
/*   Updated: 2021/03/25 11:59:35 by lnicosia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
**	Go backward into the list until the compare function fails
*/

void	ft_dlstbackward(t_dlist **lst, int (*compare)(void *, void *))
{
	if (!compare)
		return ;
	while (*lst && (*lst)->prev && compare((*lst)->content, (*lst)->prev->content) > 0)
	{
		*lst = (*lst)->prev;
	}
}