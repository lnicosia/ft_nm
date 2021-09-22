/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dlstlen.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnicosia <lnicosia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/25 14:19:49 by lnicosia          #+#    #+#             */
/*   Updated: 2021/03/25 14:21:55 by lnicosia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_dlstlen(t_dlist *lst)
{
	size_t	len;

	len = 0;
	while (lst && lst->prev)
		lst = lst->prev;
	while (lst && lst->next)
	{
		len++;
		lst = lst->next;
	}
	return (len);
}