/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dlstnew.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnicosia <lnicosia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/25 10:30:44 by lnicosia          #+#    #+#             */
/*   Updated: 2021/03/25 10:31:28 by lnicosia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"


t_dlist	*ft_dlstnew(void const *content, size_t content_size)
{
	t_dlist	*new;

	if (!(new = (t_dlist*)ft_memalloc(sizeof(*new))))
		return (0);
	if (content == NULL)
	{
		new->content = NULL;
		new->content_size = 0;
		new->next = NULL;
		new->prev = NULL;
		return (new);
	}
	if (!(new->content = ft_memalloc(content_size)))
	{
		ft_memdel((void**)&new);
		return (0);
	}
	ft_memmove(new->content, content, content_size);
	new->content_size = content_size;
	new->next = NULL;
	new->prev = NULL;
	return (new);
}
