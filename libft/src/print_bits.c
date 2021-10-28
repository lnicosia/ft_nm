/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_bits.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnicosia <lnicosia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/29 19:13:47 by lnicosia          #+#    #+#             */
/*   Updated: 2021/09/22 13:23:54 by lnicosia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	print_bits(void *ptr, size_t length)
{
	uint8_t *bytes = (uint8_t *)ptr;
	for (size_t i = 0; i < length / 8; i++)
	{
		for (size_t j = 0; j < 8; j++)
		{
			ft_printf("%d ", (bytes[(length / 8) - i - 1] >> (8 - j - 1)) & 1);
		}
	}
}
