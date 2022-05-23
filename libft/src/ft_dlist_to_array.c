/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dlist_to_array.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/15 20:54:27 by lnicosia          #+#    #+#             */
/*   Updated: 2021/01/11 19:01:39 by lnicosia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void*	ft_dlist_to_array(t_dlist* lst)
{
	void*	res;
	size_t	elem_size;
	size_t	lstlen;

	if (!lst)
		return (NULL);
	while (lst && lst->prev)
		lst = lst->prev;
	elem_size = lst->content_size;
	lstlen = ft_dlstlen(lst);
	res = ft_memalloc(lstlen * elem_size);
	if (!res)
		return (NULL);
	for (size_t i = 0; i < lstlen; i++)
	{
		ft_memcpy(res + (i * elem_size), lst->content, elem_size);
		lst = lst->next;
	}
	return (res);
}
