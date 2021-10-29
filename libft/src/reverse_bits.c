/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reverse_bits.c                                     :+:      :+:    :+:   */
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
** Length in bits
*/

void	reverse_bits(void *ptr, size_t length)
{
	uint8_t *bytes = (uint8_t *)ptr;
	uint8_t tmp[MAX_BYTES] = { 0 };
	
	if (length > MAX_BYTES * 8)
	{
		custom_error("reverse_bits error: Trying to reverse bits of an array bigger than %d\n",
		MAX_BYTES * 8);
		return ;
	}
	for (size_t i = 0; i < length / 8; i++)
	{
		for (size_t j = 0; j < 8; j++)
		{
			tmp[i] = (tmp[i] << 1) | ((bytes[(length / 8) - i - 1] >> j) & 1);
		}
	}
	for (size_t i = 0; i < length / 8; i++)
	{
		bytes[i] = tmp[i];
	}
}
