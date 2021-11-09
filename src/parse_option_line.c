/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_option_line.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnicosia <lnicosia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/06 15:30:41 by lnicosia          #+#    #+#             */
/*   Updated: 2021/09/23 09:18:10 by lnicosia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "options.h"
#include "libft.h"

int		check_opt(char *av, int *opt)
{
	if (*av == 'a')
		*opt |= OPT_A;
	else if (*av == 'c')
		*opt |= OPT_C;
	else if (*av == 'p')
		*opt |= OPT_P;
	else if (*av == 's')
		*opt |= OPT_S;
	else if (*av == 'o' || *av == 'A')
		*opt |= OPT_O;
	return (0);
}

int		parse_option_line(char *av, int *opt)
{
	av++;
	if (ft_strequ(av, "-verbose"))
		*opt |= OPT_VERBOSE;
	else
	{
		while (*av)
		{
			check_opt(av, opt);
			av++;
		}
	}
	return (0);
}

/*
**	Checks if the given string is an option line (starting with '-')
*/

int		is_arg_an_option_line(char *av)
{
	return (ft_strlen(av) >= 1 && av[0] == '-');
}

/*
**	Parse all the options by checking arguments starting with '-'
*/

int		parse_nm_options(int ac, char **av, int *opt)
{
	int	i;
	int nb_files;
	
	i = 1;
	nb_files = 0;
	while (i < ac)
	{
		if (is_arg_an_option_line(av[i]))
		{
			parse_option_line(av[i], opt);
		}
		else
			nb_files++;
		i++;
	}
	return (nb_files);
}
