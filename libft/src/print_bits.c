/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_bits.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnicosia <lnicosia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/08 11:39:13 by lnicosia          #+#    #+#             */
/*   Updated: 2021/04/08 12:13:19 by lnicosia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdio.h>

/*
**	Print bits of a variable
*/

void	print_bits(void *ptr, size_t size, int flags)
{
	size_t		i;
	int	bits;

	(void)flags;
	bits = * (int *) ptr;
	i = 0;
	//ft_printf("%d", bits >> 63);
	while (i < size)
	{
		ft_printf("%hhd", (bits << i) >> 31);
		i++;
	}
	write(STDOUT_FILENO, "\n", 1);
}