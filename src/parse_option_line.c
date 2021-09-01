/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_option_line.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnicosia <lnicosia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/06 15:30:41 by lnicosia          #+#    #+#             */
/*   Updated: 2021/04/06 15:30:51 by lnicosia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "options.h"
#include "libft.h"

int		check_opt(char av, int *opt)
{
	if (av == 'a')
	{
		*opt |= OPT_A;
	}
	else if (av == 'v')
	{
		*opt |= OPT_VERBOSE;
	}
	return (0);
}

int		parse_option_line(char *av, int *opt)
{
	av++;
	while (*av)
	{
		check_opt(*av, opt);
		av++;
	}
	return (0);
}