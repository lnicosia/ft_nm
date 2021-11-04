/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reverse_endian.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnicosia <lnicosia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/29 19:13:47 by lnicosia          #+#    #+#             */
/*   Updated: 2021/09/22 13:23:54 by lnicosia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

#define MAX_BYTES 32

/*
** Length in bytes
*/

void	reverse_endian(void *ptr, size_t length)
{
	uint8_t *bytes = (uint8_t *)ptr;
	uint8_t tmp[MAX_BYTES] = { 0 };
	
	if (length > MAX_BYTES)
	{
		custom_error("reverse_bits error: Trying to reverse the endian of an array bigger than %d\n",
		MAX_BYTES);
		return ;
	}
	for (size_t i = 0; i < length; i++)
	{
		tmp[i] = bytes[length - 1 - i];
	}
	for (size_t i = 0; i < length; i++)
	{
		bytes[i] = tmp[i];
	}
}
