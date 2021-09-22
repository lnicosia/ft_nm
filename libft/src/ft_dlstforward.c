/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dlstforward.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnicosia <lnicosia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/25 10:58:43 by lnicosia          #+#    #+#             */
/*   Updated: 2021/03/25 12:27:12 by lnicosia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
**	Go forward into the list until the compare function fails
*/

void	ft_dlstforward(t_dlist **lst, int (*compare)(void *, void *))
{
	if (!compare)
		return ;
	while (*lst && (*lst)->next && compare((*lst)->content, (*lst)->next->content) >= 0)
	{
		ft_printf("------ |Comparison between |%s| and |%s| = %d\n",
		(*lst)->content, (*lst)->next->content, compare((*lst)->content, (*lst)->next->content));
		*lst = (*lst)->next;
	}
}