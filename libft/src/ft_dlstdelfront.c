/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dlstdelfront.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnicosia <lnicosia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/25 10:33:21 by lnicosia          #+#    #+#             */
/*   Updated: 2021/03/25 14:54:29 by lnicosia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_dlstpopfront(t_dlist **alst, void (*del)(void *, size_t))
{
	t_dlist	*new;

	if (!alst)
		return ;
	new = (*alst)->next;
	if ((*alst)->content)
	{
		del((*alst)->content, (*alst)->content_size);
		free((*alst)->content);
	}
	free(*alst);
	*alst = new;
}


void	ft_dlstdelfront(t_dlist **lst, void (*del)(void *, size_t))
{
	while (*lst && (*lst)->prev)
		*lst = (*lst)->prev;
	while (*lst)
		ft_dlstpopfront(lst, del);
}
