/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_data.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnicosia <lnicosia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/05 32:05:36 by lnicosia          #+#    #+#             */
/*   Updated: 2021/09/23 09:21:42 by lnicosia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "options.h"

uint16_t	read_uint16(uint16_t nb, int endian)
{
	uint16_t	res;

	res = nb;
	if (endian & OPT_BIG_ENDIAN)
		reverse_endian(&res, sizeof(uint16_t));
	return (res);
}

uint32_t	read_uint32(uint32_t nb, int endian)
{
	uint32_t	res;

	res = nb;
	if (endian & OPT_BIG_ENDIAN)
		reverse_endian(&res, sizeof(uint32_t));
	return (res);
}

uint64_t	read_uint64(uint32_t nb, int endian)
{
	uint64_t	res;

	res = nb;
	if (endian & OPT_BIG_ENDIAN)
		reverse_endian(&res, sizeof(uint64_t));
	return (res);
}

unsigned int	read_unsigned_int(unsigned int nb, int endian)
{
	unsigned int	res;

	res = nb;
	if (endian & OPT_BIG_ENDIAN)
		reverse_endian(&res, sizeof(unsigned int));
	return (res);
}

long unsigned int	read_long_unsigned_int(long unsigned int nb, int endian)
{
	long unsigned int	res;

	res = nb;
	if (endian & OPT_BIG_ENDIAN)
		reverse_endian(&res, sizeof(long unsigned int));
	return (res);
}

int			read_int(int nb, int endian)
{
	int	res;

	res = nb;
	if (endian & OPT_BIG_ENDIAN)
		reverse_endian(&res, sizeof(int));
	return (res);
}
